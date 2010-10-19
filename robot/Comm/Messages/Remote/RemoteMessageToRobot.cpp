#include "RemoteMessageToRobot.h"
#include "Comm/Messages/Remote/ToRobot/SetCameraMessage.h"
#include "Comm/Messages/Remote/ToRobot/StartRemoteControlMessage.h"
#include "Comm/Messages/Remote/ToRobot/StopRemoteControlMessage.h"
#include "Comm/Messages/Remote/ToRobot/SetConfigValueMessage.h"
#include "Comm/Messages/Remote/ToRobot/SetHeadStiffnessMessage.h"
#include "Comm/Messages/Remote/ToRobot/RestoreHeadStiffnessMessage.h"

#include <cstring>

RemoteMessageToRobot::RemoteMessageToRobot(int _type, unsigned int _size, void const * _data)
  : RemoteMessage(_type, _size, _data),
    type(unknown) {
  if ((_type >= 0) && (_type < RemoteMessageToRobot::maxTypes)) {
    type = (RemoteMessageToRobot::MessageType)_type;
  }
}

RemoteMessageToRobot::RemoteMessageToRobot(MessageType _type, unsigned int _size, void const * _data)
  : RemoteMessage((int)_type, _size, _data),
    type(_type) {
}

RemoteMessageToRobot::~RemoteMessageToRobot() {
}

RemoteMessageToRobot::MessageType RemoteMessageToRobot::getType() const {
  return type;
}

RemoteMessageToRobot const * RemoteMessageToRobot::create(int typeInt, unsigned int size, void const * data) {
  RemoteMessageToRobot::MessageType type = ((typeInt < 0) || (typeInt >= RemoteMessageToRobot::maxTypes)) ? RemoteMessageToRobot::unknown : (RemoteMessageToRobot::MessageType)typeInt;

  switch (type) {
  case RemoteMessageToRobot::startRemoteControl:
    return new StartRemoteControlMessage();
    break;
  case RemoteMessageToRobot::stopRemoteControl:
    return new StopRemoteControlMessage();
    break;
  case RemoteMessageToRobot::setCamera:
    return SetCameraMessage::create(size, data);
    break;
  case RemoteMessageToRobot::setConfigValue:
    return SetConfigValueMessage::create(size, data);
  case RemoteMessageToRobot::setHeadStiffness:
    return SetHeadStiffnessMessage::create(size, data);
  case RemoteMessageToRobot::restoreHeadStiffness:
    return new RestoreHeadStiffnessMessage();
  default:
    if (size > 0) {
      return new RemoteMessageToRobot(RemoteMessageToRobot::unknown, size, data);
    }
    else {
      return new RemoteMessageToRobot(RemoteMessageToRobot::unknown, 0, NULL);
    }
  }
}
