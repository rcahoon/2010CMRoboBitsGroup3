#include "CircleVisionObject.h"

// Define the logging constants
#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

CircleVisionObject::CircleVisionObject(Log & _log)
  : VisionObject(_log,
                 CircleVisionObject::Circle)
{
}

CircleVisionObject::CircleVisionObject(const CircleVisionObject & other)
  : VisionObject(other)
{
}

CircleVisionObject::~CircleVisionObject() {
}

void CircleVisionObject::copyFrom(const VisionObject & other) {
  VisionObject::copyFrom(other);
}
