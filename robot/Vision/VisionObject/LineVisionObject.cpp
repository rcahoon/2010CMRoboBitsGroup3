#include "LineVisionObject.h"

// Define the logging constants
#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"


LineVisionObject::LineVisionObject(Log & _log,
                                   const Vector2D & _endPoint1,
                                   const Vector2D & _endPoint2,
                                   const Vector2D & _cameraEndPoint1,
                                   const Vector2D & _cameraEndPoint2)
  : VisionObject(_log,
                 VisionObject::Line),
    endPoint1(_endPoint1),
    endPoint2(_endPoint2),
    cameraEndPoint1(_cameraEndPoint1),
    cameraEndPoint2(_cameraEndPoint2) {
  // We set the position to be the middle of the line
  setPosition((endPoint1 + endPoint2) / 2);
}

LineVisionObject::LineVisionObject(const LineVisionObject & other)
  : VisionObject(other),
    endPoint1(other.endPoint1),
    endPoint2(other.endPoint2),
    cameraEndPoint1(other.cameraEndPoint1),
    cameraEndPoint2(other.cameraEndPoint2) {
}

LineVisionObject::~LineVisionObject() {
}

const Vector2D & LineVisionObject::getEndPoint1() const {
  return endPoint1;
}

void LineVisionObject::setEndPoint1(Vector2D _endPoint1 ){
    endPoint1 = _endPoint1;
}

const Vector2D & LineVisionObject::getEndPoint2() const {
  return endPoint2;
}

void LineVisionObject::setEndPoint2(Vector2D _endPoint2 ){
    endPoint2 = _endPoint2;
}

const Vector2D & LineVisionObject::getCameraEndPoint1() const {
  return cameraEndPoint1;
}

const Vector2D & LineVisionObject::getCameraEndPoint2() const {
  return cameraEndPoint2;
}

void LineVisionObject::clear() {
  VisionObject::clear();

  endPoint1.x = endPoint1.y = endPoint2.x = endPoint2.y = 0;
  cameraEndPoint1.x = cameraEndPoint1.y = cameraEndPoint2.x = cameraEndPoint2.y = 0;
}

void LineVisionObject::copyFrom(const VisionObject & other) {
  VisionObject::copyFrom(other);

  // Try to cast the other object as a line
  LineVisionObject const *otherLine = dynamic_cast<LineVisionObject const *>(&other);
  if (otherLine != NULL) {
    endPoint1 = otherLine->endPoint1;
    endPoint2 = otherLine->endPoint2;
    cameraEndPoint1 = otherLine->cameraEndPoint1;
    cameraEndPoint2 = otherLine->cameraEndPoint2;
  }
  // Default values, but give a warning
  else {
    endPoint1.x = endPoint1.y = endPoint2.x = endPoint2.y = 0;
    cameraEndPoint1.x = cameraEndPoint1.y = cameraEndPoint2.x = cameraEndPoint2.y = 0;

    LOG_WARN("Couldn't cast vision object as a line.");
  }
}
