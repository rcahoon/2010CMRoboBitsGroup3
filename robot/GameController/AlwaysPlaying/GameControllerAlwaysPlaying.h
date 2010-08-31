#ifndef GAMECONTROLLERALWAYSPLAYING_H_
#define GAMECONTROLLERALWAYSPLAYING_H_

#include "GameController/GameController.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

class GameControllerAlwaysPlaying: public GameController {
public:
  GameControllerAlwaysPlaying(ConfigFile & configFile, Log & _log);
  virtual ~GameControllerAlwaysPlaying();

  virtual bool run(const RobotState & robotState,
                         Messages & messages,
                         GameState & gameState);

private:
  PREVENT_COPY_AND_ASSIGNMENT(GameControllerAlwaysPlaying);
  const bool isBlueTeam;
  Log & log;
};

#endif /* GAMECONTROLLERALWAYSPLAYING_H_ */
