#include "MoveToBall.h"
#include "Agent/RobotState.h"
#include "shared/ConfigFile/ConfigFile.h"
#include <vector>

// Define the logging constants
#define COMPONENT BEHAVIORS
#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

namespace RoboCup2010 {

MoveToBall::MoveToBall(ConfigFile & configFile, Log & _log)
  : BehaviorBase(),
    log(_log),
    fsm(_log) {
  init(_log.getTimestamp());
}

MoveToBall::~MoveToBall() {
}

void MoveToBall::init(unsigned long timestamp) {
  // Initialize the fsm
  fsm.reset();
  FSM_ADDSTATE(fsm, Search);
  FSM_ADDSTATE(fsm, Center);
  FSM_ADDSTATE(fsm, Approach);
  fsm.finishInit(Search, timestamp);
}

void MoveToBall::reset() {
  fsm.setState(Search, "Reset");
}

bool MoveToBall::run(BEHAVIOR_PARAMS) {

  unsigned long timestamp = robotState.getTimestamp();
  VisionObject const *foundBall;
  vector<VisionObject const *> ballVector;
  int imagexCenter = 160;
  int xCenter = 0;

  try {
    fsm.startLoop(timestamp);
    while(fsm.isRunning()) {
      ballVector = visionFeatures.getVisionObjects(VisionObject::Ball);
      if (fsm.inState(Search)) {
	if(ballVector.size()==0){
		LOG_DEBUG("Finding");
		//We can't see the ball, keep searching. Rotate the robot right	for
		//a new image
		command.getMotionCommand().walk(0, 0, .25);
		fsm.endLoop();
		continue;
	}
	LOG_DEBUG("ball is on screen, moving to next state");
	fsm.trans(Center, "toCenter");
        continue;
      }

      else if (fsm.inState(Center)) {
	if(ballVector.size()==0){
		LOG_DEBUG("no ball exists while in center state. go back to find");
		fsm.trans(Search,"search");
		continue;
	}
	foundBall = ballVector[0];
	LOG_DEBUG("Centering");
	xCenter = (((*foundBall).getBoundingBoxX1() + (*foundBall).getBoundingBoxX2())/2);
	if(xCenter < (imagexCenter-50)){
		command.getMotionCommand().walk(0,0,.15);
		LOG_DEBUG("we need to turn left");
		fsm.endLoop();
		continue;
	}
	else if(xCenter > (imagexCenter+50)){
		command.getMotionCommand().walk(0,0,-.15);
		LOG_DEBUG("we need to turn right");
		fsm.endLoop();
		continue;
	}
	//The ball is within the 'acceptable zone' of +/- 10 of the image center.
	LOG_DEBUG("Ball is within our 'deadzone', transitioning to approach");
	fsm.trans(Approach, "toApproach");
        continue;
      } 

      else if (fsm.inState(Approach)) {
	
	//Simply move forward while the ball is still on the screen. 
	//We will need to know when the ball will fall
	//off the bottom of the screen due to getting too close &
	//switch to chest camera.
	LOG_DEBUG("Approach");
	if(ballVector.size()==0){
		LOG_DEBUG("no ball exists while in approach state. go back to find");
		fsm.trans(Search,"search");
		continue;
	}
	foundBall = ballVector[0];
	xCenter = (((*foundBall).getBoundingBoxX1() + (*foundBall).getBoundingBoxX2())/2);
	if((xCenter < (imagexCenter-50) || (xCenter > (imagexCenter+50)))){
		LOG_DEBUG("We are off center while approaching. recenter the ball.");
		fsm.trans(Center,"center");
		continue;
	}	
	LOG_DEBUG("We're apparently in the center still, ball xCenter at %i",xCenter);
	command.getMotionCommand().walk(6,0,0);
        fsm.endLoop();
	continue;
      }

      else {
        fsm.trans(Search, "Invalid state");
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
