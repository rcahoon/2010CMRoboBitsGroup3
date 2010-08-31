#ifndef WORLDMODEL_H_
#define WORLDMODEL_H_

#include "shared/Vector/Vector2D.h"
#include "shared/UsefulMacros.h"
#include <iostream>


class RobotState;
class GameState;
class VisionFeatures;
class Pose;
class Messages;
class WorldFeatures;

/**
 * Interface for World Model component.
 */
class WorldModel {
public:
  /**
   * Default constructor (empty since this is an interface).
   */
  WorldModel() {}

  /**
   * Destructor (empty since this is an interface).
   */
  virtual ~WorldModel() {}

  /**
   * Runs the World Model component, which updates the WorldFeatures.
   *
   * @param robotState state of the robot (e.g., timestamp, camera image)
   * @param gameState state of the game (e.g., playing, penalized)
   * @param visionFeatures vision features (according to Vision)
   * @param pose pose of the robot (according to Localization)
   * @param messages messages received from other robots
   * @param worldFeatures models of objects in the world
   *
   * @return whether an error occurred (false = everything is fine)
   */
  virtual bool run(const RobotState & robotState,
                   const GameState & gameState,
                   const VisionFeatures & visionFeatures,
                   const Pose & pose,
                         Messages & messages,
                         WorldFeatures & worldFeatures) = 0;

  virtual void setScanningForGoals(bool scanningForGoals) {};

  virtual void setScanningForObstacles(bool scanningForObstacles) {};

  virtual void setClearFreeSpaceModel(bool clearFreeSpaceModel) {};

  virtual void setRobotConditions(bool gettingUp, bool lifted) {};

  virtual void addKickEffect(const Vector2D & kickEffect) {};

  virtual void invalidateBall() {};

private:
  PREVENT_COPY_AND_ASSIGNMENT(WorldModel);

};

#endif /* WORLDMODEL_H_ */
