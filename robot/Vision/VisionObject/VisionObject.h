#ifndef VISIONOBJECT_H_
#define VISIONOBJECT_H_

#include "shared/Vector/Vector2D.h"

class Log;

class VisionObject {
public:

  enum Type {
    Unknown = 0,
    Ball,
    BlueGoalPost,
    YellowGoalPost,
    BlueGoalBar,
    YellowGoalBar,
    Robot,
    Line,
    Corner,
	Circle,
    FreeSpace,
    ConvexHull,
    numTypes // this should always be last
  };

  /**
   * Basic constructor.
   *
   * @param _log reference to the Log component
   * @param _type type of object, e.g., ball, goal post, etc
   */
  VisionObject(Log & _log,
               const Type _type = Unknown);

  /**
   * Copy constructor.
   */
  VisionObject(const VisionObject & other);

  /**
   * Destructor.
   */
  virtual ~VisionObject();

  /**
   * Retrieves the type of vision object.
   *
   * @return type of vision object
   */
  Type getType() const;

  /**
   * Retrieves the position of the vision object in local coordinates.
   *
   * @return reference to the local coordinates of the vision object
   */
  Vector2D const & getPosition() const;

  /**
   * Retrieves the bounding box of the vision object in image coordinates.
   *
   * @param _x1 reference to top-left x-coordinate to be filled in
   * @param _y1 reference to top-left y-coordinate to be filled in
   * @param _x2 reference to bottom-right x-coordinate to be filled in
   * @param _y2 reference to bottom-right y-coordinate to be filled in
   */
  void getBoundingBox(unsigned int & _x1, unsigned int & _y1,
                      unsigned int & _x2, unsigned int & _y2) const;

  /**
   * Retrieves individual coordinates of the bounding box in image coordinates.
   */
  int getBoundingBoxX1() const;
  int getBoundingBoxY1() const;
  int getBoundingBoxX2() const;
  int getBoundingBoxY2() const;

  /**
   * Retrieves the confidence of the vision object.
   *
   * @return the confidence of the vision object (higher is better)
   */
  float getConfidence() const;

  /**
   * Sets the position of the vision object in local coordinates
   *
   * @param _position position of the object in local coordinates
   */
  void setPosition(const Vector2D & _position);

  /**
   * Sets the confidence of the vision object, typically between 0 and 1.
   *
   * @param _confidence the confidence of the vision object
   */
  void setConfidence(const float _confidence);

  /**
   * Sets the bounding box of the vision object in image coordinates.
   *
   * @param x1 x-coordinate of the top-left point
   * @param y1 y-coordinate of the top-left point
   * @param x2 x-coordinate of the bottom-right point
   * @param y2 y-coordinate of the bottom-right point
   */
  void setBoundingBox(const unsigned int _x1, const unsigned int _y1,
                      const unsigned int _x2, const unsigned int _y2);

  /**
   * Clears the position, confidence and bounding box of the vision object. The
   * type remains the same.
   */
  virtual void clear();

  /**
   * Copies the content of another vision object. We use this instead of having
   * an assignment operator to prevent issues with inheritance.
   *
   * @param other other vision object to copy from
   */
  virtual void copyFrom(const VisionObject & other);

  /**
   * Assignment operator - calls copyFrom method.
   *
   * @param other other vision object to be assigned from
   */
  VisionObject & operator=(const VisionObject & other);

protected:
  Log & log;

private:
  // What type of vision object is this?
  Type type;

  // Position in local coordinates
  Vector2D position;
  // How likely is the object really there?
  float    confidence;

  // Bounding box in camera image
  unsigned int x1, y1, x2, y2;

};

#endif /* VISIONOBJECT_H_ */
