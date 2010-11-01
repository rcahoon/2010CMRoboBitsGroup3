#include "PlayerMessage.h"
#include "shared/ConfigFile/ConfigFile.h"

PlayerMessage::PlayerMessage(ConfigFile & configFile)
  : teamNumber  (configFile.getInt("team/teamNumber",   -1)),
    playerNumber(configFile.getInt("team/playerNumber", -1)),
    ballSeen(false) {
}


PlayerMessage::PlayerMessage(int _teamNumber, int _playerNumber)
  : teamNumber(_teamNumber),
    playerNumber(_playerNumber),
    ballSeen(false) {
}

PlayerMessage::PlayerMessage()
  : teamNumber(-1),
    playerNumber(-1),
    ballSeen(false) {
}

PlayerMessage::~PlayerMessage() {
}

int PlayerMessage::getSenderTeamNumber() const {
  return teamNumber;
}

int PlayerMessage::getSenderPlayerNumber() const {
  return playerNumber;
}

bool PlayerMessage::isBallSeen() const {
  return ballSeen;
}

void PlayerMessage::setBallSeen(bool _ballSeen) {
  ballSeen = _ballSeen;
}
