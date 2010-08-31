#include "RobotVisionObject.h"

// Define the logging constants
#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

RobotVisionObject::RobotVisionObject(Log & _log,
                                     const RobotColor _color)
  : VisionObject(_log,
                 VisionObject::Robot),
    color(_color) {
}

RobotVisionObject::RobotVisionObject(const RobotVisionObject & other)
  : VisionObject(other),
    color(other.color){
}

RobotVisionObject::~RobotVisionObject() {
}

RobotVisionObject::RobotColor RobotVisionObject::getRobotColor() const {
  return color;
}

void RobotVisionObject::copyFrom(const VisionObject & other) {
  VisionObject::copyFrom(other);

  // Try to cast the other object as a robot
  RobotVisionObject const *otherRobot = dynamic_cast<RobotVisionObject const *>(&other);
  if (otherRobot != NULL) {
    color = otherRobot->color;
  }
  // Default values, but give a warning
  else {
    color = RobotVisionObject::Unknown;

    LOG_WARN("Couldn't cast vision object as a line.");
  }
}
