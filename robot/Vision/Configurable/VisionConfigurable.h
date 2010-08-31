#ifndef VISIONCONFIGURABLE_H_
#define VISIONCONFIGURABLE_H_

#include "Vision/Vision.h"

class ConfigFile;
class Log;

class VisionConfigurable : public Vision {
public:
  VisionConfigurable(ConfigFile & configFile, Log & log);

  virtual ~VisionConfigurable();

  virtual bool run(const RobotState & robotState,
                         VisionFeatures & visionFeatures);

  virtual void disableBallDetectionNextFrame();

private:
  PREVENT_COPY_AND_ASSIGNMENT(VisionConfigurable);

  Log & log;

  Vision *vision;
};

#endif /* VISIONCONFIGURABLE_H_ */
