#ifndef POSE_H_
#define POSE_H_

#include "shared/Vector/Vector2D.h"
#include "WorldModel/WorldObject/WorldObject.h"

#define POSE_NUM_RANDOM_SAMPLES 10

class Pose {
public:
  Pose(const Vector2D & _position = Vector2D(0, 0),
       const float _angle = 0,
       const Matrix& _covariance = Matrix::I<3>()*DEFAULT_VARIANCE,
       const bool _lost = true);

  /**
   * Copy constructor.
   */
  Pose(const Pose & other);

  virtual ~Pose();

  const Vector2D getPosition() const;
  void setPosition(const Vector2D & _position);
  void setPosition(const float x, const float y);

  float getAngle() const;
  void setAngle(const float _angle);

  const Matrix& getCovariance() const;
  void setCovariance(const Matrix& _covariance);

  bool isLost() const;
  void setLost(const bool _lost);

  Vector2D convertRelativeToGlobal(const Vector2D & relativeCoords) const;
  float convertRelativeAngleToGlobal(const float relativeAngle) const;

  Vector2D convertGlobalToRelative(const Vector2D & globalCoords) const;
  float convertGlobalAngleToRelative(const float globalAngle) const;

  /**
   * Allow assignment.
   */
  Pose & operator=(const Pose & other);

private:
  Vector2D position;
  float    angle;
  Matrix   covariance;
  bool     lost;
};

#endif

