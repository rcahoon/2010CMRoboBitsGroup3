#include "NullBehaviors.h"
#include "Agent/RobotState.h"
#include "Agent/Command/Command.h"
#include "Agent/Feedback/Feedback.h"
#include "Comm/Messages/Messages.h"
#include "GameController/GameState.h"
#include "Localization/Pose.h"
#include "Vision/VisionFeatures.h"
#include "WorldModel/WorldFeatures.h"

NullBehaviors::NullBehaviors() {
}

NullBehaviors::~NullBehaviors() {
}

bool NullBehaviors::run(const RobotState     & robotState,
                        const GameState      & gameState,
                        const VisionFeatures & visionFeatures,
                        const WorldFeatures  & worldFeatures,
                        const Pose           & pose,
                              Messages       & messages,
                              Command        & command,
                              Feedback       & feedback) {
//  command.getMotionCommand().standNeutral();
  return false;
}
