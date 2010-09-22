#ifndef ROBOCUP2010_SERVOTOBALL_H_
#define ROBOCUP2010_SERVOTOBALL_H_

#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/shared/FSM.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

namespace RoboCup2010 {

class ServoToBall : public BehaviorBase {
public:
  ServoToBall(ConfigFile & configFile, Log & log);

  virtual ~ServoToBall();

  virtual bool run(BEHAVIOR_PARAMS);
  
private:
  PREVENT_COPY_AND_ASSIGNMENT(ServoToBall);
  
  int searchState;

  Log & log;
};

} // end namespace

#endif /* ROBOCUP2010_SERVOTOBALL_H_ */

