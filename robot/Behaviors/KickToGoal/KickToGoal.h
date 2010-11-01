#ifndef ROBOCUP2010_KICKTOGOAL_H_
#define ROBOCUP2010_KICKTOGOAL_H_

#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/shared/FSM.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

namespace RoboCup2010 {

class KickToGoal : public BehaviorBase {
public:
  KickToGoal(ConfigFile & configFile, Log & log);

  virtual ~KickToGoal();

  virtual bool run(BEHAVIOR_PARAMS);
  virtual bool servoToBall(BEHAVIOR_PARAMS);
  
private:
  PREVENT_COPY_AND_ASSIGNMENT(KickToGoal);
  
  enum States {
    TO_BALL,
    CHECK_GOAL,
    CENTER_GOAL,
    KICK,
    WAIT
  } state;

  Log & log;
};

} // end namespace

#endif /* ROBOCUP2010_KICKTOGOAL_H_ */

