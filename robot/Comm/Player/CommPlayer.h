#ifndef COMMPLAYER_H_
#define COMMPLAYER_H_

#include <vector>
#include <pthread.h>
#include <netinet/in.h>

#include "Comm/Comm.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;
class Messages;
class GameControllerMessage;
class PlayerMessage;

/**
 * This class handles all communications with the Game Controller, and other
 * robots through UDP.
 */
class CommPlayer: public Comm {
public:
  CommPlayer(ConfigFile & configFile, Log & _log);

  virtual ~CommPlayer();

  virtual bool run(Messages & messages, Feedback & feedback);

private:
  PREVENT_COPY_AND_ASSIGNMENT(CommPlayer);

  void runThread();

  void createUDPSockets();

  static void * startThread(void *ptr);

  const int   teamNumber;
  const int   playerNumber;
  const int   gameControllerPort;
  const int   playerPort;
  const int   maxPacketSize;
  const float socketTimeout;

  Log & log;

  int gameControllerSocket, playerSocket;
  bool threadRunning;
  pthread_t thread;
  pthread_mutex_t dataMutex;

  std::vector<GameControllerMessage const *> newGameControllerMessagesReceived;
  std::vector<PlayerMessage const *>         newPlayerMessagesReceived;

  std::vector<PlayerMessage const *> playerMessagesToSend;

  struct sockaddr_in playerBroadcastAddress;

};

#endif /* COMMPLAYER_H_ */
