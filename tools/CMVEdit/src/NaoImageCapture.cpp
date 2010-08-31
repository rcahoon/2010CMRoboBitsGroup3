#include <alvision/alvisiondefinitions.h>

#include "NaoImageCapture.h"
#include "ConfigFile.h"

// Disable log commands
#define LOG_TRACE(format, ...)
#define LOG_DEBUG(format, ...)
#define LOG_INFO(format, ...)
#define LOG_WARN(format, ...)
#define LOG_ERROR(format, ...)

NaoImageCapture::NaoImageCapture(AL::ALPtr<AL::ALBroker> broker)
  : camera(),
    gvmName(ConfigFile::getSingleton().getString("naoImage/gvmName")),
    cameraResolution(AL::kQVGA), // Override below
    cameraColorSpace(AL::kYUVColorSpace),     // Override below
    cameraFPS(ConfigFile::getSingleton().getInt("naoImage/cameraFPS")),
    isImageClean(true),
    localImage(NULL),
    remoteImage(),
    usingBottomCamera(false),
    setTopCamera(false),
    setBottomCamera(false) {

  // Convert the strings in the config file into Aldebaran values
  std::string resolution = ConfigFile::getSingleton().getString("naoImage/cameraResolution");
  if (resolution == "320x240") {
    cameraResolution = AL::kQVGA;
  }
  else if (resolution == "640x480") {
    cameraResolution = AL::kVGA;
  }
  else if (resolution == "160x120") {
    cameraResolution = AL::kQQVGA;
  }
  // Default resolution
  else {
    cameraResolution = AL::kQVGA;
  }
  std::string colorSpace = ConfigFile::getSingleton().getString("naoImage/cameraColorSpace");
  if (colorSpace == "YUV") {
    cameraColorSpace = AL::kYUVColorSpace;
  }
  else if (colorSpace == "YUV422") {
    cameraColorSpace = AL::kYUV422InterlacedColorSpace;
  }

  //Create a proxy to the video input module
  try {
    camera = broker->getProxy("ALVideoDevice");
    LOG_INFO("Created a proxy to ALVideoDevice.")
  }
  catch(const AL::ALError &error) {
    LOG_ERROR("Could not create a proxy to ALVideoDevice.\n%s", error.toString().c_str())
  }

  registerToVIM();

  setCameraParameters();

  // Setting of camera parameters may have switched cameras, so we set it to
  // the default
  setCamera(ConfigFile::getSingleton().getInt("behaviors/defaultUseBottomCamera"));

  remoteImage.arraySetSize(7);
}

NaoImageCapture::NaoImageCapture(std::string ip, int port)
  : camera(),
    gvmName(ConfigFile::getSingleton().getString("naoImage/gvmName")),
    cameraResolution(AL::kQVGA), // Override below
    cameraColorSpace(AL::kYUVColorSpace),     // Override below
    cameraFPS(ConfigFile::getSingleton().getInt("naoImage/cameraFPS")),
    isImageClean(true),
    localImage(NULL),
    remoteImage(),
    usingBottomCamera(false) {

  // Convert the strings in the config file into Aldebaran values
  std::string resolution = ConfigFile::getSingleton().getString("naoImage/cameraResolution");
  if (resolution == "320x240") {
    cameraResolution = AL::kQVGA;
  }
  else if (resolution == "640x480") {
    cameraResolution = AL::kVGA;
  }
  else if (resolution == "160x120") {
    cameraResolution = AL::kQQVGA;
  }
  // Default resolution
  else {
    cameraResolution = AL::kQVGA;
  }
  std::string colorSpace = ConfigFile::getSingleton().getString("naoImage/cameraColorSpace");
  if (colorSpace == "YUV") {
    cameraColorSpace = AL::kYUVColorSpace;
  }
  else if (colorSpace == "YUV422") {
    cameraColorSpace = AL::kYUV422InterlacedColorSpace;
  }

  //Create a proxy to the video input module
  try {
    //TODO: Use a specialized proxy
    camera = AL::ALPtr<AL::ALProxy>(new AL::ALProxy("ALVideoDevice", ip, port));
    LOG_INFO("Created a proxy to ALVideoDevice.")
  }
  catch(const AL::ALError &error) {
    LOG_ERROR("Could not create a proxy to ALVideoDevice.\n%s", error.toString().c_str())
  }

  registerToVIM();

  setCameraParameters();

  // Setting of camera parameters may have switched cameras, so we set it to
  // the default
  setCamera(ConfigFile::getSingleton().getInt("behaviors/defaultUseBottomCamera"));
}

NaoImageCapture::~NaoImageCapture() {
  cleanUpImage();
  unregisterFromVIM();
}

char const * NaoImageCapture::getImage() {
  LOG_TRACE("getImage started: %d", Log::getTimestamp())

  char *ret = NULL;

#ifndef REMOTE
  LOG_DEBUG("Calling getImageLocal")

  LOG_TRACE("Cleaning image: %d", Log::getTimestamp())

  if (!isImageClean) {
    cleanUpImage();
  }

  LOG_TRACE("Finished cleaning image: %d", Log::getTimestamp())

  localImage = NULL;

  LOG_TRACE("Calling getImageLocal: %d", Log::getTimestamp())

  // You get the pointer to the video structure.
  try {
    localImage = (AL::ALVisionImage *)(camera->call<int>("getDirectRawImageLocal", gvmName));
    isImageClean = false;
    ret = (char *)localImage->getFrame();
  }
  catch (const AL::ALError &error) {
    LOG_ERROR("Could not call getImageLocal of the NaoCam module.\n%s", error.toString().c_str())
    ret = NULL;
  }

  LOG_TRACE("Finished retrieving local image: %d", Log::getTimestamp())

#else // REMOTE
  LOG_DEBUG("Calling getImageRemote")

  // The module is not running on the robot

  LOG_TRACE("Calling getImageRemote: %d", Log::getTimestamp())

  // You get the result as an ALValue.
  try {
    // hopefully clear out buffer somewhat...
    remoteImage = camera->call<AL::ALValue>("getImageRemote", gvmName);
    remoteImage = camera->call<AL::ALValue>("getImageRemote", gvmName);
    // Warning, this pointer will only be valid as long as the remoteImage
    // (ALValue) is alive.
    ret = (char *)remoteImage[6].GetBinary();
  }
  catch (const AL::ALError &error) {
    LOG_ERROR("Could not call getImageRemote of the NaoCam module.\n%s", error.toString().c_str())
    ret = NULL;
  }

  LOG_TRACE("Finished retrieving remote image: %d", Log::getTimestamp())

#endif // REMOTE/not REMOTE

  LOG_TRACE("getImage ended: %d", Log::getTimestamp())

  return ret;
}

bool NaoImageCapture::isUsingBottomCamera() const {
  return usingBottomCamera;
}

void NaoImageCapture::setCamera(const bool useBottomCamera) {
  if (useBottomCamera == usingBottomCamera) {
    return;
  }

  // Clean the image before changing cameras
  cleanUpImage();

  usingBottomCamera = useBottomCamera;

  try {
    camera->callVoid("setParam", AL::kCameraSelectID, (int)useBottomCamera);
    LOG_INFO("Set the camera to %d.", useBottomCamera)

    resetCameraParameters();
//    if (useBottomCamera && !setBottomCamera) {
//      resetCameraParameters();
//      setBottomCamera = true;
//    }
//    else if (!useBottomCamera && !setTopCamera) {
//      resetCameraParameters();
//      setTopCamera = true;
//    }

  }
  catch(const AL::ALError &error) {
    LOG_ERROR("Could not set the camera to %d.\n%s", useBottomCamera, error.toString().c_str())
  }

}

void NaoImageCapture::registerToVIM() {
  //You only have to call the function "subscribe" with required parameters for your process
  try {
    gvmName = camera->call<std::string>("subscribe", gvmName, cameraResolution, cameraColorSpace, cameraFPS );
    LOG_INFO("Registered the class with the NaoCam module.")
  }
  catch(const AL::ALError &error) {
    LOG_ERROR("Could not call the register method of the NaoCam module.\n%s", error.toString().c_str())
  }
}

void NaoImageCapture::unregisterFromVIM() {
  try {
    camera->callVoid("unsubscribe", gvmName);
    LOG_INFO("Unregistered the class with the NaoCam module.")
  }
  catch(const AL::ALError &error) {
    LOG_ERROR("Could not call the unsubscribe method of the NaoCam module.\n%s", error.toString().c_str())
  }
}

void NaoImageCapture::cleanUpImage() {
#ifndef REMOTE
  if (isImageClean) {
    return;
  }

  try {
    camera->call<int>("releaseDirectRawImage", gvmName);
    localImage = NULL;
  }
  catch(const AL::ALError &error) {
    LOG_ERROR("Could not release image of NaoCam module.\n%s", error.toString().c_str())
  }

  isImageClean = true;
#endif
}

void NaoImageCapture::resetCameraParameters() {
  try {
    // Auto gain
    camera->callVoid("setParam", AL::kCameraAutoGainID, bottomAutoGain);
    // Auto white balance
    camera->callVoid("setParam", AL::kCameraAutoWhiteBalanceID, bottomAutoWhiteBalance);
    // Auto exposure
    camera->callVoid("setParam", AL::kCameraAutoExpositionID, bottomAutoExposition);
    // Gain
    camera->callVoid("setParam", AL::kCameraGainID, bottomGain);
    // Blue chroma
    camera->callVoid("setParam", AL::kCameraBlueChromaID, bottomBlueChroma);
    // Red chroma
    camera->callVoid("setParam", AL::kCameraRedChromaID, bottomRedChroma);
    // Brightness
    camera->callVoid("setParam", AL::kCameraBrightnessID, bottomBrightness);
    // Contrast
    camera->callVoid("setParam", AL::kCameraContrastID, bottomContrast);
    // Saturation
    camera->callVoid("setParam", AL::kCameraSaturationID, bottomSaturation);
    // Hue
    camera->callVoid("setParam", AL::kCameraHueID, bottomHue);
    // Lens correction X
    camera->callVoid("setParam", AL::kCameraLensXID, bottomLensCorrectionX);
    // Lens correction Y
    camera->callVoid("setParam", AL::kCameraLensYID, bottomLensCorrectionY);
    // Exposure
    camera->callVoid("setParam", AL::kCameraExposureID, bottomExposure);
  }
  catch(const AL::ALError &error) {
    LOG_ERROR("Could not set parameters of the camera.\n%s", error.toString().c_str())
  }
}

void NaoImageCapture::setCameraParameters() {
  // Set up bottom camera
  try {
    // Set the camera to the bottom
    camera->callVoid("setParam", AL::kCameraSelectID, (int)true);

    // Auto gain
    camera->callVoid("setParam", AL::kCameraAutoGainID, bottomAutoGain);
    // Auto white balance
    camera->callVoid("setParam", AL::kCameraAutoWhiteBalanceID, bottomAutoWhiteBalance);
    // Auto exposure
    camera->callVoid("setParam", AL::kCameraAutoExpositionID, bottomAutoExposition);
    // Gain
    camera->callVoid("setParam", AL::kCameraGainID, bottomGain);
    // Blue chroma
    camera->callVoid("setParam", AL::kCameraBlueChromaID, bottomBlueChroma);
    // Red chroma
    camera->callVoid("setParam", AL::kCameraRedChromaID, bottomRedChroma);
    // Brightness
    camera->callVoid("setParam", AL::kCameraBrightnessID, bottomBrightness);
    // Contrast
    camera->callVoid("setParam", AL::kCameraContrastID, bottomContrast);
    // Saturation
    camera->callVoid("setParam", AL::kCameraSaturationID, bottomSaturation);
    // Hue
    camera->callVoid("setParam", AL::kCameraHueID, bottomHue);
    // Lens correction X
    camera->callVoid("setParam", AL::kCameraLensXID, bottomLensCorrectionX);
    // Lens correction Y
    camera->callVoid("setParam", AL::kCameraLensYID, bottomLensCorrectionY);
    // Exposure
    camera->callVoid("setParam", AL::kCameraExposureID, bottomExposure);
  }
  catch(const AL::ALError &error) {
    LOG_ERROR("Could not set parameters of the bottom camera.\n%s", error.toString().c_str())
  }
  LOG_INFO("Set the parameters of the bottom camera.")

  // Set up top camera
  try {
    // Set the camera to the top
    camera->callVoid("setParam", AL::kCameraSelectID, (int)false);

    // Auto gain
    camera->callVoid("setParam", AL::kCameraAutoGainID, topAutoGain);
    // Auto white balance
    camera->callVoid("setParam", AL::kCameraAutoWhiteBalanceID, topAutoWhiteBalance);
    // Auto exposure
    camera->callVoid("setParam", AL::kCameraAutoExpositionID, topAutoExposition);
    // Gain
    camera->callVoid("setParam", AL::kCameraGainID, topGain);
    // Blue chroma
    camera->callVoid("setParam", AL::kCameraBlueChromaID, topBlueChroma);
    // Red chroma
    camera->callVoid("setParam", AL::kCameraRedChromaID, topRedChroma);
    // Brightness
    camera->callVoid("setParam", AL::kCameraBrightnessID, topBrightness);
    // Contrast
    camera->callVoid("setParam", AL::kCameraContrastID, topContrast);
    // Saturation
    camera->callVoid("setParam", AL::kCameraSaturationID, topSaturation);
    // Hue
    camera->callVoid("setParam", AL::kCameraHueID, topHue);
    // Lens correction X
    camera->callVoid("setParam", AL::kCameraLensXID, topLensCorrectionX);
    // Lens correction Y
    camera->callVoid("setParam", AL::kCameraLensYID, topLensCorrectionY);
    // Exposure
    camera->callVoid("setParam", AL::kCameraExposureID, topExposure);
  }
  catch(const AL::ALError &error) {
    LOG_ERROR("Could not set parameters of the top camera.\n%s", error.toString().c_str())
  }
  LOG_INFO("Set the parameters of the top camera.")

  // We last used the top camera
  usingBottomCamera = false;

}

const int NaoImageCapture::bottomAutoGain(ConfigFile::getSingleton().getInt("naoImage/autoGain"));
const int NaoImageCapture::bottomAutoWhiteBalance(ConfigFile::getSingleton().getInt("naoImage/autoWhiteBalance"));
const int NaoImageCapture::bottomAutoExposition(ConfigFile::getSingleton().getInt("naoImage/autoExposition"));
const int NaoImageCapture::bottomGain(ConfigFile::getSingleton().getInt("naoImage/gain"));
const int NaoImageCapture::bottomBlueChroma(ConfigFile::getSingleton().getInt("naoImage/blueChroma"));
const int NaoImageCapture::bottomRedChroma(ConfigFile::getSingleton().getInt("naoImage/redChroma"));
const int NaoImageCapture::bottomBrightness(ConfigFile::getSingleton().getInt("naoImage/brightness"));
const int NaoImageCapture::bottomContrast(ConfigFile::getSingleton().getInt("naoImage/contrast"));
const int NaoImageCapture::bottomSaturation(ConfigFile::getSingleton().getInt("naoImage/saturation"));
const int NaoImageCapture::bottomHue(ConfigFile::getSingleton().getInt("naoImage/hue"));
const int NaoImageCapture::bottomLensCorrectionX(ConfigFile::getSingleton().getInt("naoImage/lensCorrectionX"));
const int NaoImageCapture::bottomLensCorrectionY(ConfigFile::getSingleton().getInt("naoImage/lensCorrectionY"));
const int NaoImageCapture::bottomExposure(ConfigFile::getSingleton().getInt("naoImage/exposure"));

const int NaoImageCapture::topAutoGain(ConfigFile::getSingleton().getInt("naoImage/autoGain"));
const int NaoImageCapture::topAutoWhiteBalance(ConfigFile::getSingleton().getInt("naoImage/autoWhiteBalance"));
const int NaoImageCapture::topAutoExposition(ConfigFile::getSingleton().getInt("naoImage/autoExposition"));
const int NaoImageCapture::topGain(ConfigFile::getSingleton().getInt("naoImage/gain"));
const int NaoImageCapture::topBlueChroma(ConfigFile::getSingleton().getInt("naoImage/blueChroma"));
const int NaoImageCapture::topRedChroma(ConfigFile::getSingleton().getInt("naoImage/redChroma"));
const int NaoImageCapture::topBrightness(ConfigFile::getSingleton().getInt("naoImage/brightness"));
const int NaoImageCapture::topContrast(ConfigFile::getSingleton().getInt("naoImage/contrast"));
const int NaoImageCapture::topSaturation(ConfigFile::getSingleton().getInt("naoImage/saturation"));
const int NaoImageCapture::topHue(ConfigFile::getSingleton().getInt("naoImage/hue"));
const int NaoImageCapture::topLensCorrectionX(ConfigFile::getSingleton().getInt("naoImage/lensCorrectionX"));
const int NaoImageCapture::topLensCorrectionY(ConfigFile::getSingleton().getInt("naoImage/lensCorrectionY"));
const int NaoImageCapture::topExposure(ConfigFile::getSingleton().getInt("naoImage/exposure"));
