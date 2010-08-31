#include "NullVision.h"
#include "Vision/VisionFeatures.h"

NullVision::NullVision() {
}

NullVision::~NullVision() {
}

bool NullVision::run(const RobotState & robotState,
                     VisionFeatures & visionFeatures) {
  visionFeatures.clear();

  return false;
}
