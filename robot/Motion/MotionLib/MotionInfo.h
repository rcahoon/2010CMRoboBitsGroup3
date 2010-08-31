#ifndef MOTIONINFO_H_
#define MOTIONINFO_H_

#include "Agent/Command/MotionCommand/BodyCommand/StaticAction.h"
#include "shared/UsefulMacros.h"
#include "shared/Vector/Vector2D.h"

class ConfigFile;
class Log;

class MotionInfo {
public:
  MotionInfo(ConfigFile & configFile, Log & log,
             StaticAction::StaticActionType _type,
             float _minX, float _maxX, float _minY, float _maxY,
             float _odometryX, float _odometryY, float _odometryTheta,
             float _resultR, float _resultTheta, float _deviationResultR, float _deviationResultTheta,
             float _timeToExecute, int _motionType);
  virtual ~MotionInfo();
  void setType(StaticAction::StaticActionType _type);
  StaticAction::StaticActionType getType() const;
  void setBoundingBox(float _minX, float _maxX, float _minY, float _maxY);
  void getBoundingBox(float & _minX, float & _maxX, float & _minY, float & _maxY) const;
  const Vector2D & getMidBounds() const;
  void setOdometryChanges(float _odometryX, float _odometryY, float _odometryTheta);
  void getOdometryChanges(Vector2D & odometryTranslation, float & _odometryTheta) const;
  void setMotionEffects(float _resultR, float _resultTheta, float _deviationResultR, float _deviationResultTheta);
  void getMotionEffects(float & _resultR, float & _resultTheta, float & _deviationResultR, float & _deviationResultTheta) const;
  void setTimeToExecute(float _timeToExecute);
  void getTimeToExecute(float & _timeToExecute) const;
  void setMotionType(int _motionType);
  void getMotionType(int & _motionType) const;

  bool isAForwardKick(bool & isLeftKick) const;
  bool isAngleGoodForKick(const Vector2D & relTarget) const;
  bool isAngleGoodForKick(float minAngleValue, float maxAngleValue) const;
  bool isBallInKickRange(const Vector2D & ballPosition) const;
  Vector2D getKickEffect(const Vector2D & ballPosition) const;
  float getKickScore(const Vector2D & relTarget, const Vector2D & ballPosition, bool through = true, bool useAngle = false, float minAngle = 0, float maxAngle = 0) const;

  float getResultTheta() const;

private:
  const float ANGLE_THRESH;

  Log & log;

  StaticAction::StaticActionType type;
  //bounding box
  float minX, maxX, minY, maxY;
  //changes in odometry
  float odometryX, odometryY, odometryTheta;
  //effect from motion
  float resultR, resultTheta, deviationResultR, deviationResultTheta;
  //time to execute
  float timeToExecute;
  //type of motion
  int motionType;
  //Weights to score the kick
  float angleWeight, distWeight, ballDistWeight;
  //Kick effects
  Vector2D kickEffect;
  //Middle of bounding box
  Vector2D midBounds;
};

#endif /* MOTIONINFO_H_ */
