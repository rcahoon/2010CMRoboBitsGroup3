#include "FieldObject.h"

FieldObject::FieldObject(const Vector2D & position, float radius,
                         int fillColor,
                         int strokeColor, float strokeThickness)
  : Circle(position, radius,
           fillColor,
           strokeColor, strokeThickness),
    _havePosition(false),
    _haveAngle(false),
    positionColor(0x00000000),
    angleColor(0x00000000),
    positionDeviation(0),
    angleDeviation(0),
    angle(0) {
  // We need to set type here, since FieldObject is a subclass of Circle
  setType(Shape::FieldObject);
}

FieldObject::FieldObject(const Vector2D & position, float radius,
                         int fillColor,
                         int strokeColor, float strokeThickness,
                         int _positionColor, float _positionDeviation)
  : Circle(position, radius,
           fillColor,
           strokeColor, strokeThickness),
    _havePosition(true),
    _haveAngle(false),
    positionColor(_positionColor),
    angleColor(0x00000000),
    positionDeviation(_positionDeviation),
    angleDeviation(0),
    angle(0) {
  // We need to set type here, since FieldObject is a subclass of Circle
  setType(Shape::FieldObject);
}

FieldObject::FieldObject(const Vector2D & position, float radius,
                         int fillColor,
                         int strokeColor, float strokeThickness,
                         int _angleColor, float _angle, float _angleDeviation)
  : Circle(position, radius,
           fillColor,
           strokeColor, strokeThickness),
    _havePosition(false),
    _haveAngle(true),
    positionColor(0x00000000),
    angleColor(_angleColor),
    positionDeviation(0),
    angleDeviation(_angleDeviation),
    angle(_angle) {
  // We need to set type here, since FieldObject is a subclass of Circle
  setType(Shape::FieldObject);
}

FieldObject::FieldObject(const Vector2D & position, float radius,
                         int fillColor,
                         int strokeColor, float strokeThickness,
                         int _positionColor, float _positionDeviation,
                         int _angleColor, float _angle, float _angleDeviation)
  : Circle(position, radius,
           fillColor,
           strokeColor, strokeThickness),
    _havePosition(true),
    _haveAngle(true),
    positionColor(_positionColor),
    angleColor(_angleColor),
    positionDeviation(_positionDeviation),
    angleDeviation(_angleDeviation),
    angle(_angle) {
  // We need to set type here, since FieldObject is a subclass of Circle
  setType(Shape::FieldObject);
}

FieldObject::~FieldObject() {
}

bool FieldObject::havePosition() const {
  return _havePosition;
}

bool FieldObject::haveAngle() const {
  return _haveAngle;
}

int FieldObject::getPositionColor() const {
  return positionColor;
}

int FieldObject::getAngleColor() const {
  return angleColor;
}

float FieldObject::getPositionDeviation() const {
  return positionDeviation;
}

float FieldObject::getAngle() const {
  return angle;
}

float FieldObject::getAngleDeviation() const {
  return angleDeviation;
}
