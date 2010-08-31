#ifndef LOCALIZATIONCONFIGURABLE_H_
#define LOCALIZATIONCONFIGURABLE_H_

#include "Localization/Localization.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;
class Field;

class LocalizationConfigurable : public Localization {
public:
  LocalizationConfigurable(ConfigFile & _config, Log & _log, Field & _field);

  virtual ~LocalizationConfigurable();

  virtual bool run(const RobotState     & robotState,
                   const GameState      & gameState,
                   const VisionFeatures & visionFeatures,
                   Pose & pose);
  virtual void setScanningForGoals(bool scanning);
  virtual void updateWorldFeatures(const WorldFeatures & worldFeatures);
  virtual void reset(ResetCase resetCase);
  virtual void setUseGoalieLocalization(bool _useGoalieLocalization);
private:
  PREVENT_COPY_AND_ASSIGNMENT(LocalizationConfigurable);

  Log & log;

  Localization *localization;
};

#endif /* LOCALIZATIONCONFIGURABLE_H_ */
