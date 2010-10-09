#include "Localization.h"
#include "Vision/VisionObject/VisionObject.h"
#include "Vision/VisionFeatures.h"
#include "Agent/RobotState.h"
#include "shared/random.h"
#include "shared/Shape/Circle.h"
#include <cmath>
#include <vector>
#include "shared/num_util.h"

#define COMPONENT VISION
#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "Log/LogSettings.h"

#define BLUE  1
#define YELLOW  -1
#define LINE_VAR  10
#define GOAL_VAR  20

namespace RCahoon {

inline float& Localization::GOAL_MAP(Vector2D position, bool flp)
{
	int idx = ((int)position.y+half_width)*2*half_length + flp*((int)position.x+half_length);
	return blue_goal_map[idx];
}
inline float& Localization::LINE_MAP(Vector2D position)
{
	int idx = ((int)position.y+half_width)*2*half_length + ((int)position.x+half_length);
	return line_map[idx];
}

Particle Localization::initParticle()
{
	return Particle(randomDbl(-half_length, half_length), randomDbl(-half_width, half_width),
		            randomDbl(0, 2*M_PI), 1.0f);
}

Localization::Localization(ConfigFile & configFile, Log & _log, Field & _field) :
	log(_log),
	field(_field),
	half_length((int)_field.getHalfFieldLength()),
	half_width((int)_field.getHalfFieldWidth()),
	particles(new Particle[NUM_PARTICLES]()),
	particles_buf(new Particle[NUM_PARTICLES]),
	line_map(new float[4*half_length*half_width]),
	blue_goal_map(new float[4*half_length*half_width])
{
	Vector2D p0;
	for(p0.x=-half_length; p0.x < half_length; p0.x++)
	for(p0.y=-half_width; p0.y < half_width; p0.y++)
	{
		for(int i=0; i < FIELD_NUM_LINES; i++)
		{
			Vector2D p2, p1;
			field.getLineSegment(i, &p1, &p2);
			float dist = (p2 - p1).cross(p1 - p0) / (p2 - p1).length();
			LINE_MAP(p0) = gaussian_pdf(0, LINE_VAR, dist);
		}
		
		GOAL_MAP(p0, BLUE) = gaussian_pdf(0, GOAL_VAR, (field.getBlueGoalPostLeft() - p0).length()) +
		                     gaussian_pdf(0, GOAL_VAR, (field.getBlueGoalPostRight() - p0).length());
	}
	
	/*for(int i=0; i < NUM_PARTICLES; i++)
	{
		particles[i] = initParticle();
	}*/
}

Localization::~Localization()
{
	delete[] line_map;
}

float movementModel(Vector2D translation, float rotation)
{
	float pr = 1.0f/(translation.length()/4+1)/(rotation+1);
	return pr;
}

Vector2D& Localization::fieldBound(Vector2D& position)
{
	position.x = max(position.x, (float)-half_length);
	position.x = min(position.x, (float)half_length);
	position.y = max(position.y, (float)-half_width);
	position.y = min(position.y, (float)half_width);
	return position;
}

bool Localization::run(const RobotState     & robotState,
                       const GameState      & gameState,
                       const VisionFeatures & visionFeatures,
                       Pose & pose)
{
	LOG_TRACE("Localization run started.");

	Vector2D translation;
	float rotation;
	robotState.getOdometryUpdate(translation, rotation);
	
	std::vector<VisionObject const *> vis_objs = visionFeatures.getAllVisionObjects();
	int bestPose = 0;
	float belNorm = 0.0f;
	for(int i=0; i < NUM_PARTICLES; i++)
	{
		fieldBound(particles[i].position += translation);
		particles[i].angle += rotation;
		particles[i].belief *= movementModel(translation, rotation);
		
		if (particles[i].belief < 1.0f/NUM_PARTICLES) particles[i] = initParticle();
		
		/*if (vis_objs.size() > 0)
		{
			//std::vector<VisionObject const *>::iterator iter = vis_objs.begin();
			float belief = 0.0f;
			//for(; iter != vis_objs.end(); iter++)
			for(unsigned j=0; j < vis_objs.size(); j++)
			{
				//const VisionObject* obj = *iter;
				const VisionObject* obj = vis_objs[j];
				Vector2D position = particles[i].convertRelativeToGlobal(obj->getPosition());
				fieldBound(position);
				switch(obj->getType())
				{
					case VisionObject::Line:
						belief += obj->getConfidence() * LINE_MAP(position);
					break;
					case VisionObject::BlueGoalPost:
						belief += obj->getConfidence() * GOAL_MAP(position, BLUE);
					break;
					case VisionObject::YellowGoalPost:
						belief += obj->getConfidence() * GOAL_MAP(position, YELLOW);
					break;
					default:
					break;
				}
			}
			particles[i].belief *= belief;
		}*/
		
		belNorm += particles[i].belief;
		if (particles[bestPose].belief < particles[i].belief) bestPose = i;
	}
	
	float totNorm = 0.0f;
	std::vector<VisionObject const *>::iterator iter = vis_objs.begin();
	for(; iter != vis_objs.end(); iter++)
	{
		totNorm += (*iter)->getConfidence();
	}
	for(int i=0; i < NUM_PARTICLES; i++)
	{
		particles[i].belief /= belNorm;
		
		printf("%f %f %f\n", particles[i].position.x, particles[i].position.y, particles[i].belief);
		//LOG_SHAPE(Log::Field, Circle(particles[i].position, max(1.0f, 30*particles[i].belief), 0x00FFFF, 3));
	}
	
	pose = particles[bestPose];
	
	/*LOG_SHAPE(Log::Field, Circle(particles[bestPose].position, 8, 0x00FFFF, 4));
	printf("Size: %d\n", vis_objs.size());
	for(unsigned j=0; j < vis_objs.size(); j++)
	{
		//const VisionObject* obj = *iter;
		const VisionObject* obj = vis_objs[j];
		Vector2D position = particles[bestPose].convertRelativeToGlobal(obj->getPosition());
		//printf("%f %f\n", obj->getPosition().x, obj->getPosition().y);
		
		switch(obj->getType())
		{
			case VisionObject::Line:
				LOG_SHAPE(Log::Field, Circle(position, 5, 0xFFFFFF, 2));
			break;
			case VisionObject::BlueGoalPost:
				LOG_SHAPE(Log::Field, Circle(position, 5, 0x0000FF, 2));
			break;
			case VisionObject::YellowGoalPost:
				LOG_SHAPE(Log::Field, Circle(position, 5, 0xFFFF00, 2));
			break;
			case VisionObject::Ball:
				LOG_SHAPE(Log::Field, Circle(position, 5, 0xFF0000, 2));
			break;
			default:
			break;
		}
	}*/
	
	int k=0;
	for(int i=0; i < NUM_PARTICLES && k < NUM_PARTICLES; i++)
	{
		for (int j=0; j < NUM_PARTICLES*particles[i].belief && k < NUM_PARTICLES; j++)
		{
			particles_buf[k] = particles[i];
			particles_buf[k].position.x += randomGaussian(0, PARTICLE_POSITION_VAR);
			particles_buf[k].position.y += randomGaussian(0, PARTICLE_POSITION_VAR);
			particles_buf[k].angle += randomGaussian(0, PARTICLE_ANGLE_VAR);
			k++;
		}
	}
	swap(particles, particles_buf);
	
	//TODO: sensor resetting
	
	LOG_TRACE("Localization run ended.");

	return false;
}

void Localization::updateWorldFeatures(const WorldFeatures & worldFeatures)
{
}

}
