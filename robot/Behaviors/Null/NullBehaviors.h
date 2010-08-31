#ifndef NULLBEHAVIORS_H_
#define NULLBEHAVIORS_H_

#include "Behaviors/Behaviors.h"
#include "shared/UsefulMacros.h"

/**
 * An implementation of Behaviors that doesn't do anything.
 */
class NullBehaviors : public Behaviors {
public:
  NullBehaviors();
  virtual ~NullBehaviors();

  virtual bool run(const RobotState     & robotState,
                   const GameState      & gameState,
                   const VisionFeatures & visionFeatures,
                   const WorldFeatures  & worldFeatures,
                   const Pose           & pose,
                         Messages       & messages,
                         Command        & command,
                         Feedback       & feedback);

private:
  PREVENT_COPY_AND_ASSIGNMENT(NullBehaviors);
};

#endif /* NULLBEHAVIORS_H_ */
