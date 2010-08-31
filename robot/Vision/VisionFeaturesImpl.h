#ifndef VISIONFEATURESIMPL_H_
#define VISIONFEATURESIMPL_H_

#include <vector>

#include "VisionFeatures.h"
#include "Vision/VisionObject/VisionObject.h"

class VisionFeaturesImpl: public VisionFeatures {
public:
  VisionFeaturesImpl();

  virtual ~VisionFeaturesImpl();

  virtual const std::vector<VisionObject const *> & getAllVisionObjects() const;

  virtual const std::vector<VisionObject const *> & getVisionObjects(const VisionObject::Type type) const;

  virtual void clear();

  virtual void addVisionObject(const VisionObject & visionObject);

  virtual void addVisionObjects(const std::vector<VisionObject const *> & visionObjects);

private:
  std::vector<VisionObject const *> visionObjects[VisionObject::numTypes];

  std::vector<VisionObject const *> allVisionObjects;
};

#endif /* VISIONFEATURESIMPL_H_ */
