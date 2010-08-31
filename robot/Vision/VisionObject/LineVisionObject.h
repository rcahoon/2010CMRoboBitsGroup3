#ifndef LINEVISIONOBJECT_H_
#define LINEVISIONOBJECT_H_

#include "VisionObject.h"
#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"

class Log;

class LineVisionObject : public VisionObject {
public:
    
  LineVisionObject(Log & _log,
                   const Vector2D & _endPoint1 = Vector2D(0, 0),
                   const Vector2D & _endPoint2 = Vector2D(0, 0),
                   const Vector2D & _cameraEndPoint1 = Vector2D(0, 0),
                   const Vector2D & _cameraEndPoint2 = Vector2D(0, 0));

  /**
   * Copy constructor.
   */
  LineVisionObject(const LineVisionObject & other);

  virtual ~LineVisionObject();

  const Vector2D & getEndPoint1() const;
  void setEndPoint1(Vector2D _endPoint1 );

  const Vector2D & getEndPoint2() const;
  void  setEndPoint2(Vector2D _endPoint2 );

  const Vector2D & getCameraEndPoint1() const;

  const Vector2D & getCameraEndPoint2() const;

  virtual void clear();

  virtual void copyFrom(const VisionObject & other);

private:
  PREVENT_ASSIGNMENT(LineVisionObject);

  // Local coordinates of the line end-points
  Vector2D endPoint1;
  Vector2D endPoint2;

  // Camera coordinates of the line end-points
  Vector2D cameraEndPoint1;
  Vector2D cameraEndPoint2;
};

#endif /* LINEVISIONOBJECT_H_ */
