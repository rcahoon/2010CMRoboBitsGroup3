#ifndef ROBOCUP2010_OBEYGAMEMANAGER_H_
#define ROBOCUP2010_OBEYGAMEMANAGER_H_

#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/shared/FSM.h"
#include "GameController/GameState.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

namespace RoboCup2010 {

class ObeyGameManager : public BehaviorBase {
public:
  enum State {
    Uninitialized,
    Initial,
    Ready,
    Set,
    Playing,
    Penalized,
    BackFromPenalty,
    Finished,
  };

  ObeyGameManager(ConfigFile & configFile, Log & log);

  virtual ~ObeyGameManager();

  virtual bool run(BEHAVIOR_PARAMS);

  void reset();

private:
  PREVENT_COPY_AND_ASSIGNMENT(ObeyGameManager);

  void init(unsigned long timestamp);

  State convertState(GameState::StateOfGame gameState) const;

  const unsigned int INITIAL_CHEST_COLOR,
                     READY_CHEST_COLOR,
                     SET_CHEST_COLOR,
                     PLAYING_CHEST_COLOR,
                     FINISHED_CHEST_COLOR,
                     PENALIZED_CHEST_COLOR,
                     BLUE_TEAM_FOOT_COLOR,
                     RED_TEAM_FOOT_COLOR,
                     KICKOFF_FOOT_COLOR,
                     NO_KICKOFF_FOOT_COLOR;

  const std::string READY_SPEECH,
                    SET_SPEECH,
                    PLAYING_SPEECH,
                    PENALIZED_SPEECH,
                    BACK_FROM_PENALTY_SPEECH,
                    BLUE_TEAM_SPEECH,
                    RED_TEAM_SPEECH,
                    KICKOFF_SPEECH,
                    NO_KICKOFF_SPEECH;

  const int PLAYER_NUMBER;

  const float HEAD_SPEED;

  Log & log;

  FSM fsm;

  State previousState, currentState;
  bool wasIBlue, wasOurKickoff, wasIPenalized;
};

}

#endif /* ROBOCUP2010_OBEYGAMEMANAGER_H_ */
