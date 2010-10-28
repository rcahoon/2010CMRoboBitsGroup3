#include <math.h>

#include "GetUp.h"
#include "Agent/RobotState.h"
#include "Agent/Command/Command.h"
#include "Agent/Command/MotionCommand/MotionCommand.h"
#include "Agent/Command/MotionCommand/BodyCommand/StaticAction.h"
#include "Agent/Feedback/Feedback.h"
#include "GameController/GameState.h"
#include "shared/ConfigFile/ConfigFile.h"


// Define the logging constants
#define COMPONENT BEHAVIORS
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

namespace RoboCup2010 {

GetUp::GetUp(ConfigFile & configFile, Log & _log)
  : BehaviorBase(),
    FALL_TIME(configFile.getMillisecondsFromSeconds("behaviors/2010/tactics/getUp/fallTime", 0.5)),
    LIFT_TIME(configFile.getMillisecondsFromSeconds("behaviors/2010/tactics/getUp/liftTime", 1)),
    FACING_UP_BODY_ANGLE_Y(configFile.getFloat("behaviors/2010/tactics/getUp/facingUpBodyAngleY", 0)),
    log(_log),
    fsm(_log),
    liftedTimestamp(0),
    fallTimeStamp(0),
    fromFront(true),
    playedKeyframe(false) {
  init(_log.getTimestamp());
}

GetUp::~GetUp() {
}

void GetUp::init(unsigned long timestamp) {
  // Initialize the fsm
  fsm.reset();
  FSM_ADDSTATE(fsm, none);
  FSM_ADDSTATE(fsm, lifted);
  FSM_ADDSTATE(fsm, spreadOut);
  FSM_ADDSTATE(fsm, getUp);
  fsm.finishInit(none, timestamp);
}

bool GetUp::isMoving(const RobotState & robotState) const {

  if (  (robotState.getStaticActionType() == StaticAction::actNone)
     && (robotState.getBodyCommandType() == bcNoBodyCommand) ){
    return false;
  }
  return true;
}

bool GetUp::hasFallen(const float bodyAngleX, const float bodyAngleY) const {
  if ((bodyAngleY < -1) || (bodyAngleY > 1) || (bodyAngleX < -1) || (bodyAngleX > 1)) {
    return true;
  }
  return false;
}

bool GetUp::run(BEHAVIOR_PARAMS) {

  if (gameState.amIPenalized()) {
    return true;
  }

  float bodyAngleX = robotState.getBodyAngleX();
  float bodyAngleY = robotState.getBodyAngleY();


  LOG_DEBUG("Body angles x-y: %.1f %.1f",
            robotState->getBodyAngleX(), robotState->getBodyAngleY())

  unsigned long timestamp = robotState.getTimestamp();

  try {
    fsm.startLoop(timestamp);
    while(fsm.isRunning()) {

      if (fsm.inState(none)) {
        if (fsm.isNewState()) {
          liftedTimestamp = timestamp;
          fallTimeStamp = timestamp;
        }

        // Check if robot is on ground or if it's playing a keyframe
        if ( (robotState.isRobotOnGround()) || ((robotState.getStaticActionType() != StaticAction::actNone)
            && (robotState.getBodyCommandType() == bcStaticAction)) ){
          liftedTimestamp = timestamp;
        }
        // Check if we have been lifted
        else if (timestamp - liftedTimestamp > LIFT_TIME) {
          fsm.trans(lifted, "Robot is lifted");
          continue;
        }

        if (!hasFallen(bodyAngleX, bodyAngleY)) {
          fallTimeStamp = timestamp;
        }
        // Check if we have fallen
        else if (timestamp - fallTimeStamp > FALL_TIME) {
          fsm.trans(spreadOut, "Robot has fallen");
          continue;
        }

        fsm.endLoop();
        continue;
      }

      else if (fsm.inState(lifted)) {
        if (fsm.isNewState()) {
          command.getSoundCommand().say("Lifted");
          liftedTimestamp = timestamp;
        }

        if (!robotState.isRobotOnGround()) {
          liftedTimestamp = timestamp;
        }
        else if (timestamp - liftedTimestamp > LIFT_TIME) {
          fsm.trans(none, "Robot is on the ground.");
          continue;
        }

        if (!hasFallen(bodyAngleX, bodyAngleY)) {
          fallTimeStamp = timestamp;
        }
        // Check if we have fallen
        else if (timestamp - fallTimeStamp > FALL_TIME) {
          fsm.trans(spreadOut, "Robot has fallen");
          continue;
        }

        feedback.setRobotIsLifted();
        command.getMotionCommand().stopWalk();
        command.getMotionCommand().noHeadCommand();

        fsm.endLoop();
        continue;
      }

      else if (fsm.inState(spreadOut)) {

        if (fsm.isNewState()) {
          // If we are in initial or set state, don't start the getup
          if ((gameState.getStateOfGame() == GameState::initial)
           || (gameState.getStateOfGame() == GameState::set)) {
            fsm.trans(none, "Don't getup when the game state is initial or set.");
            continue;
          }
          //command.getSoundCommand().say("I am going to spread out");
          playedKeyframe = false;
        }

        bool playingKeyframe = (robotState.getBodyCommandType() == bcStaticAction)
                            && (robotState.getStaticActionType() == StaticAction::actSpreadOut);
        if (playingKeyframe && !fsm.isNewState()) {
          playedKeyframe = true;
        }

        // Check if we have already spread out
        if (playedKeyframe && !playingKeyframe) {
          fsm.trans(getUp, "Finished spreading out");
          continue;
        }

        if (!playedKeyframe) {
          command.getMotionCommand().staticAction(StaticAction::actSpreadOut);
        }
        else {
          command.getMotionCommand().noBodyCommand();
        }
        command.getMotionCommand().noHeadCommand();

        feedback.setRobotIsGettingUp();

        fsm.endLoop();
        continue;
      }

      else if (fsm.inState(getUp)) {

        if (fsm.isNewState()) {
//          command.getSoundCommand().say("I am going to spread out");
          playedKeyframe = false;

          // Is the robot on its back or front?
          if (bodyAngleY > FACING_UP_BODY_ANGLE_Y) {
            fromFront = true;
            command.getSoundCommand().say("I'm getting up from my belly.");
          }
          else {
            fromFront = false;
            command.getSoundCommand().say("I'm getting up from my back.");
          }
        }

        StaticAction::StaticActionType keyframeType = (fromFront ? StaticAction::actGetUpFromFront : StaticAction::actGetUpFromBack);
        bool playingKeyframe = (robotState.getBodyCommandType() == bcStaticAction)
                            && (robotState.getStaticActionType() == keyframeType);
        if (playingKeyframe && !fsm.isNewState()) {
          playedKeyframe = true;
        }

        // Check if we have already spread out
        if (playedKeyframe && !playingKeyframe) {
          fsm.trans(none, "Robot has finished playing get-up keyframe");
          continue;
        }

        if (!playedKeyframe) {
          command.getMotionCommand().staticAction(keyframeType);
        }
        else {
          command.getMotionCommand().noBodyCommand();
        }
        command.getMotionCommand().noHeadCommand();

        feedback.setRobotIsGettingUp();

        fsm.endLoop();
        continue;
      }

      else {
        fsm.trans(none, "Invalid state");
        continue;
      }

    }
  }
  catch (int e){
    if (fsm.isRunning()) {
      fsm.endLoop();
    }
  }
  fsm.printTransitions();

  return false;
}

}//end namespace
