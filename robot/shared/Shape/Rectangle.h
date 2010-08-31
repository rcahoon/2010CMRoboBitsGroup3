#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Shape.h"
#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"

class Rectangle: public Shape {
public:
  Rectangle(const Vector2D & topLeft,
            const Vector2D & bottomRight,
            int fillColor);

  Rectangle(const Vector2D & topLeft,
            const Vector2D & bottomRight,
            int strokeColor, float strokeThickness);

  Rectangle(const Vector2D & topLeft,
            const Vector2D & bottomRight,
            int fillColor,
            int strokeColor, float strokeThickness);

  virtual ~Rectangle();

  const Vector2D & getTopLeft() const;

  const Vector2D & getBottomRight() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(Rectangle);

  Vector2D topLeft;
  Vector2D bottomRight;

};

#endif /* RECTANGLE_H_ */
