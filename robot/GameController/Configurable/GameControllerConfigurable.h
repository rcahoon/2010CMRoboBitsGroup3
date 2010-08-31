#ifndef GAMECONTROLLERCONFIGURABLE_H_
#define GAMECONTROLLERCONFIGURABLE_H_

#include "GameController/GameController.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

class GameControllerConfigurable: public GameController {
public:
  GameControllerConfigurable(ConfigFile & configFile, Log & log);

  virtual ~GameControllerConfigurable();

  virtual bool run(const RobotState & robotState,
                         Messages & messages,
                         GameState & gameState);

private:
  PREVENT_COPY_AND_ASSIGNMENT(GameControllerConfigurable);

  Log & log;

  GameController *gameController;
};

#endif /* GAMECONTROLLERCONFIGURABLE_H_ */
