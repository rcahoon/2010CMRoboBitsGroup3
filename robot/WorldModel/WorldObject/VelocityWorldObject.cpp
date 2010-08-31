#include "VelocityWorldObject.h"

// Define the logging constants
#define COMPONENT WORLD_MODEL
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

VelocityWorldObject::VelocityWorldObject(Log & _log,
                                         WorldObject::Type _type)
  : WorldObject(_log, _type),
    velocity(0, 0) {
}

VelocityWorldObject::VelocityWorldObject(const VelocityWorldObject & other)
  : WorldObject(other),
    velocity(other.velocity) {
}

VelocityWorldObject::~VelocityWorldObject() {
}

const Vector2D & VelocityWorldObject::getVelocity() const {
  return velocity;
}

void VelocityWorldObject::setVelocity(const Vector2D & _velocity) {
  velocity = _velocity;
}

void VelocityWorldObject::clear() {
  WorldObject::clear();

  velocity.x = velocity.y = 0;
}

void VelocityWorldObject::copyFrom(const WorldObject & other) {
  WorldObject::copyFrom(other);

  // Try to cast the other object as a world object with velocity
  VelocityWorldObject const *otherVelocity = dynamic_cast<VelocityWorldObject const *>(&other);
  if (otherVelocity != NULL) {
    velocity = otherVelocity->velocity;
  }
  // Default values, but give a warning
  else {
    velocity.x = velocity.y = 0;

    LOG_WARN("Couldn't cast world object as a world object with velocity.");
  }
}

