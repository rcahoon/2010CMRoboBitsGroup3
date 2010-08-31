#ifndef WORLDFEATURES_H_
#define WORLDFEATURES_H_

#include <vector>

#include "WorldModel/WorldObject/WorldObject.h"

/**
 * Interface for world features. This class contains all the modeled
 * objects of the World Model component.
 *
 * When world objects are added to WorldFeatures, ownership does not transfer
 * to WorldFeatures, so they will not be deleted when clear() is called.
 */
class WorldFeatures {
public:
  /**
   * Default constructor (empty since this is an interface).
   */
  WorldFeatures() {};

  /**
   * Destructor (empty since this is an interface).
   */
  virtual ~WorldFeatures() {};

  /**
   * Retrieves reference to a vector containing all the world objects.
   *
   * @return reference to all world objects
   */
  virtual const std::vector<WorldObject const *> & getAllWorldObjects() const = 0;

  /**
   * Retrieves reference to a vector containing world objects of a certain type.
   *
   * @return reference to a vector of one type of world objects.
   */
  virtual const std::vector<WorldObject const *> & getWorldObjects(WorldObject::Type type) const = 0;

  /**
   * Retrieves the pointer to the first world object of a certain type.
   *
   * @return const pointer to the first world object of a certain type, NULL if it doesn't exist
   */
  virtual WorldObject const * getFirstWorldObject(WorldObject::Type type) const = 0;

  /**
   * Clears the vectors of world objects. These objects are not deleted by
   * WorldFeatures as ownership was not transferred here.
   */
  virtual void clear() = 0;

  /**
   * Adds a world object into the features.
   */
  virtual void addWorldObject(const WorldObject & worldObject) = 0;

  /**
   * Adds a vector of world objects into the features. Ownership does not
   * transfer to WorldFeatures, so these objects will not be deleted when
   * clear() is called.
   *
   * @param worldObjects vector of world objects
   */
  virtual void addWorldObjects(const std::vector<WorldObject const *> & worldObjects) = 0;

};

#endif /* WORLDFEATURES_H_ */
