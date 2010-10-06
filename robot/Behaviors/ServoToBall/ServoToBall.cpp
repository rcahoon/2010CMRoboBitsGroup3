#include "ServoToBall.h"
#include "Agent/RobotState.h"
#include "shared/ConfigFile/ConfigFile.h"
#include <vector>
#include "Vision/VisionObject/VisionObject.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

namespace RoboCup2010 {

ServoToBall::ServoToBall(ConfigFile & configFile, Log & _log)
  : BehaviorBase(),
    searchState(0),
    log(_log) {
}

ServoToBall::~ServoToBall() {
}

#define IMAGE_PAN_SCALE   -0.002f
#define IMAGE_TILT_SCALE  0.002f
#define TURN_PAN_SCALE   ((1.5f - robotState.getHeadTilt())/2.2f)

bool ServoToBall::run(BEHAVIOR_PARAMS) {

  std::vector<VisionObject const *> balls = visionFeatures.getVisionObjects(VisionObject::Ball);
  
  if (balls.empty())
  {
    float angle = robotState.getHeadPan() > 0 ? 1.0f : -1.0f;
    command.getMotionCommand().headAngles(angle/3, 0, 0.5);
    command.getMotionCommand().walk(0,0,angle);
    command.getLEDCommand().setLeftEye(0xff0000);
    command.getLEDCommand().setRightEye(0xff0000);
  }
  else
  {
    std::vector<VisionObject const *>::iterator iter = balls.begin();
    const VisionObject * bestBall = *iter;
    for(; iter != balls.end(); iter++)
    {
      if (bestBall->getConfidence() < (*iter)->getConfidence())
        bestBall = *iter;
    }
    
    int x = (bestBall->getBoundingBoxX1() + bestBall->getBoundingBoxX2()) / 2;
    int y = (bestBall->getBoundingBoxY1() + bestBall->getBoundingBoxY2()) / 2;
    
    command.getMotionCommand().headAngles(robotState.getHeadPan() + (x - 160)*IMAGE_PAN_SCALE, robotState.getHeadTilt() + (y - 120)*IMAGE_TILT_SCALE, 0.5);
    
    LOG_INFO("Head: %f %f", robotState.getHeadPan(), robotState.getHeadTilt());
    
    LOG_DEBUG("Turning: %f\n", TURN_PAN_SCALE*robotState.getHeadPan());
    
    if (fabs(robotState.getHeadPan()) > 0.5)
    {
      command.getMotionCommand().walk(0,0, TURN_PAN_SCALE*robotState.getHeadPan());
      if (robotState.getHeadPan() > 0)
      {
	    command.getLEDCommand().setLeftEye(0x0000ff);
	    command.getLEDCommand().setRightEye(0x000000);
	  }
	  else
	  {
	    command.getLEDCommand().setLeftEye(0x000000);
        command.getLEDCommand().setRightEye(0x0000ff);
      }
    }
    else if (robotState.getHeadTilt() > 0.48f)// && y > 190)
    {
      //command.getMotionCommand().stopWalk();
      command.getLEDCommand().setLeftEye(0x00ff00);
      command.getLEDCommand().setRightEye(0x00ff00);
    }
    else
    {
      command.getMotionCommand().walk(7,0,TURN_PAN_SCALE*robotState.getHeadPan());
      command.getLEDCommand().setLeftEye(0xffff00);
      command.getLEDCommand().setRightEye(0xffff00);
    }
  }

  return false;
}

} // end namespace

