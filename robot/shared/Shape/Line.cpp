#include "Line.h"

Line::Line(const Vector2D & _end1, const Vector2D & _end2,
           int strokeColor, float strokeThickness)
  : Shape(Shape::Line, strokeColor, strokeThickness),
    end1(_end1),
    end2(_end2) {
}

Line::~Line() {
}

const Vector2D & Line::getEnd1() const {
  return end1;
}

const Vector2D & Line::getEnd2() const {
  return end2;
}
