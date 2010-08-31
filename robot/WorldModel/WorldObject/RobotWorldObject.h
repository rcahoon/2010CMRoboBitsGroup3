#ifndef ROBOTWORLDOBJECT_H_
#define ROBOTWORLDOBJECT_H_

#include "TeamColorWorldObject.h"

class RobotWorldObject : public TeamColorWorldObject {
public:
  RobotWorldObject(Log & log, WorldObject::Type type, bool teamIsBlue);

  virtual ~RobotWorldObject();

  bool isPenalized() const;

  bool isLifted() const;

  bool isFallen() const;

  void setPenalized(bool penalized = true);

  void setLifted(bool lifted = true);

  void setFallen(bool fallen = true);

  virtual void clear();

  virtual void copyFrom(const WorldObject & other);

private:
  bool penalized, lifted, fallen;

};

#endif /* ROBOTWORLDOBJECT_H_ */
