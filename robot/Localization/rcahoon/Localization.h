#ifndef _RCAHOON_LOCALIZATION_H_
#define _RCAHOON_LOCALIZATION_H_

#define NUM_PARTICLES  100
#define PARTICLE_POSITION_DECAY 1.0f
#define PARTICLE_ANGLE_DECAY ((float)M_PI/2)

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

#define DEFAULT_PROB 0.25

template <class T>
class NoisyAngle
{
protected:
	T _val;
	float _var;
	
public:
	NoisyAngle() : _val(), _var(DEFAULT_PROB) {}
	//NoisyAngle(float conf) : _val((T)randomGaussian(0, var)), _var(var) {}
	NoisyAngle(T val, float conf) : _val(val), _conf(conf)
	{
		if (conf > 1.0f || conf < 0.0f) throw;
	}
	
	inline const T val() const
	{
		return _val;
	}
	inline float conf() const
	{
		return _conf;
	}
	
	inline NoisyAngle operator + (const NoisyAngle& b) const
	{
		return Noisy(norm_angle(_val+b._val), _conf*b._conf);
	}
	inline NoisyAngle operator - (const NoisyAngle& b) const
	{
		return Noisy(norm_angle(_val-b._val), _conf*b._conf);
	}
	inline NoisyAngle& operator += (const NoisyAngle& b)
	{
		_val = norm_angle(_val + b._val);
		_conf *= b._conf;
		return *this;
	}
	inline NoisyAngle& operator -= (const NoisyAngle& b)
	{
		_val = norm_angle(_val - b._val);
		_conf *= b._conf;
		return *this;
	}
	
	// fusion/averaging operator
	inline NoisyAngle operator | (const NoisyAngle& b) const
	{
		if (isnan(b.val())) return *this;
		if (isnan(this->val())) return b;
		Vector2D vec, vecb;
		vec.heading(_val);
		vecb.heading(b._val);
		return NoisyAngle((vec*_conf+vecb*b._conf).angle(), 1-(1-_conf)*(1-b._conf));
	}
	NoisyAngle& operator |= (const NoisyAngle& b)
	{
		return (*this = *this | b);
	}
	
	// sub-estimate combination
	inline NoisyAngle operator & (const NoisyAngle& b) const
	{
		if (isnan(b.val())) return *this;
		if (isnan(this->val())) return b;
		Vector2D vec, vecb;
		vec.heading(_val);
		vecb.heading(b._val);
		return NoisyAngle((vec*_conf+vecb*b._conf).angle(), _conf*b._conf);
	}
	NoisyAngle& operator &= (const NoisyAngle& b)
	{
		return (*this = *this & b);
	}
	
	// overriding operator
	NoisyAngle& operator ^= (const NoisyAngle& b)
	{
		Vector2D vec, vecb;
		vec.heading(_val);
		vecb.heading(b._val);
		_val = (vec*(1-b._conf) + vecb*b._conf).angle();
		_conf = 1-(1-_conf)*(1-b._conf);
		return *this;
	}
};

template <class T>
class Noisy
{
protected:
	T _val;
	float _conf;
	
public:
	Noisy() : _val(), _conf(DEFAULT_PROB) {}
	//Noisy(float var) : _val((T)randomGaussian(0, var)), _var(var) {}
	Noisy(T val, float conf) : _val(val), _conf(conf)
	{
		if (conf > 1.0f || conf < 0.0f) throw;
	}
	
	inline const T val() const
	{
		return _val;
	}
	inline float conf() const
	{
		return _conf;
	}
	
	inline Noisy operator + (const Noisy& b) const
	{
		return Noisy(_val+b._val, _conf*b._conf);
	}
	inline Noisy operator - (const Noisy& b) const
	{
		return Noisy(_val-b._val, _conf*b._conf);
	}
	inline Noisy& operator += (const Noisy& b)
	{
		_val += b._val;
		_conf *= b._conf;
		return *this;
	}
	inline Noisy& operator -= (const Noisy& b)
	{
		_val -= b._val;
		_conf *= b._conf;
		return *this;
	}
	
	// fusion/averaging operator
	inline Noisy operator | (const Noisy& b) const
	{
		if (isnan(b.val())) return *this;
		if (isnan(this->val())) return b;
		return Noisy((_val*_conf+b._val*b._conf)/(_conf+b._conf+0.0001), 1-(1-_conf)*(1-b._conf));
	}
	Noisy& operator |= (const Noisy& b)
	{
		return (*this = *this | b);
	}
	
	// sub-estimate combination
	inline Noisy operator & (const Noisy& b) const
	{
		if (isnan(b.val())) return *this;
		if (isnan(this->val())) return b;
		return Noisy((_val*_conf+b._val*b._conf)/(_conf+b._conf+0.0001), _conf*b._conf);
	}
	Noisy& operator &= (const Noisy& b)
	{
		return (*this = *this & b);
	}
	
	// overriding operator
	Noisy& operator ^= (const Noisy& b)
	{
		_val = _val*(1-b._conf) + b._val*b._conf;
		_conf = 1-(1-_conf)*(1-b._conf);
		return *this;
	}
};

struct Particle
{
	Noisy<float> pos_x;
	Noisy<float> pos_y;
	NoisyAngle<float> angle;
	
	Particle() : pos_x(), pos_y(), angle() {}
	Particle(Noisy<float> x, Noisy<float> y, NoisyAngle<float> ang) : pos_x(x), pos_y(y), angle(ang) {}
	Particle(float x, float y, float ang, float conf) : pos_x(x, pow(conf, 0.3333f)), pos_y(y, pow(conf, 0.3333f)), angle(ang, pow(conf, 0.3333f)) {}
	Particle(Vector2D pos, float ang, float conf) : pos_x(pos.x, pow(conf, 0.3333f)), pos_y(pos.y, pow(conf, 0.3333f)), angle(ang, pow(conf, 0.3333f)) {}
	
	void init(Field& field);
	void update(Localization& loc, std::vector<VisionObject const *> vis_objs, Noisy<float> t_x, Noisy<float> t_y, NoisyAngle<float> rot);
	
	inline float belief() const
	{
		return pos_x.conf()*pos_y.conf()*angle.conf();
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
	/*Particle operator + (const Particle& b)
	{
		Vector2d pos = b.position().rotate(angle.val()) + position();
		return Particle(Noisy<float>(pos.x, b.pos_x.conf()*);
	}*/
	
	// fusion/averaging operator
	inline Particle operator | (const Particle& b) const
	{
		Particle newVal(pos_x | b.pos_x, pos_y | b.pos_y, angle | b.angle);
		return newVal;
	}
	Particle& operator |= (const Particle& b)
	{
		return (*this = *this | b);
	}
	
	Particle& operator ^= (const Particle& b)
	{
		pos_x ^= b.pos_x;
		pos_y ^= b.pos_y;
		angle ^= b.angle;
		return *this;
	}
	
	// sub-estimate combination
	inline Particle operator & (const Particle& b) const
	{
		Particle newVal(pos_x & b.pos_x, pos_y & b.pos_y, angle & b.angle);
		return newVal;
	}
	Particle& operator &= (const Particle& b)
	{
		return (*this = *this & b);
	}
	
	inline operator Pose() const
	{
		return Pose(position(), angle.val(), belief());
	}
	
	bool isValid()
	{
		return !isnan(pos_x.val()) && !isnan(pos_y.val()) && !isnan(angle.val());
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
