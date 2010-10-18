#include "SetConfigValueMessage.h"

#include <string.h>

SetConfigValueMessage::SetConfigValueMessage(const std::string & _name, const std::string & _value)
  : RemoteMessageToRobot(RemoteMessageToRobot::setConfigValue, 0, NULL),
    name(_name),
    value(_value) {
}

SetConfigValueMessage::~SetConfigValueMessage() {
}

const std::string & SetConfigValueMessage::getName() const {
  return name;
}

const std::string & SetConfigValueMessage::getValue() const {
  return value;
}

SetConfigValueMessage const * SetConfigValueMessage::create(unsigned int size, void const *data) {
  // The last byte should be a \0
  char const *string = reinterpret_cast<char const *>(data);
  if (string[size - 1] != '\0') {
    return NULL;
  }

  unsigned int nameLength = strlen(string);
  // We should have 2 strings
  if (nameLength == size - 1) {
    return NULL;
  }
  unsigned int valueLength = strlen(string + nameLength + 1);
  // The length of the 2 strings should add up to the data size (and 2 \0s)
  if (nameLength + valueLength != size - 2) {
    return NULL;
  }

  std::string name(string);
  std::string value(string + nameLength + 1);

  return new SetConfigValueMessage(name, value);
}
