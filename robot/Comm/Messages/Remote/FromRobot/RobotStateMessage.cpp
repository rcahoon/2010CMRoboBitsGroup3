#include <sstream>
#include <boost/archive/binary_oarchive.hpp>

#include "RobotStateMessage.h"
#include "Agent/RobotState.h"

RobotStateMessage::RobotStateMessage(const RobotState & robotState)
  : RemoteMessageFromRobot(RemoteMessageFromRobot::robotState, 0, NULL) {
  createData(robotState);
}

RobotStateMessage::~RobotStateMessage() {
}

void RobotStateMessage::createData(const RobotState & robotState) {

  // Serialize the robot state
  std::ostringstream oss(std::stringstream::out | std::stringstream::binary);
  boost::archive::binary_oarchive output(oss);
  output & robotState;
  std::string str = oss.str();
  char const *serializedData = str.c_str();
  int serializedSize = str.length();

  // How many bytes do we need?
  // serialized robot state
  // image data
  size = serializedSize + robotState.getRawImageSize();

  // Generate the data in bytes
  data = new char[size];

  if (data == NULL) {
    size = 0;
    return;
  }

  int offset = 0;

  // Copy the serialized RobotState
  memcpy(data + offset, serializedData, serializedSize);
  offset += serializedSize;

  // Copy the raw image
  memcpy(data + offset, robotState.getRawImage(), robotState.getRawImageSize());

}
