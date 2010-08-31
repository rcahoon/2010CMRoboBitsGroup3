#ifndef VISION_H_
#define VISION_H_

#include "shared/UsefulMacros.h"

class RobotState;
class VisionFeatures;

/**
 * Interface for Vision component.
 */
class Vision {
public:
  /**
   * Default constructor (empty since this is an interface).
   */
  Vision() {}

  /**
   * Destructor (empty since this is an interface).
   */
  virtual ~Vision() {}

  /**
   * Runs the Vision component, which updates the VisionFeatures.
   *
   * @param robotState state of the robot (e.g., timestamp, camera image)
   * @param visionFeatures vision objects detected
   *
   * @return whether an error occurred (false = everything is fine)
   */
  virtual bool run(const RobotState & robotState,
                         VisionFeatures & visionFeatures) = 0;

  virtual void disableBallDetectionNextFrame() {}

private:
  PREVENT_COPY_AND_ASSIGNMENT(Vision);

};

#endif /* VISION_H_ */
