#ifndef _RCAHOON_LOCALIZATION_H_
#define _RCAHOON_LOCALIZATION_H_

/*#define LINE_VAR  10
#define GOAL_VAR  20*/
#define LINE_VAR 250
#define GOAL_VAR 500
#define NUM_PARTICLES  100
#define PARTICLE_POSITION_VAR  0.25f
#define PARTICLE_ANGLE_VAR  0.4f

#define BLUE  1
#define YELLOW  -1

#include "Localization/Localization.h"
#include "Localization/Pose.h"
#include "shared/random.h"
#include "shared/Vector/Vector2D.h"
#include "shared/Field/Field.h"
#include "Vision/VisionObject/VisionObject.h"
#include <vector>

class Log;

namespace RCahoon {

class Localization;

template <class T>
class Noisy
{
protected:
	T _val;
	float _var;
	
public:
	Noisy() : _val(), _var(1.0) {}
	Noisy(float var) : _val((T)randomGaussian(0, var)), _var(var) {}
	Noisy(T val, float var) : _val(val), _var(var) {}
	
	inline const T val() const
	{
		return _val;
	}
	inline float var() const
	{
		return _var;
	}
	
	void angle_norm()
	{
		_val = norm_angle(_val);
	}
	
	inline Noisy operator + (const Noisy& b) const
	{
		return Noisy(_val+b._val, _var+b._var);
	}
	inline Noisy operator - (const Noisy& b) const
	{
		return Noisy(_val-b._val, _var+b._var);
	}
	inline Noisy& operator += (const Noisy& b)
	{
		_val += b._val;
		_var += b._var;
		return *this;
	}
	inline Noisy& operator -= (const Noisy& b)
	{
		_val -= b._val;
		_var += b._var;
		return *this;
	}
	
	// fusion/averaging operator
	inline Noisy operator | (const Noisy& b) const
	{
		return Noisy((_val*b._var+b._val*_var)/(_var+b._var), _var*b._var/(_var+b._var));
	}
	Noisy& operator |= (const Noisy& b)
	{
		return (*this = *this | b);
	}
};

struct Particle
{
	Noisy<float> pos_x;
	Noisy<float> pos_y;
	Noisy<float> angle;
	
	Particle() : pos_x(), pos_y(), angle() {}
	Particle(Noisy<float> x, Noisy<float> y, Noisy<float> ang) : pos_x(x), pos_y(y), angle(ang) {}
	
	void init(Field& field);
	void update(Localization& loc, std::vector<VisionObject const *> vis_objs, Noisy<float> t_x, Noisy<float> t_y, Noisy<float> rot);
	
	#define RECIP_2PI  0.1592f
	inline float belief() const
	{
		return sqrt(RECIP_2PI/pos_x.var() + RECIP_2PI/pos_y.var() + RECIP_2PI/angle.var());
	}
	Vector2D position() const
	{
		return Vector2D(pos_x.val(), pos_y.val());
	}
	
	//TODO: update to make probabilistic
	Vector2D convertRelativeToGlobal(const Vector2D & relativeCoords) const
	{
		float s = sin(angle.val());
		float c = cos(angle.val());
		float x = relativeCoords.x;
		float y = relativeCoords.y;
		
		Vector2D globalCoords = position();
		globalCoords.x += c * x - s * y;
		globalCoords.y += s * x + c * y;
		
		return globalCoords;
	}
	
	// fusion/averaging operator
	inline Particle operator | (const Particle& b) const
	{
		Particle newVal(pos_x | b.pos_x, pos_y | b.pos_y, angle | b.angle);
		newVal.angle.angle_norm();
		return newVal;
	}
	Particle& operator |= (const Particle& b)
	{
		return (*this = *this | b);
	}
	
	inline operator Pose() const
	{
		return Pose(position(), angle.val(), belief());
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
	
	virtual void reset(ResetCase resetCase);

private:
	Log & log;
	Field & field;
	int l_half_length;
	int l_half_width;
	/*Particle* particles, *particles_buf;
	Vector2D* line_map;
	Vector2D* blue_goal_map;

	Vector2D& GOAL_MAP(Vector2D position, bool flp);
	Vector2D& LINE_MAP(Vector2D position);*/
	
	Vector2D& fieldBound(Vector2D& position);
	
	/**/Particle position;
	
friend class Particle;
};

}

#endif /* _RCAHOON_LOCALIZATION_H_ */
