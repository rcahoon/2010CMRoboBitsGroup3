#include "GameControllerAlwaysPlaying.h"
#include "Agent/RobotState.h"
#include "Comm/Messages/Messages.h"
#include "GameController/GameState.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT GAME_CONTROLLER
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

GameControllerAlwaysPlaying::GameControllerAlwaysPlaying(ConfigFile & configFile, Log & _log)
 : isBlueTeam(configFile.getBool("gameController/isBlueTeam", 1)),
   log(_log)
{
}

GameControllerAlwaysPlaying::~GameControllerAlwaysPlaying() {
}

bool GameControllerAlwaysPlaying::run(const RobotState & robotState,
                                      Messages & messages,
                                      GameState & gameState) {
  // The game state is always playing, and the robot is never penalized,
  // and we are always on the blue team
  gameState.setStateOfGame(GameState::playing);
  gameState.setPenalized(false);
  gameState.setTeamColor(isBlueTeam);

  // Log the team color
  LOG_TEAM_COLOR(isBlueTeam);
  return false;
}
