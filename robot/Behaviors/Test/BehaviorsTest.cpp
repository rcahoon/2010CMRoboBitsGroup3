/*
 * BehaviourTest.cpp
 *
 *  Created on: Sep 8, 2010
 *      Author: labrot1
 */

#include "BehaviorsTest.h"
#include "Log/Log.h"
#include "shared/ConfigFile/ConfigFile.h"


// Define the logging constants
#define COMPONENT BEHAVIORS
#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

namespace RoboCup2010 {

BehaviorsTest::BehaviorsTest(ConfigFile & _configFile, Log & _log, Field & _field)
  : Behaviors(),
    configFile(_configFile),
    log(_log),
    field(_field) {
}

BehaviorsTest::~BehaviorsTest() {
}

bool BehaviorsTest::run(BEHAVIOR_PARAMS){

    static unsigned long startTime = 0;

    if (robotState.getLeftFootButton()) {
    	startTime = robotState.getTimestamp();
    	command.getMotionCommand().walk(0, 10, 0);
    }
    if (robotState.getTimestamp() - startTime > 5000) {
    	command.getMotionCommand().stopWalk();
    }

	return false;
}

} // end namespace
