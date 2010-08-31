#ifndef FREESPACEVISIONOBJECT_H_
#define FREESPACEVISIONOBJECT_H_

#include "VisionObject.h"
#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"

class Log;


class FreeSpaceVisionObject : public VisionObject {
public:
    
  FreeSpaceVisionObject(Log & _log);

  /**
   * Copy constructor.
   */
  FreeSpaceVisionObject(const FreeSpaceVisionObject & other);

  virtual ~FreeSpaceVisionObject();

  virtual void clear();

  virtual void copyFrom(const VisionObject & other);

  void setStartRelative(Vector2D _startRelative);
  void setEndRelative(Vector2D _endRelative);
  Vector2D getStartRelative();
  Vector2D getEndRelative();

  /*
  Vector2D points[MAX_NUM_OF_FREE_SPACE_LINES];
  Vector2D groundPoints[MAX_NUM_OF_FREE_SPACE_LINES];

  RoboCup2010::Segment segments[MAX_NUM_OF_FREE_SPACE_SEGMENTS];
  RoboCup2010::Segment groundSegments[MAX_NUM_OF_FREE_SPACE_SEGMENTS];
  
  int numPoints;
  int numSegments;
*/
  
private:
  PREVENT_ASSIGNMENT(FreeSpaceVisionObject);
  
  Vector2D startRelative, endRelative, startCam, endCam;

};

#endif /* FREESPACEVISIONOBJECT_H_ */
