#ifndef ROBOT_VISION_OBJECT_H_
#define ROBOT_VISION_OBJECT_H_

#include "VisionObject.h"
#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"

class Log;

class RobotVisionObject : public VisionObject {
public:

  enum RobotColor {
    Unknown = 0,
    Red,
    Blue,
  };

  RobotVisionObject(Log & _log,
                    const RobotColor = Unknown);

  /**
   * Copy constructor.
   */
  RobotVisionObject(const RobotVisionObject & other);

  virtual ~RobotVisionObject();

  RobotColor getRobotColor() const;

  virtual void copyFrom(const VisionObject & other);

private:
  PREVENT_ASSIGNMENT(RobotVisionObject);

  RobotColor color;
};

#endif 
