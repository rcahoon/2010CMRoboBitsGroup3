#ifndef GOALWORLDOBJECT_H_
#define GOALWORLDOBJECT_H_

#include "TeamColorWorldObject.h"
#include "shared/Vector/Vector2D.h"

class ConfigFile;
class Log;

class GoalWorldObject : public TeamColorWorldObject {
public:
  GoalWorldObject(ConfigFile & configFile,
                  Log & log,
                  float localAngle,
                  bool _teamIsBlue = true);

  virtual ~GoalWorldObject();

  float getLocalAngle() const;

  const Vector2D & getLeftGoalPostLocal() const;

  const Vector2D & getRightGoalPostLocal() const;

  virtual void clear();

  virtual void copyFrom(const WorldObject & other);

  // setAngle() will recalculate the goal posts' positions based on the local
  // position of the goal.
  void setLocalAngle(float localAngle);

private:
  void calculateGoalPosts();

  const float HALF_GOAL_WIDTH;

  Log & log;

  float localAngle;
  Vector2D leftGoalPostLocal, rightGoalPostLocal;

};

#endif /* GOALWORLDOBJECT_H_ */
