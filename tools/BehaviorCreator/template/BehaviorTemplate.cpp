#include "$CLASS$.h"
#include "Agent/RobotState.h"
#include "shared/ConfigFile/ConfigFile.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

namespace RoboCup2010 {

$CLASS$::$CLASS$(ConfigFile & configFile, Log & _log)
  : BehaviorBase(),
    log(_log),
    fsm(_log) {
  init(_log.getTimestamp());
}

$CLASS$::~$CLASS$() {
}

void $CLASS$::init(unsigned long timestamp) {
  // Initialize the fsm
  fsm.reset();
  FSM_ADDSTATE(fsm, $FSM_STATE$);
  fsm.finishInit($FSM_INITIAL_STATE$, timestamp);
}

void $CLASS$::reset() {
  fsm.setState($FSM_INITIAL_STATE$, "Reset");
}

bool $CLASS$::run(BEHAVIOR_PARAMS) {

  unsigned long timestamp = robotState.getTimestamp();

  try {
    fsm.startLoop(timestamp);
    while(fsm.isRunning()) {

#BEGIN_REPEAT#	
      if (fsm.inState($FSM_STATE$)) {
        fsm.endLoop();
        continue;
      }

#END_REPEAT#
      else {
        fsm.trans($FSM_INITIAL_STATE$, "Invalid state");
        continue;
      }

    }

  }
  catch (int e) {
    if (fsm.isRunning()) {
      fsm.endLoop();
    }
  }
  fsm.printTransitions();

  return false;
}

} // end namespace
