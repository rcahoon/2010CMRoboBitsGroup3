#include "Localization.h"
#include "Vision/rcahoon/Vision.h"
#include "Vision/VisionObject/VisionObject.h"
#include "Vision/VisionFeatures.h"
#include "Agent/RobotState.h"
#include "GameController/GameState.h"
#include "shared/ConfigFile/ConfigFile.h"
#include "shared/random.h"
#include "shared/Shape/Circle.h"
#include <cmath>
#include <vector>
#include "shared/num_util.h"

#define COMPONENT VISION
#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

namespace RCahoon {

Localization::Localization(ConfigFile & configFile, Log & _log, Field & _field) :
	log(_log),
	field(_field),
	position()
{
	reset(none);
}

Localization::~Localization()
{
}

Vector2D& Localization::fieldBound(Vector2D& position)
{
	bound(position.x, -field.getHalfFieldLength(), field.getHalfFieldLength());
	bound(position.y, -field.getHalfFieldWidth(), field.getHalfFieldWidth());
	return position;
}

Particle Localization::movementModel(Vector2D T, float R)
{
	Matrix cov(3,3);
	cov(0,0) = abs(T.x) * MV_POS_VAR;
	cov(1,1) = abs(T.y) * MV_POS_VAR;
	cov(2,2) = abs(R) * MV_ANGLE_VAR;
	return Particle(T, R, cov);
}

bool Localization::run(const RobotState     & robotState,
                       const GameState      & gameState,
                       const VisionFeatures & visionFeatures,
                       Pose & pose)
{
	LOG_TRACE("Localization motion update started.");

	Vector2D _T;
	float _R;
	robotState.getOdometryUpdate(_T, _R);
	position *= movementModel(_T, -_R);
	
	LOG_INFO("Motion update: %f %f %f $ %f", position.x(), position.y(), position.angle(), position.cov().trace());
	position.cov().print();
	
	pose = position;
	
	LOG_TRACE("Localization motion update ended.");

	return false;
}

void Localization::updateWorldFeatures(const WorldFeatures & worldFeatures)
{
	LOG_TRACE("Localization observation update started.");
	
	Vector2D blueGoal = gameState.isOurColorBlue() ? field.getOurGoal() : field.getOpponentGoal();
	
	std::vector<WorldObject const *> b_posts = visionFeatures.getVisionObjects(VisionObject::BlueGoalPost);
	for(std::vector<VisionObject const *>::iterator iter1 = b_posts.begin();
		iter1 != b_posts.end(); iter1++)
	{
		for(std::vector<VisionObject const *>::iterator iter2 = iter1+1;
			iter2 != b_posts.end(); iter2++)
		{
			Vector2D postL = (*iter1)->getPosition();
			float confL = (*iter1)->getConfidence();
			Vector2D postR = (*iter2)->getPosition();
			float confR = (*iter2)->getConfidence();
			if (angle_diff(postL.angle(), postR.angle()) < 0)
			{
				swap(postL, postR);
				swap(confL, confR);
			}
			
			/*Particle estimateL, estimateR;
			{
				float A = postL.length();
				float t = postL.angle() - postR.angle();
				float B = postL.length() > postR.length() ? 
				          A*cos(t) - sqrt((140*140)-(A*sin(t))*(A*sin(t))) :
				          A*cos(t) + sqrt((140*140)-(A*sin(t))*(A*sin(t)));
				printf("Left sol %f <- %f $ %f\n", B, A, confL);
				Vector2D _postR = postR.norm(B);
				
				Vector2D goal = (postL+_postR)/2;
				Vector2D goalline = postL - _postR;
				
				float angle = -angle_diff(goalline.angle(), (float)-M_PI/2);
				Vector2D posEst = -goal.rotate(angle) + blueGoal;
				estimateL = Particle(posEst, angle, confL);
			}
			{
				float A = postR.length();
				float t = postL.angle() - postR.angle();
				float B = postR.length() > postL.length() ?
				          A*cos(t) - sqrt((140*140)-(A*sin(t))*(A*sin(t))) :
				          A*cos(t) + sqrt((140*140)-(A*sin(t))*(A*sin(t)));
				printf("Right sol %f <- %f $ %f\n", B, A, confR);
				Vector2D _postL = postL.norm(B);
				
				Vector2D goal = (_postL+postR)/2;
				Vector2D goalline = _postL - postR;
				
				float angle = -angle_diff(goalline.angle(), (float)-M_PI/2);
				Vector2D posEst = -goal.rotate(angle) + blueGoal;
				estimateR = Particle(posEst, angle, confR);
			}
			
			Particle estimate = estimateL & estimateR;*/
			
			Vector2D goal = (postL+postR)/2;
			Vector2D goalline = postL - postR;
			
			printf("PostL %f %f $ %f\n", postL.x, postL.y, confL);
			printf("PostR %f %f $ %f\n", postR.x, postR.y, confR);
			
			float angle = angle_diff((float)-M_PI/2, goalline.angle());
			Vector2D posEst = blueGoal - goal.rotate(angle)*(140.0f/goalline.length());
			//TODO: do separate var calculations for each position component
			Matrix zcov = Matrix::I<3>() / (3*sqrt(confL*confR));
			Particle estimate(posEst, angle, zcov);
			
			LOG_SHAPE(Log::Field, Circle(estimate.position(), 4, 0x0080FF, 2));
			//LOG_INFO("BGOAL estimateL: %f %f %f $ %f", estimateL.pos_x.val(), estimateL.pos_y.val(), estimateL.angle.val(), estimateL.belief());
			//LOG_INFO("BGOAL estimateR: %f %f %f $ %f", estimateR.pos_x.val(), estimateR.pos_y.val(), estimateR.angle.val(), estimateR.belief());
			LOG_INFO("BGOAL estimateT: %f %f %f $ %f", estimate.x(), estimate.y(), estimate.angle(), estimate.cov().trace());
			if (estimate.isValid())
				position |= estimate;
		}
	}
	
	Vector2D yellowGoal = gameState.isOurColorBlue() ? field.getOpponentGoal() : field.getOurGoal();
	
	std::vector<VisionObject const *> y_posts = visionFeatures.getVisionObjects(VisionObject::YellowGoalPost);
	for(std::vector<VisionObject const *>::iterator iter1 = y_posts.begin();
		iter1 != y_posts.end(); iter1++)
	{
		for(std::vector<VisionObject const *>::iterator iter2 = iter1+1;
			iter2 != y_posts.end(); iter2++)
		{
			Vector2D postL = (*iter1)->getPosition();
			float confL = (*iter1)->getConfidence();
			Vector2D postR = (*iter2)->getPosition();
			float confR = (*iter2)->getConfidence();
			if (angle_diff(postL.angle(), postR.angle()) < 0)
			{
				swap(postL, postR);
				swap(confL, confR);
			}
			
			/*Particle estimateL, estimateR;
			{
				float A = postL.length();
				float t = postL.angle() - postR.angle();
				float B = postL.length() > postR.length() ? 
				          A*cos(t) - sqrt((140*140)-(A*sin(t))*(A*sin(t))) :
				          A*cos(t) + sqrt((140*140)-(A*sin(t))*(A*sin(t)));
				printf("Left sol %f <- %f $ %f\n", B, A, confL);
				Vector2D _postR = postR.norm(B);
				
				Vector2D goal = (postL+_postR)/2;
				Vector2D goalline = postL - _postR;
				
				float angle = -angle_diff(goalline.angle(), (float)M_PI/2);
				Vector2D posEst = -goal.rotate(angle) + yellowGoal;
				estimateL = Particle(posEst, angle, confL);
			}
			{
				float A = postR.length();
				float t = postL.angle() - postR.angle();
				float B = postR.length() > postL.length() ?
				          A*cos(t) - sqrt((140*140)-(A*sin(t))*(A*sin(t))) :
				          A*cos(t) + sqrt((140*140)-(A*sin(t))*(A*sin(t)));
				printf("Right sol %f <- %f $ %f\n", B, A, confR);
				Vector2D _postL = postL.norm(B);
				
				Vector2D goal = (_postL+postR)/2;
				Vector2D goalline = _postL - postR;
				
				float angle = -angle_diff(goalline.angle(), (float)M_PI/2);
				Vector2D posEst = -goal.rotate(angle) + yellowGoal;
				estimateR = Particle(posEst, angle, confR);
			}
			
			Particle estimate = estimateL & estimateR;*/
			
			Vector2D goal = (postL+postR)/2;
			Vector2D goalline = postL - postR;
			
			float angle = angle_diff((float)M_PI/2, goalline.angle());
			Vector2D posEst = -goal.rotate(angle)*(140.0f/goalline.length()) + yellowGoal;
			//TODO: do separate var calculations for each position component
			Particle estimate(posEst, angle, Matrix::I<3>()/(3*sqrt(confL*confR)));
			
			LOG_SHAPE(Log::Field, Circle(estimate.position(), 4, 0x80FF80, 2));
			//LOG_INFO("YGOAL estimateL: %f %f %f $ %f", estimateL.pos_x.val(), estimateL.pos_y.val(), estimateL.angle.val(), estimateL.belief());
			//LOG_INFO("YGOAL estimateR: %f %f %f $ %f", estimateR.pos_x.val(), estimateR.pos_y.val(), estimateR.angle.val(), estimateR.belief());
			LOG_INFO("YGOAL estimateT: %f %f %f $ %f", estimate.x(), estimate.y(), estimate.angle(), estimate.cov().trace());
			if (estimate.isValid())
				position |= estimate;
		}
	}
	
	LOG_TRACE("Localization observation update ended.");
}

void Localization::reset(ResetCase resetCase)
{
	//TODO: no magic constants!
	position += Particle(Matrix(3,1), Matrix::I<3>()*DEFAULT_VARIANCE);
}

}
