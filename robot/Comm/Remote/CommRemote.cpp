#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <math.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <iostream>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "CommRemote.h"
#include "Comm/Messages/Messages.h"
#include "Comm/Messages/Remote/RemoteMessageFromRobot.h"
#include "Comm/Messages/Remote/RemoteMessageToRobot.h"
#include "Agent/Feedback/Feedback.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT COMM
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

CommRemote::CommRemote(ConfigFile & configFile, Log & _log)
  : listenPort            (configFile.getInt  ("comm/remoteListenPort", 10000)),
    maxClientQueue        (configFile.getInt  ("comm/remoteMaxClientQueue", 1)),
    maxPacketSize         (configFile.getInt  ("comm/maxPacketSize", 1024)),
    socketTimeout         (configFile.getFloat("comm/socketTimeout", 1)),
    robotMessageHeaderSize(configFile.getInt  ("comm/robotMessageHeaderSize", 3)),
    log(_log),
    serverSocket(-1),
    clientSocket(-1),
    threadRunning(false),
    thread(),
    dataMutex(),
    messagesToRobot(),
    messagesFromRobot(),
    wasConnectedToClient(false) {
  // Create the TCP socket and bind to the right port
  createTCPSocket();

  // Create the data mutex
  if (pthread_mutex_init(&dataMutex, NULL) < 0) {
    LOG_ERROR("Error creating data mutex.");
    return;
  }

  // Start the thread to read/send packets
  if (pthread_create(&thread, NULL, startThread, this) < 0) {
    LOG_ERROR("Error creating thread.");
    return;
  }
}

CommRemote::~CommRemote() {
  // Stop the thread
  if (threadRunning) {
    threadRunning = false;
    pthread_join(thread, NULL);
  }

  // Close the listening port, and any connected clients
  if (serverSocket >= 0) {
    close(serverSocket);
    serverSocket = -1;
  }
  if (clientSocket >= 0) {
    close(clientSocket);
    clientSocket = -1;
  }

  // Delete any data we have
  pthread_mutex_lock(&dataMutex);

  std::vector<RemoteMessageToRobot const *>::iterator toIter;
  for (toIter = messagesToRobot.begin();
       toIter != messagesToRobot.end();
       toIter++) {
    delete (*toIter);
  }
  messagesToRobot.clear();

  std::vector<RemoteMessageFromRobot const *>::iterator fromIter;
  for (fromIter = messagesFromRobot.begin();
      fromIter != messagesFromRobot.end();
      fromIter++) {
    delete (*fromIter);
  }
  messagesFromRobot.clear();

  pthread_mutex_unlock(&dataMutex);
}

bool CommRemote::run(Messages & messages, Feedback & feedback) {
  // Delete all the previous remote messages to the robot
  messages.clearRemoteMessagesToRobot();

  pthread_mutex_lock(&dataMutex);

  // Add all the received remote messages to the robot
  std::vector<RemoteMessageToRobot const *>::iterator toIter;
  for (toIter = messagesToRobot.begin();
       toIter != messagesToRobot.end();
       toIter++) {
    messages.addRemoteMessageToRobot(*toIter);
  }
  messagesToRobot.clear();

  // Take ownership of all the remote messages to be sent from the robot
  messagesFromRobot.insert(messagesFromRobot.end(),
                           messages.getRemoteMessagesToSend().begin(),
                           messages.getRemoteMessagesToSend().end());
  messages.clearRemoteMessagesFromRobot();

  pthread_mutex_unlock(&dataMutex);

  // Are we connected to a client now?
  bool isConnectedToClient = (clientSocket >= 0);
  if (isConnectedToClient != wasConnectedToClient) {
    feedback.setConnectedToClient(isConnectedToClient);
    wasConnectedToClient = isConnectedToClient;
  }

  return false;
}

void CommRemote::runThread() {
  fd_set sockets;
  int maxSocket = serverSocket + 1;

  struct timeval timeout;
  timeout.tv_sec  = (int)floor(socketTimeout);
  timeout.tv_usec = (int)floor((socketTimeout - timeout.tv_sec) * 1000);

  LOG_DEBUG("Timeouts = %d %d", timeout.tv_sec, timeout.tv_usec);

  struct sockaddr_in from;
  int                fromLength = sizeof(from);

  unsigned char clientBuffer[maxPacketSize];
  unsigned int  offset = 0;
  unsigned int  size = 0;

  FD_ZERO(&sockets);
  FD_SET(serverSocket, &sockets);

  LOG_TRACE("Starting thread");

  threadRunning = true;
  while (threadRunning) {

    FD_ZERO(&sockets);
    FD_SET(serverSocket, &sockets);
    if (clientSocket >= 0) {
      FD_SET(clientSocket, &sockets);
    }
    maxSocket = serverSocket > clientSocket ? serverSocket + 1 : clientSocket + 1;

    int result = select(maxSocket, &sockets, NULL, NULL, &timeout);
    if (result < 0) {
      if (errno == EINTR) {
//        printf("EINTR caught in select()\n.");
        usleep((int)(socketTimeout * 1000000));
        continue;
      }
      LOG_ERROR("Error reading from sockets using select().")
      threadRunning = false;
      break;
    }

    // Read data from connected client (if any)
    if ((clientSocket >= 0) && FD_ISSET(clientSocket, &sockets)) {
      bool closeConnection = false;

      // Read as much as we can
      int numRead = recv(clientSocket, clientBuffer + offset, maxPacketSize - offset, 0);
      if (numRead > 0) {
        LOG_INFO("Read %d bytes.", numRead);
      }

      // Client ended the connection
      if (numRead == 0) {
        closeConnection = true;
        LOG_INFO("Client ended the connection.");
      }
      // Error reading from the socket
      else if ((numRead == -1) && (errno != EAGAIN) && (errno != EINTR)) {
        closeConnection = true;
        LOG_INFO("Error reading from client socket: %d.", errno);
      }

      if (closeConnection) {
        LOG_INFO("Disconnecting client");
        disconnectClient(sockets, maxSocket);
      }

      // We actually read some data
      while (numRead > 0) {
        // Do we have a size for the packet?
        if ((numRead + offset >= robotMessageHeaderSize) && (size == 0)) {
          // We want to read the last 4 bytes (32 bits)
          uint32_t *sizePointer = (uint32_t *)(clientBuffer + robotMessageHeaderSize - 4);
          size = ntohl(*sizePointer);

          LOG_INFO("Read a size of %d", size);

          // Update the buffer's offset and numRead counter
          offset += robotMessageHeaderSize;
          numRead -= robotMessageHeaderSize;
        }
        // Has the whole packet been read?
        if ((size >= 0) && (offset + numRead >= size + robotMessageHeaderSize)) {
          // Update the buffer's offset and numRead counter
          numRead -= (size + robotMessageHeaderSize - offset);
          offset = size + robotMessageHeaderSize; // same as: offset += (size + robotMessageHeaderSize - offset)

          LOG_INFO("Received a packet of type %d, size %d.", clientBuffer[0], size);

          // Convert it into a packet
          // Assumption: the type fits in 1 byte
          RemoteMessageToRobot const *message = RemoteMessageToRobot::create(clientBuffer[0],
                                                                             size,
                                                                             clientBuffer + robotMessageHeaderSize);
          if (message != NULL) {
            pthread_mutex_lock(&dataMutex);
            messagesToRobot.push_back(message);
            pthread_mutex_unlock(&dataMutex);
          }

          // Move the buffer's data as necessary
          if (numRead > 0) {
            memcpy(clientBuffer, clientBuffer + offset, numRead);
            offset = 0;
            size = 0;
          }
        }
      }

    }

    // Accept connection from server socket
    if (FD_ISSET(serverSocket, &sockets)) {
      int newClientSocket = accept(serverSocket,
                                   (struct sockaddr *)&from,
                                   (socklen_t *)&fromLength);
      if (newClientSocket == -1) {
        if (errno == EINTR) {
          LOG_INFO("Caught EINTR while accepting client connection.");
        }
        else {
          LOG_ERROR("Error accepting client.")
        }
      }
      // New client connected
      else {
        // Kill any existing client connection
        if (clientSocket >= 0) {
          disconnectClient(sockets, maxSocket);
        }

        // Save the new client socket
        clientSocket = newClientSocket;

        LOG_INFO("New client connected from %s", inet_ntoa(from.sin_addr));

        // Make the client socket non-blocking
        int flags = 1;
        if (ioctl(clientSocket, FIONBIO, &flags) == -1) {
          LOG_WARN("Could not make client socket non-blocking.");
        }

        offset = 0;
        size = 0;
      }
    }


    bool sendRobotMessages = true;

    // Send all the robot messages
    while (sendRobotMessages) {
      RemoteMessageFromRobot const *message = NULL;

      pthread_mutex_lock(&dataMutex);
      if (messagesFromRobot.size() > 0) {
        // Retrieve the first message
        message = messagesFromRobot.front();
        messagesFromRobot.erase(messagesFromRobot.begin());
      }
      else {
        sendRobotMessages = false;
      }
      pthread_mutex_unlock(&dataMutex);

      // Transmit the message
      if (message != NULL) {
        // Send data to connected client (if any)
        if (clientSocket >= 0) {
//          LOG_DEBUG("Sending a message to client: %d %d.", message->getType(), message->getSize());
          uint8_t     type = (uint8_t)message->getType();
          uint32_t    size = htonl((uint32_t)message->getSize());
//          LOG_DEBUG("Size bytes: %x %x %d", message->getSize(), size, message->getSize());

          void const *data = message->getData();

          bool errorOccurred = false;

          // Write the type of the packet
          if (!errorOccurred) {
            errorOccurred = sendDataToClient((unsigned char const *)&type,
                                              sizeof(type));
          }

          // Write the size of the packet
          if (!errorOccurred) {
            errorOccurred = sendDataToClient((unsigned char const *)&size,
                                              sizeof(size));
          }

          // Write the data
          if (!errorOccurred) {
            errorOccurred = sendDataToClient((unsigned char const *)data,
                                             message->getSize());
          }

          // Did any error occur?
          if (errorOccurred) {
            LOG_INFO("Error sending packet to connected client.");
            disconnectClient(sockets, maxSocket);
          }
        }

        // Delete the message (even if no client was connected)
        delete message;
      }
      else {
        sendRobotMessages = false;
      }

    }

  }

  LOG_TRACE("Ending thread");
}

void CommRemote::createTCPSocket() {
  struct sockaddr_in server;
  int length = sizeof(server);

  // Create the server socket (that will listen for client connections)
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket < 0) {
    LOG_ERROR("Failed to create server socket.");
    return;
  }
  LOG_INFO("Created server socket.");

  // Eliminate "Address already in use" error from bind
  int optionValue = 1;
  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,
                 (const void *)&optionValue, sizeof(optionValue)) < 0) {
    LOG_ERROR("Failed to set options for server socket.");
    close(serverSocket);
    serverSocket = -1;
    return;
  }

  // Bind the server socket to the listening port
  bzero(&server, length);
  server.sin_family      = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port        = htons(listenPort);
  if (bind(serverSocket, (struct sockaddr *)&server, length) < 0) {
    LOG_ERROR("Failed to bind server socket to port %d.",
              listenPort);
    close(serverSocket);
    serverSocket = -1;
    return;
  }
  LOG_INFO("Bound server sockets to port %d.", listenPort);

  // Start listening
  if (listen(serverSocket, maxClientQueue) < 0) {
    LOG_ERROR("Error listening on server socket.");
    close(serverSocket);
    serverSocket = -1;
    return;
  }
}

void CommRemote::disconnectClient(fd_set & sockets, int & maxSocket) {
  if (clientSocket < 0) {
    return;
  }

  close(clientSocket);
  clientSocket = -1;

  LOG_INFO("Disconnected existing client.");
}

bool CommRemote::sendDataToClient(unsigned char const *data, int size) {
  int numWritten;

  while (size > 0) {
    numWritten = send(clientSocket, data, size, MSG_DONTWAIT);
    if ((numWritten < 0) && (errno != EAGAIN) && (errno != EINTR)) {
//      LOG_ERROR("Error = %d", errno);
//      LOG_ERROR("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
//                EACCES, EAGAIN,   EWOULDBLOCK, EBADF,      ECONNRESET, EDESTADDRREQ,
//                EFAULT, EINTR,    EINVAL,      EISCONN,    EMSGSIZE,   ENOBUFS,
//                ENOMEM, ENOTCONN, ENOTSOCK,    EOPNOTSUPP, EPIPE);
      return true;
    }
    else if (numWritten > 0) {
      size -= numWritten;
      data += numWritten;
//      LOG_DEBUG("numWritten = %d, size = %d", numWritten, size);
    }
  }

  return false;
}

void * CommRemote::startThread(void *ptr) {
  ((CommRemote *)ptr)->runThread();
  return NULL;
}
