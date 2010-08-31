#ifndef REMOTEMESSAGEFROMROBOT_H_
#define REMOTEMESSAGEFROMROBOT_H_

#include "RemoteMessage.h"
#include "shared/UsefulMacros.h"

class RemoteMessageFromRobot: public RemoteMessage {
public:

  // This should match network.message.RemoteMessageFromRobot.java
  enum MessageType {
    unknown = 0,
    logBegin,
    logEnd,
    robotState,
    originalImage,
    segmentedImage,
    logText,
    shape,
    teamColor,
  };

  RemoteMessageFromRobot(MessageType type, unsigned int size, void const * data);

  virtual ~RemoteMessageFromRobot();

  MessageType getType() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(RemoteMessageFromRobot);

  MessageType type;
};

#endif /* REMOTEMESSAGEFROMROBOT_H_ */
