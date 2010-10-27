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

void movementModel(Vector2D T, float R, Noisy<float>& t_x, Noisy<float>& t_y, Noisy<float>& rot)
{
	t_x = Noisy<float>(T.x, T.x*PARTICLE_POSITION_VAR);
	t_y = Noisy<float>(T.y, T.y*PARTICLE_POSITION_VAR);
	rot = Noisy<float>(R, R*PARTICLE_ANGLE_VAR);
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
	Noisy<float> t_x, t_y, rot;
	movementModel(_T, _R, t_x, t_y, rot);
	
	position.pos_x += t_x;
	position.pos_y += t_y;
	position.angle += rot;
	position.angle.angle_norm();
	
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
	for(std::vector<VisionObject const *>::iterator iter = b_posts.begin();
		iter != b_posts.end(); iter++)
	{
		for(std::vector<VisionObject const *>::iterator iter2 = b_posts.begin();
			iter2 != b_posts.end(); iter2++)
		{
			if (*iter==*iter2) continue;
			float d = sqdistance((*iter)->getPosition(), (*iter2)->getPosition());
			if (d < (200*200) && d > (50*50))
			{
				Vector2D post1 = (*iter)->getPosition();
				Vector2D post2 = (*iter)->getPosition();
				Vector2D goal = (post1+post2)/2;
				float angle = M_PI/2 - acos((post1.sqlength()+post2.sqlength()-(140*140))/(post1.length()*post2.length()));
				Vector2D posEst;
				posEst.heading(angle);
				posEst = posEst * goal.length() + blueGoal;
				
				float conf = (*iter)->getConfidence()*(*iter2)->getConfidence();
				position |= Particle(Noisy<float>(posEst.x, conf), Noisy<float>(posEst.y, conf), Noisy<float>(norm_angle(M_PI + angle - goal.angle())));
			}
		}
	}
	
	
	Vector2D yellowGoal = gameState.isOurColorBlue() ? field.getOpponentGoal() : field.getOurGoal();
	
	std::vector<VisionObject const *> y_posts = visionFeatures.getVisionObjects(VisionObject::YellowGoalPost);
	for(std::vector<VisionObject const *>::iterator iter = y_posts.begin();
		iter != y_posts.end(); iter++)
	{
		for(std::vector<VisionObject const *>::iterator iter2 = y_posts.begin();
			iter2 != y_posts.end(); iter2++)
		{
			if (*iter==*iter2) continue;
			float d = sqdistance((*iter)->getPosition(), (*iter2)->getPosition());
			if (d < (200*200) && d > (50*50))
			{
				Vector2D post1 = (*iter)->getPosition();
				Vector2D post2 = (*iter)->getPosition();
				Vector2D goal = (post1+post2)/2;
				float angle = M_PI/2 + acos((post1.sqlength()+post2.sqlength()-(140*140))/(post1.length()*post2.length()));
				Vector2D posEst;
				posEst.heading(angle);
				posEst = posEst * goal.length() + yellowGoal;
				
				float conf = (*iter)->getConfidence()*(*iter2)->getConfidence();
				position |= Particle(Noisy<float>(posEst.x, conf), Noisy<float>(posEst.y, conf), Noisy<float>(norm_angle(M_PI + angle - goal.angle())));
			}
		}
	}
	
	pose = position;
	
	LOG_TRACE("Localization run ended.");

	return false;
}

void Localization::reset(ResetCase resetCase)
{
	Noisy<float> saturation(0.0f, 1e16f);
	position.pos_x += saturation;
	position.pos_y += saturation;
	position.angle += saturation;
}

}
