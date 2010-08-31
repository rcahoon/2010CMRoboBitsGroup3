#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include "shared/UsefulMacros.h"

class RobotState;
class GameState;
class VisionFeatures;
class WorldFeatures;
class Pose;

class Localization {
public:
  enum ResetCase {
    none,
    ourPenaltyBox,
    opponentPenaltyBox,
    goalieStartInPenaltyBox,
    opponentGoalieStartInPenaltyBox,
    backFromPenalty,
    fall,
    lifted,
    dribblerPosition,
    goaliePenaltyKick,
    attackerPenaltyKick,
  };

  Localization() {};
  virtual ~Localization() {};

  virtual bool run(const RobotState     & robotState,
                   const GameState      & gameState,
                   const VisionFeatures & visionFeatures,
                   Pose & pose) = 0;
  virtual void setScanningForGoals(bool scanning) {};
  virtual void updateWorldFeatures(const WorldFeatures & worldFeatures) {};

  virtual void reset(ResetCase resetCase) {};
  virtual void setUseGoalieLocalization(bool _useGoalieLocalization) {};
private:
  PREVENT_COPY_AND_ASSIGNMENT(Localization);

};

#endif /* LOCALIZATION_H_ */
