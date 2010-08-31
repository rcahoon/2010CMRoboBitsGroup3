#include "CornerVisionObject.h"

// Define the logging constants
#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

CornerVisionObject::CornerVisionObject(Log & _log,
                                       const CornerType _cornerType,
                                       const float _angle)
  : VisionObject(_log,
                 CornerVisionObject::Corner),
    cornerType(_cornerType),
    angle(_angle) {
}

CornerVisionObject::CornerVisionObject(const CornerVisionObject & other)
  : VisionObject(other),
    cornerType(other.cornerType),
    angle(other.angle) {
}

CornerVisionObject::~CornerVisionObject() {
}

CornerVisionObject::CornerType CornerVisionObject::getCornerType() const {
  return cornerType;
}

void CornerVisionObject::setCornerType(CornerType _cornerType)
{
    cornerType = _cornerType;
}

float CornerVisionObject::getAngle() const {
  return angle;
}

void CornerVisionObject::setAngle( float _angle )
{
    angle = _angle;
}

void CornerVisionObject::clear() {
  VisionObject::clear();

  // We don't clear the type, since that can never be set - not anymore --Cetin
  angle = 0;
  cornerType = Unknown;
}
void CornerVisionObject::copyFrom(const VisionObject & other) {
  VisionObject::copyFrom(other);

  // Try to cast the other object as a corner
  CornerVisionObject const *otherCorner = dynamic_cast<CornerVisionObject const *>(&other);
  if (otherCorner != NULL) {
    cornerType = otherCorner->cornerType;
    angle      = otherCorner->angle;
  }
  // Default values, but give a warning
  else {
    cornerType = CornerVisionObject::Unknown;
    angle      = 0;

    LOG_WARN("Couldn't cast vision object as a corner.");
  }
}
