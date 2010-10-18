#include "Comm/Messages/Remote/RemoteMessageToRobot.h"

#ifndef RESTOREHEADSTIFFNESSMESSAGE_H_
#define RESTOREHEADSTIFFNESSMESSAGE_H_

class RestoreHeadStiffnessMessage : public RemoteMessageToRobot {
public:
  RestoreHeadStiffnessMessage();
  virtual ~RestoreHeadStiffnessMessage();
};

#endif /* RESTOREHEADSTIFFNESSMESSAGE_H_ */
