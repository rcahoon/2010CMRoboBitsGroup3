#include "GameState.h"
#include "RoboCupGameControlData.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT GAME_CONTROLLER
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

GameState::GameState(ConfigFile & configFile, Log & _log)
  : teamNumber(configFile.getInt("team/teamNumber", 0)),
    playerNumber(configFile.getInt("team/playerNumber", 2)),
    numberOfPlayersPerTeam(configFile.getInt("rules/numberOfPlayersPerTeam", 3)),
    log(_log),
    stateOfGame(initial),
    firstHalf(true),
    ourKickoff(true),
    ourColorBlue(true),
    numSecondsRemaining(0),
    ourScore(0),
    opponentScore(0),
    numOurPlayersPenalized(0),
    numOpponentPlayersPenalized(0),
    thisPlayerPenalized(false),
    penalized(new bool[numberOfPlayersPerTeam]),
    numSecondsTillUnpenalized(0),
    penaltyShoot(false) {
  for (int i = 0; i < numberOfPlayersPerTeam; i++) {
    penalized[i] = false;
  }
}

GameState::~GameState() {
  if (penalized != NULL) {
    delete [] penalized;
  }
}

int GameState::getTeamNumber() const {
  return teamNumber;
}

int GameState::getPlayerNumber() const {
  return playerNumber;
}

GameState::StateOfGame GameState::getStateOfGame() const {
  return stateOfGame;
}

bool GameState::isFirstHalf() const {
  return firstHalf;
}

bool GameState::isOurKickoff() const {
  return ourKickoff;
}

bool GameState::isOurColorBlue() const {
  return ourColorBlue;
}

unsigned int GameState::getNumSecondsRemaining() const {
  return numSecondsRemaining;
}

unsigned int GameState::getOurScore() const {
  return ourScore;
}

unsigned int GameState::getOpponentScore() const {
  return opponentScore;
}

unsigned int GameState::getNumOurPlayersPenalized() const {
  return numOurPlayersPenalized;
}

unsigned int GameState::getNumOpponentPlayersPenalized() const {
  return numOpponentPlayersPenalized;
}

bool GameState::amIPenalized() const {
  return thisPlayerPenalized;
}

bool GameState::getPlayerPenalized(int id) const {
  if(id < 1 || id > numberOfPlayersPerTeam){
    LOG_WARN("an invalid id number was requested");
    return false;
  }
  // Game Controller sends player 1 (goalie) with id 0, and so on
  return penalized[id - 1];
}

unsigned int GameState::getNumSecondsTillUnpenalized() const {
  return numSecondsTillUnpenalized;
}

bool GameState::isPenaltyShoot() const {
  return penaltyShoot;
}

void GameState::setData(const RoboCupGameControlData & data) {
  stateOfGame         = (StateOfGame)data.state;
  firstHalf           = (bool)data.firstHalf;
  numSecondsRemaining = data.secsRemaining;
  penaltyShoot        = (data.secondaryState == STATE2_PENALTYSHOOT);

  LOG_DEBUG("data's kickOffTeam = %d", data.kickOffTeam);

  for (int i = 0; i < 2; i++) {
    TeamInfo team = data.teams[i];
    if (team.teamNumber == teamNumber) {
      ourColorBlue = !team.teamColour;
      ourScore     = team.score;
      numOurPlayersPenalized = 0;
      thisPlayerPenalized = false;
      for (int j = 0; j < data.playersPerTeam; j++) {
        if (team.players[j].penalty > 0) {
          penalized[j] = true;
          numOurPlayersPenalized++;
          // Game Controller sends player 1 (goalie) with id 0, and so on
          if (j == playerNumber - 1) {
            thisPlayerPenalized = true;
            numSecondsTillUnpenalized = team.players[j].secsTillUnpenalised;
          }
        }
        else
          penalized[j] = false;
      }
    }
    else {
      opponentScore = team.score;
      numOpponentPlayersPenalized = 0;
      for (int j = 0; j < data.playersPerTeam; j++) {
        if (team.players[j].penalty > 0) {
          numOpponentPlayersPenalized++;
        }
      }
    }
  }

  // Kick-off team: 0 = blue, 1 = red
  ourKickoff = (data.kickOffTeam == !ourColorBlue);
}

void GameState::setStateOfGame(StateOfGame _stateOfGame) {
  stateOfGame = _stateOfGame;
}

void GameState::setTeamColor(bool _ourColorBlue) {
  ourColorBlue = _ourColorBlue;
}

void GameState::setKickoff(bool _ourKickoff) {
  ourKickoff = _ourKickoff;
}

void GameState::setPenalized(bool _thisPlayerPenalized) {
  thisPlayerPenalized = _thisPlayerPenalized;
  if ((playerNumber >= 0) && (playerNumber < numberOfPlayersPerTeam)) {
    penalized[playerNumber] = false;
  }
}
