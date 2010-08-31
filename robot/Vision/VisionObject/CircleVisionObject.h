#ifndef CIRCLEVISIONOBJECT_H_
#define CIRCLEVISIONOBJECT_H_

#include "VisionObject.h"
#include "shared/UsefulMacros.h"

class Log;

class CircleVisionObject : public VisionObject {
public:

  CircleVisionObject(Log & _log);

  /**
   * Copy constructor.
   */
  CircleVisionObject(const CircleVisionObject & other);

  virtual ~CircleVisionObject();

  void copyFrom(const VisionObject & other);

private:
  PREVENT_ASSIGNMENT(CircleVisionObject);

};

#endif /* CIRCLEVISIONOBJECT_H_ */
