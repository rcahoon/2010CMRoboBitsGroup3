#include <vector>

#include "GameController2010.h"
#include "Agent/RobotState.h"
#include "Comm/Messages/Messages.h"
#include "Comm/Messages/GameController/GameControllerMessage.h"
#include "GameController/GameState.h"
#include "shared/ConfigFile/ConfigFile.h"

#define COMPONENT GAME_CONTROLLER
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

GameController2010::GameController2010(ConfigFile & configFile, Log & _log)
  : GameController(),
    stateChangePeriod(configFile.getMillisecondsFromSeconds("gameController/stateChangePeriod", 3000)),
    teamChangePeriod (configFile.getMillisecondsFromSeconds("gameController/teamChangePeriod", 1000)),
    kickoffChangePeriod(configFile.getMillisecondsFromSeconds("gameController/kickoffChangePeriod", 1000)),
    teamNumber(configFile.getInt("team/teamNumber", 0)),
    log(_log),
    lastStateChange(0),
    lastTeamChange(0),
    lastKickoffChange(0),
    wasOurColorBlue(true) {
}

GameController2010::~GameController2010() {
}

bool GameController2010::run(const RobotState & robotState,
                                   Messages & messages,
                                   GameState & gameState) {
  unsigned long timestamp = robotState.getTimestamp();

  const std::vector<GameControllerMessage const *> & gameControllerMessages = messages.getReceivedGameControllerMessages();

  if (gameControllerMessages.size() > 0) {
    // We only need to take the most recent message
    GameControllerMessage const *lastMessage = gameControllerMessages.back();
    if (lastMessage->isValid(teamNumber)) {
      gameState.setData(lastMessage->getData());

      LOG_DEBUG("%d seconds remaining, score is %d : %d",
                gameState.getNumSecondsRemaining(),
                gameState.getOurScore(),
                gameState.getOpponentScore());
      LOG_DEBUG("%d of our team is penalized",
                gameState.getNumOurPlayersPenalized());
      LOG_DEBUG("%d of opponent's team is penalized",
                gameState.getNumOpponentPlayersPenalized());
    }

    // Did our color change?
    if (gameState.isOurColorBlue() != wasOurColorBlue) {
      wasOurColorBlue = gameState.isOurColorBlue();
    }
  }

  // Check for button presses
  GameState::StateOfGame stateOfGame = gameState.getStateOfGame();
  switch (stateOfGame) {
  case GameState::initial:
    if (robotState.getChestButton() &&
        (timestamp - lastStateChange > stateChangePeriod)) {
      lastStateChange = timestamp;
      gameState.setStateOfGame(GameState::playing);
      gameState.setPenalized(true);
      LOG_DEBUG("Changing to playing state (penalized)");
    }
    if (robotState.getLeftFootButton() &&
        (timestamp - lastTeamChange > teamChangePeriod)) {
      lastTeamChange = timestamp;
      gameState.setTeamColor(!gameState.isOurColorBlue());

      wasOurColorBlue = gameState.isOurColorBlue();

      LOG_DEBUG("Changing team color to %d",
                gameState.isOurColorBlue());
    }
    if (robotState.getRightFootButton() &&
        (timestamp - lastKickoffChange > kickoffChangePeriod)) {
      lastKickoffChange = timestamp;
      gameState.setKickoff(!gameState.isOurKickoff());
      LOG_DEBUG("Changing kickoff state to %d",
                gameState.isOurKickoff());
    }

    break;
  case GameState::ready:
    if (robotState.getChestButton() &&
        (timestamp - lastStateChange > stateChangePeriod)) {
      lastStateChange = timestamp;
      gameState.setStateOfGame(GameState::playing);
      gameState.setPenalized(true);
      LOG_DEBUG("Changing to playing state (penalized)");
    }

    break;
  case GameState::set:
    if (robotState.getChestButton() &&
        (timestamp - lastStateChange > stateChangePeriod)) {
      lastStateChange = timestamp;
      gameState.setStateOfGame(GameState::playing);
      gameState.setPenalized(true);
      LOG_DEBUG("Changing to playing state (penalized)");
    }

    break;
  case GameState::playing:
    if (robotState.getChestButton() &&
        (timestamp - lastStateChange > stateChangePeriod)) {
      lastStateChange = timestamp;
      gameState.setPenalized(!gameState.amIPenalized());
      LOG_DEBUG("Setting penalized to %d",
                gameState.amIPenalized());
    }

    break;
  case GameState::finished:
    LOG_DEBUG("Game has finished.");
    break;
  case GameState::unknown:
    LOG_DEBUG("Game state is unknown.");
    break;
  }

  // Log the team color
  LOG_TEAM_COLOR(gameState.isOurColorBlue());

  return false;
}
