#ifndef GAMECONTROLLER2010_H_
#define GAMECONTROLLER2010_H_

#include "GameController/GameController.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

class GameController2010: public GameController {
public:
  GameController2010(ConfigFile & configFile, Log & _log);

  virtual ~GameController2010();

  virtual bool run(const RobotState & robotState,
                         Messages & messages,
                         GameState & gameState);

private:
  PREVENT_COPY_AND_ASSIGNMENT(GameController2010);

  const unsigned long stateChangePeriod, teamChangePeriod, kickoffChangePeriod;
  const int teamNumber;

  Log & log;
  unsigned long lastStateChange, lastTeamChange, lastKickoffChange;
  bool wasOurColorBlue;
};

#endif /* GAMECONTROLLER2010_H_ */
