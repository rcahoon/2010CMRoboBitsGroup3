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
