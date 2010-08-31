#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "Shape.h"
#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"

class Circle: public Shape {
public:
  Circle(const Vector2D & center, float radius,
         int fillColor);

  Circle(const Vector2D & center, float radius,
         int strokeColor, float strokeThickness);

  Circle(const Vector2D & center, float radius,
         int fillColor,
         int strokeColor, float strokeThickness);

  virtual ~Circle();

  const Vector2D & getCenter() const;

  float getRadius() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(Circle);

  Vector2D center;
  float    radius;
};

#endif /* CIRCLE_H_ */
