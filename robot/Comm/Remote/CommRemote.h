#ifndef COMMREMOTE_H_
#define COMMREMOTE_H_

#include <pthread.h>
#include <vector>

#include "Comm/Comm.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;
class RemoteMessageToRobot;
class RemoteMessageFromRobot;

/**
 * This class handles communications between a remote client (PC), for debugging
 * purposes.
 */
class CommRemote: public Comm {
public:
  CommRemote(ConfigFile & configFile, Log & _log);
  virtual ~CommRemote();

  virtual bool run(Messages & messages, Feedback & feedback);

private:
  PREVENT_COPY_AND_ASSIGNMENT(CommRemote);

  void runThread();

  void createTCPSocket();

  void disconnectClient(fd_set & sockets, int & maxSocket);

  /**
   * Sends data to the client, until the data is completely sent, or an error
   * occurs.
   *
   * @param data pointer to the data to the sent
   * @param size size of the data in bytes
   *
   * @return whether an error occurred
   */
  bool sendDataToClient(unsigned char const * data, int size);

  static void * startThread(void *ptr);

  const int listenPort;
  const unsigned int maxClientQueue;
  const unsigned int maxPacketSize;
  const float socketTimeout;
  const unsigned int robotMessageHeaderSize;
  const unsigned int sendBufferSize;

  Log & log;

  int serverSocket, clientSocket;
  bool threadRunning;
  pthread_t thread;
  pthread_mutex_t dataMutex;

  std::vector<RemoteMessageToRobot const *>   messagesToRobot;
  std::vector<RemoteMessageFromRobot const *> messagesFromRobot;

  bool wasConnectedToClient;

  unsigned char * sendBuffer;
  int             sendBufferOffset;
};

#endif /* COMMREMOTE_H_ */
