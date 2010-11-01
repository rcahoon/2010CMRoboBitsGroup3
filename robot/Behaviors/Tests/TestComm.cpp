#include "TestComm.h"
#include "Comm/Messages/Messages.h"
#include "Comm/Messages/Player/PlayerMessage.h"
#include "shared/ConfigFile/ConfigFile.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

namespace RoboCupTests {

TestComm::TestComm(ConfigFile & _configFile, Log & _log)
  : BehaviorBase(),
    configFile(_configFile),
    log(_log) {
  init(_log.getTimestamp());
}

TestComm::~TestComm() {
}

void TestComm::init(unsigned long timestamp) {
}

void TestComm::reset() {
}

bool TestComm::run(BEHAVIOR_PARAMS) {

  // Create a message
  PlayerMessage *message = new PlayerMessage(configFile);
  message->setBallSeen(true);
  // Add the message into queue
  // Note: ownership is transferred, so don't delete the message!
  messages.addPlayerMessageToSend(message);


  // Check for messages
  const std::vector<PlayerMessage const *> & receivedMessages = messages.getReceivedPlayerMessages();
  std::vector<PlayerMessage const *>::const_iterator iter;
  for (iter = receivedMessages.begin(); iter != receivedMessages.end(); iter++) {
    PlayerMessage const *receivedMessage = *iter;
    LOG_INFO("Received a message from player %d on team %d, ballSeen = %d.",
             receivedMessage->getSenderPlayerNumber(),
             receivedMessage->getSenderTeamNumber(),
             receivedMessage->isBallSeen());
  }
  LOG_DEBUG("End of received messages.")

  return false;
}

} // end namespace

