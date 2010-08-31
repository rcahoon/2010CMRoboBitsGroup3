#include <string.h>

#include "LogBeginMessage.h"

LogBeginMessage::LogBeginMessage(unsigned long timestamp)
  : RemoteMessageFromRobot(RemoteMessageFromRobot::logBegin, 0, NULL) {
  createData(timestamp);
}

LogBeginMessage::~LogBeginMessage() {
}

void LogBeginMessage::createData(unsigned long timestamp) {
  data = new char[4];
  if (data == NULL) {
    size = 0;
    return;
  }

  writeInt(data, timestamp);
  size = 4;
}
