#include "Localization.h"
#include "Vision/VisionObject/VisionObject.h"
#include "Vision/VisionFeatures.h"
#include "Agent/RobotState.h"
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

Vector2D& Localization::GOAL_MAP(Vector2D position, bool flp)
{
	int idx = ((int)position.y+l_half_width)*2*l_half_length + flp*((int)position.x+l_half_length);
	return blue_goal_map[idx];
}
Vector2D& Localization::LINE_MAP(Vector2D position)
{
	int idx = ((int)position.y+l_half_width)*2*l_half_length + ((int)position.x+l_half_length);
	return line_map[idx];
}

//#define pdf(x, var) gaussian_pdf(0, var, x)
static inline float pdf(float x, float var)
{
	return var/(var+x*x);
}

Localization::Localization(ConfigFile & configFile, Log & _log, Field & _field) :
	log(_log),
	field(_field),
	particles(new Particle[NUM_PARTICLES]()),
	particles_buf(new Particle[NUM_PARTICLES])
{
	FILE* fid = fopen(configFile.getPath("localization/lineMapPath").c_str(), "r");
	int length, width;
	fscanf(fid, "%d %d", &length, &width);
	line_map = new Vector2D[length*width];
	l_half_width = width/2;
	l_half_length = length/2;
	
	Vector2D p0;
	for(p0.y=-l_half_width; p0.y < l_half_width; p0.y++)
	for(p0.x=-l_half_length; p0.x < l_half_length; p0.x++)
	{
		float x1;
		fscanf(fid, "%f", &x1);
		LINE_MAP(p0).x = x1;
	}
	for(p0.y=-l_half_width; p0.y < l_half_width; p0.y++)
	for(p0.x=-l_half_length; p0.x < l_half_length; p0.x++)
	{
		float y1;
		fscanf(fid, "%f", &y1);
		LINE_MAP(p0).y = y1;
	}
	fclose(fid);
	
	blue_goal_map = new Vector2D[length*width];

	fid = fopen("map.out", "w+");
	for(p0.y=-l_half_width; p0.y < l_half_width; p0.y++)
	{
		for(p0.x=-l_half_length; p0.x < l_half_length; p0.x++)
		{
			/*for(int i=0; i < FIELD_NUM_LINES; i++)
			{
				Vector2D p2, p1;
				field.getLineSegment(i, &p1, &p2);
				Vector2D vec1 = (p2 - p1);
				Vector2D vec2 = (p1 - p0);
				float dist = vec1.cross(vec2) / vec1.length();
				float pr = pdf(dist, LINE_VAR);
				Vector2D subvec = vec1.perp();
				subvec *= subvec.dot(vec2);
				if (subvec.length() > 0.01)
					subvec.normalize();
			
				LINE_MAP(p0) += subvec*pr;
			}*/
			fprintf(fid,"%f ", LINE_MAP(p0).length());
		
			float pr1 = pdf((field.getBlueGoalPostLeft() - p0).length(), GOAL_VAR);
			float pr2 = pdf((field.getBlueGoalPostRight() - p0).length(), GOAL_VAR);
			GOAL_MAP(p0, BLUE) = (field.getBlueGoalPostLeft() - p0).norm(pr1) + (field.getBlueGoalPostRight() - p0).norm(pr2);
		}
		fprintf(fid,"\n");
	}
	fclose(fid);
	
	for(int i=0; i < NUM_PARTICLES; i++)
		particles[i].init(field);
}

Localization::~Localization()
{
	delete[] particles;
	delete[] particles_buf;
	delete[] line_map;
	delete[] blue_goal_map;
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
	
	std::vector<VisionObject const *> vis_objs = visionFeatures.getAllVisionObjects();
	
	int bestPose = 0;
	float belNorm = 0.0f;
	for(int i=0; i < NUM_PARTICLES; i++)
	{
		particles[i].update(*this, vis_objs, t_x, t_y, rot);
		
		belNorm += particles[i].belief();
		if (particles[bestPose].belief() < particles[i].belief()) bestPose = i;
	}
	
	pose = particles[bestPose];	
	
	for(int i=0; i < NUM_PARTICLES; i++)
	{
		//printf("%f %f %f\n", particles[i].position.x, particles[i].position.y, particles[i].belief);
		LOG_SHAPE(Log::Field, Circle(particles[i].position(), max(1.0f, 30*particles[i].belief()), 0x00FFFF, 3));
	}
	LOG_SHAPE(Log::Field, Circle(particles[bestPose].position(), 8, 0x00FFFF, 4));
	
	int k=0;
	for(int i=0; i < NUM_PARTICLES && k < NUM_PARTICLES; i++)
	{
		for (int j=0; j < NUM_PARTICLES*particles[i].belief()/belNorm && k < NUM_PARTICLES; j++)
		{
			particles_buf[k] = particles[i];
			particles_buf[k].pos_x += Noisy<float>(PARTICLE_POSITION_VAR);
			particles_buf[k].pos_y += Noisy<float>(PARTICLE_POSITION_VAR);
			particles_buf[k].angle += Noisy<float>(PARTICLE_ANGLE_VAR);
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
