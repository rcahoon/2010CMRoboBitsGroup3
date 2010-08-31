#include "NullFactory.h"

#include "shared/ConfigFile/ConfigFile.h"
#include "shared/Field/Field.h"
#include "Log/Null/NullLog.h"

#include "Behaviors/Null/NullBehaviors.h"
#include "Comm/Null/NullComm.h"
#include "GameController/AlwaysPlaying/GameControllerAlwaysPlaying.h"
#include "Localization/Null/NullLocalization.h"
#include "Vision/Null/NullVision.h"
#include "WorldModel/Null/NullWorldModel.h"

#include "Vision/VisionFeaturesImpl.h"
#include "WorldModel/WorldFeaturesImpl.h"

NullFactory::NullFactory(const std::string & _configFilePath, const std::string & _configFileName)
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

NullFactory::~NullFactory() {
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

ConfigFile & NullFactory::getConfigFile() {
  if (configFile == NULL) {
    configFile = new ConfigFile(configFilePath, configFileName);
  }
  return *configFile;
}

Log & NullFactory::getLog() {
  if (log == NULL) {
    log = new NullLog(getConfigFile());
  }
  return *log;
}

Field & NullFactory::getField() {
  if (field == NULL) {
    field = new Field(getConfigFile());
  }
  return *field;
}

Behaviors & NullFactory::getBehaviors() {
  if (behaviors == NULL) {
    behaviors = new NullBehaviors();
  }
  return *behaviors;
}

Comm & NullFactory::getComm() {
  if (comm == NULL) {
    comm = new NullComm();
  }
  return *comm;
}

GameController & NullFactory::getGameController() {
  if (gameController == NULL) {
    gameController = new GameControllerAlwaysPlaying(getConfigFile(), getLog());
  }
  return *gameController;
}

Localization & NullFactory::getLocalization() {
  if (localization == NULL) {
    localization = new NullLocalization();
  }
  return *localization;
}

Vision & NullFactory::getVision() {
  if (vision == NULL) {
    vision = new NullVision();
  }
  return *vision;
}

WorldModel & NullFactory::getWorldModel() {
  if (worldModel == NULL) {
    worldModel = new NullWorldModel();
  }
  return *worldModel;
}

VisionFeatures & NullFactory::getVisionFeatures() {
  if (visionFeatures == NULL) {
    visionFeatures = new VisionFeaturesImpl();
  }
  return *visionFeatures;
}

WorldFeatures & NullFactory::getWorldFeatures() {
  if (worldFeatures == NULL) {
    worldFeatures = new WorldFeaturesImpl();
  }
  return *worldFeatures;
}
