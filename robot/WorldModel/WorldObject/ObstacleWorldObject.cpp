#include "ObstacleWorldObject.h"

// Define the logging constants
#define COMPONENT WORLD_MODEL
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

ObstacleWorldObject::ObstacleWorldObject(Log & _log)
  : WorldObject(_log, WorldObject::Obstacle),
    obstacle(noObstacle),
    leftObstacleDistance(0),
    rightObstacleDistance(0)
{

}

ObstacleWorldObject::~ObstacleWorldObject() {

}

void ObstacleWorldObject::setObstacleType(const ObstacleType & _obstacleType){
  obstacle = _obstacleType;
}

ObstacleWorldObject::ObstacleType ObstacleWorldObject::getObstacleType() const{
  return obstacle;
}


void ObstacleWorldObject::setLeftObstacleDistance(float _obstacleDistance) {
  leftObstacleDistance = _obstacleDistance;
}

float ObstacleWorldObject::getLeftObstacleDistance() const {
  return leftObstacleDistance;
}

void ObstacleWorldObject::setRightObstacleDistance(float _obstacleDistance) {
  rightObstacleDistance = _obstacleDistance;
}

float ObstacleWorldObject::getRightObstacleDistance() const {
  return rightObstacleDistance;
}
