#include <cstring>
#include <stdint.h>
#include <netinet/in.h>

#include "RemoteMessage.h"

RemoteMessage::RemoteMessage(int _type, unsigned int _size, void const * _data)
  : size(0),
    data(NULL),
    type(_type) {
  if ((_size > 0) && (_data != NULL)) {
    size = _size;
    data = new char[_size];
    memcpy(data, _data, _size);
  }
}

RemoteMessage::~RemoteMessage() {
  if (data != NULL) {
    delete [] data;
  }
}

unsigned int RemoteMessage::getSize() const {
  return size;
}

void const * RemoteMessage::getData() const {
  return data;
}

int RemoteMessage::getTypeID() const {
  return type;
}

void RemoteMessage::writeInt(char *data, int value) {
  uint32_t valueNetwork = htonl(value);
  memcpy(data, &valueNetwork, 4);
}

void RemoteMessage::writeFloat(char *data, float value) {
  uint32_t *floatBitsPointer = reinterpret_cast<uint32_t *>(&value);
  uint32_t floatBits = htonl(*floatBitsPointer);
  memcpy(data, &floatBits, 4);
}

int RemoteMessage::readInt(char const *data) {
  uint32_t valueNetwork(0);
  memcpy(&valueNetwork, data, 4);

  return ntohl(valueNetwork);
}

float RemoteMessage::readFloat(char const *data) {
  uint32_t valueNetwork(0);
  memcpy(&valueNetwork, data, 4);
  valueNetwork = ntohl(valueNetwork);
  float *floatBitsPointer = reinterpret_cast<float *>(&valueNetwork);

  return *floatBitsPointer;
}

