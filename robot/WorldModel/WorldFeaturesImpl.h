#ifndef WORLDFEATURESIMPL_H_
#define WORLDFEATURESIMPL_H_

#include <vector>

#include "WorldFeatures.h"
#include "WorldModel/WorldObject/WorldObject.h"

class WorldFeaturesImpl: public WorldFeatures {
public:
  WorldFeaturesImpl();

  virtual ~WorldFeaturesImpl();

  virtual const std::vector<WorldObject const *> & getAllWorldObjects() const;

  virtual const std::vector<WorldObject const *> & getWorldObjects(WorldObject::Type type) const;

  virtual WorldObject const * getFirstWorldObject(WorldObject::Type type) const;

  virtual void clear();

  virtual void addWorldObject(const WorldObject & worldObject);

  virtual void addWorldObjects(const std::vector<WorldObject const *> & worldObjects);

private:
  std::vector<WorldObject const *> worldObjects[WorldObject::numTypes];

  std::vector<WorldObject const *> allWorldObjects;
};

#endif /* WORLDFEATURESIMPL_H_ */
