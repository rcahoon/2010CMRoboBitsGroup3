#ifndef TEAMCOLORMESSAGE_H_
#define TEAMCOLORMESSAGE_H_

#include "Comm/Messages/Remote/RemoteMessageFromRobot.h"
#include "shared/UsefulMacros.h"

class TeamColorMessage : public RemoteMessageFromRobot {
public:
  TeamColorMessage(bool isBlue);

  virtual ~TeamColorMessage();

private:
  PREVENT_COPY_AND_ASSIGNMENT(TeamColorMessage);

  void createData(bool isBlue);

};

#endif /* TEAMCOLORMESSAGE_H_ */
