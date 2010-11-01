#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <math.h>
#include <errno.h>

#include <iostream>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "CommPlayer.h"
#include "Comm/Messages/Messages.h"
#include "Comm/Messages/GameController/GameControllerMessage.h"
#include "Comm/Messages/Player/PlayerMessage.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT COMM
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

CommPlayer::CommPlayer(ConfigFile & configFile, Log & _log)
  : Comm(),
    teamNumber        (configFile.getInt  ("team/teamNumber", 0)),
    playerNumber      (configFile.getInt  ("team/playerNumber", 2)),
    gameControllerPort(configFile.getInt  ("comm/gameControllerPort", 3838)),
    playerPort        (configFile.getInt  ("comm/playerPort", 9999)),
    maxPacketSize     (configFile.getInt  ("comm/maxPacketSize", 1024)),
    socketTimeout     (configFile.getFloat("comm/socketTimeout", 1)),
    log(_log),
    gameControllerSocket(-1),
    playerSocket(-1),
    threadRunning(false),
    thread(),
    dataMutex() {
  // Create the UDP sockets and bind to the right ports
  createUDPSockets();

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

CommPlayer::~CommPlayer() {
  // Stop the thread
  if (threadRunning) {
    threadRunning = false;
    pthread_join(thread, NULL);
  }

  if (gameControllerSocket >= 0) {
    close(gameControllerSocket);
    gameControllerSocket = -1;
  }
  if (playerSocket >= 0) {
    close(playerSocket);
    playerSocket = -1;
  }

  pthread_mutex_lock(&dataMutex);
  // Delete all the game controller messages that we received but haven't passed
  // to other components
  std::vector<GameControllerMessage const *>::iterator gameControllerIter;
  for (gameControllerIter = newGameControllerMessagesReceived.begin();
       gameControllerIter != newGameControllerMessagesReceived.end();
       gameControllerIter++) {
    delete *gameControllerIter;
  }
  newGameControllerMessagesReceived.clear();
  pthread_mutex_unlock(&dataMutex);
}

bool CommPlayer::run(Messages & messages, Feedback & feedback) {
  // Clear the list of messages received
  messages.clearReceivedMessages();

  pthread_mutex_lock(&dataMutex);

  // Add all the game controller messages
  std::vector<GameControllerMessage const *>::iterator gameControllerIter;
  for (gameControllerIter = newGameControllerMessagesReceived.begin();
       gameControllerIter != newGameControllerMessagesReceived.end();
       gameControllerIter++) {
    messages.addReceivedMessage(*gameControllerIter);
  }
  newGameControllerMessagesReceived.clear();

  std::vector<PlayerMessage const *>::const_iterator playerIter;

  // Add all the received player messages
  for (playerIter = newPlayerMessagesReceived.begin();
       playerIter != newPlayerMessagesReceived.end();
       playerIter++) {
    PlayerMessage const *playerMessage = *playerIter;
    // Ignore message from other teams
    if (playerMessage->getSenderTeamNumber() != teamNumber) {
      continue;
    }
    // Ignore messages that we sent
    if (playerMessage->getSenderPlayerNumber() == playerNumber) {
      continue;
    }
    messages.addReceivedMessage(playerMessage);
  }
  newPlayerMessagesReceived.clear();

  // Add all the player messages to send
  for (playerIter = messages.getPlayerMessagesToSend().begin();
       playerIter != messages.getPlayerMessagesToSend().end();
       playerIter++) {
    playerMessagesToSend.push_back(*playerIter);
  }
  messages.clearPlayerMessagesToSend();

  pthread_mutex_unlock(&dataMutex);
  return false;
}

void CommPlayer::runThread() {
  fd_set sockets;
  int maxSocket = gameControllerSocket > playerSocket ? gameControllerSocket + 1 : playerSocket + 1;

  struct timeval timeout;
  timeout.tv_sec  = (int)floor(socketTimeout);
  timeout.tv_usec = (int)floor((socketTimeout - timeout.tv_sec) * 1000);

  unsigned char messageData[maxPacketSize];
  struct sockaddr_in from;
  int                fromLength = sizeof(from);

  LOG_TRACE("Starting thread");

  threadRunning = true;
  while (threadRunning) {
    FD_ZERO(&sockets);
    FD_SET(gameControllerSocket, &sockets);
    FD_SET(playerSocket, &sockets);

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

    // Read incoming data from game controller
    if (FD_ISSET(gameControllerSocket, &sockets)) {
      // Read the bytes in the packet
      bzero(messageData, maxPacketSize);
      int numBytes = recvfrom(gameControllerSocket,
                              messageData,
                              maxPacketSize,
                              0,
                              (struct sockaddr *)&from,
                              (socklen_t *)&fromLength);

      // Convert the packet into a RoboCupGameControlData
      if (numBytes == sizeof(struct RoboCupGameControlData)) {
        struct RoboCupGameControlData *roboCupData = (struct RoboCupGameControlData *)messageData;
        GameControllerMessage *gameControllerMessage = new GameControllerMessage(*roboCupData);
        if (gameControllerMessage->isValid(teamNumber)) {
          pthread_mutex_lock(&dataMutex);
          newGameControllerMessagesReceived.push_back(gameControllerMessage);
          pthread_mutex_unlock(&dataMutex);
          LOG_INFO("Received a game controller message.");
        }
        else {
          delete gameControllerMessage;
        }
      }
      else if (numBytes > 0) {
        LOG_WARN("Size of packet from game controller does not match RoboCupGameControlData.");
      }
    }

    // Read incoming data from other players
    if (FD_ISSET(playerSocket, &sockets)) {
      // Read the bytes in the packet
      bzero(messageData, maxPacketSize);
      int numBytes = recvfrom(playerSocket,
                              messageData,
                              maxPacketSize,
                              0,
                              (struct sockaddr *)&from,
                              (socklen_t *)&fromLength);
      if (numBytes > 0) {
        LOG_DEBUG("Received a player message of %d bytes.", numBytes);

        PlayerMessage *playerMessage = new PlayerMessage();

        try {
          // De-serialize the player message
          std::string playerMessageStr((char const *)messageData, numBytes);
          std::istringstream iss(playerMessageStr, std::stringstream::in | std::stringstream::binary);
          boost::archive::binary_iarchive input(iss);
          input & (*playerMessage);

          // Ignore message from other teams
          // Ignore messages that we sent
          if ((playerMessage->getSenderTeamNumber() == teamNumber)
           && (playerMessage->getSenderPlayerNumber() != playerNumber)) {
            pthread_mutex_lock(&dataMutex);
            newPlayerMessagesReceived.push_back(playerMessage);
            pthread_mutex_unlock(&dataMutex);
          }
          else {
            delete playerMessage;
          }
        }
        catch (std::exception const& error) {
          LOG_INFO("I couldn't de-serialize the incoming packet.");
          delete playerMessage;
        }
      }

    }

    // Send player messages
    pthread_mutex_lock(&dataMutex);
    std::vector<PlayerMessage const *>::const_iterator iter;
    for (iter = playerMessagesToSend.begin(); iter != playerMessagesToSend.end(); iter++) {
      // Serialize the player message
      std::ostringstream oss(std::stringstream::out | std::stringstream::binary);
      boost::archive::binary_oarchive output(oss);
      PlayerMessage const *playerMessage = *iter;
      output & (*playerMessage);
      std::string str = oss.str();
      char const *data = str.c_str();
      int numBytes = str.length();

      // Send the serialized bytes
      // This can fail if EINTR occurs
      sendto(playerSocket,
             data, numBytes,
             0,
             (struct sockaddr *)&playerBroadcastAddress, sizeof(playerBroadcastAddress));

      LOG_DEBUG("Sent a player message of %d bytes.", numBytes);

      // Delete the player message
      delete playerMessage;
    }
    playerMessagesToSend.clear();
    pthread_mutex_unlock(&dataMutex);
  }

  LOG_TRACE("Ending thread");
}

void CommPlayer::createUDPSockets() {
  struct sockaddr_in server;
  int length = sizeof(server);

  // Create the game controller socket (to listen for packets from the game
  // controller)
  gameControllerSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (gameControllerSocket < 0) {
    LOG_ERROR("Failed to create game controller socket.");
    return;
  }

  // Create the player socket (to listen for packets from other players)
  playerSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (playerSocket < 0) {
    LOG_ERROR("Failed to create player socket.");
    close(gameControllerSocket);
    gameControllerSocket = -1;
    return;
  }

  LOG_INFO("Created sockets for game controller and player.");

  // Eliminate "Address already in use" error from bind in game controller socket
  int optionValue = 1;
  if (setsockopt(gameControllerSocket, SOL_SOCKET, SO_REUSEADDR,
                 (const void *)&optionValue, sizeof(optionValue)) < 0) {
    LOG_ERROR("Failed to set options for game controller socket.");
    close(gameControllerSocket);
    close(playerSocket);
    gameControllerSocket = -1;
    playerSocket = -1;
    return;
  }

  // Eliminate "Address already in use" error from bind in player socket
  optionValue = 1;
  if (setsockopt(playerSocket, SOL_SOCKET, SO_REUSEADDR,
                 (const void *)&optionValue, sizeof(optionValue)) < 0) {
    LOG_ERROR("Failed to set options for player socket.");
    close(gameControllerSocket);
    close(playerSocket);
    gameControllerSocket = -1;
    playerSocket = -1;
    return;
  }

  bzero(&server, length);
  server.sin_family      = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port        = htons(gameControllerPort);
  if (bind(gameControllerSocket, (struct sockaddr *)&server, length) < 0) {
    LOG_ERROR("Failed to bind game controller socket to port %d.",
              gameControllerPort);
    close(gameControllerSocket);
    close(playerSocket);
    gameControllerSocket = -1;
    playerSocket = -1;
    return;
  }

  bzero(&server, length);
  server.sin_family      = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port        = htons(playerPort);
  if (bind(playerSocket, (struct sockaddr *)&server, length) < 0) {
    LOG_ERROR("Failed to bind player socket to port %d.",
             playerPort);
    close(gameControllerSocket);
    close(playerSocket);
    gameControllerSocket = -1;
    playerSocket = -1;
    return;
  }

  LOG_INFO("Bound both sockets to their relevant ports.");

  // Allow broadcast on both sockets
  optionValue = 1;
  if (setsockopt(gameControllerSocket,
                 SOL_SOCKET, SO_BROADCAST,
                 &optionValue, sizeof(optionValue)) < 0) {
    LOG_ERROR("Failed to set socket option on game controller socket.");
    close(gameControllerSocket);
    close(playerSocket);
    gameControllerSocket = -1;
    playerSocket = -1;
    return;
  }
  if (setsockopt(playerSocket,
                 SOL_SOCKET, SO_BROADCAST,
                 &optionValue, sizeof(optionValue)) < 0) {
    LOG_ERROR("Failed to set socket option on player socket.");
    close(gameControllerSocket);
    close(playerSocket);
    gameControllerSocket = -1;
    playerSocket = -1;
    return;
  }

  // Set the destination for player sockets
  bzero(&playerBroadcastAddress, sizeof(struct sockaddr_in));
  playerBroadcastAddress.sin_family      = AF_INET;
  playerBroadcastAddress.sin_port        = (in_port_t)htons(playerPort);
  playerBroadcastAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);
}

void * CommPlayer::startThread(void *ptr) {
  ((CommPlayer *)ptr)->runThread();
  return NULL;
}
