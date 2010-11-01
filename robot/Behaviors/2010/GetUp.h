#ifndef ROBOCUP2010_GETUP_H_
#define ROBOCUP2010_GETUP_H_

#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/shared/FSM.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;
class MotionCommand;

namespace RoboCup2010 {

class GetUp : public BehaviorBase {
public:
  enum State {
    none,
    lifted,
    spreadOut,
    getUp,
  };
  GetUp(ConfigFile & configFile, Log & _log);
  ~GetUp();

  virtual bool run(BEHAVIOR_PARAMS);

private:
  PREVENT_COPY_AND_ASSIGNMENT(GetUp);

  void init(unsigned long timestamp);
  bool isMoving(const RobotState & robotState) const;
  bool hasFallen(const float bodyAngleX, const float bodyAngleY) const;

  const unsigned long FALL_TIME, LIFT_TIME;
  const float FACING_UP_BODY_ANGLE_Y;
  Log & log;
  FSM fsm;
  unsigned long liftedTimestamp, fallTimeStamp;
  bool fromFront;
  bool playedKeyframe;


};

} //end namespace

#endif /* ROBOCUP2010_GETUP_H_ */
