#ifndef STARTREMOTECONTROLMESSAGE_H_
#define STARTREMOTECONTROLMESSAGE_H_

#include "Comm/Messages/Remote/RemoteMessageToRobot.h"

class StartRemoteControlMessage : public RemoteMessageToRobot {
public:
  StartRemoteControlMessage();

  virtual ~StartRemoteControlMessage();

};

#endif /* STARTREMOTECONTROLMESSAGE_H_ */
