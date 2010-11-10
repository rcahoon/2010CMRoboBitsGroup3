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
	l_half_length(_field.getHalfFieldLength()),
	l_half_width(_field.getHalfFieldWidth()),
	position()
{
	reset(none);
}

Localization::~Localization()
{
}

Vector2D& Localization::fieldBound(Vector2D& position)
{
	bound(position.x, -l_half_length, l_half_length);
	bound(position.y, -l_half_width, l_half_width);
	return position;
}

void movementModel(Vector2D T, float R, Noisy<float>& t_x, Noisy<float>& t_y, NoisyAngle<float>& rot)
{
	t_x = Noisy<float>(T.x, PARTICLE_POSITION_DECAY/(PARTICLE_POSITION_DECAY+abs(T.x)));
	t_y = Noisy<float>(T.y, PARTICLE_POSITION_DECAY/(PARTICLE_POSITION_DECAY+abs(T.y)));
	rot = NoisyAngle<float>(R, PARTICLE_ANGLE_DECAY/(PARTICLE_ANGLE_DECAY+abs(R)));
}

bool Localization::run(const RobotState     & robotState,
                       const GameState      & gameState,
                       const VisionFeatures & visionFeatures,
                       Pose & pose)
{
	LOG_TRACE("Localization run started.");

	Vector2D _T;
	float _R;
	robotState.getOdometryUpdate(_T, _R);
	_T = _T.rotate(position.angle.val());
	Noisy<float> t_x, t_y;
	NoisyAngle<float> rot;
	movementModel(_T, -_R, t_x, t_y, rot);
	
	position.pos_x += t_x;
	position.pos_y += t_y;
	position.angle += rot;
	
	LOG_INFO("Motion estimate: %f %f %f $ %f", position.pos_x.val(), position.pos_y.val(), position.angle.val(), position.belief());
	
#if 1
	Vector2D blueGoal = gameState.isOurColorBlue() ? field.getOurGoal() : field.getOpponentGoal();
	
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
	
	std::vector<VisionObject const *> b_posts = visionFeatures.getVisionObjects(VisionObject::BlueGoalPost);
	for(std::vector<VisionObject const *>::iterator iter1 = b_posts.begin();
		iter1 != b_posts.end(); iter1++)
	{
		for(std::vector<VisionObject const *>::iterator iter2 = iter1+1;
			iter2 != b_posts.end(); iter2++)
		{
			/*float d = sqdistance((*iter1)->getPosition(), (*iter2)->getPosition());
			if (d < (200*200) && d > (50*50))
			{*/
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
				
				float angle = -angle_diff(goalline.angle(), (float)-M_PI/2);
				Vector2D posEst = -goal.rotate(angle)*(140.0f/goalline.length()) + blueGoal;
				printf("%f %f %f\n", confL, confR, sqrt(confL*confR));
				//TODO: do separate prob calculations for each position component
				Particle estimate(posEst, angle, sqrt(confL*confR));
				
				LOG_SHAPE(Log::Field, Circle(estimate.position(), 4, 0x0080FF, 2));
				//LOG_INFO("BGOAL estimateL: %f %f %f $ %f", estimateL.pos_x.val(), estimateL.pos_y.val(), estimateL.angle.val(), estimateL.belief());
				//LOG_INFO("BGOAL estimateR: %f %f %f $ %f", estimateR.pos_x.val(), estimateR.pos_y.val(), estimateR.angle.val(), estimateR.belief());
				LOG_INFO("BGOAL estimateT: %f %f %f $ %f", estimate.pos_x.val(), estimate.pos_y.val(), estimate.angle.val(), estimate.belief());
				if (estimate.isValid())
					position ^= estimate;
			/*}
			else
				LOG_ERROR("Posts too close");*/
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
			/*float d = sqdistance((*iter1)->getPosition(), (*iter2)->getPosition());
			if (d < (200*200) && d > (50*50))
			{*/
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
				
				float angle = -angle_diff(goalline.angle(), (float)M_PI/2);
				Vector2D posEst = -goal.rotate(angle)*(140.0f/goalline.length()) + yellowGoal;
				//TODO: do separate prob calculations for each position component
				Particle estimate(posEst, angle, sqrt(confL*confR));
				
				LOG_SHAPE(Log::Field, Circle(estimate.position(), 4, 0x80FF80, 2));
				//LOG_INFO("YGOAL estimateL: %f %f %f $ %f", estimateL.pos_x.val(), estimateL.pos_y.val(), estimateL.angle.val(), estimateL.belief());
				//LOG_INFO("YGOAL estimateR: %f %f %f $ %f", estimateR.pos_x.val(), estimateR.pos_y.val(), estimateR.angle.val(), estimateR.belief());
				LOG_INFO("YGOAL estimateT: %f %f %f $ %f", estimate.pos_x.val(), estimate.pos_y.val(), estimate.angle.val(), estimate.belief());
				if (estimate.isValid())
					position ^= estimate;
			/*}
			else
				LOG_ERROR("Posts too close");*/
		}
	}
#endif
	
	pose = position;
	//LOG_INFO("Pose %f %f %f $ %f\n", position.pos_x.val(), position.pos_y.val(), position.angle.val(), position.belief());
	
	LOG_TRACE("Localization run ended.");

	return false;
}

void Localization::reset(ResetCase resetCase)
{
	Noisy<float> resetPos(0.0f, 0.0f);
	NoisyAngle<float> resetAng(0.0f, 0.0f);
	position.pos_x += resetPos;
	position.pos_y += resetPos;
	position.angle += resetAng;
}

}
