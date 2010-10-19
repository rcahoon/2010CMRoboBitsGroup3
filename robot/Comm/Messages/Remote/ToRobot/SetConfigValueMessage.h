#ifndef SETCONFIGVALUEMESSAGE_H_
#define SETCONFIGVALUEMESSAGE_H_

#include "Comm/Messages/Remote/RemoteMessageToRobot.h"

#include <string>

class SetConfigValueMessage : public RemoteMessageToRobot {
public:
  SetConfigValueMessage(const std::string & name, const std::string & value);

  virtual ~SetConfigValueMessage();

  const std::string & getName() const;

  const std::string & getValue() const;

  static SetConfigValueMessage const * create(unsigned int size, void const *data);

private:
  std::string name, value;

};

#endif /* SETCONFIGVALUEMESSAGE_H_ */
