#include "Comm/Messages/Remote/RemoteMessageToRobot.h"

#ifndef SETHEADSTIFFNESSMESSAGE_H_
#define SETHEADSTIFFNESSMESSAGE_H_

class SetHeadStiffnessMessage : public RemoteMessageToRobot {
public:
  SetHeadStiffnessMessage(float stiffness);
  virtual ~SetHeadStiffnessMessage();

  float getStiffness() const;

  static SetHeadStiffnessMessage const * create(unsigned int size, void const *data);

private:
  float stiffness;

};

#endif /* SETHEADSTIFFNESSMESSAGE_H_ */
