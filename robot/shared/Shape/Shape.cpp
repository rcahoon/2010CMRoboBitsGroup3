#include "Shape.h"

Shape::Shape(Type _type)
  : type(_type),
    filled(true),
    stroked(false),
    fillColor(0xFFFFFF),
    strokeColor(0x000000),
    strokeThickness(0) {
}

Shape::Shape(Type _type, int _fillColor, int _strokeColor, float _strokeThickness)
  : type(_type),
    filled(true),
    stroked(true),
    fillColor(_fillColor),
    strokeColor(_strokeColor),
    strokeThickness(_strokeThickness) {
}

Shape::Shape(Type _type, int _fillColor)
  : type(_type),
    filled(true),
    stroked(false),
    fillColor(_fillColor),
    strokeColor(0x000000),
    strokeThickness(0) {
}

Shape::Shape(Type _type, int _strokeColor, float _strokeThickness)
  : type(_type),
    filled(false),
    stroked(true),
    fillColor(0xFFFFFF),
    strokeColor(_strokeColor),
    strokeThickness(_strokeThickness) {
}

Shape::~Shape() {
}

Shape::Type Shape::getType() const {
  return type;
}

bool Shape::isFilled() const {
  return filled;
}

bool Shape::isStroked() const {
  return stroked;
}

int Shape::getFillColor() const {
  return fillColor;
}

int Shape::getStrokeColor() const {
  return strokeColor;
}

float Shape::getStrokeThickness() const {
  return strokeThickness;
}

void Shape::setType(Shape::Type _type) {
  type = _type;
}
