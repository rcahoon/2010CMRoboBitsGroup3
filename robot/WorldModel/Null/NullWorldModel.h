#ifndef NULLWORLDMODEL_H_
#define NULLWORLDMODEL_H_

#include "WorldModel/WorldModel.h"

class NullWorldModel : public WorldModel {
public:
  NullWorldModel();
  virtual ~NullWorldModel();

  virtual bool run(const RobotState & robotState,
                   const GameState & gameState,
                   const VisionFeatures & visionFeatures,
                   const Pose & pose,
                         Messages & messages,
                         WorldFeatures & worldFeatures);

};

#endif /* NULLWORLDMODEL_H_ */
