#include "Feedback.h"
#include "Factory/Factory.h"
#include "Vision/Vision.h"
#include "WorldModel/WorldModel.h"

#define COMPONENT AGENT
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"
#include <iostream>

Feedback::Feedback(Factory & factory)
  : log(factory.getLog()),
    worldModel(factory.getWorldModel()),
    localization(factory.getLocalization()),
    vision(factory.getVision()),
    connectedToClient(Unknown),
    scanGoalStatus(Unknown),
    scanObstaclesStatus(Unknown),
    gettingUp(false),
    lifted(false),
    doResetLocalization(false),
    resetLocalizationCase(Localization::none),
    kickEffect(0, 0),
    haveKickEffect(false),
    ballInvalidated(false),
    useGoalieLocalization(Unknown),
    disableBallDetection(false) {

}

Feedback::~Feedback() {
}

void Feedback::clear() {
  connectedToClient         = Unknown;
  scanGoalStatus            = Unknown;
  scanObstaclesStatus       = Unknown;
  clearFreeSpaceModelStatus = Unknown;
  gettingUp = false;
  lifted    = false;
  haveKickEffect = false;
  doResetLocalization = false;
  resetLocalizationCase = Localization::none;
  ballInvalidated = false;
  disableBallDetection = false;
}

bool Feedback::run() {

  // Do we have an update on connection to remote client?
  if (connectedToClient != Unknown) {
    log.setConnectedToClient(connectedToClient == True);
  }

  // Do we know if we're scanning for goals?
  if (scanGoalStatus != Unknown) {
    // Tell World Model
    worldModel.setScanningForGoals(scanGoalStatus == True);
    // Tell localization
    localization.setScanningForGoals(scanGoalStatus == True);
  }

  // Do we know if we're scanning for obstacles?
  if (scanObstaclesStatus != Unknown) {
    // Tell World Model
    worldModel.setScanningForObstacles(scanObstaclesStatus == True);
  }

  // Do we know if we're supposed to clear the free space model?
  if (clearFreeSpaceModelStatus != Unknown) {
    // Tell World Model
    worldModel.setClearFreeSpaceModel(clearFreeSpaceModelStatus == True);
  }
  
  // Tell the world model whether we are getting up, or lifted
  worldModel.setRobotConditions(gettingUp, lifted);

  // Do we have a kick effect?
  if (haveKickEffect) {
    worldModel.addKickEffect(kickEffect);
    haveKickEffect = false;
  }

  if (doResetLocalization) {
    localization.reset(resetLocalizationCase);
  }

  // Should we invalidate the ball?
  if (ballInvalidated) {
    worldModel.invalidateBall();
  }

  if (useGoalieLocalization != Unknown) {
    localization.setUseGoalieLocalization(useGoalieLocalization == True);
  }

  // Should we disable ball detection?
  if (disableBallDetection) {
    LOG_INFO("Disabling ball detection");
    vision.disableBallDetectionNextFrame();
  }
  return false;
}

void Feedback::setConnectedToClient(bool _connectedToClient) {
  connectedToClient = _connectedToClient ? True : False;
}

void Feedback::setScanningForGoals(bool scanningForGoals) {
  scanGoalStatus = scanningForGoals ? True : False;
}

void Feedback::setScanningForObstacles(bool scanningForObstacles) {
  scanObstaclesStatus = scanningForObstacles ? True : False;
}

void Feedback::setClearFreeSpaceModel(bool clearFreeSpaceModel) {
  clearFreeSpaceModelStatus = clearFreeSpaceModel ? True : False;
}

void Feedback::setRobotIsGettingUp() {
  gettingUp = true;
  resetLocalizationCase = Localization::fall;
  doResetLocalization = true;
}

void Feedback::setRobotIsLifted() {
  lifted = true;
  resetLocalizationCase = Localization::lifted;
  doResetLocalization = true;
}

void Feedback::setKickEffect(const Vector2D & _kickEffect) {
  haveKickEffect = true;
  kickEffect = _kickEffect;
}

void Feedback::resetFromPenalty(bool isOpponentGoalOnTheLeftSide) {

}

void Feedback::resetLocalization(Localization::ResetCase resetCase) {
  resetLocalizationCase = resetCase;
  doResetLocalization = true;
}

void Feedback::invalidateBall() {
  ballInvalidated = true;
}

void Feedback::setGoalieLocalization(bool _useGoalieLocalization) {
  useGoalieLocalization = _useGoalieLocalization ? True : False;
}

void Feedback::disableBallDetectionForNextFrame() {
  disableBallDetection = true;
}
