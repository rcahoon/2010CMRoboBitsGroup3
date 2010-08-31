#ifndef LOGENDMESSAGE_H_
#define LOGENDMESSAGE_H_

#include "Comm/Messages/Remote/RemoteMessageFromRobot.h"
#include "shared/UsefulMacros.h"

class LogEndMessage : public RemoteMessageFromRobot {
public:
  LogEndMessage();

  virtual ~LogEndMessage();

private:
  PREVENT_COPY_AND_ASSIGNMENT(LogEndMessage);

  void createData();

};

#endif /* LOGENDMESSAGE_H_ */
