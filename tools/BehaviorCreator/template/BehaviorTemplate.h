#ifndef ROBOCUP2010_$CLASS_UPPER_CASE$_H_
#define ROBOCUP2010_$CLASS_UPPER_CASE$_H_

#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/shared/FSM.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

namespace RoboCup2010 {

class $CLASS$ : public BehaviorBase {
public:
  enum State {
    $FSM_STATE$,
  };

  $CLASS$(ConfigFile & configFile, Log & log);

  virtual ~$CLASS$();

  virtual bool run(BEHAVIOR_PARAMS);

  void reset();

private:
  PREVENT_COPY_AND_ASSIGNMENT($CLASS$);

  void init(unsigned long timestamp);

  Log & log;

  FSM fsm;
};

} // end namespace

#endif /* ROBOCUP2010_$CLASS_UPPER_CASE$_H_ */
