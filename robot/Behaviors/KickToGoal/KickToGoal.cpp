#include "KickToGoal.h"
#include "Agent/RobotState.h"
#include "shared/ConfigFile/ConfigFile.h"
#include <vector>
#include "Vision/VisionObject/VisionObject.h"
#include "Agent/Command/MotionCommand/BodyCommand/StaticAction.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

namespace RoboCup2010 {

KickToGoal::KickToGoal(ConfigFile & configFile, Log & _log)
  : BehaviorBase(),
    state(TO_BALL),
    log(_log) {
}

KickToGoal::~KickToGoal() {
}

#define IMAGE_PAN_SCALE   -0.002f
#define IMAGE_TILT_SCALE  0.002f
#define TURN_PAN_SCALE   ((1.5f - robotState.getHeadTilt())/2.2f)

bool KickToGoal::run(BEHAVIOR_PARAMS) {

	static int count = 0;

	while(true)
	{
		LOG_INFO("State: %d", state);
		float goalAngle, ballAngle;
		switch(state)
		{
		case TO_BALL:
			if (servoToBall(BEHAVIOR_CALL)) state = CENTER_GOAL;
			else return false;
		break;
		case CHECK_GOAL:
			goalAngle = worldFeatures.getFirstWorldObject(WorldObject::BlueGoal)->getLocalPosition().angle();
			if (fabs(goalAngle) < 0.2) state = KICK;
			else state = CENTER_GOAL;
		break;
		case CENTER_GOAL:
			goalAngle = worldFeatures.getFirstWorldObject(WorldObject::BlueGoal)->getLocalPosition().angle();
			if (goalAngle > 0.0f)
				command.getMotionCommand().walk(0, -3, 0.3);
			else
				command.getMotionCommand().walk(0, 3, -0.3);
			if (fabs(goalAngle) < 0.2) state = TO_BALL;
			else return false;
		break;
		case KICK:
			command.getMotionCommand().stopWalk();
			ballAngle = worldFeatures.getFirstWorldObject(WorldObject::Ball)->getLocalPosition().angle();
			if (ballAngle > 0.0f)
				command.getMotionCommand().staticAction(StaticAction::actForwardKickLeft);
			else
				command.getMotionCommand().staticAction(StaticAction::actForwardKickRight);
			state = WAIT;
			count = 120;
			return false;
		break;
		case WAIT:
			if (!--count) state = TO_BALL;
		break;
		}
	}
	
	return false;
}

static float commandPan = 0.0f;
static float commandTilt = 0.0f;

static void headAngles(Command& command, float pan, float tilt)
{
	commandPan = pan;
	commandTilt = tilt;
	command.getMotionCommand().headAngles(pan, tilt, 0.5);
}

bool KickToGoal::servoToBall(BEHAVIOR_PARAMS) {

  std::vector<VisionObject const *> balls = visionFeatures.getVisionObjects(VisionObject::Ball);
  
  command.useBottomCamera(true);
  
  if (fabs(robotState.getHeadPan() - commandPan) + fabs(robotState.getHeadTilt() - commandTilt) > 0.1)
  {
    headAngles(command, commandPan, commandTilt);
    return false;
  }
  
  if (balls.empty())
  {
    float angle = robotState.getHeadPan() > 0 ? 1.0f : -1.0f;
    headAngles(command, angle/3, 0);
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
    
    headAngles(command, robotState.getHeadPan() + (x - 160)*IMAGE_PAN_SCALE, robotState.getHeadTilt() + (y - 120)*IMAGE_TILT_SCALE);
    
    LOG_INFO("Head: %f %f", robotState.getHeadPan(), robotState.getHeadTilt());
    
    LOG_DEBUG("Turning: %f\n", TURN_PAN_SCALE*robotState.getHeadPan());
    
    if (fabs(robotState.getHeadPan()) > 0.5)
    {
      command.getMotionCommand().walk(0,10*TURN_PAN_SCALE*robotState.getHeadPan(), 0);
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
      command.getMotionCommand().stopWalk();
      command.getLEDCommand().setLeftEye(0x00ff00);
      command.getLEDCommand().setRightEye(0x00ff00);
      return true;
    }
    else
    {
      command.getMotionCommand().walk(7,3*TURN_PAN_SCALE*robotState.getHeadPan(), 0);
      command.getLEDCommand().setLeftEye(0xffff00);
      command.getLEDCommand().setRightEye(0xffff00);
    }
  }

  return false;
}

} // end namespace

