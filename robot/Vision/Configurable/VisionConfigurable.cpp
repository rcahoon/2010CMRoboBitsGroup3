#include "VisionConfigurable.h"
#include "Vision/Null/NullVision.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

VisionConfigurable::VisionConfigurable(ConfigFile & configFile, Log & _log)
  : log(_log),
    vision(NULL) {
  std::string name = configFile.getString("configurable/vision");

  // Should we use NullVision?
  if (name.compare("Null") == 0) {
    vision = new NullVision();
  }
  else {
    LOG_WARN("Configurable Vision was not defined.");
  }
}

VisionConfigurable::~VisionConfigurable() {
  if (vision != NULL) {
    delete vision;
    vision = NULL;
  }
}

bool VisionConfigurable::run(const RobotState & robotState,
                                   VisionFeatures & visionFeatures) {
  if (vision != NULL) {
    return vision->run(robotState, visionFeatures);
  }
  else {
    return false;
  }
}

void VisionConfigurable::disableBallDetectionNextFrame() {
  if (vision != NULL) {
    vision->disableBallDetectionNextFrame();
  }
}
