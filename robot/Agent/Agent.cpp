#include "Agent.h"
#include "RobotState.h"
#include "Factory/Factory.h"
#include "Behaviors/Behaviors.h"
#include "Comm/Comm.h"
#include "GameController/GameController.h"
#include "Localization/Localization.h"
#include "Log/Log.h"
#include "Vision/Vision.h"
#include "WorldModel/WorldModel.h"

#include "shared/ConfigFile/ConfigFile.h"
#include "shared/Field/Field.h"

#define COMPONENT AGENT
#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

Agent::Agent()
  : FRAME_RATE_FACTOR          (Factory::getDefaultFactory().getConfigFile().getFloat("agent/frameRateFactor")),
    FRAME_RATE_ONE_MINUS_FACTOR(1 - FRAME_RATE_FACTOR),
    configFile(Factory::getDefaultFactory().getConfigFile()),
    field(Factory::getDefaultFactory().getField()),
    behaviors(Factory::getDefaultFactory().getBehaviors()),
    comm(Factory::getDefaultFactory().getComm()),
    gameController(Factory::getDefaultFactory().getGameController()),
    localization(Factory::getDefaultFactory().getLocalization()),
    log(Factory::getDefaultFactory().getLog()),
    vision(Factory::getDefaultFactory().getVision()),
    worldModel(Factory::getDefaultFactory().getWorldModel()),
    visionFeatures(Factory::getDefaultFactory().getVisionFeatures()),
    worldFeatures(Factory::getDefaultFactory().getWorldFeatures()),
    command(),
    feedback(Factory::getDefaultFactory()),
    messages(),
    gameState(configFile, log),
    pose(),
    prevTimestamp(0),
    frameRate(15) {
}

Agent::Agent(Factory & factory)
  : FRAME_RATE_FACTOR          (factory.getConfigFile().getFloat("agent/frameRateFactor")),
    FRAME_RATE_ONE_MINUS_FACTOR(1 - FRAME_RATE_FACTOR),
    configFile    (factory.getConfigFile()),
    field         (factory.getField()),
    behaviors     (factory.getBehaviors()),
    comm          (factory.getComm()),
    gameController(factory.getGameController()),
    localization  (factory.getLocalization()),
    log           (factory.getLog()),
    vision        (factory.getVision()),
    worldModel    (factory.getWorldModel()),
    visionFeatures(factory.getVisionFeatures()),
    worldFeatures (factory.getWorldFeatures()),
    command(),
    feedback(factory),
    messages(),
    gameState(configFile, log),
    pose(),
    prevTimestamp(log.getTimestamp()),
    frameRate(15) {
}

Agent::~Agent() {
}

const Command & Agent::run(const RobotState & robotState) {

#ifdef LOG_TRACE_ACTIVE
  unsigned long startTime, elapsed;
#endif

  // Begin log for this cycle
  LOG_BEGIN(robotState.getTimestamp());

  // Log function call
  LOG_TRACE("Agent's run started.");

  // Log the robot state
#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  LOG_ROBOTSTATE(robotState);
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("Serializing RobotState took %d ms.", elapsed);

  // Keep track of frame rate
  unsigned long timestamp = robotState.getTimestamp();

  if ((prevTimestamp != 0) && (timestamp - prevTimestamp > 0)) {
    // time is in milliseconds
    float localRate = 1000.0f / (timestamp - prevTimestamp);
    LOG_DEBUG("localRate = %.1f", localRate);

    // Compute the smoothed average of the instantaneous frame rate
    frameRate = FRAME_RATE_FACTOR * frameRate + FRAME_RATE_ONE_MINUS_FACTOR * localRate;
  }
  prevTimestamp = timestamp;

  LOG_INFO("Frame rate: %f", frameRate);
  //printf("Frame rate: %f\n", frameRate);

  // Clear the command and feedback
  command.clear();
  feedback.clear();

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Run GameController component (update game state)
  if (gameController.run(robotState,
                         messages,
                         gameState)) {
    LOG_ERROR("Error running GameController component.");
  }
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("GameController took %d ms.", elapsed);

  // Update the field based on team color
  field.update(gameState);

  // Log the game state
  LOG_GAMESTATE(gameState);

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Run Vision component (find vision features)
  if (vision.run(robotState,
                 visionFeatures)) {
    LOG_ERROR("Error running Vision component.");
  }
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("Vision took %d ms.", elapsed);

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Run Localization component
  if (localization.run(robotState,
                       gameState,
                       visionFeatures,
                       pose)) {
    LOG_ERROR("Error running Localization component.");
  }
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("Localization took %d ms.", elapsed);

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Run WorldModel component (update world features)
  if (worldModel.run(robotState,
                     gameState,
                     visionFeatures,
                     pose,
                     messages,
                     worldFeatures)) {
    LOG_ERROR("Error running WorldModel component.");
  }
  localization.updateWorldFeatures(worldFeatures);
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("World Model took %d ms.", elapsed);

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Run Behaviors component (update motion command)
  if (behaviors.run(robotState,
                    gameState,
                    visionFeatures,
                    worldFeatures,
                    pose,
                    messages,
                    command,
                    feedback)) {
    LOG_ERROR("Error running Behaviors component.");
  }
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("Behaviors took %d ms.", elapsed);

  // End log for this cycle
  LOG_END();

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Run Log component (to queue messages to be sent)
  if (log.run(messages)) {
    LOG_ERROR("Error running Log component.");
  }
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("Log took %d ms.", elapsed);

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Run Comm component (send and receive messages)
  if (comm.run(messages, feedback)) {
    LOG_ERROR("Error running Comm component.");
  }
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("Comm took %d ms.", elapsed);

#ifdef LOG_TRACE_ACTIVE
  startTime = log.getTimestamp();
#endif
  // Process the feedback
  if (feedback.run()) {
    LOG_ERROR("Error processing feedback.");
  }
#ifdef LOG_TRACE_ACTIVE
  elapsed = log.getTimestamp() - startTime;
#endif
  LOG_TRACE("Processing feedback took %d ms.", elapsed);

  // Log function call
  LOG_TRACE("Agent's run completed.");

  return command;
}
