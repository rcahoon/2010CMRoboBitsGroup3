#include "Scheduler.h"
#include "Agent/RobotState.h"
#include "shared/ConfigFile/ConfigFile.h"
#include <vector>
#include "Vision/VisionObject/VisionObject.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

namespace RoboCup2010 {

Scheduler::Scheduler(ConfigFile & configFile, Log & _log)
	: BehaviorBase(),
	  gLoc(configFile, _log),
	  kick2goal(configFile, _log),
	  state(LOCALIZE),
	  log(_log) {
}

Scheduler::~Scheduler() {
}

bool Scheduler::run(BEHAVIOR_PARAMS) {
	bool ret = false;
	switch(state)
	{
	case LOCALIZE:
		LOG_INFO("Running Global Localization");
		ret = gLoc.run(BEHAVIOR_CALL);
	break;
	case ATTACK:
		LOG_INFO("Running goal behavior");
		ret = kick2goal.run(BEHAVIOR_CALL);
	break;
	}
	
	if (ret)
	{
		state = (pose.getCovariance().trace()/3 > POSE_COVARIANCE_THRESH) ? LOCALIZE : ATTACK;
	}

	//setKickEffect(const Vector2D & kickEffect);
	
	if (!robotState.isRobotOnGround())
		feedback.setRobotIsLifted();

	return false;
}

} // end namespace

