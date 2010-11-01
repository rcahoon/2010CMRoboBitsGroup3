#ifndef ROBOCUP2010_PLAYER_H_
#define ROBOCUP2010_PLAYER_H_

#include "Behaviors/Behaviors.h"
#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/2010/GetUp.h"
#include "Behaviors/2010/ObeyGameManager.h"

class ConfigFile;
class Log;
class Field;

namespace RoboCup2010 {

class Player : public Behaviors {
public:
  Player(ConfigFile & configFile, Log & log, Field & field);

  virtual ~Player();

  virtual bool run(BEHAVIOR_PARAMS);

private:
  ConfigFile & configFile;
  Log & log;
  Field & field;

  GetUp           getUp;
  ObeyGameManager obeyGameManager;
};

} // ROBOCUP2010_PLAYER_H_


#endif /* BEHAVIORSTEST_H_ */
