#ifndef OBSTACLEMODELOBJECT_H_
#define OBSTACLEMODELOBJECT_H_

#include "WorldObject.h"

class ConfigFile;
class Log;

class ObstacleWorldObject : public WorldObject
{
public:
  enum ObstacleType {
    noObstacle = 0,
    leftObstacle,
    rightObstacle,
    frontObstacle
  };

  ObstacleWorldObject(Log & _log);
  ~ObstacleWorldObject();

  void setObstacleType(const ObstacleType & _obstacleType);
  ObstacleWorldObject::ObstacleType getObstacleType() const;

  void setLeftObstacleDistance(float _obstacleDistance);
  float getLeftObstacleDistance() const;
  void setRightObstacleDistance(float _obstacleDistance);
  float getRightObstacleDistance() const;
private:

  ObstacleType obstacle;
  float leftObstacleDistance;
  float rightObstacleDistance;
};

#endif /* OBSTACLEMODELOBJECT_H_ */
