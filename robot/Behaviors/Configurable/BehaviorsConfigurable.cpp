#include "BehaviorsConfigurable.h"
#include "Behaviors/Null/NullBehaviors.h"
#include "Behaviors/Test/BehaviorsTest.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT BEHAVIORS
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

BehaviorsConfigurable::BehaviorsConfigurable(ConfigFile & configFile,
                                             Log & _log,
                                             Field & field)
  : log(_log),
    behaviors(NULL) {
  std::string name = configFile.getString("configurable/behaviors");

  // Should we use NullBehaviors?
  if (name.compare("Null") == 0) {
    behaviors = new NullBehaviors();
  }
  // Should we use TestBehaviors?
   if (name.compare("Test") == 0) {
     behaviors = new RoboCup2010::BehaviorsTest(configFile,_log,field);
   }
  else {
    LOG_WARN("Configurable Behaviors was not defined.");
  }
}

BehaviorsConfigurable::~BehaviorsConfigurable() {
  if (behaviors != NULL) {
    delete behaviors;
    behaviors = NULL;
  }
}

bool BehaviorsConfigurable::run(const RobotState     & robotState,
                                const GameState      & gameState,
                                const VisionFeatures & visionFeatures,
                                const WorldFeatures  & worldFeatures,
                                const Pose           & pose,
                                      Messages       & messages,
                                      Command        & command,
                                      Feedback       & feedback) {
  if (behaviors != NULL) {
    return behaviors->run(robotState,
                          gameState,
                          visionFeatures,
                          worldFeatures,
                          pose,
                          messages,
                          command,
                          feedback);
  }
  else {
    return false;
  }
}
