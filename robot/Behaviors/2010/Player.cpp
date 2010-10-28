#include "Player.h"
#include "Log/Log.h"
#include "shared/ConfigFile/ConfigFile.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
//#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

namespace RoboCup2010 {

Player::Player(ConfigFile & _configFile, Log & _log, Field & _field)
  : Behaviors(),
    configFile(_configFile),
    log(_log),
    field(_field),
    getUp(_configFile, _log),
    obeyGameManager(_configFile, _log) {
}

Player::~Player() {
}

bool Player::run(BEHAVIOR_PARAMS) {
  // Call your main behavior here

  // ---- Do not add anything below this line ----

  // These MUST be called at the end
  getUp.run(BEHAVIOR_CALL);
  obeyGameManager.run(BEHAVIOR_CALL);
  return false;
}

} // end namespace
