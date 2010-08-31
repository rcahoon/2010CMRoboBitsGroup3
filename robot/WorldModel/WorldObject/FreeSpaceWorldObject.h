#ifndef FREESPACEWORLDOBJECT_H_
#define FREESPACEWORLDOBJECT_H_

#include "WorldObject.h"
#include "shared/Vector/Vector2D.h"

class Log;

//

#define FREE_SPACE_COVERAGE M_PI 
#define NUM_FREE_SPACE_SLOTS 45 
#define SLOT_ANGLE (FREE_SPACE_COVERAGE / NUM_FREE_SPACE_SLOTS)
#define MAX_FREE_SPACE_DISTANCE 600

/**
 * This represents world objects that have velocity, such as the ball.
 */
class FreeSpaceWorldObject: public WorldObject {
public:

  FreeSpaceWorldObject(Log & _log, WorldObject::Type _type = WorldObject::FreeSpace);

  FreeSpaceWorldObject(const FreeSpaceWorldObject & other);

  virtual ~FreeSpaceWorldObject();

  virtual void clear();

  virtual void copyFrom(const WorldObject & other);

  float getDistance(int slot) const;
  float getAngleDistance(float angle) const;
  int getAngleSlot(float distance) const;
  float getSlotAngle(int slot) const;
  int getMaxDistanceSlot(float startAngle = -M_PI/2, float endAngle = M_PI/2 ) const;
  bool isOccupied(float startAngle, float endAngle, float distanceThreshold) const;
  void  setDistance(int slot, float distance);
  Vector2D getMaxFreeSpacePoint( float startAngle, float endAngle ) const;

private:

  float distances[NUM_FREE_SPACE_SLOTS];

};

#endif /* FREESPACEWORLDOBJECT_H_ */
