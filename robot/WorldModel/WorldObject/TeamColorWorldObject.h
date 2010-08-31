#ifndef TEAMCOLORWORLDOBJECT_H_
#define TEAMCOLORWORLDOBJECT_H_

#include "WorldObject.h"

class TeamColorWorldObject : public WorldObject {
public:

  TeamColorWorldObject(Log & _log,
                       WorldObject::Type _type,
                       bool _teamIsBlue = true);

  TeamColorWorldObject(const TeamColorWorldObject & other);

  virtual ~TeamColorWorldObject();

  bool isTeamBlue() const;

  void setTeamColor(const bool _teamIsBlue = true);

  virtual void clear();

  virtual void copyFrom(const WorldObject & other);

private:

  bool teamIsBlue;

};

#endif /* TEAMCOLORWORLDOBJECT_H_ */
