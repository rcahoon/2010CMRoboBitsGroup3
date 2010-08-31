#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include "shared/UsefulMacros.h"

class RobotState;
class Messages;
class GameState;

/**
 * Interface for Game Controller component.
 */
class GameController {
public:
  /**
   * Default constructor (empty since this is an interface).
   */
  GameController() {};

  /**
   * Destructor (empty since this is an interface).
   */
  virtual ~GameController() {};

  /**
   * Runs the Game Controller component, which updates the GameState.
   *
   * @param robotState state of the robot (e.g., timestamp, camera image)
   * @param messages messages received from other robots, humans, and messages to be sent
   * @param gameState state of the game (e.g., ready, playing, penalized)
   *
   * @return whether an error occurred (false = everything is fine)
   */
  virtual bool run(const RobotState & robotState,
                         Messages & messages,
                         GameState & gameState) = 0;

private:
  PREVENT_COPY_AND_ASSIGNMENT(GameController);
};

#endif /* GAMECONTROLLER_H_ */
