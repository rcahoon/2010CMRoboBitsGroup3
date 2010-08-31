#ifndef CONVEXHULLVISIONOBJECT_H_
#define CONVEXHULLVISIONOBJECT_H_

#include "VisionObject.h"
#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"
#include <vector>

class Log;


class ConvexHullVisionObject : public VisionObject {
public:
    
  ConvexHullVisionObject(Log & _log);

  /**
   * Copy constructor.
   */
  ConvexHullVisionObject(const ConvexHullVisionObject & other);

  virtual ~ConvexHullVisionObject();

  virtual void clear();

  virtual void copyFrom(const VisionObject & other);

  std::vector<Vector2D> getPoints();
  void set(std::vector<Vector2D> source);
  unsigned int getSize();
  
private:
  PREVENT_ASSIGNMENT(ConvexHullVisionObject);
  
  std::vector<Vector2D> points;

};

#endif /* CONVEXHULLVISIONOBJECT_H_ */
