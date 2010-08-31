#ifndef NULLVISION_H_
#define NULLVISION_H_

#include "Vision/Vision.h"

class NullVision : public Vision {
public:
  NullVision();
  virtual ~NullVision();

  virtual bool run(const RobotState & robotState,
                         VisionFeatures & visionFeatures);
};

#endif /* NULLVISION_H_ */
