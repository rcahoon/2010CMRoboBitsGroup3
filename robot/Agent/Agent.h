#ifndef AGENT_H_
#define AGENT_H_

#include "shared/UsefulMacros.h"
#include "Agent/Command/Command.h"
#include "Agent/Feedback/Feedback.h"

#include "Comm/Messages/Messages.h"
#include "GameController/GameState.h"
#include "Localization/Pose.h"

class ConfigFile;
class Field;
class Factory;
class RobotState;

class Behaviors;
class Comm;
class GameController;
class Localization;
class Log;
class Vision;
class WorldModel;

class VisionFeatures;
class WorldFeatures;

class Agent {
public:

  /**
   * Default constructor of Agent. Uses the defaultFactory to generate the
   * components of Agent, e.g., Vision, Behaviors.
   */
  Agent();

  /**
   * Constructor of Agent. Uses the Factory to generate the components of Agent.
   *
   * @param factory Reference to the Factory to use
   */
  Agent(Factory & factory);

  /**
   * Destructor.
   */
  ~Agent();

  /**
   * Main loop of the Agent. Runs each component, and returns the command that
   * the robot should execute.
   *
   * @param state state of the robot (e.g., timestamp, camera image)
   *
   * @return command the robot should execute
   */
  const Command & run(const RobotState & state);

private:
  PREVENT_COPY_AND_ASSIGNMENT(Agent);

  const float FRAME_RATE_FACTOR;
  const float FRAME_RATE_ONE_MINUS_FACTOR;

  ConfigFile & configFile;
  Field & field;

  Behaviors      & behaviors;
  Comm           & comm;
  GameController & gameController;
  Localization   & localization;
  Log            & log;
  Vision         & vision;
  WorldModel     & worldModel;

  VisionFeatures & visionFeatures;
  WorldFeatures  & worldFeatures;

  Command  command;
  Feedback feedback;

  Messages  messages;
  GameState gameState;
  Pose      pose;

  unsigned long prevTimestamp;
  float         frameRate;
};

#endif /* AGENT_H_ */
