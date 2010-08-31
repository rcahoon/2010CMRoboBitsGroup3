#ifndef FIELDOBJECT_H_
#define FIELDOBJECT_H_

#include "Circle.h"
#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"

class FieldObject : public Circle {
public:
  FieldObject(const Vector2D & position, float radius,
             int fillColor,
             int strokeColor, float strokeThickness);

  FieldObject(const Vector2D & position, float radius,
             int fillColor,
             int strokeColor, float strokeThickness,
             int positionColor, float positionDeviation);

  FieldObject(const Vector2D & position, float radius,
             int fillColor,
             int strokeColor, float strokeThickness,
             int angleColor, float angle, float angleDeviation);

  FieldObject(const Vector2D & position, float radius,
             int fillColor,
             int strokeColor, float strokeThickness,
             int positionColor, float positionDeviation,
             int angleColor, float angle, float angleDeviation);

  virtual ~FieldObject();

  bool havePosition() const;

  bool haveAngle() const;

  int getPositionColor() const;

  int getAngleColor() const;

  float getPositionDeviation() const;

  float getAngle() const;

  float getAngleDeviation() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(FieldObject);

  bool _havePosition,     _haveAngle;
  int   positionColor,     angleColor;
  float positionDeviation, angleDeviation;
  float angle;
};

#endif /* FIELDOBJECT_H_ */
