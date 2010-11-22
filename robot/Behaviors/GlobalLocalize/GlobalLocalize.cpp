#include "GlobalLocalize.h"
#include "Agent/RobotState.h"
#include "shared/ConfigFile/ConfigFile.h"
#include <vector>
#include "Vision/VisionObject/VisionObject.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

namespace RoboCup2010 {

GlobalLocalize::GlobalLocalize(ConfigFile & configFile, Log & _log)
	: BehaviorBase(),
	  log(_log),
	  headAngle(0.0f),
	  scan_dir(1.0f) {
}

GlobalLocalize::~GlobalLocalize() {
}

bool GlobalLocalize::run(BEHAVIOR_PARAMS) {

	feedback.setScanningForGoals(true);
	command.useBottomCamera(false);
	
	command.getMotionCommand().stopWalk();
	
	command.getMotionCommand().headAngles(headAngle, 0.2, 0.5f);
	headAngle += scan_dir*SCAN_INCREMENT;
	
	if (scan_dir*headAngle > 1.5f)
	{
		scan_dir *= -1;
		return true;
	}
	else
		return false;
}

} // end namespace

