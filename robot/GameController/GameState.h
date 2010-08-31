#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

struct RoboCupGameControlData;

class GameState {
public:

  enum StateOfGame {
    initial = 0,
    ready,
    set,
    playing,
    finished,
    unknown,
  };

  /**
   * Constructor.
   *
   * @param configFile configuration file to load settings from
   */
  GameState(ConfigFile & configFile, Log & _log);

  /**
   * Destructor.
   */
  ~GameState();

  /**
   * Returns the team number of the player.
   *
   * @return the team number
   */
  int getTeamNumber() const;

  /**
   * Returns the player number of the player.
   *
   * @return the player number
   */
  int getPlayerNumber() const;

  /**
   * Returns the state of the game (initial, ready, set, playing, finished)
   *
   * @return state of the game
   */
  StateOfGame getStateOfGame() const;

  /**
   * Returns whether the game is in the first half or second half. If penalty
   * shoot is occurring, this result is unknown.
   *
   * @return whether the game is in the first half
   */
  bool isFirstHalf() const;

  /**
   * Returns whether it is our team's turn to kick off.
   *
   * @return whether is is our team's turn to kick off
   */
  bool isOurKickoff() const;

  /**
   * Returns whether our team color is blue or red
   *
   * @return whether our team color is blue or red
   */
  bool isOurColorBlue() const;

  /**
   * Returns the number of seconds remaining in the half (or penalty shoot)
   *
   * @return number of seconds remaining in the half (or penalty shoot)
   */
  unsigned int getNumSecondsRemaining() const;

  /**
   * Returns our team's score.
   *
   * @return our team's score
   */
  unsigned int getOurScore() const;

  /**
   * Returns the opponent team's score
   *
   * @return the opponent team's score
   */
  unsigned int getOpponentScore() const;

  /**
   * Returns how many of our players are penalized.
   *
   * @returns the number of players in our team that are penalized
   */
  unsigned int getNumOurPlayersPenalized() const;

  /**
   * Returns how many of the opponent players are penalized.
   *
   * @returns the number of players in the opponent team that are penalized
   */
  unsigned int getNumOpponentPlayersPenalized() const;

  /**
   * Returns whether this player is penalized.
   *
   * @return whether this player is penalized
   */
  bool amIPenalized() const;

  /**
   * Returns whether a specified player is penalized
   */
  bool getPlayerPenalized(int id) const;

  /**
   * Returns how many seconds until this player is unpenalized.
   *
   * @return how many seconds until this player is unpenalized
   */
  unsigned int getNumSecondsTillUnpenalized() const;

  /**
   * Returns whether a penalty shoot is taking place.
   *
   * @return whether a penalty shoot is taking place
   */
  bool isPenaltyShoot() const;

  void setData(const struct RoboCupGameControlData & data);

  void setStateOfGame(StateOfGame _stateOfGame);
  void setTeamColor(bool _ourColorBlue);
  void setKickoff(bool _ourKickoff);
  void setPenalized(bool _thisPlayerPenalized);

private:
  PREVENT_COPY_AND_ASSIGNMENT(GameState);

  const int teamNumber;
  const int playerNumber;
  const int numberOfPlayersPerTeam;

  Log & log;

  StateOfGame stateOfGame;
  bool firstHalf, ourKickoff, ourColorBlue;
  unsigned int numSecondsRemaining;
  unsigned int ourScore, opponentScore;
  unsigned int numOurPlayersPenalized, numOpponentPlayersPenalized;
  bool thisPlayerPenalized;
  bool* penalized;
  unsigned int numSecondsTillUnpenalized;
  bool penaltyShoot;
};

#endif /* GAMESTATE_H_ */
