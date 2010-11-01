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
	  state(INIT) {
}

GlobalLocalize::~GlobalLocalize() {
}

bool GlobalLocalize::run(BEHAVIOR_PARAMS) {

	feedback.setScanningForGoals(true);
	command.useBottomCamera(false);
	
	command.getMotionCommand().stopWalk();

	LOG_DEBUG("State: %d", state);
	switch(state)
	{
	case INIT:
		/*float angleB = worldFeatures.getFirstWorldObject(WorldObject::BlueGoal)->getLocalPosition().angle();
		float angleY = worldFeatures.getFirstWorldObject(WorldObject::YellowGoal)->getLocalPosition().angle();
		command.getMotionCommand().headAngles(abs_min(angleB, angleY), 0.0f, 0.5f);*/
		headAngle = -1.5f;
		scan_dir = 1;
		state = SCAN;
	break;
	case SCAN:
		if (scan_dir*headAngle > 1.5f) scan_dir *= -1;

		command.getMotionCommand().headAngles(headAngle, 0.2, 0.5f);
		headAngle += scan_dir*SCAN_INCREMENT;
	break;
	}

	return false;
}

void GlobalLocalize::reset() {
	state = INIT;
}

} // end namespace

