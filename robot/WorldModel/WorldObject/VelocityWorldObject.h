#ifndef VELOCITYWORLDOBJECT_H_
#define VELOCITYWORLDOBJECT_H_

#include "WorldObject.h"
#include "shared/Vector/Vector2D.h"

class Log;

/**
 * This represents world objects that have velocity, such as the ball.
 */
class VelocityWorldObject: public WorldObject {
public:
  VelocityWorldObject(Log & _log, WorldObject::Type _type);

  VelocityWorldObject(const VelocityWorldObject & other);

  virtual ~VelocityWorldObject();

  const Vector2D & getVelocity() const;

  void setVelocity(const Vector2D & _velocity);

  virtual void clear();

  virtual void copyFrom(const WorldObject & other);

private:
  Vector2D velocity;

};

#endif /* VELOCITYWORLDOBJECT_H_ */
