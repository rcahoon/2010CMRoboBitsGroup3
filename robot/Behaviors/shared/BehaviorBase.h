#ifndef BEHAVIORBASE_H_
#define BEHAVIORBASE_H_

#define BEHAVIOR_PARAMS const RobotState & robotState, const GameState & gameState, const VisionFeatures & visionFeatures, const WorldFeatures & worldFeatures, const Pose & pose, Messages & messages, Command & command, Feedback & feedback

#define BEHAVIOR_CALL robotState, gameState, visionFeatures, worldFeatures, pose, messages, command, feedback

#include "shared/ConfigFile/ConfigFile.h"

#include "Agent/RobotState.h"
#include "GameController/GameState.h"
#include "Vision/VisionFeatures.h"
#include "WorldModel/WorldFeatures.h"
#include "Localization/Pose.h"
#include "Comm/Messages/Messages.h"
#include "Agent/Command/Command.h"
#include "Agent/Feedback/Feedback.h"

#include "Behaviors/shared/FSM.h"
#include "shared/UsefulMacros.h"
#include "shared/Vector/Vector2D.h"
#include "shared/Field/Field.h"

class ConfigFile;
class Log;

class BehaviorBase {
public:
  BehaviorBase();

  virtual ~BehaviorBase();

  virtual bool run(BEHAVIOR_PARAMS) = 0;

  virtual void reset() {}

	int getStatus();

protected:
	int status;
};

#endif // BEHAVIORBASE_H_
