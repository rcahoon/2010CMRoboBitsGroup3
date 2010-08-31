#include "Rectangle.h"

Rectangle::Rectangle(const Vector2D & _topLeft,
                     const Vector2D & _bottomRight,
                     int fillColor)
  : Shape(Shape::Rectangle, fillColor),
    topLeft(_topLeft),
    bottomRight(_bottomRight) {
}

Rectangle::Rectangle(const Vector2D & _topLeft,
                     const Vector2D & _bottomRight,
                     int strokeColor, float strokeThickness)
  : Shape(Shape::Rectangle, strokeColor, strokeThickness),
    topLeft(_topLeft),
    bottomRight(_bottomRight) {
}

Rectangle::Rectangle(const Vector2D & _topLeft,
                     const Vector2D & _bottomRight,
                     int fillColor,
                     int strokeColor, float strokeThickness)
  : Shape(Shape::Rectangle, fillColor, strokeColor, strokeThickness),
    topLeft(_topLeft),
    bottomRight(_bottomRight) {
}

Rectangle::~Rectangle() {
}

const Vector2D & Rectangle::getTopLeft() const {
  return topLeft;
}

const Vector2D & Rectangle::getBottomRight() const {
  return bottomRight;
}
