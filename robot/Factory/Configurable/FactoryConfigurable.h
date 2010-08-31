#ifndef FACTORYCONFIGURABLE_H_
#define FACTORYCONFIGURABLE_H_

#include <string>

#include "Factory/Factory.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;
class Field;

class Behaviors;
class Comm;
class GameController;
class Localization;
class Vision;
class WorldModel;

class VisionFeatures;
class WorldFeatures;

class FactoryConfigurable : public Factory {
public:
  FactoryConfigurable(const std::string & configFilePath, const std::string & configFileName);
  virtual ~FactoryConfigurable();

  virtual ConfigFile & getConfigFile();
  virtual Log        & getLog();
  virtual Field      & getField();

  virtual Behaviors      & getBehaviors();
  virtual Comm           & getComm();
  virtual GameController & getGameController();
  virtual Localization   & getLocalization();
  virtual Vision         & getVision();
  virtual WorldModel     & getWorldModel();

  virtual VisionFeatures & getVisionFeatures();
  virtual WorldFeatures  & getWorldFeatures();

private:
  PREVENT_COPY_AND_ASSIGNMENT(FactoryConfigurable);

  std::string configFilePath, configFileName;

  ConfigFile *configFile;
  Log        *log;
  Field      *field;

  Behaviors      *behaviors;
  Comm           *comm;
  GameController *gameController;
  Localization   *localization;
  Vision         *vision;
  WorldModel     *worldModel;

  VisionFeatures *visionFeatures;
  WorldFeatures  *worldFeatures;
};

#endif /* FACTORYCONFIGURABLE_H_ */
