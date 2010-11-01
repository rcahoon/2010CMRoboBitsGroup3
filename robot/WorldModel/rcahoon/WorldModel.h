#ifndef RCAHOON_WORLDMODEL_H_
#define RCAHOON_WORLDMODEL_H_

#include "WorldModel/WorldModel.h"
#include "WorldModel/WorldObject/WorldObject.h"
#include "WorldModel/WorldObject/GoalWorldObject.h"
#include "WorldModel/WorldObject/VelocityWorldObject.h"
#include "shared/Vector/Vector2D.h"

#define BALL_VELOCITY_CONF  0.08
#define BALL_VELOCITY_DECAY  0.7
#define BALL_CONFIDENCE_DECAY 0.987

class Log;
class Field;
class ConfigFile;

namespace RCahoon {

class WorldModel : public ::WorldModel {
public:
	WorldModel(ConfigFile & configFile,
               Log & _log,
               Field & field);
	
	virtual ~WorldModel();

	virtual bool run(const RobotState & robotState,
		           const GameState & gameState,
		           const VisionFeatures & visionFeatures,
		           const Pose & pose,
		                 Messages & messages,
		                 WorldFeatures & worldFeatures);

	virtual void setRobotConditions(bool gettingUp, bool lifted);
	
	virtual void addKickEffect(const Vector2D & kickEffect);

	virtual void invalidateBall();

private:
	Log& log;
	
	Field& field;
	
	VelocityWorldObject ball;
	GoalWorldObject b_goal, y_goal;
	WorldObject self;

};

}

#endif /* WORLDMODEL_H_ */
