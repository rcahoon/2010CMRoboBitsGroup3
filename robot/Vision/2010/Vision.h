#ifndef ROBOCUP2010_VISION_H_
#define ROBOCUP2010_VISION_H_

#include "Vision/Vision.h"
#include "Vision/2010/cmvision/vision.h"


class RobotState;
class ConfigFile;
class Log;
class VisionFeatures;

namespace RoboCup2010 {

class Vision : public ::Vision {
public:
  Vision(ConfigFile & configFile, Log & log);
  ~Vision();

  bool run(const RobotState & robotState,
           VisionFeatures & outputVisionFeatures);

  virtual void disableBallDetectionNextFrame();

private:
  // Prevent copy constructor, and assignment
  Vision(const Vision &);
  Vision & operator=(const Vision &);

  // Size of image
  const int imageWidth, imageHeight, imagePitch;


  Log & log;

  // Used by CMVision
  LowVision cmv;
  vision_image cmvImage;
};

} // end namespace

#endif /* ROBOCUP2010_VISION_H_ */
