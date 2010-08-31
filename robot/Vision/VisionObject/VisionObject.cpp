#include "VisionObject.h"

// Define the logging constants
#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

VisionObject::VisionObject(Log & _log,
                           const VisionObject::Type _type)
  : log(_log),
    type(_type),
    position(),
    confidence(0),
    x1(0), y1(0),
    x2(0), y2(0) {
}

VisionObject::VisionObject(const VisionObject & other)
  : log(other.log),
    type(other.type),
    position(other.position),
    confidence(other.confidence),
    x1(other.x1), y1(other.y1),
    x2(other.x2), y2(other.y2) {
}

VisionObject::~VisionObject() {
}

VisionObject::Type VisionObject::getType() const {
  return type;
}

Vector2D const & VisionObject::getPosition() const {
  return position;
}

float VisionObject::getConfidence() const {
  return confidence;
}

void VisionObject::getBoundingBox(unsigned int & _x1, unsigned int & _y1,
                                  unsigned int & _x2, unsigned int & _y2) const {
  _x1 = x1;
  _y1 = y1;
  _x2 = x2;
  _y2 = y2;
}

int VisionObject::getBoundingBoxX1() const {
  return x1;
}

int VisionObject::getBoundingBoxY1() const {
  return y1;
}

int VisionObject::getBoundingBoxX2() const {
  return x2;
}

int VisionObject::getBoundingBoxY2() const {
  return y2;
}

void VisionObject::setPosition(const Vector2D & _position) {
  position = _position;
}

void VisionObject::setConfidence(const float _confidence) {
  confidence = _confidence;
}

void VisionObject::setBoundingBox(const unsigned int _x1, const unsigned int _y1,
                                  const unsigned int _x2, const unsigned int _y2) {
  x1 = _x1;
  y1 = _y1;
  x2 = _x2;
  y2 = _y2;
}

void VisionObject::clear() {
  position.x = position.y = 0;
  confidence = 0;
  x1 = y1 = x2 = y2 = 0;
}

void VisionObject::copyFrom(const VisionObject & other) {
  log        = other.log;
  type       = other.type;
  position   = other.position;
  confidence = other.confidence;
  x1         = other.x1;
  y1         = other.y1;
  x2         = other.x2;
  y2         = other.y2;
}

VisionObject & VisionObject::operator=(const VisionObject & other) {
  if (this != &other) {
    copyFrom(other);
  }

  return (*this);
}
