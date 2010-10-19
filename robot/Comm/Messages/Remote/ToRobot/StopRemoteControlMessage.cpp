#include "StopRemoteControlMessage.h"

#include <cstring>

StopRemoteControlMessage::StopRemoteControlMessage()
  : RemoteMessageToRobot(RemoteMessageToRobot::stopRemoteControl, 0, NULL) {
}

StopRemoteControlMessage::~StopRemoteControlMessage() {
}
