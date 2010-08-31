#include <string.h>

#include "LogEndMessage.h"

LogEndMessage::LogEndMessage()
  : RemoteMessageFromRobot(RemoteMessageFromRobot::logEnd, 0, NULL) {
  createData();
}

LogEndMessage::~LogEndMessage() {
}

void LogEndMessage::createData() {
  data = new char[1];
  if (data == NULL) {
    size = 0;
    return;
  }

  data[0] = 0;
  size = 1;
}
