#ifndef KICKTARGETWORLDOBJECT_H_
#define KICKTARGETWORLDOBJECT_H_

#include "WorldObject.h"
#include "shared/Vector/Vector2D.h"

class Log;


class KickTargetWorldObject: public WorldObject {
public:

  KickTargetWorldObject(Log & _log, WorldObject::Type _type = WorldObject::FreeSpace);

  KickTargetWorldObject(const KickTargetWorldObject & other);

  virtual ~KickTargetWorldObject();

  virtual void clear();

  virtual void copyFrom(const WorldObject & other);

private:


};

#endif /* FREESPACEWORLDOBJECT_H_ */
