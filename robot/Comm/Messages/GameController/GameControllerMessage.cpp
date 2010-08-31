#include <string.h>

#include "GameControllerMessage.h"
#include "GameController/GameState.h"

GameControllerMessage::GameControllerMessage() {
  memset(&data, 0, sizeof(struct RoboCupGameControlData));
}

GameControllerMessage::GameControllerMessage(const struct RoboCupGameControlData & _data) {
  data = _data;
}

GameControllerMessage::~GameControllerMessage() {
}

bool GameControllerMessage::isValid(int teamNumber) const {
  if (strncmp(data.header, GAMECONTROLLER_STRUCT_HEADER, 4) != 0) {
    return false;
  }

  bool foundOurTeam = false;
  for (int i = 0; i < 2; i++) {
    TeamInfo team = data.teams[i];
    if (team.teamNumber == teamNumber) {
      foundOurTeam = true;
      break;
    }
  }

  return foundOurTeam;
}

struct RoboCupGameControlData const & GameControllerMessage::getData() const {
  return data;
}
