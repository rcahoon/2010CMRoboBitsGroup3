#include "RestoreHeadStiffnessMessage.h"

#include <cstring>

RestoreHeadStiffnessMessage::RestoreHeadStiffnessMessage()
  : RemoteMessageToRobot(RemoteMessageToRobot::restoreHeadStiffness, 0, NULL) {
}

RestoreHeadStiffnessMessage::~RestoreHeadStiffnessMessage() {
}
