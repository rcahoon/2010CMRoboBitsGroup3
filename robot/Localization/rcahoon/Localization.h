#ifndef _RCAHOON_LOCALIZATION_H_
#define _RCAHOON_LOCALIZATION_H_

#define NUM_PARTICLES  100
#define MV_POS_VAR 1.0f
#define MV_ANGLE_VAR 1.0f;

#define COVAR_THRESH  15.0f
#define COVAR_DECAY  0.2f

#define DIST_THRESH  50

#define MAX_OBJ_COUNT  30

#define BLUE  1
#define YELLOW  -1

#include "Localization/Localization.h"
#include "Localization/Pose.h"
#include "shared/random.h"
#include "shared/Vector/Vector2D.h"
#include "shared/Field/Field.h"
#include "WorldModel/WorldObject/WorldObject.h"
#include "Vision/VisionObject/VisionObject.h"
#include <vector>
#include "shared/Matrix.h"
#include "Log/Log.h"

namespace RCahoon {

class Localization;

class Particle
{
private:
	Matrix S;
	Matrix mcov;
	bool fresh;
	
public:
	Particle(const Matrix& _S, const Matrix& _cov) : S(_S), mcov(_cov), fresh(true) {
		assert(_cov.rows()==3 && _cov.cols()==3);
		assert(_S.rows()==3 && _S.cols()==1);
	}
	Particle(const Vector2D& _pos = Vector2D(0,0), float _angle = 0, const Matrix& _cov = Matrix::I<3>()*DEFAULT_VARIANCE) : S(3,1), mcov(_cov), fresh(true) {
		assert(_cov.rows()==3 && _cov.cols()==3);
		
		S(0) = _pos.x; S(1) = _pos.y; S(2) = _angle;
	}
	
	//void update(Localization& loc, std::vector<WorldObject const *> vis_objs, Particle delta);
	
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
	inline const Matrix& state() const
	{
		return S;
	}
	inline const Matrix& cov() const
	{
		return mcov;
	}
	
	inline void setFresh(bool _f)
	{
		fresh = _f;
	}
	inline bool getFresh() const
	{
		return fresh;
	}
	
	bool operator < (const Particle& b) const
	{
		//TODO:
		return mcov.trace() > b.mcov.trace();
	}
	bool operator > (const Particle& b) const
	{
		//TODO
		return mcov.trace() < b.mcov.trace();
	}
	
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
		//TODO: set isLost intelligently
		return Pose(position(), angle(), mcov, false);
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
	virtual void setScanningForGoals(bool scanning);
	
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
	void goalLocalize(std::vector<Particle>& estimates, std::vector<Particle>& posts, float goalAngle, Vector2D goalPosition);
	void updateGoalPosts(std::vector<Particle>& gposts, const std::vector<VisionObject const *> & vposts, const Particle& odometry);
	
	std::vector<Particle> gbposts;
	std::vector<Particle> gyposts;
	
	bool scanForGoals;
	
	bool isBlue;
	
	std::vector<Particle> particles;
};

}

#endif /* _RCAHOON_LOCALIZATION_H_ */
