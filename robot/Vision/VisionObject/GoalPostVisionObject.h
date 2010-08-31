#ifndef GOALPOSTVISIONOBJECT_H_
#define GOALPOSTVISIONOBJECT_H_

#include "VisionObject.h"
#include "shared/UsefulMacros.h"

class Log;

class GoalPostVisionObject : public VisionObject {
public:

  enum Side {
    Unknown = 0,
    Left = -1,
    Right = 1,
  };


  GoalPostVisionObject(Log & _log,
                       const bool _isBlue = true,
                       const Side _side = Unknown);

  /**
   * Copy constructor.
   */
  GoalPostVisionObject(const GoalPostVisionObject & other);

  virtual ~GoalPostVisionObject();

  Side getSide() const;

  bool isLeftPost() const;

  bool isRightPost() const;

  bool isUnknownPost() const;

  bool isBluePost() const;

  void setSide(Side _side);

  virtual void clear();

  virtual void copyFrom(const VisionObject & other);

private:
  PREVENT_ASSIGNMENT(GoalPostVisionObject);

  bool isBlue;
  Side side;
};

#endif /* GOALPOSTVISIONOBJECT_H_ */
