#ifndef WORLDMODELCONFIGURABLE_H_
#define WORLDMODELCONFIGURABLE_H_

#include "WorldModel/WorldModel.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;
class Field;

class WorldModelConfigurable : public WorldModel {
public:
  WorldModelConfigurable(ConfigFile & configFile, Log & log, Field & field);

  virtual ~WorldModelConfigurable();

  virtual bool run(const RobotState & robotState,
                   const GameState & gameState,
                   const VisionFeatures & visionFeatures,
                   const Pose & pose,
                         Messages & messages,
                         WorldFeatures & worldFeatures);

  virtual void setScanningForGoals(bool scanningForGoals);

  virtual void setScanningForObstacles(bool scanningForObstacles);

  virtual void setClearFreeSpaceModel(bool clearFreeSpaceModel);
  
  virtual void setRobotConditions(bool gettingUp, bool lifted);

  virtual void addKickEffect(const Vector2D & kickEffect);

  virtual void invalidateBall();

private:
  PREVENT_COPY_AND_ASSIGNMENT(WorldModelConfigurable);

  Log & log;

  WorldModel *worldModel;
};

#endif /* WORLDMODELCONFIGURABLE_H_ */
