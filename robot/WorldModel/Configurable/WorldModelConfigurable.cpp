#include "WorldModelConfigurable.h"
#include "WorldModel/Null/NullWorldModel.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT WORLD_MODEL
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

WorldModelConfigurable::WorldModelConfigurable(ConfigFile & configFile,
                                               Log & _log,
                                               Field & field)
  : log(_log),
    worldModel(NULL) {
  std::string name = configFile.getString("configurable/worldModel");

  // Should we use NullWorldModel?
  if (name.compare("Null") == 0) {
    worldModel = new NullWorldModel();
  }
  else {
    LOG_WARN("Configurable World Model was not defined.");
  }
}

WorldModelConfigurable::~WorldModelConfigurable() {
  if (worldModel != NULL) {
    delete worldModel;
    worldModel = NULL;
  }
}

bool WorldModelConfigurable::run(const RobotState & robotState,
                                 const GameState & gameState,
                                 const VisionFeatures & visionFeatures,
                                 const Pose & pose,
                                       Messages & messages,
                                       WorldFeatures & worldFeatures) {
  if (worldModel != NULL) {
    return worldModel->run(robotState,
                           gameState,
                           visionFeatures,
                           pose,
                           messages,
                           worldFeatures);
  }
  else {
    return false;
  }
}

void WorldModelConfigurable::setScanningForGoals(bool scanningForGoals) {
  if (worldModel != NULL) {
    worldModel->setScanningForGoals(scanningForGoals);
  }
}

void WorldModelConfigurable::setScanningForObstacles(bool scanningForObstacles) {
  if (worldModel != NULL) {
    worldModel->setScanningForObstacles(scanningForObstacles);
  }
}

void WorldModelConfigurable::setClearFreeSpaceModel(bool clearFreeSpaceModel) {
  if (worldModel != NULL) {
    worldModel->setClearFreeSpaceModel(clearFreeSpaceModel);
  }
}

void WorldModelConfigurable::setRobotConditions(bool gettingUp, bool lifted) {
  if (worldModel != NULL) {
    worldModel->setRobotConditions(gettingUp, lifted);
  }
}

void WorldModelConfigurable::addKickEffect(const Vector2D & kickEffect) {
  if (worldModel != NULL) {
    worldModel->addKickEffect(kickEffect);
  }
}

void WorldModelConfigurable::invalidateBall() {
  if (worldModel != NULL) {
    worldModel->invalidateBall();
  }
}
