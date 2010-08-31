#include "GoalPostWorldObject.h"

// Define the logging constants
#define COMPONENT WORLD_MODEL
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

GoalPostWorldObject::GoalPostWorldObject(Log & _log,
                                         bool _teamIsBlue,
                                         GoalPostWorldObject::Side _side)
  : TeamColorWorldObject(_log,
                         _teamIsBlue ? WorldObject::BlueGoalPost : WorldObject::YellowGoalPost,
                         _teamIsBlue),
    side(_side) {
}

GoalPostWorldObject::GoalPostWorldObject(const GoalPostWorldObject & other)
  : TeamColorWorldObject(other),
    side(other.side) {
}

GoalPostWorldObject::~GoalPostWorldObject() {
}

GoalPostWorldObject::Side GoalPostWorldObject::getSide() const {
  return side;
}

void GoalPostWorldObject::setSide(GoalPostWorldObject::Side _side) {
  side = _side;
}

void GoalPostWorldObject::clear() {
  TeamColorWorldObject::clear();

  side = GoalPostWorldObject::Unknown;
}

void GoalPostWorldObject::copyFrom(const WorldObject & other) {
  TeamColorWorldObject::copyFrom(other);

  // Try to cast the other object as a goal post
  GoalPostWorldObject const *otherGoalPost = dynamic_cast<GoalPostWorldObject const *>(&other);
  if (otherGoalPost != NULL) {
    side = otherGoalPost->side;
  }
  // Default values, but give a warning
  else {
    side = GoalPostWorldObject::Unknown;

    LOG_WARN("Couldn't cast world object as a goal post.");
  }
}

