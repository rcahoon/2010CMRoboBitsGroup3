#ifndef REMOTEMESSAGETOROBOT_H_
#define REMOTEMESSAGETOROBOT_H_

#include "RemoteMessage.h"
#include "shared/UsefulMacros.h"

class RemoteMessageToRobot : public RemoteMessage {
public:

  enum MessageType {
    unknown = 0,
    protoBufToRobot,
    maxTypes
  };

  RemoteMessageToRobot(int _type, unsigned int _size, void const * _data);

  RemoteMessageToRobot(MessageType _type, unsigned int _size, void const * _data);

  virtual ~RemoteMessageToRobot();

  MessageType getType() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(RemoteMessageToRobot);

  MessageType type;
};

#endif /* REMOTEMESSAGETOROBOT_H_ */
