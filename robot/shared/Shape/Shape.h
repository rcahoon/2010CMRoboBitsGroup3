#ifndef SHAPE_H_
#define SHAPE_H_

#include "shared/UsefulMacros.h"

/**
 * This is the superclass for all shapes.
 */
class Shape {
public:

  enum Type {
    Unknown = 0,
    Circle,
    Rectangle,
    Triangle,
    Line,
    FieldObject,
    numTypes // this should always be last
  };

  Shape(Type type);

  Shape(Type type, int fillColor, int strokeColor, float strokeThickness);

  Shape(Type type, int fillColor);

  Shape(Type type, int strokeColor, float strokeThickness);

  virtual ~Shape();

  Type getType() const;

  bool isFilled() const;

  bool isStroked() const;

  int getFillColor() const;

  int getStrokeColor() const;

  float getStrokeThickness() const;

protected:
  void setType(Type type);

private:
  PREVENT_COPY_AND_ASSIGNMENT(Shape);

  Type type;

  bool  filled,    stroked;
  int   fillColor, strokeColor;
  float strokeThickness;

};

#endif /* SHAPE_H_ */
