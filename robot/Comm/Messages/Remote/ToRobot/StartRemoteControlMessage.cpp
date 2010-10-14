#include "StartRemoteControlMessage.h"

#include <cstring>

StartRemoteControlMessage::StartRemoteControlMessage()
  : RemoteMessageToRobot(RemoteMessageToRobot::startRemoteControl, 0, NULL) {
}

StartRemoteControlMessage::~StartRemoteControlMessage() {
}
