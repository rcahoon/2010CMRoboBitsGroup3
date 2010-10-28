#include "ObeyGameManager.h"
#include "Agent/Command/Command.h"
#include "Agent/Command/MotionCommand/MotionCommand.h"
#include "Agent/Command/SoundCommand/SoundCommand.h"
#include "Agent/Command/LEDCommand/LEDCommand.h"
#include "GameController/GameState.h"
#include "shared/ConfigFile/ConfigFile.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"


namespace RoboCup2010 {

ObeyGameManager::ObeyGameManager(ConfigFile & configFile, Log & _log)
  : INITIAL_CHEST_COLOR  (configFile.getInt("rules/initialChestColor",   0x000000)),
    READY_CHEST_COLOR    (configFile.getInt("rules/readyChestColor",     0x0000FF)),
    SET_CHEST_COLOR      (configFile.getInt("rules/setChestColor",       0xFFFF00)),
    PLAYING_CHEST_COLOR  (configFile.getInt("rules/playingChestColor",   0x00FF00)),
    FINISHED_CHEST_COLOR (configFile.getInt("rules/finishedChestColor",  0x000000)),
    PENALIZED_CHEST_COLOR(configFile.getInt("rules/penalisedChestColor", 0xFF0000)),
    BLUE_TEAM_FOOT_COLOR (configFile.getInt("rules/blueTeamFootColor",   0x0000FF)),
    RED_TEAM_FOOT_COLOR  (configFile.getInt("rules/redTeamFootColor",    0xFF0000)),
    KICKOFF_FOOT_COLOR   (configFile.getInt("rules/kickoffFootColor",    0xFFFFFF)),
    NO_KICKOFF_FOOT_COLOR(configFile.getInt("rules/noKickoffFootColor",  0x000000)),
    READY_SPEECH            (configFile.getString("behaviors/2010/tactics/obeyGameManager/readySpeech",           "I will get ready for the game.")),
    SET_SPEECH              (configFile.getString("behaviors/2010/tactics/obeyGameManager/setSpeech",             "I am all set!")),
    PLAYING_SPEECH          (configFile.getString("behaviors/2010/tactics/obeyGameManager/playingSpeech",         "It's time to play.")),
    PENALIZED_SPEECH        (configFile.getString("behaviors/2010/tactics/obeyGameManager/penalizedSpeech",       "Please don't penalize me!")),
    BACK_FROM_PENALTY_SPEECH(configFile.getString("behaviors/2010/tactics/obeyGameManager/backFromPenaltySpeech", "The taste of freedom is sweet.")),
    BLUE_TEAM_SPEECH        (configFile.getString("behaviors/2010/tactics/obeyGameManager/blueTeamSpeech",        "I am now on the blue team.")),
    RED_TEAM_SPEECH         (configFile.getString("behaviors/2010/tactics/obeyGameManager/redTeamSpeech",         "I am now on the red team.")),
    KICKOFF_SPEECH          (configFile.getString("behaviors/2010/tactics/obeyGameManager/kickoffSpeech",         "We have kickoff.")),
    NO_KICKOFF_SPEECH       (configFile.getString("behaviors/2010/tactics/obeyGameManager/noKickoffSpeech",       "We don't have kickoff.")),
    PLAYER_NUMBER(configFile.getInt("team/playerNumber", 2)),
    HEAD_SPEED(configFile.getFloat("motion/defaultHeadSpeed", 0.8f)),
    log(_log),
    fsm(_log),
    previousState(Uninitialized),
    currentState(Uninitialized),
    wasIBlue(false),
    wasOurKickoff(false),
    wasIPenalized(false) {
  init(_log.getTimestamp());
}

ObeyGameManager::~ObeyGameManager() {
}

void ObeyGameManager::init(unsigned long timestamp) {
  // Initialize the fsm
  fsm.reset();
  FSM_ADDSTATE(fsm, Uninitialized);
  FSM_ADDSTATE(fsm, Initial);
  FSM_ADDSTATE(fsm, Ready);
  FSM_ADDSTATE(fsm, Set);
  FSM_ADDSTATE(fsm, Playing);
  FSM_ADDSTATE(fsm, Penalized);
  FSM_ADDSTATE(fsm, BackFromPenalty);
  FSM_ADDSTATE(fsm, Finished);
  fsm.finishInit(Uninitialized, timestamp);
}

void ObeyGameManager::reset() {
  init(log.getTimestamp());
}

bool ObeyGameManager::run(BEHAVIOR_PARAMS) {
  MotionCommand & motionCommand = command.getMotionCommand();
  SoundCommand  & soundCommand  = command.getSoundCommand();
  LEDCommand    & ledCommand    = command.getLEDCommand();

  GameState::StateOfGame stateOfGame = gameState.getStateOfGame();

  // Only allow text-to-speech of other behaviors when playing
  if (stateOfGame != GameState::playing) {
    soundCommand.clear();
  }

  try {
    fsm.startLoop(log.getTimestamp());
    while(fsm.isRunning()) {

      if (fsm.inState(Uninitialized)) {
        wasIBlue      = gameState.isOurColorBlue();
        wasOurKickoff = gameState.isOurKickoff();

        fsm.trans(convertState(stateOfGame), "Initialized");
        continue;
      }

      else if (fsm.inState(Initial)) {
        State state = convertState(stateOfGame);
        if (state != Initial) {
          fsm.trans(state, "Game state changed");
          continue;
        }

        ledCommand.setChest(INITIAL_CHEST_COLOR);

        // Go to stand neutral position
        motionCommand.standNeutral();
        // Look straight ahead
        motionCommand.headAngles(0, 0, HEAD_SPEED);

        fsm.endLoop();
        continue;
      }

      else if (fsm.inState(Ready)) {
        State state = convertState(stateOfGame);
        if (state != Ready) {
          fsm.trans(state, "Game state changed");
          continue;
        }

        if (fsm.isNewState()) {
          soundCommand.say(READY_SPEECH);
        }

        ledCommand.setChest(READY_CHEST_COLOR);

        // Stand neutral
        //motionCommand.standNeutral();

        fsm.endLoop();
        continue;
      }

      else if (fsm.inState(Set)) {
        State state = convertState(stateOfGame);
        if (state != Set) {
          fsm.trans(state, "Game state changed");
          continue;
        }

        if (fsm.isNewState()) {
          soundCommand.say(SET_SPEECH);
        }

        ledCommand.setChest(SET_CHEST_COLOR);

        // Stand neutral
        motionCommand.standNeutral();

        fsm.endLoop();
        continue;
      }

      else if (fsm.inState(Playing)) {
        State state = convertState(stateOfGame);
        if (state != Playing) {
          fsm.trans(state, "Game state changed");
          continue;
        }

        if (gameState.amIPenalized()) {
          fsm.trans(Penalized, "Penalized");
          continue;
        }

        if (fsm.isNewState()) {
          if (!wasIPenalized) {
            soundCommand.say(PLAYING_SPEECH);
          }
          else {
            wasIPenalized = false;
            soundCommand.say(BACK_FROM_PENALTY_SPEECH);
          }
        }

        ledCommand.setChest(PLAYING_CHEST_COLOR);

        fsm.endLoop();
        continue;
      }

      else if (fsm.inState(Penalized)) {
        State state = convertState(stateOfGame);
        if (state != Playing) {
          fsm.trans(state, "Game state changed");
          continue;
        }

        if (!gameState.amIPenalized()) {
          fsm.trans(BackFromPenalty, "No longer penalized");
          continue;
        }

        if (fsm.isNewState()) {
          wasIPenalized = true;
          soundCommand.say(PENALIZED_SPEECH);
        }

        ledCommand.setChest(PENALIZED_CHEST_COLOR);
        // Go to stand neutral position
        motionCommand.standNeutral();
        // Look straight ahead
        motionCommand.headAngles(0, 0, HEAD_SPEED);

        fsm.endLoop();
        continue;
      }

      else if (fsm.inState(BackFromPenalty)) {
        State state = convertState(stateOfGame);
        if (state != Playing) {
          fsm.trans(state, "Game state changed");
          continue;
        }

        if (gameState.amIPenalized()) {
          fsm.trans(Penalized, "Penalized again");
          continue;
        }

        // No checking for now
        fsm.trans(Playing, "No checks for now");
        continue;
      }

      else if (fsm.inState(Finished)) {
        State state = convertState(stateOfGame);
        if (state != Finished) {
          fsm.trans(state, "Game state changed");
          continue;
        }

        ledCommand.setChest(FINISHED_CHEST_COLOR);
        // Go to stand neutral position
        motionCommand.standNeutral();
        // Look straight ahead
        motionCommand.headAngles(0, 0, HEAD_SPEED);

        fsm.endLoop();
        continue;
      }

      else {
        fsm.trans(Uninitialized, "Invalid state");
        continue;
      }

    }
  }
  catch (int e) {
    if (fsm.isRunning()) {
      fsm.endLoop();
    }
  }
  fsm.printTransitions();

  switch (stateOfGame) {
  case GameState::initial:
  case GameState::ready:
  case GameState::set:
    if (gameState.isOurColorBlue()) {
      ledCommand.setLeftFoot(BLUE_TEAM_FOOT_COLOR);
    }
    else {
      ledCommand.setLeftFoot(RED_TEAM_FOOT_COLOR);
    }
    if (gameState.isOurKickoff()) {
      ledCommand.setRightFoot(KICKOFF_FOOT_COLOR);
    }
    else {
      ledCommand.setRightFoot(NO_KICKOFF_FOOT_COLOR);
    }
    break;
  default:
    if (gameState.isOurColorBlue()) {
      ledCommand.setLeftFoot(BLUE_TEAM_FOOT_COLOR);
      ledCommand.setRightFoot(BLUE_TEAM_FOOT_COLOR);
    }
    else {
      ledCommand.setLeftFoot(RED_TEAM_FOOT_COLOR);
      ledCommand.setRightFoot(RED_TEAM_FOOT_COLOR);
    }
  }

  if (wasOurKickoff != gameState.isOurKickoff()) {
    wasOurKickoff = gameState.isOurKickoff();
    if (wasOurKickoff) {
      soundCommand.say(KICKOFF_SPEECH);
    }
    else {
      soundCommand.say(NO_KICKOFF_SPEECH);
    }
  }
  if (wasIBlue != gameState.isOurColorBlue()) {
    wasIBlue = gameState.isOurColorBlue();
    if (wasIBlue) {
      soundCommand.say(BLUE_TEAM_SPEECH);
    }
    else {
      soundCommand.say(RED_TEAM_SPEECH);
    }
  }

  return false;
}

ObeyGameManager::State ObeyGameManager::convertState(GameState::StateOfGame gameState) const {
  switch (gameState) {
  case GameState::initial:
    return Initial;
  case GameState::ready:
    return Ready;
  case GameState::set:
    return Set;
  case GameState::playing:
    return Playing;
  case GameState::finished:
    return Finished;
  default:
    return Uninitialized;
  }
}

} // end namespace
