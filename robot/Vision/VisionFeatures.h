#ifndef VISIONFEATURES_H_
#define VISIONFEATURES_H_

#include <vector>
#include "Vision/VisionObject/VisionObject.h"

/**
 * Interface for vision features. This class contains all the sensed vision
 * objects detected by the Vision component.
 *
 * When vision objects are added to VisionFeatures, ownership does not transfer
 * to VisionFeatures, so they will not be deleted when clear() is called.
 */
class VisionFeatures {
public:
  /**
   * Default constructor (empty since this is an interface).
   */
  VisionFeatures() {};

  /**
   * Destructor (empty since this is an interface).
   */
  virtual ~VisionFeatures() {};

  /**
   * Retrieves reference to a vector containing all the vision objects.
   *
   * @return reference to all vision objects
   */
  virtual const std::vector<VisionObject const *> & getAllVisionObjects() const = 0;

  /**
   * Retrieves reference to a vector containing vision objects of a certain
   * type.
   *
   * @return reference to a vector of one type of vision objects.
   */
  virtual const std::vector<VisionObject const *> & getVisionObjects(const VisionObject::Type type) const = 0;

  /**
   * Clears the vectors of vision objects. These objects are not deleted by
   * VisionFeatures as ownership was not transferred here.
   */
  virtual void clear() = 0;

  /**
   * Adds a vision object into the features.
   */
  virtual void addVisionObject(const VisionObject & visionObject) = 0;

  /**
   * Adds a vector of vision objects into the features. Ownership does not
   * transfer to VisionFeatures, so these objects will not be deleted when
   * clear() is called.
   *
   * @param visionObjects vector of vision objects
   */
  virtual void addVisionObjects(const std::vector<VisionObject const *> & visionObjects) = 0;
};

#endif /* VISIONFEATURES_H_ */
