#include "Triangle.h"

Triangle::Triangle(const Vector2D & _end1,
                   const Vector2D & _end2,
                   const Vector2D & _end3,
                   int fillColor)
  : Shape(Shape::Triangle, fillColor),
    end1(_end1),
    end2(_end2),
    end3(_end3) {
}

Triangle::Triangle(const Vector2D & _end1,
                   const Vector2D & _end2,
                   const Vector2D & _end3,
                   int strokeColor,
                   float strokeThickness)
  : Shape(Shape::Triangle, strokeColor, strokeThickness),
    end1(_end1),
    end2(_end2),
    end3(_end3) {
}

Triangle::Triangle(const Vector2D & _end1,
                   const Vector2D & _end2,
                   const Vector2D & _end3,
                   int fillColor,
                   int strokeColor,
                   float strokeThickness)
  : Shape(Shape::Triangle, fillColor, strokeColor, strokeThickness),
    end1(_end1),
    end2(_end2),
    end3(_end3) {
}

Triangle::~Triangle() {
}

const Vector2D & Triangle::getEnd1() const {
  return end1;
}

const Vector2D & Triangle::getEnd2() const {
  return end2;
}

const Vector2D & Triangle::getEnd3() const {
  return end3;
}
