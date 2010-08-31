#ifndef LOGBEGINMESSAGE_H_
#define LOGBEGINMESSAGE_H_

#include "Comm/Messages/Remote/RemoteMessageFromRobot.h"
#include "shared/UsefulMacros.h"

class LogBeginMessage : public RemoteMessageFromRobot {
public:
  LogBeginMessage(unsigned long timestamp);

  virtual ~LogBeginMessage();

private:
  PREVENT_COPY_AND_ASSIGNMENT(LogBeginMessage);

  void createData(unsigned long timestamp);

};

#endif /* LOGBEGINMESSAGE_H_ */
