#include "Localization.h"
#include "WorldModel/WorldObject/GoalPostWorldObject.h"
#include "WorldModel/WorldFeatures.h"
#include "Vision/VisionFeatures.h"
#include "Agent/RobotState.h"
#include "GameController/GameState.h"
#include "shared/ConfigFile/ConfigFile.h"
#include "shared/random.h"
#include "shared/Shape/Circle.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include "shared/num_util.h"
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

#define COMPONENT WORLD_MODEL
#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

namespace RCahoon {

Localization::Localization(ConfigFile & configFile, Log & _log, Field & _field) :
	log(_log),
	field(_field),
	scanForGoals(false),
	isBlue(true)
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
	cov(0,0) = fabs(T.x) * MV_POS_VAR;
	cov(1,1) = fabs(T.y) * MV_POS_VAR;
	cov(2,2) = fabs(R) * MV_ANGLE_VAR;
	return Particle(T, R, cov);
}

/*std::vector<VisionObject const *> lines = visionFeatures.getVisionObjects(VisionObject::Line);
for(std::vector<VisionObject const *>::iterator iter = lines.begin();
    iter != lines.end(); iter++)
{
	LOG_SHAPE(Log::Field, Circle(pose.convertRelativeToGlobal((*iter)->getPosition()), 2, 0x000000, 1));
}*/

//TODO: set covariance correctly
#define VISION_TO_WORLD(obj)  (Particle((obj).getPosition(), 0, Matrix::I<3>()*3/(obj).getConfidence()))

void Localization::updateGoalPosts(std::vector<Particle>& gposts, const std::vector<VisionObject const *> & vposts, const Particle& odometry)
{
	
	for(std::vector<Particle>::iterator iter = gposts.begin();
		iter != gposts.end();)
	{
		Particle& obj = *iter;
		
		//LOG_SHAPE(Log::Field, Circle(obj.position(), (obj.cov().trace()/3), 0xFFFF80, 3));
		//LOG_SHAPE(Log::Field, Circle(obj.position(), 6, 0x0000FF, 3));
		
		obj -= odometry;
		//obj += Particle(Matrix(3,1), Matrix::I<3>()*COVAR_DECAY);
		obj.setFresh(false);
		
		//LOG_INFO("RPost: %f %f %f $ %f", obj.x(), obj.y(), obj.angle(), (obj.cov().trace()/3));
		
		if ((obj.cov().trace()/3) > COVAR_THRESH)
		{
			printf("dropped\n");
			iter = gposts.erase(iter);
		}
		else
			iter++;
	}
	
	//TODO: do reduction more intelligently
	if (gposts.size() > MAX_OBJ_COUNT)
	{
		gposts.erase(gposts.begin(), gposts.end()-MAX_OBJ_COUNT);
	}
	
	//TODO: store side (left/right) information?
	std::vector<VisionObject const *> _vposts = vposts;
	foreach(const VisionObject* obj, _vposts)
	{
		//LOG_SHAPE(Log::Field, Circle(obj->getPosition(), 3/obj->getConfidence(), 0xFFFF80, 3));
		//LOG_SHAPE(Log::Field, Circle(obj->getPosition(), 6, 0x0000FF, 3));
		gposts.push_back(VISION_TO_WORLD(*obj));
	}
}

void Localization::goalLocalize(std::vector<Particle>& estimates, std::vector<Particle>& posts, float goalAngle, Vector2D goalPosition)
{
	fprintf(stderr, "count %d\n", posts.size());
	for(std::vector<Particle>::iterator iter1 = posts.begin();
		iter1 != posts.end(); iter1++)
	{
		for(std::vector<Particle>::iterator iter2 = iter1+1;
			iter2 != posts.end(); iter2++)
		{
			//TODO: is there a way we can get around making copies while still facilitating the swap below?
			Particle objL = *iter1;
			Particle objR = *iter2;
			
			if (!objL.getFresh() && !objR.getFresh()) continue;
			
			Vector2D postL = objL.position();
			Vector2D postR = objR.position();
			if (angle_diff(postL.angle(), postR.angle()) < 0)
			{
				swap(postL, postR);
				swap(objL, objR);
			}
			/*if (objL->getSide()==GoalPostWorldObject::Right) continue;
			if (objR->getSide()==GoalPostWorldObject::Left) continue;*/
			
			float sqdist = sqdistance(postL, postR);
			if (sqdist < (100*100) || sqdist > (400*400)) continue;
			//printf("dist: %f\n", sqrt(sqdist));
			
			Vector2D goal = (postL+postR)/2;
			Vector2D goalline = postL - postR;
			
			//printf("PostL %f %f $ %f\n", postL.x, postL.y, (covL.trace()/3));
			//printf("PostR %f %f $ %f\n", postR.x, postR.y, (covR.trace()/3));
			
			float angle = angle_diff(goalAngle, goalline.angle());
			Vector2D posEst = goalPosition - goal.rotate(angle)*(140.0f/goalline.length());
			//TODO: do separate var calculations for each position component
			Matrix zcov = objL.cov()+objR.cov();
			Particle estimate(posEst, angle, zcov);
			
			LOG_SHAPE(Log::Field, Circle(estimate.position(), 4, 0x0080FF, 2));
			//LOG_INFO("BGOAL estimateL: %f %f %f $ %f", estimateL.pos_x.val(), estimateL.pos_y.val(), estimateL.angle.val(), estimateL.belief());
			//LOG_INFO("BGOAL estimateR: %f %f %f $ %f", estimateR.pos_x.val(), estimateR.pos_y.val(), estimateR.angle.val(), estimateR.belief());
			//LOG_INFO("GOAL estimateT: %f %f %f $ %f", estimate.x(), estimate.y(), estimate.angle(), (estimate.cov().trace()/3));
			if (estimate.isValid())
				estimates.push_back(estimate);
		}
	}
}

bool Localization::run(const RobotState     & robotState,
                       const GameState      & gameState,
                       const VisionFeatures & visionFeatures,
                       Pose & pose)
{
	LOG_TRACE("Localization started.");

	isBlue = gameState.isOurColorBlue();

	Vector2D _T;
	float _R;
	robotState.getOdometryUpdate(_T, _R);
	Particle odometry = movementModel(_T, -_R);
	
	LOG_INFO("Odometry: %f %f %f $ %f", odometry.x(), odometry.y(), odometry.angle(), (odometry.cov().trace()/3));
	
	foreach(Particle& p, particles)
		p *= odometry;
	
	if (scanForGoals)
	{
		updateGoalPosts(gbposts, visionFeatures.getVisionObjects(VisionObject::BlueGoalPost), odometry);
		updateGoalPosts(gyposts, visionFeatures.getVisionObjects(VisionObject::YellowGoalPost), odometry);
		
		std::vector<Particle> estimates;
		
		Vector2D blueGoal = isBlue ? field.getOurGoal() : field.getOpponentGoal();
		goalLocalize(estimates, gbposts, (float)-M_PI/2, blueGoal);
		
		Vector2D yellowGoal = isBlue ? field.getOpponentGoal() : field.getOurGoal();
		goalLocalize(estimates, gyposts, (float)M_PI/2, yellowGoal);
		
		printf("Ecount: %d\n", estimates.size());
		
		foreach(Particle& e, estimates)
		{
			foreach(Particle& p, particles)
			{
				//TODO: make this better
				if ((p.position() - e.position()).sqlength() < (DIST_THRESH*DIST_THRESH))
				{
					p |= e;
					e.setFresh(false);
				}
			}
			if (e.getFresh())
			{
				particles.push_back(e);
			}
		}
		for(std::vector<Particle>::iterator iter = particles.begin();
			iter != particles.end();)
		{
			Particle& p = *iter;
			
			LOG_SHAPE(Log::Field, Circle(p.position(), 5*(p.cov().trace()/3+0.5f), 0xFFFFFF, 3));
			
			/*if ((p.cov().trace()/3) > COVAR_THRESH)
			{
				printf("dropped\n");
				iter = particles.erase(iter);
			}
			else*/
				iter++;
		}
	}
	
	if (particles.empty())
		pose.setLost(true);
	else
		pose = *max_element(particles.begin(), particles.end());
	
	LOG_INFO("Position: %f %f %f $ %f", pose.getPosition().x, pose.getPosition().y, pose.getAngle(), (pose.getCovariance().trace()/3));
	
	LOG_TRACE("Localization ended.");

	return false;
}

void Localization::setScanningForGoals(bool scanning)
{
	scanForGoals = scanning;
}

void Localization::reset(ResetCase resetCase)
{
	particles.clear();
}

}
