#ifndef FACTORY_H_
#define FACTORY_H_

#include <string>

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

/**
 * Interface for factories. These factories provide Agent with the other
 * components, and the vision and world features class.
 */
class Factory {
public:
  Factory() {};
  virtual ~Factory() {};

  virtual ConfigFile & getConfigFile() = 0;
  virtual Log        & getLog() = 0;
  virtual Field      & getField() = 0;

  virtual Behaviors      & getBehaviors() = 0;
  virtual Comm           & getComm() = 0;
  virtual GameController & getGameController() = 0;
  virtual Localization   & getLocalization() = 0;
  virtual Vision         & getVision() = 0;
  virtual WorldModel     & getWorldModel() = 0;

  virtual VisionFeatures & getVisionFeatures() = 0;
  virtual WorldFeatures  & getWorldFeatures() = 0;

  static Factory & getDefaultFactory();

  static Factory & getNullFactory();

private:
  PREVENT_COPY_AND_ASSIGNMENT(Factory);

  static const std::string CONFIG_FILE;

  static std::string getCMurfsPath();

};

#endif /* FACTORY_H_ */
