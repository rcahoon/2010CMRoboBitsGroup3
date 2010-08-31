#ifndef GOALPOSTWORLDOBJECT_H_
#define GOALPOSTWORLDOBJECT_H_

#include "TeamColorWorldObject.h"

class Log;

class GoalPostWorldObject : public TeamColorWorldObject {
public:

  enum Side {
    Unknown = 0,
    Left = -1,
    Right = 1,
  };

  GoalPostWorldObject(Log & _log,
                      bool _teamIsBlue = true,
                      Side _side = Unknown);

  GoalPostWorldObject(const GoalPostWorldObject & other);

  virtual ~GoalPostWorldObject();

  Side getSide() const;

  void setSide(Side _side);

  virtual void clear();

  virtual void copyFrom(const WorldObject & other);

private:

  Side side;

};

#endif /* GOALPOSTWORLDOBJECT_H_ */
