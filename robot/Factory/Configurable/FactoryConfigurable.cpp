#include "FactoryConfigurable.h"

#include "shared/ConfigFile/ConfigFile.h"
#include "shared/Field/Field.h"
#include "Log/Configurable/LogConfigurable.h"

#include "Behaviors/Configurable/BehaviorsConfigurable.h"
#include "Comm/Configurable/CommConfigurable.h"
#include "GameController/Configurable/GameControllerConfigurable.h"
#include "Localization/Configurable/LocalizationConfigurable.h"
#include "Vision/Configurable/VisionConfigurable.h"
#include "WorldModel/Configurable/WorldModelConfigurable.h"

#include "Vision/VisionFeaturesImpl.h"
#include "WorldModel/WorldFeaturesImpl.h"

FactoryConfigurable::FactoryConfigurable(const std::string & _configFilePath, const std::string & _configFileName)
  : configFilePath(_configFilePath),
    configFileName(_configFileName),
    configFile(NULL),
    log(NULL),
    field(NULL),
    behaviors(NULL),
    comm(NULL),
    gameController(NULL),
    localization(NULL),
    vision(NULL),
    worldModel(NULL),
    visionFeatures(NULL),
    worldFeatures(NULL) {
}

FactoryConfigurable::~FactoryConfigurable() {
  if (behaviors != NULL) {
    delete behaviors;
  }
  if (comm != NULL) {
    delete comm;
  }
  if (gameController != NULL) {
    delete gameController;
  }
  if (localization != NULL) {
    delete localization;
  }
  if (vision != NULL) {
    delete vision;
  }
  if (worldModel != NULL) {
    delete worldModel;
  }

  if (visionFeatures != NULL) {
    delete visionFeatures;
  }
  if (worldFeatures != NULL) {
    delete worldFeatures;
  }

  if (configFile != NULL) {
    delete configFile;
  }
  if (field != NULL) {
    delete field;
  }
  if (log != NULL) {
    delete log;
  }
}

ConfigFile & FactoryConfigurable::getConfigFile() {
  if (configFile == NULL) {
    configFile = new ConfigFile(configFilePath, configFileName);
  }
  return *configFile;
}

Log & FactoryConfigurable::getLog() {
  if (log == NULL) {
    log = new LogConfigurable(getConfigFile());
  }
  return *log;
}

Field & FactoryConfigurable::getField() {
  if (field == NULL) {
    field = new Field(getConfigFile());
  }
  return *field;
}

Behaviors & FactoryConfigurable::getBehaviors() {
  if (behaviors == NULL) {
    behaviors = new BehaviorsConfigurable(getConfigFile(), getLog(), getField());
  }
  return *behaviors;
}

Comm & FactoryConfigurable::getComm() {
  if (comm == NULL) {
    comm = new CommConfigurable(getConfigFile(), getLog());
  }
  return *comm;
}

GameController & FactoryConfigurable::getGameController() {
  if (gameController == NULL) {
    gameController = new GameControllerConfigurable(getConfigFile(), getLog());
  }
  return *gameController;
}

Localization & FactoryConfigurable::getLocalization() {
  if (localization == NULL) {
    localization = new LocalizationConfigurable(getConfigFile(), getLog(), getField());
  }
  return *localization;
}

Vision & FactoryConfigurable::getVision() {
  if (vision == NULL) {
    vision = new VisionConfigurable(getConfigFile(), getLog());
  }
  return *vision;
}

WorldModel & FactoryConfigurable::getWorldModel() {
  if (worldModel == NULL) {
    worldModel = new WorldModelConfigurable(getConfigFile(), getLog(), getField());
  }
  return *worldModel;
}

VisionFeatures & FactoryConfigurable::getVisionFeatures() {
  if (visionFeatures == NULL) {
    visionFeatures = new VisionFeaturesImpl();
  }
  return *visionFeatures;
}

WorldFeatures & FactoryConfigurable::getWorldFeatures() {
  if (worldFeatures == NULL) {
    worldFeatures = new WorldFeaturesImpl();
  }
  return *worldFeatures;
}
