#ifndef NULLLOCALIZATION_H_
#define NULLLOCALIZATION_H_

#include "Localization/Localization.h"

class NullLocalization: public Localization {
public:
  NullLocalization();

  virtual ~NullLocalization();

  virtual bool run(const RobotState     & robotState,
                   const GameState      & gameState,
                   const VisionFeatures & visionFeatures,
                   Pose & pose);
  virtual void setScanningForGoals(bool scanning);
  virtual void updateWorldFeatures(const WorldFeatures & worldFeatures);
};

#endif /* NULLLOCALIZATION_H_ */
