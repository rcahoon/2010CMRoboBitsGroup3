#include "GoalWorldObject.h"
#include "shared/ConfigFile/ConfigFile.h"
#include "shared/Field/Field.h"

// Define the logging constants
#define COMPONENT WORLD_MODEL
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

GoalWorldObject::GoalWorldObject(ConfigFile & configFile,
                                 Log & _log,
                                 float _localAngle,
                                 bool _teamIsBlue)
  : TeamColorWorldObject(_log,
                         _teamIsBlue ? WorldObject::BlueGoal : WorldObject::YellowGoal,
                         _teamIsBlue),
    HALF_GOAL_WIDTH(configFile.getFloat("field/goalWidth", 140) / 2),
    log(_log),
    localAngle(_localAngle),
    leftGoalPostLocal(0, 0),
    rightGoalPostLocal(0, 0) {
  calculateGoalPosts();
}

GoalWorldObject::~GoalWorldObject() {
}

float GoalWorldObject::getLocalAngle() const {
  return localAngle;
}

const Vector2D & GoalWorldObject::getLeftGoalPostLocal() const {
  return leftGoalPostLocal;
}

const Vector2D & GoalWorldObject::getRightGoalPostLocal() const {
  return rightGoalPostLocal;
}

void GoalWorldObject::clear() {
  TeamColorWorldObject::clear();

  localAngle = 0;
  calculateGoalPosts();
}

void GoalWorldObject::copyFrom(const WorldObject & other) {
  TeamColorWorldObject::copyFrom(other);

  // Try to cast the other object as a goal
  GoalWorldObject const *otherGoal = dynamic_cast<GoalWorldObject const *>(&other);
  if (otherGoal != NULL) {
    localAngle = otherGoal->localAngle;
    leftGoalPostLocal  = otherGoal->leftGoalPostLocal;
    rightGoalPostLocal = otherGoal->rightGoalPostLocal;
  }
  // Default values, but give a warning
  else {
    localAngle = 0;
    calculateGoalPosts();

    LOG_WARN("Couldn't cast world object as a goal.");
  }
}

void GoalWorldObject::setLocalAngle(float _localAngle) {
  localAngle = _localAngle;
  calculateGoalPosts();
}

void GoalWorldObject::calculateGoalPosts() {
  leftGoalPostLocal.heading(localAngle + M_PI / 2);
  leftGoalPostLocal *= HALF_GOAL_WIDTH;
  leftGoalPostLocal += getLocalPosition();

  rightGoalPostLocal.heading(localAngle - M_PI / 2);
  rightGoalPostLocal *= HALF_GOAL_WIDTH;
  rightGoalPostLocal += getLocalPosition();
}
