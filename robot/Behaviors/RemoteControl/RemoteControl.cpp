#include "RemoteControl.h"
#include "Comm/Messages/Remote/RemoteMessageToRobot.h"
#include "Comm/Messages/Remote/ToRobot/SetCameraMessage.h"
#include "Comm/Messages/Remote/ToRobot/SetConfigValueMessage.h"
#include "Comm/Messages/Remote/ToRobot/SetHeadStiffnessMessage.h"
#include "shared/ConfigFile/ConfigFile.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

RemoteControl::RemoteControl(ConfigFile & _configFile, Log & _log)
  : BehaviorBase(),
    configFile(_configFile),
    log(_log),
    enabled(false),
    controlCamera(false),
    useBottomCamera(false),
    setHeadStiffness(false) {
}

RemoteControl::~RemoteControl() {
}

void RemoteControl::reset() {
  enabled = false;

  controlCamera = false;
  useBottomCamera = true;
}

bool RemoteControl::run(BEHAVIOR_PARAMS) {

  const std::vector<RemoteMessageToRobot const *> & remoteMessages = messages.getReceivedRemoteMessages();
  std::vector<RemoteMessageToRobot const *>::const_iterator iter;

  // Look through the messages to see if we should enable or disable remote
  // control, or change values in the config file
  for (iter = remoteMessages.begin(); iter != remoteMessages.end(); iter++) {
    RemoteMessageToRobot const * message = *iter;

    if (message->getType() == RemoteMessageToRobot::startRemoteControl) {
      reset();
      enabled = true;
      LOG_INFO("Remote control ENABLED.");
    }
    else if (message->getType() == RemoteMessageToRobot::stopRemoteControl) {
      reset();
      enabled = false;
      LOG_INFO("Remote control DISABLED.");
    }
    // Should we change a value in the config file?
    else if (message->getType() == RemoteMessageToRobot::setConfigValue) {
      SetConfigValueMessage const *setConfigValueMessage = dynamic_cast<SetConfigValueMessage const *>(message);
      if (setConfigValueMessage == NULL) {
        continue;
      }

      configFile.setString(setConfigValueMessage->getName(), setConfigValueMessage->getValue());
      LOG_DEBUG("Set config file: %s = %s", setConfigValueMessage->getName().c_str(), setConfigValueMessage->getValue().c_str());
    }
  }

  // If we're not enabled, don't do anything
  if (!enabled) {
    // If we set the head stiffness previously, reset it
    if (setHeadStiffness) {
      setHeadStiffness = false;
      command.getMotionCommand().restoreHeadStiffness();
    }
    return false;
  }

  // Look through the messages to remote control the robot
  for (iter = remoteMessages.begin(); iter != remoteMessages.end(); iter++) {
    RemoteMessageToRobot const * message = *iter;

    if (message->getType() == RemoteMessageToRobot::setCamera) {
      SetCameraMessage const *setCameraMessage = dynamic_cast<SetCameraMessage const *>(message);
      if (setCameraMessage == NULL) {
        continue;
      }
      controlCamera = setCameraMessage->shouldControlCamera();
      if (controlCamera) {
        useBottomCamera = setCameraMessage->shouldUseBottomCamera();
        LOG_DEBUG("Use bottom camera: %d.", useBottomCamera);
      }
    }
    else if (message->getType() == RemoteMessageToRobot::setHeadStiffness) {
      SetHeadStiffnessMessage const *setHeadStiffnessMessage = dynamic_cast<SetHeadStiffnessMessage const *>(message);
      if (setHeadStiffnessMessage == NULL) {
        continue;
      }
      setHeadStiffness = true;
      command.getMotionCommand().setHeadStiffness(setHeadStiffnessMessage->getStiffness());
      LOG_DEBUG("Head stiffness set to %f.", setHeadStiffnessMessage->getStiffness());
    }
    else if (message->getType() == RemoteMessageToRobot::restoreHeadStiffness) {
      setHeadStiffness = false;
      command.getMotionCommand().restoreHeadStiffness();
      LOG_DEBUG("Head stiffness restored.");
    }
  }

  if (controlCamera) {
    command.useBottomCamera(useBottomCamera);
  }

  return false;
}
