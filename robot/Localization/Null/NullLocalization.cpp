#include "NullLocalization.h"
#include "Localization/Pose.h"

NullLocalization::NullLocalization() {
}

NullLocalization::~NullLocalization() {
}

bool NullLocalization::run(const RobotState     & robotState,
                           const GameState      & gameState,
                           const VisionFeatures & visionFeatures,
                           Pose & pose) {
  pose.setPosition(0, 0);
  pose.setAngle(0);

  return false;
}

void NullLocalization::setScanningForGoals(bool scanning)
{

}

void NullLocalization::updateWorldFeatures(const WorldFeatures & worldFeatures)
{

}
