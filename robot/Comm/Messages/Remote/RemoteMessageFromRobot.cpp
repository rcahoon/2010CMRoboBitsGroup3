#include "RemoteMessageFromRobot.h"

RemoteMessageFromRobot::RemoteMessageFromRobot(MessageType _type,
                                               unsigned int _size,
                                               void const * _data)
  : RemoteMessage((int)_type, _size, _data),
    type(_type) {
}

RemoteMessageFromRobot::~RemoteMessageFromRobot() {
}

RemoteMessageFromRobot::MessageType RemoteMessageFromRobot::getType() const {
  return type;
}
