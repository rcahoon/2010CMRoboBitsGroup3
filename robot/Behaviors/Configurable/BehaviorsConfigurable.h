#ifndef BEHAVIORSCONFIGURABLE_H_
#define BEHAVIORSCONFIGURABLE_H_

#include "Behaviors/Behaviors.h"
#include "shared/UsefulMacros.h"

class BehaviorBase;
class ConfigFile;
class Log;
class Field;

class BehaviorsConfigurable: public Behaviors {
public:
  BehaviorsConfigurable(ConfigFile & configFile, Log & log, Field & field);

  virtual ~BehaviorsConfigurable();

  virtual bool run(const RobotState     & robotState,
                   const GameState      & gameState,
                   const VisionFeatures & visionFeatures,
                   const WorldFeatures  & worldFeatures,
                   const Pose           & pose,
                         Messages       & messages,
                         Command        & command,
                         Feedback       & feedback);

private:
  PREVENT_COPY_AND_ASSIGNMENT(BehaviorsConfigurable);

  Log & log;

  Behaviors *behaviors;
  BehaviorBase *remoteControl;
};

#endif /* BEHAVIORSCONFIGURABLE_H_ */
