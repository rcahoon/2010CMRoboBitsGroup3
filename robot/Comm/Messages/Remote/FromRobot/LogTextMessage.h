#ifndef LOGTEXTMESSAGE_H_
#define LOGTEXTMESSAGE_H_

#include "Comm/Messages/Remote/RemoteMessageFromRobot.h"
#include "shared/UsefulMacros.h"

class LogTextMessage: public RemoteMessageFromRobot {
public:
  LogTextMessage(int logLevel,
                 int component,
                 char const * className,
                 char const * functionName,
                 char const * text);

  virtual ~LogTextMessage();

private:
  PREVENT_COPY_AND_ASSIGNMENT(LogTextMessage);

  void createData(int logLevel,
                  int component,
                  char const * className,
                  char const * functionName,
                  char const * text);
};

#endif /* LOGTEXTMESSAGE_H_ */
