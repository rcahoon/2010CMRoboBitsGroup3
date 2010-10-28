#include "BehaviorsConfigurable.h"
#include "Behaviors/RemoteControl/RemoteControl.h"
#include "Behaviors/Null/NullBehaviors.h"
#include "Behaviors/Tests/TestComm.h"
#include "Behaviors/2010/Player.h"
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
  // Should we test communications?
  else if (name.compare("tests/testComm") == 0) {
    behaviors = new RoboCupTests::TestComm(configFile, _log);
  }
  // Should we run the player?
  else if (name.compare("Player") == 0) {
    behaviors = new RoboCup2010::Player(configFile, _log, field);
  }
  // Should we use TestBehaviors?
  else if (name.compare("Test") == 0) {
    behaviors = new RoboCup2010::BehaviorsTest(configFile,_log, field);
  }
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
