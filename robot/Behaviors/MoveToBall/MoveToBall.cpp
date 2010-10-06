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
  FSM_ADDSTATE(fsm, Victory);
  fsm.finishInit(Search, timestamp);
}

void MoveToBall::reset() {
  fsm.setState(Search, "Reset");
}
bool lookRight = 0;
int sinceLD = 0;
bool MoveToBall::run(BEHAVIOR_PARAMS) {

  unsigned long timestamp = robotState.getTimestamp();
  VisionObject const *foundBall;
  vector<VisionObject const *> ballVector;
  int imagexCenter = 160;
  int imageyCenter = 120;
  int xCenter = 0;
  int yCenter = 0;
	LOG_ERROR("before try")
  try {
    fsm.startLoop(timestamp);
    while(fsm.isRunning()) {
      ballVector = visionFeatures.getVisionObjects(VisionObject::Ball);

      if (fsm.inState(Search)) {
	command.getLEDCommand().setLeftEye(1);
	command.getLEDCommand().setRightEye(1);
	
	if(sinceLD >= 180){
		command.getMotionCommand().headAngles(0,.5,.7);
		fsm.endLoop();
		sinceLD=0;
		continue;
	}

	else if(sinceLD >= 30){
		command.getMotionCommand().headAngles(0,0,.7);
		fsm.endLoop();
		sinceLD+=1;
		continue;
	}

	if(ballVector.size()==0){
		if(!lookRight){
			LOG_DEBUG("Searching Left");
			command.getMotionCommand().walk(0, 0, .25);
			fsm.endLoop();
			sinceLD+=1;
			continue;
		}
		else{
			LOG_DEBUG("Searching Right");
			command.getMotionCommand().walk(0, 0, -.25);
			fsm.endLoop();
			sinceLD+=1;
			continue;
		}
	}
	LOG_DEBUG("ball is on screen, moving to next state");
	command.getMotionCommand().stopWalk();
	fsm.trans(Center, "toCenter");
        continue;
      }

      else if (fsm.inState(Center)) {
	if(ballVector.size()==0){
		LOG_DEBUG("no ball exists while in center state. go back to search");
		fsm.trans(Search,"search");
		continue;
	}

	foundBall = ballVector[0];
	for(int i=1; i <ballVector.size();i++){
		if(ballVector[i]->getConfidence() > foundBall->getConfidence() ){
			foundBall = ballVector[i];
		}
	}
	command.getLEDCommand().setLeftEye(5);
	command.getLEDCommand().setRightEye(5);
	LOG_DEBUG("Centering");

	xCenter = (((*foundBall).getBoundingBoxX1() + (*foundBall).getBoundingBoxX2())/2);
	
	if(xCenter < (imagexCenter-50)){
		lookRight =false;
		command.getMotionCommand().walk(0,0,.2);
		LOG_DEBUG("we need to turn left");
		fsm.endLoop();
		continue;
	}
	else if(xCenter > (imagexCenter+50)){
		lookRight = true;
		command.getMotionCommand().walk(0,0,-.2);
		LOG_DEBUG("we need to turn right");
		fsm.endLoop();
		continue;
	}
	command.getMotionCommand().stopWalk();
	LOG_DEBUG("Ball is within our 'deadzone', transitioning to approach");
	command.getSoundCommand().say("Centered");
	fsm.trans(Approach, "toApproach");
        continue;
      } 

      else if (fsm.inState(Approach)) {
	
	LOG_DEBUG("Approach");
	if(ballVector.size()==0) {
		LOG_DEBUG("No ball while approaching. go to search.");
		command.getMotionCommand().headAngles(0,0,1);
		command.getSoundCommand().say("Lost the ball");
		fsm.trans(Search,"toSearch");
		continue;
	}
	
	foundBall = ballVector[0];
	for(int i=1;i<ballVector.size();i++){
		if(ballVector[i]->getConfidence() > foundBall->getConfidence() ){
			foundBall = ballVector[i];
		}
	}

	xCenter = (((*foundBall).getBoundingBoxX1() + (*foundBall).getBoundingBoxX2())/2);
	yCenter = (((*foundBall).getBoundingBoxY1() + (*foundBall).getBoundingBoxY2())/2);
	int area = ((*foundBall).getBoundingBoxX2()-(*foundBall).getBoundingBoxX1()) * ((*foundBall).getBoundingBoxY2()-(*foundBall).getBoundingBoxY1());

	if((xCenter < (imagexCenter-50) || (xCenter > (imagexCenter+50)))){
		LOG_DEBUG("We are off X center while approaching. X recenter the ball.");
		//If the ball is far off (small area), transition to center state, otherwise just sidestep.
		if(area < 1500){
			fsm.trans(Center,"center");
			fsm.endLoop();
			continue;
		}
		LOG_DEBUG("We should sidestep here");
		if(xCenter < (imagexCenter-50)){
			command.getMotionCommand().walk(0,4,0);
			lookRight=true;
		}
		else{
			command.getMotionCommand().walk(0,-4,0);
			lookRight=false;
		}
		fsm.endLoop();
		continue;
	}	

	if((yCenter < (imageyCenter-50)) && robotState.getHeadTilt() > -.5){
		LOG_DEBUG("Y RECENTERING Up VIA TILT");
		command.getMotionCommand().headAngles(0,robotState.getHeadTilt()-.1,.7);
		fsm.endLoop();		
		continue;
	}
	else if((yCenter > (imageyCenter+50)) && robotState.getHeadTilt() < .5){
		LOG_DEBUG("Y RECENTERING Down VIA TILT");
		command.getMotionCommand().headAngles(0,robotState.getHeadTilt()+.1,.7);
		fsm.endLoop();
		continue;
	}

	if((yCenter-20) < imageyCenter && robotState.getHeadTilt() > .5){
		LOG_DEBUG("Looking down, and its below the halfway point. victory!");
		command.getSoundCommand().say("Ball Reached");
		command.getLEDCommand().setLeftEye(3);
		command.getLEDCommand().setRightEye(3);
		command.getMotionCommand().stopWalk();
		fsm.trans(Victory,"Victory");
		continue;
	}
	command.getMotionCommand().walk(8,0,0);
        fsm.endLoop();
	continue;
      }

      else if(fsm.inState(Victory)){
	if (ballVector.size()==0){
		LOG_DEBUG("Ball removed while in Victory state. Transition to Search");
		command.getSoundCommand().say("Give it back!");
		command.getMotionCommand().headAngles(0,0,.7);
		fsm.trans(Search,"toSearch");
		fsm.endLoop();
		continue;
	}
	LOG_DEBUG("In Victory state, we still have the ball.");
	command.getMotionCommand().walk(2,0,0);
	foundBall = ballVector[0];
	for(int i=1;i<ballVector.size();i++){
		if(ballVector[i]->getConfidence() > foundBall->getConfidence() ){
			foundBall = ballVector[i];
		}
	}
	xCenter = (((*foundBall).getBoundingBoxX1() + (*foundBall).getBoundingBoxX2())/2);
	
	if(xCenter < (imagexCenter-50)){
		lookRight =false;
		fsm.endLoop();
		continue;
	}
	else if(xCenter > (imagexCenter+50)){
		lookRight = true;
		fsm.endLoop();
		continue;
	}
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
