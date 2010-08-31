#ifndef ROBOTSTATEMESSAGE_H_
#define ROBOTSTATEMESSAGE_H_

#include "Comm/Messages/Remote/RemoteMessageFromRobot.h"
#include "shared/UsefulMacros.h"

class RobotState;

class RobotStateMessage : public RemoteMessageFromRobot {
public:
  RobotStateMessage(const RobotState & robotState);

  virtual ~RobotStateMessage();

private:
  PREVENT_COPY_AND_ASSIGNMENT(RobotStateMessage);

  void createData(const RobotState & robotState);
};

#endif /* ROBOTSTATEMESSAGE_H_ */
