#ifndef _RCAHOON_LOCALIZATION_H_
#define _RCAHOON_LOCALIZATION_H_

#define NUM_PARTICLES  100
#define MV_POS_VAR 1.0f
#define MV_ANGLE_VAR 1.0f;

#define BLUE  1
#define YELLOW  -1

#include "Localization/Localization.h"
#include "Localization/Pose.h"
#include "shared/random.h"
#include "shared/Vector/Vector2D.h"
#include "shared/Field/Field.h"
#include "Vision/VisionObject/VisionObject.h"
#include <vector>
#include "shared/Matrix.h"

class Log;

namespace RCahoon {

class Localization;

class Particle
{
private:
	Matrix S;
	Matrix mcov;
	
public:
	Particle() : S(3,1), mcov(Matrix::I<3>()) {}
	Particle(const Matrix& _S, const Matrix& _cov) : S(_S), mcov(_cov) {
		assert(_cov.rows()==3 && _cov.cols()==3);
		assert(_S.rows()==3 && _S.cols()==1);
	}
	Particle(const Vector2D& _pos, float _angle, const Matrix& _cov) : S(3,1), mcov(_cov) {
		assert(_cov.rows()==3 && _cov.cols()==3);
		
		S(0) = _pos.x; S(1) = _pos.y; S(2) = _angle;
	}
	
	void init(Field& field);
	void update(Localization& loc, std::vector<VisionObject const *> vis_objs, Particle delta);
	
	inline float x() const
	{
		return S(0);
	}
	inline float y() const
	{
		return S(1);
	}
	inline Vector2D position() const
	{
		return Vector2D(S(0), S(1));
	}
	inline float angle() const
	{
		return S(2);
	}
	inline const Matrix& cov()
	{
		return mcov;
	}
	/*float belief() const
	{
		return 1.0f/mcov.trace();
	}*/
	
	// global-coordinates addition operators
	inline Particle& operator += (const Particle& b)
	{
		S += b.S;
		mcov += b.mcov;
		return *this;
	}
	inline Particle operator + (const Particle& b) const
	{
		return Particle(*this) += b;
	}
	inline Particle& operator -= (const Particle& b)
	{
		S -= b.S;
		mcov += b.mcov;
		return *this;
	}
	inline Particle operator - (const Particle& b) const
	{
		return Particle(*this) -= b;
	}
	
	// composition operators
	inline Particle operator * (const Particle& b) const
	{
		return (b.rotate(angle()) += *this); // depends on commutability of + operator
	}
	inline Particle& operator *= (const Particle& b)
	{
		return (*this = *this * b);
	}
	
	// observation operators
	Particle& operator |= (const Particle& z)
	{
		Matrix K = mcov * (mcov + z.mcov).inv();
		S += K * (z.S - S);
		mcov = (Matrix::I<3>() - K) * mcov;
		return *this;
	}
	inline Particle operator | (const Particle& b) const
	{
		return Particle(*this) |= b;
	}
	
	inline operator Pose() const
	{
		return Pose(position(), angle(), mcov.trace());
	}
	
	bool isValid() const
	{
		return !isnan(S(0)) && !isnan(S(1)) && !isnan(S(2));
	}
	
	Particle rotate(float ang) const
	{
		Matrix R(3,3);
		float c = cos(ang);
		float s = sin(ang);
		R(0,0) = c;
		R(1,0) = s;
		R(0,1) = -s;
		R(1,1) = c;
		R(2,2) = 1;
		return Particle(R*S, R*mcov*R.T());
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
	
    virtual void updateWorldFeatures(const WorldFeatures & worldFeatures);
    
    static Particle movementModel(Vector2D T, float R);

private:
	Log & log;
	Field & field;
	
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
