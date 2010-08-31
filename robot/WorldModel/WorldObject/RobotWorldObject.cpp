#include "RobotWorldObject.h"

// Define the logging constants
#define COMPONENT WORLD_MODEL
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

RobotWorldObject::RobotWorldObject(Log & log, WorldObject::Type type, bool teamIsBlue)
  : TeamColorWorldObject(log, type, teamIsBlue),
    penalized(false),
    lifted(false),
    fallen(false) {
}

RobotWorldObject::~RobotWorldObject() {
}

bool RobotWorldObject::isPenalized() const {
  return penalized;
}

bool RobotWorldObject::isLifted() const {
  return lifted;
}

bool RobotWorldObject::isFallen() const {
  return fallen;
}

void RobotWorldObject::setPenalized(bool _penalized) {
  penalized = _penalized;
}

void RobotWorldObject::setLifted(bool _lifted) {
  lifted = _lifted;
}

void RobotWorldObject::setFallen(bool _fallen) {
  fallen = _fallen;
}

void RobotWorldObject::clear() {
  TeamColorWorldObject::clear();

  penalized = false;
  lifted    = false;
  fallen    = false;
}

void RobotWorldObject::copyFrom(const WorldObject & other) {
  TeamColorWorldObject::copyFrom(other);

  // Try to cast the other object as a robot
  RobotWorldObject const *otherRobot = dynamic_cast<RobotWorldObject const *>(&other);
  if (otherRobot != NULL) {
    penalized = otherRobot->penalized;
    lifted    = otherRobot->lifted;
    fallen    = otherRobot->fallen;
  }
  // Default values, but give a warning
  else {
    penalized = false;
    lifted    = false;
    fallen    = false;

    LOG_WARN("Couldn't cast world object as a robot.");
  }
}

