#ifndef REMOTECONTROL_H_
#define REMOTECONTROL_H_

#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/shared/FSM.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

class RemoteControl : public BehaviorBase {
public:
  RemoteControl(ConfigFile & configFile, Log & log);

  virtual ~RemoteControl();

  virtual bool run(BEHAVIOR_PARAMS);

  void reset();

private:
  PREVENT_COPY_AND_ASSIGNMENT(RemoteControl);

  ConfigFile & configFile;
  Log & log;

  bool enabled;
  bool controlCamera, useBottomCamera;
  bool setHeadStiffness;
};

#endif /* REMOTECONTROL_H_ */

