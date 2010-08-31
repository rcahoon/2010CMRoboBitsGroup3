#ifndef NAOIMAGECAPTURE_H_
#define NAOIMAGECAPTURE_H_

#include <albroker.h>
#include <alerror.h>
#include <alproxy.h>
#include <alptr.h>
#include <alvision/alimage.h>

class NaoImageCapture {
public:
  NaoImageCapture(AL::ALPtr<AL::ALBroker> broker);

  NaoImageCapture(std::string ip, int port);

  ~NaoImageCapture();

  char const * getImage();

  bool isUsingBottomCamera() const;

  void setCamera(const bool useBottomCamera);

  void setCameraParameters();

  void resetCameraParameters();

private:
  void registerToVIM();

  void unregisterFromVIM();

  void cleanUpImage();

  AL::ALPtr<AL::ALProxy> camera;

  std::string gvmName;
  int         cameraResolution;
  int         cameraColorSpace;
  int         cameraFPS;

  bool         isImageClean;
  AL::ALImage *localImage;
  AL::ALValue  remoteImage;

  bool usingBottomCamera;

  bool setTopCamera, setBottomCamera;

  static const int bottomAutoGain,         topAutoGain;
  static const int bottomAutoWhiteBalance, topAutoWhiteBalance;
  static const int bottomAutoExposition,   topAutoExposition;
  static const int bottomGain,             topGain;
  static const int bottomBlueChroma,       topBlueChroma;
  static const int bottomRedChroma,        topRedChroma;
  static const int bottomBrightness,       topBrightness;
  static const int bottomContrast,         topContrast;
  static const int bottomSaturation,       topSaturation;
  static const int bottomHue,              topHue;
  static const int bottomLensCorrectionX,  topLensCorrectionX;
  static const int bottomLensCorrectionY,  topLensCorrectionY;
  static const int bottomExposure,         topExposure;
};

#endif /* NAOIMAGECAPTURE_H_ */
