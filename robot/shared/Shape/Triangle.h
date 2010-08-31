#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Shape.h"
#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"

class Triangle: public Shape {
public:
  Triangle(const Vector2D & end1,
           const Vector2D & end2,
           const Vector2D & end3,
           int fillColor);

  Triangle(const Vector2D & end1,
           const Vector2D & end2,
           const Vector2D & end3,
           int strokeColor, float strokeThickness);

  Triangle(const Vector2D & end1,
           const Vector2D & end2,
           const Vector2D & end3,
           int fillColor,
           int strokeColor, float strokeThickness);

  virtual ~Triangle();

  const Vector2D & getEnd1() const;

  const Vector2D & getEnd2() const;

  const Vector2D & getEnd3() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(Triangle);

  Vector2D end1, end2, end3;

};

#endif /* TRIANGLE_H_ */
