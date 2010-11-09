/*
 * BehaviourTest.cpp
 *
 *  Created on: Sep 8, 2010
 *      Author: labrot1
 */

#include "BehaviorsTest.h"
#include "Log/Log.h"
#include "shared/ConfigFile/ConfigFile.h"
#include "shared/random.h"


// Define the logging constants
#define COMPONENT BEHAVIORS
#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

namespace RoboCup2010 {

BehaviorsTest::BehaviorsTest(ConfigFile & _configFile, Log & _log)
  : Behaviors(),
    configFile(_configFile),
    log(_log),
    count(500) {
}

BehaviorsTest::~BehaviorsTest() {
}

bool BehaviorsTest::run(BEHAVIOR_PARAMS){

command.getMotionCommand().walk(4, 0, 0.1);
/*    if (count==0)
    {
    	count = 500;
    }
    count--;
    printf("Count %d\n", count);

    if (robotState.getLeftFootButton()) {
    	command.getSoundCommand().playWavFile("/home/nao/config/fcnt.wav");
    }*/

	return false;
}

} // end namespace
