#ifndef CORNERVISIONOBJECT_H_
#define CORNERVISIONOBJECT_H_

#include "VisionObject.h"
#include "shared/UsefulMacros.h"

class Log;

class CornerVisionObject : public VisionObject {
public:

  enum CornerType {
    Unknown = 0,
    Cross,
    T,
    L,
  };

  CornerVisionObject(Log & _log,
                     const CornerType _cornerType = Unknown,
                     const float _angle = 0);

  /**
   * Copy constructor.
   */
  CornerVisionObject(const CornerVisionObject & other);

  virtual ~CornerVisionObject();

  CornerType getCornerType() const;
  void setCornerType(CornerType _cornerType);

  float getAngle() const;

  void setAngle(float _angle);

  virtual void clear();

  virtual void copyFrom(const VisionObject & other);

private:
  PREVENT_ASSIGNMENT(CornerVisionObject);

  CornerType cornerType;

  /*
   * For cross type: angle is along one of the four lines coming
   * out of the center
   *
   * For T type: one line continues and one line terminates, the
   * angle is from the center of corner along the line that
   * terminates
   *
   * For L type: the angle is from the center of the corner, out
   * halfway between the two lines, (so 45 degrees greater than
   * one and 45 degrees less than the other)
   */
  float angle;
};

#endif /* CORNERVISIONOBJECT_H_ */
