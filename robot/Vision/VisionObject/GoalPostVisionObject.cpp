#include "GoalPostVisionObject.h"

// Define the logging constants
#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

GoalPostVisionObject::GoalPostVisionObject(Log & _log,
                                           const bool _isBlue,
                                           const Side _side)
  : VisionObject(_log,
                 _isBlue ? VisionObject::BlueGoalPost : VisionObject::YellowGoalPost),
    isBlue(_isBlue),
    side(_side) {
}

GoalPostVisionObject::GoalPostVisionObject(const GoalPostVisionObject & other)
  : VisionObject(other),
    isBlue(other.isBlue),
    side(other.side) {
}

GoalPostVisionObject::~GoalPostVisionObject() {
}

GoalPostVisionObject::Side GoalPostVisionObject::getSide() const {
  return side;
}

bool GoalPostVisionObject::isLeftPost() const {
  return (side == GoalPostVisionObject::Left);
}

bool GoalPostVisionObject::isRightPost() const {
  return (side == GoalPostVisionObject::Right);
}

bool GoalPostVisionObject::isUnknownPost() const {
  return (side == GoalPostVisionObject::Unknown);
}

bool GoalPostVisionObject::isBluePost() const {
  return isBlue;
}

void GoalPostVisionObject::setSide(GoalPostVisionObject::Side _side) {
  side = _side;
}

void GoalPostVisionObject::clear() {
  VisionObject::clear();

  // We don't clear the color, since that can never be set
  side   = GoalPostVisionObject::Unknown;
}

void GoalPostVisionObject::copyFrom(const VisionObject & other) {
  VisionObject::copyFrom(other);

  // Try to cast the other object as a goal post
  GoalPostVisionObject const *otherGoalPost = dynamic_cast<GoalPostVisionObject const *>(&other);
  if (otherGoalPost != NULL) {
    isBlue = otherGoalPost->isBlue;
    side   = otherGoalPost->side;
  }
  // Default values, but give a warning
  else {
    isBlue = true;
    side   = GoalPostVisionObject::Unknown;

    LOG_WARN("Couldn't cast vision object as a goal post.");
  }
}
