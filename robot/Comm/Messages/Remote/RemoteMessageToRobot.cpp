#include "RemoteMessageToRobot.h"

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
