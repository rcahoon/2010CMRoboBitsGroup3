#include "SetCameraMessage.h"

#include <cstring>

SetCameraMessage::SetCameraMessage(bool _controlCamera, bool _useBottomCamera)
  : RemoteMessageToRobot(RemoteMessageToRobot::setCamera, 0, NULL),
    controlCamera(_controlCamera),
    useBottomCamera(_useBottomCamera) {
}

SetCameraMessage::~SetCameraMessage() {
}

bool SetCameraMessage::shouldControlCamera() const {
  return controlCamera;
}

bool SetCameraMessage::shouldUseBottomCamera() const {
  return useBottomCamera;
}

SetCameraMessage const * SetCameraMessage::create(unsigned int size, void const *data) {
  // The size should be either 1 or 2
  if ((size < 1) || (size > 2)) {
    return NULL;
  }

  char const * charData = reinterpret_cast<char const *>(data);

  // The first byte is whether we should control the camera
  bool controlCamera = (charData[0] != 0);

  // If we're controlling the camera, the size should be 2
  if (controlCamera && (size != 2)) {
    return NULL;
  }
  else if (!controlCamera && (size != 1)) {
    return NULL;
  }

  // The second byte is whether we should use the bottom camera
  bool useBottomCamera = (!controlCamera) || (charData[1] != 0);

  return new SetCameraMessage(controlCamera, useBottomCamera);
}

