#ifndef STOPREMOTECONTROLMESSAGE_H_
#define STOPREMOTECONTROLMESSAGE_H_

#include "Comm/Messages/Remote/RemoteMessageToRobot.h"

class StopRemoteControlMessage: public RemoteMessageToRobot {
public:
  StopRemoteControlMessage();

  virtual ~StopRemoteControlMessage();

};

#endif /* STOPREMOTECONTROLMESSAGE_H_ */
