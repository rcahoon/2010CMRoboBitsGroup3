#include "SetHeadStiffnessMessage.h"

#include <cstring>

SetHeadStiffnessMessage::SetHeadStiffnessMessage(float _stiffness)
  : RemoteMessageToRobot(RemoteMessageToRobot::setHeadStiffness, 0, NULL),
    stiffness(_stiffness) {
}

SetHeadStiffnessMessage::~SetHeadStiffnessMessage() {
}

float SetHeadStiffnessMessage::getStiffness() const {
  return stiffness;
}

SetHeadStiffnessMessage const * SetHeadStiffnessMessage::create(unsigned int size, void const *data) {
  // We need exactly 4 bytes
  if (size != 4) {
    return NULL;
  }

  float stiffness = readFloat(reinterpret_cast<char const *>(data));
  return new SetHeadStiffnessMessage(stiffness);
}
