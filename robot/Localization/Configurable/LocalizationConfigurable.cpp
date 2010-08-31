#include "LocalizationConfigurable.h"
#include "Localization/Null/NullLocalization.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT LOCALIZATION
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

LocalizationConfigurable::LocalizationConfigurable(ConfigFile & configFile, Log & _log, Field & _field)
  : log(_log),
    localization(NULL) {
  std::string name = configFile.getString("configurable/localization");

  // Should we use NullLocalization?
  if (name.compare("Null") == 0) {
    localization = new NullLocalization();
  }
  else {
    LOG_WARN("Configurable Localization was not defined.");
  }
}

LocalizationConfigurable::~LocalizationConfigurable() {
  if (localization != NULL) {
    delete localization;
    localization = NULL;
  }
}

bool LocalizationConfigurable::run(const RobotState     & robotState,
                                   const GameState      & gameState,
                                   const VisionFeatures & visionFeatures,
                                         Pose & pose) {
  if (localization != NULL) {
    return localization->run(robotState, gameState, visionFeatures, pose);
  }
  else {
    return false;
  }
}

void LocalizationConfigurable::setScanningForGoals(bool scanning)
{
	localization->setScanningForGoals(scanning);
}

void LocalizationConfigurable::updateWorldFeatures(const WorldFeatures & worldFeatures)
{
	localization->updateWorldFeatures(worldFeatures);
}


void LocalizationConfigurable::reset(ResetCase resetCase){
  localization->reset(resetCase);
}

void LocalizationConfigurable::setUseGoalieLocalization(bool _useGoalieLocalization){
  localization->setUseGoalieLocalization(_useGoalieLocalization);
}
