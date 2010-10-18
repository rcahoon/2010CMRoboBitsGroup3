#include "BehaviorsConfigurable.h"
#include "Behaviors/RemoteControl/RemoteControl.h"
#include "Behaviors/Null/NullBehaviors.h"
#include "Behaviors/MoveToBall/MoveToBall.h"
#include "Behaviors/ServoToBall/ServoToBall.h"
#include "Behaviors/Test/BehaviorsTest.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT BEHAVIORS
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

BehaviorsConfigurable::BehaviorsConfigurable(ConfigFile & configFile,
                                             Log & _log,
                                             Field & field)
  : log(_log),
    behaviors(NULL),
    remoteControl(NULL) {
  std::string name = configFile.getString("configurable/behaviors");

  // Should we use NullBehaviors?
  if (name.compare("Null") == 0) {
    behaviors = new NullBehaviors();
  }
  // Should we use TestBehaviors?
<<<<<<< HEAD
  if (name.compare("Test") == 0) {
    behaviors = new RoboCup2010::BehaviorsTest(configFile,_log,field);
  }
  else if (name.compare("ServoToBall") == 0) {
    behaviors = new RoboCup2010::ServoToBall(configFile,_log);
  }
  else if (name.compare("MoveToBall") == 0) {
    behaviors = new RoboCup2010::MoveToBall(configFile,_log);
  }
=======
  else if (name.compare("Test") == 0) {
    behaviors = new RoboCup2010::BehaviorsTest(configFile,_log,field);
  }
>>>>>>> 87e45019bb7721a6b4cf21672ed1d5200c75d8a2
  else {
    LOG_WARN("Configurable Behaviors was not defined.");
  }

  // Should we allow remote control?
  if (configFile.getBool("configurable/behaviors/useRemoteControl", false)) {
    remoteControl = new RemoteControl(configFile, _log);
  }
}

BehaviorsConfigurable::~BehaviorsConfigurable() {
  if (behaviors != NULL) {
    delete behaviors;
    behaviors = NULL;
  }
  if (remoteControl != NULL) {
    delete remoteControl;
    remoteControl = NULL;
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
  bool result = false;
  if (behaviors != NULL) {
    result = behaviors->run(robotState,
                            gameState,
                            visionFeatures,
                            worldFeatures,
                            pose,
                            messages,
                            command,
                            feedback);
  }
  if (remoteControl != NULL) {
    remoteControl->run(BEHAVIOR_CALL);
  }

  return result;
}
