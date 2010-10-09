#ifndef _RCAHOON_LOCALIZATION_H_
#define _RCAHOON_LOCALIZATION_H_

#define NUM_PARTICLES  1000
#define PARTICLE_POSITION_VAR  4
#define PARTICLE_ANGLE_VAR  0.1f

#include "Localization/Localization.h"
#include "shared/Vector/Vector2D.h"
#include "shared/Field/Field.h"
#include "Localization/Pose.h"

class Log;

namespace RCahoon {

struct Particle
{
	Vector2D position;
	float angle;
	float belief;
	
	Particle() : position(0,0), angle(0.0f), belief(0.0f) {}
	Particle(float x, float y, float ang, float bel) : position(x,y), angle(ang), belief(bel) {}
	Particle(Vector2D pos, float ang, float bel) : position(pos), angle(ang), belief(bel) {}
	
	Vector2D convertRelativeToGlobal(const Vector2D & relativeCoords) const
	{
		float s = sin(angle);
		float c = cos(angle);
		float x = relativeCoords.x;
		float y = relativeCoords.y;
		
		Vector2D globalCoords = position;
		globalCoords.x += c * x - s * y;
		globalCoords.y += s * x + c * y;
		
		return globalCoords;
	}
	
	operator Pose()
	{
		return Pose(position, angle, belief);
	}
};

class Localization: public ::Localization
{
public:
	Localization(ConfigFile & configFile, Log & _log, Field & _field);

	virtual ~Localization();

	virtual bool run(const RobotState   & robotState,
		           const GameState      & gameState,
		           const VisionFeatures & visionFeatures,
		           Pose & pose);
	virtual void updateWorldFeatures(const WorldFeatures & worldFeatures);

private:
	Log & log;
	Field & field;
	int half_length;
	int half_width;
	Particle* particles, *particles_buf;
	float* line_map;
	float* blue_goal_map;

	float& GOAL_MAP(Vector2D position, bool flp);
	float& LINE_MAP(Vector2D position);
	
	Particle initParticle();
	Vector2D& fieldBound(Vector2D& position);
};

}

#endif /* _RCAHOON_LOCALIZATION_H_ */
