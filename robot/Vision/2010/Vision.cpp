#include <fstream>
#include <ios>

#include "Vision/VisionFeatures.h"
#include "Vision/2010/Vision.h"
#include "Vision/2010/cmvision/vision.h"
#include "Vision/VisionObject/VisionObject.h"
#include "Agent/RobotState.h"
#include "shared/ConfigFile/ConfigFile.h"
#include "Vision/VisionObject/ConvexHullVisionObject.h"

#define COMPONENT VISION
#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

namespace RoboCup2010 {

Vision::Vision(ConfigFile & configFile, Log & _log)
  : imageWidth(configFile.getInt ("vision/imageWidth")),
    imageHeight(configFile.getInt("vision/imageHeight")),
    imagePitch(configFile.getInt ("vision/imagePitch")),
    log(_log),
    cmv(configFile, _log),
    cmvImage() {
  // Retrieve constants from the config file
  std::string colors    = configFile.getPath("vision/colorsPath");
  std::string threshold = configFile.getPath("vision/thresholdPath");

  cmvImage.width  = imageWidth;
  cmvImage.height = imageHeight;
  cmvImage.pitch = imagePitch;
  cmvImage.field = 0;

  // Initialize CMVision
  if (!cmv.init(colors.c_str(),
                threshold.c_str(),
                imageWidth,
                imageHeight)) {
    LOG_ERROR("CMVision failed to initialize.")
  }
}

Vision::~Vision() {
}

bool Vision::run(const RobotState & robotState,
                VisionFeatures & outputVisionFeatures) {
#ifdef LOG_TRACE_ACTIVE
  unsigned long startTime;
  unsigned long elapsed;
#endif

  LOG_TRACE("Vision run started.");

  // Retrieve the raw yuv pixels from camera
  char const *yuvImage = robotState.getRawImage();
  // Convert it into a CMVision image
  cmvImage.buf = (pixel *)yuvImage;

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Threshold the image using CMVision
  if (!cmv.processFrame(cmvImage)) {
    LOG_ERROR("CMVision failed to threshold the image.")
  }

#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("Thresholding took %d ms.", elapsed);

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Retrieve the transformation matrix from the camera
  HMatrix const* transformationFromCamera = &(robotState.getTransformationFromCamera());
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("Retrieving transformation matrix from RobotState took %d ms.", elapsed);

  // Insert object detectors here

  cmv.

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Log the segmented image
  LOG_SEGMENTEDIMAGE(cmv.getSegmentedImage());
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("Logging segmented image took %d ms.", elapsed);
  
  LOG_TRACE("Vision run ended.");
  return false;
}

void Vision::disableBallDetectionNextFrame() {
}

} // end namespace
