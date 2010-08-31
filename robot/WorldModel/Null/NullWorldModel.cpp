#include "NullWorldModel.h"
#include "WorldModel/WorldFeatures.h"

NullWorldModel::NullWorldModel() {
}

NullWorldModel::~NullWorldModel() {
}

bool NullWorldModel::run(const RobotState & robotState,
                         const GameState & gameState,
                         const VisionFeatures & visionFeatures,
                         const Pose & pose,
                               Messages & messages,
                               WorldFeatures & worldFeatures) {
  worldFeatures.clear();

  return false;
}
