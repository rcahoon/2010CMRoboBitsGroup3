#ifndef LINE_H_
#define LINE_H_

#include "Shape.h"
#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"

class Line : public Shape {
public:

  Line(const Vector2D & end1, const Vector2D & end2,
       int strokeColor, float strokeThickness);

  virtual ~Line();

  const Vector2D & getEnd1() const;

  const Vector2D & getEnd2() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(Line);

  Vector2D end1, end2;
};

#endif /* LINE_H_ */
