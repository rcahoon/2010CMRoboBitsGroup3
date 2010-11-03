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
	  log(_log) {
}

Scheduler::~Scheduler() {
}

bool Scheduler::run(BEHAVIOR_PARAMS) {

	//setKickEffect(const Vector2D & kickEffect);
	
	if (pose.getConfidence() < POSE_CONFIDENCE_THRESH)
	{
		LOG_INFO("Running Global Localization");
		gLoc.run(BEHAVIOR_CALL);
	}
	else
	{
		LOG_INFO("Running goal behavior");
		kick2goal.run(BEHAVIOR_CALL);
		
		gLoc.reset();
	}
	
	if (!robotState.isRobotOnGround())
		feedback.setRobotIsLifted();

	return false;
}

} // end namespace

