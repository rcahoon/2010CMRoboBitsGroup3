#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <vector>

#include "shared/UsefulMacros.h"

class GameControllerMessage;
class PlayerMessage;
class RemoteMessageFromRobot;
class RemoteMessageToRobot;

class Messages {
public:
  /**
   * Constructor.
   */
  Messages();

  /**
   * Destructor.
   */
  ~Messages();

  /**
   * Add a Game Controller message that was received. The message is now owned
   * by this class, i.e., it will delete the message as necessary.
   *
   * @param gameControllerMessage message that was received
   */
  void addReceivedMessage(GameControllerMessage const *gameControllerMessage);

  /**
   * Adds a Player message that was received. The message is now owned by this
   * class, i.e., it will delete the message as necessary.
   *
   * @param playerMessage message that was received
   */
  void addReceivedMessage(PlayerMessage const *playerMessage);

  /**
   * Adds a Player message to be transmitted. The message is now owned by this
   * class, i.e., it will delete the message as necessary.
   *
   * @param playerMessage message to be sent
   */
  void addPlayerMessageToSend(PlayerMessage const *playerMessage);

  /**
   * Adds a remote message to be transmitted. The message is now owned by this
   * class, i.e., it will delete the message as necessary.
   *
   * @param message remote message to be transmitted
   */
  void addRemoteMessageFromRobot(RemoteMessageFromRobot const *message);

  /**
   * Adds a remote message that was received. The message is now owned by this
   * class, i.e., it will delete the message as necessary.
   *
   * @param message remote message that was received
   */
  void addRemoteMessageToRobot(RemoteMessageToRobot const *message);

  /**
   * Retrieves the vector of Game Controller messages that were received.
   *
   * @return reference to vector of Game Controller messages
   */
  const std::vector<GameControllerMessage const *> & getReceivedGameControllerMessages() const;

  /**
   * Retrieves the vector of Player messages that were received.
   *
   * @return reference to vector of Player messages
   */
  const std::vector<PlayerMessage const *> & getReceivedPlayerMessages() const;

  /**
   * Retrieves the vector of Player messages to be transmitted.
   *
   * @return reference to vector of Player messages
   */
  const std::vector<PlayerMessage const *> & getPlayerMessagesToSend() const;

  /**
   * Retrieves the vector of remote messages that were received.
   *
   * @return reference to vector of remote messages
   */
  const std::vector<RemoteMessageToRobot const *> & getReceivedRemoteMessages() const;

  /**
   * Retrieves the vector of remote messages to be transmitted.
   *
   * @return reference to vector of remote messages
   */
  const std::vector<RemoteMessageFromRobot const *> & getRemoteMessagesToSend() const;

  /**
   * Clears the list of received messages (Game Controller and Player messages),
   * deleting the contents.
   */
  void clearReceivedMessages();

  /**
   * Clear the list of player messages to send, but DOES NOT delete the contents.
   * This function is intended to be used after the player messages have been
   * owned by another class.
   */
  void clearPlayerMessagesToSend();

  /**
   * Clears the list of messages sent from the remote client
   * (RemoteMessageToRobot), deleting the contents.
   */
  void clearRemoteMessagesToRobot();

  /**
   * Clear the list of messages to send to the remote client
   * (RemoteMessageFromRobot), but DOES NOT delete the contents. This function
   * is intended to be used after the remote messages have been owned by another
   * class.
   */
  void clearRemoteMessagesFromRobot();

private:
  PREVENT_COPY_AND_ASSIGNMENT(Messages);

  std::vector<GameControllerMessage const *> receivedGameControllerMessages;
  std::vector<PlayerMessage const *>         receivedPlayerMessages;
  std::vector<PlayerMessage const *>         playerMessagesToSend;

  std::vector<RemoteMessageToRobot const *>   remoteMessagesToRobot;
  std::vector<RemoteMessageFromRobot const *> remoteMessagesFromRobot;
};

#endif /* MESSAGES_H_ */
