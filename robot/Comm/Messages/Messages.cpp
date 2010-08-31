#include "Messages.h"
#include "Comm/Messages/GameController/GameControllerMessage.h"
#include "Comm/Messages/Player/PlayerMessage.h"
#include "Comm/Messages/Remote/RemoteMessageFromRobot.h"
#include "Comm/Messages/Remote/RemoteMessageToRobot.h"

Messages::Messages()
  : receivedGameControllerMessages(),
    receivedPlayerMessages(),
    playerMessagesToSend(),
    remoteMessagesToRobot(),
    remoteMessagesFromRobot() {
}

Messages::~Messages() {
  // Delete all the messages that are still in the lists
  clearReceivedMessages();
  clearRemoteMessagesToRobot();

  std::vector<PlayerMessage const *>::iterator playerIter;
  for (playerIter = playerMessagesToSend.begin();
       playerIter != playerMessagesToSend.end();
       playerIter++) {
    delete (*playerIter);
  }
  playerMessagesToSend.clear();

  std::vector<RemoteMessageFromRobot const *>::iterator remoteIter;
  for (remoteIter = remoteMessagesFromRobot.begin();
       remoteIter != remoteMessagesFromRobot.end();
       remoteIter++) {
    delete (*remoteIter);
  }
  remoteMessagesFromRobot.clear();
}

void Messages::addReceivedMessage(GameControllerMessage const *gameControllerMessage) {
  receivedGameControllerMessages.push_back(gameControllerMessage);
}

void Messages::addReceivedMessage(PlayerMessage const *playerMessage) {
  receivedPlayerMessages.push_back(playerMessage);
}

void Messages::addPlayerMessageToSend(PlayerMessage const *playerMessage) {
  playerMessagesToSend.push_back(playerMessage);
}

void Messages::addRemoteMessageFromRobot(RemoteMessageFromRobot const *message) {
  remoteMessagesFromRobot.push_back(message);
}

void Messages::addRemoteMessageToRobot(RemoteMessageToRobot const *message) {
  remoteMessagesToRobot.push_back(message);
}

const std::vector<GameControllerMessage const *> & Messages::getReceivedGameControllerMessages() const {
  return receivedGameControllerMessages;
}

const std::vector<PlayerMessage const *> & Messages::getReceivedPlayerMessages() const {
  return receivedPlayerMessages;
}

const std::vector<PlayerMessage const *> & Messages::getPlayerMessagesToSend() const {
  return playerMessagesToSend;
}

const std::vector<RemoteMessageToRobot const *> & Messages::getReceivedRemoteMessages() const {
  return remoteMessagesToRobot;
}

const std::vector<RemoteMessageFromRobot const *> & Messages::getRemoteMessagesToSend() const {
  return remoteMessagesFromRobot;
}

void Messages::clearReceivedMessages() {
  // Clear the received vectors, but delete the contents properly first

  std::vector<GameControllerMessage const *>::iterator gameControllerIter;
  for (gameControllerIter = receivedGameControllerMessages.begin();
       gameControllerIter != receivedGameControllerMessages.end();
       gameControllerIter++) {
    delete (*gameControllerIter);
  }
  receivedGameControllerMessages.clear();

  std::vector<PlayerMessage const *>::iterator playerIter;
  for (playerIter = receivedPlayerMessages.begin();
       playerIter != receivedPlayerMessages.end();
       playerIter++) {
    delete (*playerIter);
  }
  receivedPlayerMessages.clear();
}

void Messages::clearPlayerMessagesToSend() {
  // Clear the vector, but don't delete the contents
  playerMessagesToSend.clear();
}

void Messages::clearRemoteMessagesToRobot() {
  // Clear the vector, but delete the contents properly first
  std::vector<RemoteMessageToRobot const *>::iterator remoteIter;
  for (remoteIter = remoteMessagesToRobot.begin();
       remoteIter != remoteMessagesToRobot.end();
       remoteIter++) {
    delete (*remoteIter);
  }
  remoteMessagesToRobot.clear();
}

void Messages::clearRemoteMessagesFromRobot() {
  // Clear the vector, but don't delete the contents
  remoteMessagesFromRobot.clear();
}
