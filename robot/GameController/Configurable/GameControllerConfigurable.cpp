#include "GameControllerConfigurable.h"
#include "GameController/AlwaysPlaying/GameControllerAlwaysPlaying.h"
#include "GameController/2010/GameController2010.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT GAME_CONTROLLER
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

GameControllerConfigurable::GameControllerConfigurable(ConfigFile & configFile, Log & _log)
  : log(_log),
    gameController(NULL) {
  std::string name = configFile.getString("configurable/gameController");

  // Should we use AlwaysPlaying?
  if (name.compare("AlwaysPlaying") == 0) {
    gameController = new GameControllerAlwaysPlaying(configFile, log);
  }
  // Should we use the RoboCup 2010 Game Controller?
  else if (name.compare("2010") == 0) {
    gameController = new GameController2010(configFile, _log);
  }
  else {
    LOG_WARN("Configurable Game Controller was not defined.");
  }
}

GameControllerConfigurable::~GameControllerConfigurable() {
  if (gameController != NULL) {
    delete gameController;
    gameController = NULL;
  }
}

bool GameControllerConfigurable::run(const RobotState & robotState,
                                           Messages & messages,
                                           GameState & gameState) {
  if (gameController != NULL) {
    return gameController->run(robotState, messages, gameState);
  }
  else {
    return false;
  }
}
