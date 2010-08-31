#include <stdint.h>
#include <string>

#include "ShapeMessage.h"
#include "shared/Shape/Circle.h"
#include "shared/Shape/Rectangle.h"
#include "shared/Shape/Triangle.h"
#include "shared/Shape/Line.h"
#include "shared/Shape/FieldObject.h"

ShapeMessage::ShapeMessage(Log::Screen screen,
                           const Shape & shape)
  : RemoteMessageFromRobot(RemoteMessageFromRobot::shape, 0, NULL) {
  createData(screen, shape);
}

ShapeMessage::~ShapeMessage() {
}

void ShapeMessage::createData(Log::Screen screen,
                              const Shape & shape) {
  std::vector<int>   ints;
  std::vector<float> floats;

  switch (shape.getType()) {
  case Shape::Circle:
  {
    Circle const *circle = dynamic_cast<Circle const *>(&shape);
    if (circle != NULL) {
      floats.push_back(circle->getCenter().x);
      floats.push_back(circle->getCenter().y);
      floats.push_back(circle->getRadius());
    }
    break;
  }
  case Shape::Rectangle:
  {
    Rectangle const *rectangle = dynamic_cast<Rectangle const *>(&shape);
    if (rectangle != NULL) {
      floats.push_back(rectangle->getTopLeft().x);
      floats.push_back(rectangle->getTopLeft().y);
      floats.push_back(rectangle->getBottomRight().x);
      floats.push_back(rectangle->getBottomRight().y);
    }
    break;
  }
  case Shape::Triangle:
  {
    Triangle const *triangle = dynamic_cast<Triangle const *>(&shape);
    if (triangle != NULL) {
      floats.push_back(triangle->getEnd1().x);
      floats.push_back(triangle->getEnd1().y);
      floats.push_back(triangle->getEnd2().x);
      floats.push_back(triangle->getEnd2().y);
      floats.push_back(triangle->getEnd3().x);
      floats.push_back(triangle->getEnd3().y);
    }
    break;
  }
  case Shape::Line:
  {
    Line const *line = dynamic_cast<Line const *>(&shape);
    if (line != NULL) {
      floats.push_back(line->getEnd1().x);
      floats.push_back(line->getEnd1().y);
      floats.push_back(line->getEnd2().x);
      floats.push_back(line->getEnd2().y);
    }
    break;
  }
  case Shape::FieldObject:
  {
    FieldObject const *fieldObject = dynamic_cast<FieldObject const *>(&shape);
    if (fieldObject != NULL) {
      floats.push_back(fieldObject->getCenter().x);
      floats.push_back(fieldObject->getCenter().y);
      floats.push_back(fieldObject->getRadius());

      ints.push_back(fieldObject->havePosition());
      ints.push_back(fieldObject->haveAngle());

      if (fieldObject->havePosition()) {
        ints.push_back(fieldObject->getPositionColor());

        floats.push_back(fieldObject->getPositionDeviation());
      }
      if (fieldObject->haveAngle()) {
        ints.push_back(fieldObject->getAngleColor());

        floats.push_back(fieldObject->getAngle());
        floats.push_back(fieldObject->getAngleDeviation());
      }
    }
  }
  default:
    break;
  }

  createData(screen, shape, ints, floats);
}

void ShapeMessage::createData(Log::Screen screen,
                              const Shape & shape,
                              const std::vector<int>   & ints,
                              const std::vector<float> & floats) {
  // We definitely need to send the type, filled, and stroked
  int headerSize = 3;
  // If it's filled, we need to send fillColor
  if (shape.isFilled()) {
    headerSize += 4;
  }
  // If it's stroked, we need to send strokeColor and strokeThickness
  if (shape.isStroked()) {
    headerSize += 8;
  }

  // How many bytes do we need?
  // x for header size
  // 1 for the screen id
  // 1 for the number of ints
  // 1 for the number of floats
  // 4 for each int
  // 4 for each float
  size = headerSize + 3 + 4 * (ints.size() + floats.size());

  // Generate the data in bytes
  data = new char[size];

  if (data == NULL) {
    size = 0;
    return;
  }

  // The first set of bytes are for the header

  unsigned int offset = 0;

  // The first byte is the shape type
  data[offset] = (uint8_t)shape.getType();
  // The second byte is filled
  data[offset + 1] = (shape.isFilled() ? 1 : 0);
  // The third byte is stroked
  data[offset + 2] = (shape.isStroked() ? 1 : 0);

  offset += 3;

  // If it's filled, we need to send fillColor
  if (shape.isFilled()) {
    writeInt(data + offset, shape.getFillColor());
    offset += 4;
  }

  // If it's stroked, we need to send strokeColor and strokeThickness
  if (shape.isStroked()) {
    writeInt(data + offset, shape.getStrokeColor());
    offset += 4;

    writeFloat(data + offset, shape.getStrokeThickness());
    offset += 4;
  }

  // Done writing the header

  // The first byte is the number of floats
  data[offset] = (uint8_t)screen;
  offset++;

  // The following byte is the number of ints
  data[offset] = (uint8_t)ints.size();
  offset++;

  // The following byte is the number of floats
  data[offset] = (uint8_t)floats.size();
  offset++;

  // Write all the ints
  std::vector<int>::const_iterator intIter;
  for (intIter = ints.begin(); intIter != ints.end(); intIter++) {
    writeInt(data + offset, *intIter);
    offset += 4;
  }

  // Write all the floats
  std::vector<float>::const_iterator floatIter;
  for (floatIter = floats.begin(); floatIter != floats.end(); floatIter++) {
    writeFloat(data + offset, *floatIter);
    offset += 4;
  }
}
