#include "WorldFeaturesImpl.h"

WorldFeaturesImpl::WorldFeaturesImpl() {
}

WorldFeaturesImpl::~WorldFeaturesImpl() {
}

const std::vector<WorldObject const *> & WorldFeaturesImpl::getAllWorldObjects() const {
  return allWorldObjects;
}

const std::vector<WorldObject const *> & WorldFeaturesImpl::getWorldObjects(WorldObject::Type type) const {
  return worldObjects[(int)type];
}

WorldObject const * WorldFeaturesImpl::getFirstWorldObject(WorldObject::Type type) const {
  if (worldObjects[(int)type].size() > 0) {
    return worldObjects[(int)type].at(0);
  }
  else {
    return NULL;
  }
}

void WorldFeaturesImpl::clear() {
  allWorldObjects.clear();

  for (int i = 0; i < WorldObject::numTypes; i++) {
    worldObjects[i].clear();
  }
}

void WorldFeaturesImpl::addWorldObject(const WorldObject & worldObject) {
  allWorldObjects.push_back(&worldObject);

  worldObjects[(int)worldObject.getType()].push_back(&worldObject);
}

void WorldFeaturesImpl::addWorldObjects(const std::vector<WorldObject const *> & worldObjects) {
  std::vector<WorldObject const *>::const_iterator iter;
  for (iter = worldObjects.begin(); iter != worldObjects.end(); iter++) {
    WorldObject const *worldObject = *iter;
    addWorldObject(*worldObject);
  }
}
