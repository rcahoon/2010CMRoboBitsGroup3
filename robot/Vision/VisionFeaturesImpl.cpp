#include "VisionFeaturesImpl.h"

VisionFeaturesImpl::VisionFeaturesImpl() {
}

VisionFeaturesImpl::~VisionFeaturesImpl() {
}

const std::vector<VisionObject const *> & VisionFeaturesImpl::getAllVisionObjects() const {
  return allVisionObjects;
}

const std::vector<VisionObject const *> & VisionFeaturesImpl::getVisionObjects(const VisionObject::Type type) const {
  return visionObjects[(int)type];
}

void VisionFeaturesImpl::clear() {
  allVisionObjects.clear();

  for (int i = 0; i < VisionObject::numTypes; i++) {
    visionObjects[i].clear();
  }
}

void VisionFeaturesImpl::addVisionObject(const VisionObject & visionObject) {
  allVisionObjects.push_back(&visionObject);

  visionObjects[(int)visionObject.getType()].push_back(&visionObject);
}

void VisionFeaturesImpl::addVisionObjects(const std::vector<VisionObject const *> & visionObjects) {
  std::vector<VisionObject const *>::const_iterator iter;
  for (iter = visionObjects.begin(); iter != visionObjects.end(); iter++) {
    VisionObject const *visionObject = *iter;
    addVisionObject(*visionObject);
  }
}
