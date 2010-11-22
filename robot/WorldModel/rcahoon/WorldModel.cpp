#include "WorldModel.h"
#include "WorldModel/WorldFeatures.h"
#include "WorldModel/WorldObject/GoalPostWorldObject.h"
#include "Vision/VisionObject/VisionObject.h"
#include "Vision/VisionFeatures.h"
#include "Localization/Pose.h"
#include "GameController/GameState.h"
#include "shared/Field/Field.h"
#include "shared/Shape/Circle.h"
#include "shared/Shape/Line.h"

#define COMPONENT VISION
#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

namespace RCahoon {

WorldModel::WorldModel(ConfigFile & configFile,
                       Log & _log,
                       Field & _field) :
                       log(_log),
                       field(_field),
                       scanForGoals(false),
                       ball(_log, WorldObject::Ball),
                       b_goal(configFile, _log, 0.0f, true),
                       y_goal(configFile, _log, 0.0f, false),
                       self(_log, WorldObject::Self) {
}

WorldModel::~WorldModel() {
}

const Matrix WorldModel::ballMotionModel(Vector2D vel)
{
	return Matrix::I<2>() * vel.length() * BALL_VELOCITY_COV_SCALE;
}

bool WorldModel::run(const RobotState & robotState,
                     const GameState & gameState,
                     const VisionFeatures & visionFeatures,
                     const Pose & pose,
                           Messages & messages,
                           WorldFeatures & worldFeatures) {
	
	/*HMatrix const* camTransform = &(robotState.getTransformationFromCamera());
	
	std::vector<VisionObject const *> b_goals = visionFeatures.getVisionObjects(VisionObject::BlueGoalBar);
	if (!b_goals.empty())
	{
		for(std::vector<VisionObject const *>::iterator iter = b_goals.begin();
			iter != b_goals.end(); iter++)
		{
			int x1, x2, y1, y2;
			x1 = (*iter)->getBoundingBoxX1();
			x2 = (*iter)->getBoundingBoxX2();
			y1 = (*iter)->getBoundingBoxY1();
			y2 = (*iter)->getBoundingBoxY2();
			//(*iter)->getBoundingBox(x1, y1, x2, y2);
			Vector2D post1 = cameraToWorld(camTransform, Vector2D(x1, y2));
			Vector2D post2 = cameraToWorld(camTransform, Vector2D(x2, y2));
			
			
		}
	}*/
	
	std::vector<VisionObject const *> balls = visionFeatures.getVisionObjects(VisionObject::Ball);
	if (balls.empty())
	{
		if (ball.isValid())
		{
			ball.setSuspicious(true);
			ball.setGlobalPosition(ball.getGlobalPosition() + ball.getVelocity());
			ball.setLocalPosition(pose.convertGlobalToRelative(ball.getGlobalPosition()));
			ball.setCovariance(ball.getCovariance() + ballMotionModel(ball.getVelocity()));
			ball.setVelocity(ball.getVelocity() * BALL_VELOCITY_DECAY);
		}
		ball.setCovariance(ball.getCovariance() + Matrix::I<2>()*BALL_COVARIANCE_DECAY);
	}
	else
	{
		std::vector<VisionObject const *>::iterator iter = balls.begin();
		//TODO: start with estimated position, so we stick with that if we get a really noisy vision measurement
		const VisionObject * bestBall = *iter;
		for(; iter != balls.end(); iter++)
		{
			if (bestBall->getConfidence() > (*iter)->getConfidence())
				bestBall = *iter;
		}
		
		if (ball.isVisible())
			ball.setVelocity(bestBall->getPosition() - ball.getLocalPosition());
		else
			ball.setVelocity(Vector2D());
		ball.setLocalPosition(bestBall->getPosition());
		ball.setGlobalPosition(pose.convertRelativeToGlobal(ball.getLocalPosition()));
		//TODO: set covariance correctly (should actually switch bestball to be a worldobject, see above)
		ball.setCovariance(Matrix::I<2>()/bestBall->getConfidence() + pose.getCovariance()(0,1,0,1));
		ball.setVisible(true);
	}
	worldFeatures.addWorldObject(ball);
	LOG_SHAPE(Log::Field, Circle(ball.getGlobalPosition(), (ball.getCovariance().trace()/3), 0xFFFF80, 3));
	LOG_SHAPE(Log::Field, Circle(ball.getGlobalPosition(), 8, 0xFF0000, 4));
	
	self.setGlobalPosition(pose.getPosition());
	self.setCovariance(pose.getCovariance()(0,1,0,1));
	self.setValid(!pose.isLost());
	worldFeatures.addWorldObject(self);
	LOG_SHAPE(Log::Field, Circle(self.getGlobalPosition(), (self.getCovariance().trace()/3), 0xFFFF80, 3));
	LOG_SHAPE(Log::Field, Circle(self.getGlobalPosition(), 8, 0x00FFFF, 4));
	LOG_SHAPE(Log::Field, Line(self.getGlobalPosition(), self.getGlobalPosition()+Vector2D(6, 0).rotate(pose.getAngle()), 0x0, 3));
	
	//this order of statements (position, angle) is important
	b_goal.setGlobalPosition(gameState.isOurColorBlue() ? field.getOurGoal() : field.getOpponentGoal());
	b_goal.setLocalPosition(pose.convertGlobalToRelative(b_goal.getGlobalPosition()));
	b_goal.setLocalAngle(pose.convertGlobalAngleToRelative(0.0f));
	b_goal.setCovariance(pose.getCovariance()(0,1,0,1));
	bool BGoalVisible = !visionFeatures.getVisionObjects(VisionObject::BlueGoalBar).empty() || !visionFeatures.getVisionObjects(VisionObject::BlueGoalPost).empty();
	b_goal.setVisible(BGoalVisible);
	worldFeatures.addWorldObject(b_goal);
	LOG_SHAPE(Log::Field, Circle(b_goal.getGlobalPosition(), 8, 0x0000FF, 4));
	
	//this order of statements (position, angle) is important
	y_goal.setGlobalPosition(gameState.isOurColorBlue() ? field.getOpponentGoal() : field.getOurGoal());
	y_goal.setLocalPosition(pose.convertGlobalToRelative(y_goal.getGlobalPosition()));
	y_goal.setLocalAngle(pose.convertGlobalAngleToRelative(0.0f));
	y_goal.setCovariance(pose.getCovariance()(0,1,0,1));
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

void WorldModel::setScanningForGoals(bool scanningForGoals)
{
	scanForGoals = scanningForGoals;
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
