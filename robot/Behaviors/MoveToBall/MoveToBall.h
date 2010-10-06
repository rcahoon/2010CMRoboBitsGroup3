#ifndef ROBOCUP2010_MOVETOBALL_H_
#define ROBOCUP2010_MOVETOBALL_H_

#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/shared/FSM.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

namespace RoboCup2010 {

class MoveToBall : public BehaviorBase {
public:
  enum State {
    Search,
    Center,
    Approach,
    Victory
  };

  MoveToBall(ConfigFile & configFile, Log & log);

  virtual ~MoveToBall();

  virtual bool run(BEHAVIOR_PARAMS);

  void reset();

private:
  PREVENT_COPY_AND_ASSIGNMENT(MoveToBall);

  void init(unsigned long timestamp);

  Log & log;

  FSM fsm;
};

} // end namespace

#endif /* ROBOCUP2010_MOVETOBALL_H_ */
