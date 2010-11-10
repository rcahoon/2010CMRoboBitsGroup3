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
#include "shared/Shape/Line.h"


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

/*    if (count==0)
    {
    	count = 500;
    }
    count--;
    printf("Count %d\n", count);

    if (robotState.getLeftFootButton()) {
    	command.getSoundCommand().playWavFile("/home/nao/config/fcnt.wav");
    }*/
    
    Vector2D target(-200, 100);
    Vector2D delta = target - pose.getPosition();
    Vector2D r = delta.rotate(-pose.getAngle());
    LOG_DEBUG("%f %f %f", delta.x, delta.y, delta.angle());
    LOG_DEBUG("%f %f %f", r.x, r.y, r.angle());
    LOG_DEBUG("%f %f %f", pose.getPosition().x, pose.getPosition().y, pose.getAngle());
    LOG_SHAPE(Log::Field, Line(pose.getPosition(), pose.getPosition()+r, 0x0000FF, 3));
    command.getMotionCommand().walk(abs_bound(r.x, 5.0f), abs_bound(r.y, 5.0f), abs_bound((-r).angle()/10, 0.5f));

	return false;
}

} // end namespace
