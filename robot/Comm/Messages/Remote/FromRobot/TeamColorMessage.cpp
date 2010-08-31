#include <string>

#include "TeamColorMessage.h"

TeamColorMessage::TeamColorMessage(bool isBlue)
  : RemoteMessageFromRobot(RemoteMessageFromRobot::teamColor, 0, NULL) {
  createData(isBlue);
}

TeamColorMessage::~TeamColorMessage() {
}

void TeamColorMessage::createData(bool isBlue) {
  data = new char[1];
  if (data == NULL) {
    size = 0;
    return;
  }

  data[0] = (isBlue ? 1 : 0);
  size = 1;
}
