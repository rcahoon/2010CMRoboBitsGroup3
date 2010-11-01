#include "WorldModel.h"
#include "WorldModel/WorldFeatures.h"
#include "Vision/VisionObject/VisionObject.h"
#include "Vision/VisionFeatures.h"
#include "Localization/Pose.h"
#include "GameController/GameState.h"
#include "shared/Field/Field.h"
#include "shared/Shape/Circle.h"

#define COMPONENT VISION
#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

namespace RCahoon {

WorldModel::WorldModel(ConfigFile & configFile,
                       Log & _log,
                       Field & _field) :
                       log(_log),
                       field(_field),
                       ball(_log, WorldObject::Ball),
                       b_goal(configFile, _log, 0.0f, true),
                       y_goal(configFile, _log, 0.0f, false),
                       self(_log, WorldObject::Self) {
}

WorldModel::~WorldModel() {
}

float ballMotionModel(Vector2D vel)
{
	return vel.length() * BALL_VELOCITY_CONF;
}

bool WorldModel::run(const RobotState & robotState,
                     const GameState & gameState,
                     const VisionFeatures & visionFeatures,
                     const Pose & pose,
                           Messages & messages,
                           WorldFeatures & worldFeatures) {

	worldFeatures.clear();
	
	std::vector<VisionObject const *> lines = visionFeatures.getVisionObjects(VisionObject::Line);
	for(std::vector<VisionObject const *>::iterator iter = lines.begin();
	    iter != lines.end(); iter++)
	{
		LOG_SHAPE(Log::Field, Circle(pose.convertRelativeToGlobal((*iter)->getPosition()), 2, 0x000000, 1));
	}

	std::vector<VisionObject const *> balls = visionFeatures.getVisionObjects(VisionObject::Ball);
	if (balls.empty())
	{
		if (ball.isValid())
		{
			ball.setSuspicious(true);
			ball.setLocalPosition(ball.getLocalPosition() + ball.getVelocity());
			ball.setConfidence(ball.getConfidence() * ballMotionModel(ball.getVelocity()));
			ball.setVelocity(ball.getVelocity() * BALL_VELOCITY_DECAY);
		}
		ball.setConfidence(ball.getConfidence() * BALL_CONFIDENCE_DECAY);
	}
	else
	{
		std::vector<VisionObject const *>::iterator iter = balls.begin();
		const VisionObject * bestBall = *iter;
		for(; iter != balls.end(); iter++)
		{
			if (bestBall->getConfidence() < (*iter)->getConfidence())
				bestBall = *iter;
		}
		
		if (ball.isVisible())
			ball.setVelocity(bestBall->getPosition() - ball.getLocalPosition());
		else
			ball.setVelocity(Vector2D());
		ball.setLocalPosition(bestBall->getPosition());
		ball.setConfidence(bestBall->getConfidence());
		ball.setVisible(true);
	}
	ball.setGlobalPosition(pose.convertRelativeToGlobal(ball.getLocalPosition()));
	worldFeatures.addWorldObject(ball);
	LOG_SHAPE(Log::Field, Circle(ball.getGlobalPosition(), 8, 0xFF0000, 4));
	
	self.setGlobalPosition(pose.getPosition());
	self.setConfidence(pose.getConfidence());
	self.setValid(!pose.isLost());
	worldFeatures.addWorldObject(self);
	LOG_SHAPE(Log::Field, Circle(self.getGlobalPosition(), 8, 0x00FFFF, 4));
	
	//this order of statements (position, angle) is important
	b_goal.setGlobalPosition(gameState.isOurColorBlue() ? field.getOurGoal() : field.getOpponentGoal());
	b_goal.setLocalPosition(pose.convertGlobalToRelative(b_goal.getGlobalPosition()));
	b_goal.setLocalAngle(pose.convertGlobalAngleToRelative(0.0f));
	b_goal.setConfidence(pose.getConfidence());
	bool BGoalVisible = !visionFeatures.getVisionObjects(VisionObject::BlueGoalBar).empty() || !visionFeatures.getVisionObjects(VisionObject::BlueGoalPost).empty();
	b_goal.setVisible(BGoalVisible);
	worldFeatures.addWorldObject(b_goal);
	LOG_SHAPE(Log::Field, Circle(b_goal.getGlobalPosition(), 8, 0x0000FF, 4));
	
	//this order of statements (position, angle) is important
	y_goal.setGlobalPosition(gameState.isOurColorBlue() ? field.getOpponentGoal() : field.getOurGoal());
	y_goal.setLocalPosition(pose.convertGlobalToRelative(y_goal.getGlobalPosition()));
	y_goal.setLocalAngle(pose.convertGlobalAngleToRelative(0.0f));
	y_goal.setConfidence(pose.getConfidence());
	bool YGoalVisible = !visionFeatures.getVisionObjects(VisionObject::YellowGoalBar).empty() || !visionFeatures.getVisionObjects(VisionObject::YellowGoalPost).empty();
	y_goal.setVisible(YGoalVisible);
	worldFeatures.addWorldObject(y_goal);
	LOG_SHAPE(Log::Field, Circle(y_goal.getGlobalPosition(), 8, 0xFFFF00, 4));

	return false;
}

//TODO: Is this actually affecting anything, if we reset validity on the next frame before
//      we reach Behaviors?
void WorldModel::setRobotConditions(bool gettingUp, bool lifted)
{
	if (lifted || gettingUp)
	{
		ball.setValid(false);
		b_goal.setValid(false);
		y_goal.setValid(false);
		self.setValid(false);
	}
}
	
void WorldModel::addKickEffect(const Vector2D & kickEffect)
{
	ball.setVelocity(kickEffect);
}

void WorldModel::invalidateBall()
{
	ball.setValid(false);
}

}
