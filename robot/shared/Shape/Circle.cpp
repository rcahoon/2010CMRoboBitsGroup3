#include "Circle.h"

Circle::Circle(const Vector2D & _center, float _radius,
               int fillColor)
  : Shape(Shape::Circle, fillColor),
    center(_center),
    radius(_radius) {
}

Circle::Circle(const Vector2D & _center, float _radius,
               int strokeColor, float strokeThickness)
  : Shape(Shape::Circle, strokeColor, strokeThickness),
    center(_center),
    radius(_radius) {
}

Circle::Circle(const Vector2D & _center, float _radius,
               int fillColor,
               int strokeColor, float strokeThickness)
  : Shape(Shape::Circle, fillColor, strokeColor, strokeThickness),
    center(_center),
    radius(_radius) {
}

Circle::~Circle() {
}

const Vector2D & Circle::getCenter() const {
  return center;
}

float Circle::getRadius() const {
  return radius;
}
