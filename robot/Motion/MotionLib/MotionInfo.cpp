#include "MotionInfo.h"
#include "shared/ConfigFile/ConfigFile.h"
#include "shared/num_util.h"

// Define the logging constants
#define COMPONENT OTHERS
//#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"

MotionInfo::MotionInfo(ConfigFile & configFile,
                       Log & _log,
                       StaticAction::StaticActionType _type,
                       float _minX, float _maxX, float _minY, float _maxY,
                       float _odometryX, float _odometryY, float _odometryTheta,
                       float _resultR, float _resultTheta, float _deviationResultR, float _deviationResultTheta,
                       float _timeToExecute, int _motionType)
  : ANGLE_THRESH(configFile.getRadiansFromDegrees("motion/angleThresh", 25)),
    log(_log),
    type(_type),
    minX(_minX),
    maxX(_maxX),
    minY(_minY),
    maxY(_maxY),
    odometryX(_odometryX),
    odometryY(_odometryY),
    odometryTheta(_odometryTheta),
    resultR(_resultR),
    resultTheta(_resultTheta),
    deviationResultR(_deviationResultR),
    deviationResultTheta(_deviationResultTheta),
    timeToExecute(_timeToExecute),
    motionType(_motionType),
    angleWeight(configFile.getFloat("motion/angleWeight", 1.2)),
    distWeight(configFile.getFloat("motion/distWeight", 0.5)),
    ballDistWeight(configFile.getFloat("motion/ballDistWeight", 1.0)){
  if (minX > maxX) {
    float temp = minX;
    minX = maxX;
    maxX = temp;
  }
  if (minY > maxY) {
    float temp = minY;
    minY = maxY;
    maxY = temp;
  }
  midBounds.x = minX + ((maxX - minX) / 4);
  midBounds.y = (maxY + minY) / 2;
//  midBounds.x = minX;
//  midBounds.y = minY;
  kickEffect.heading(resultTheta);
  kickEffect *= resultR;
}


MotionInfo::~MotionInfo() {
  // TODO Auto-generated destructor stub
}

void MotionInfo::setType(StaticAction::StaticActionType _type) {
  type = _type;
}

StaticAction::StaticActionType MotionInfo::getType() const {
  return type;
}

void MotionInfo::setBoundingBox(float _minX, float _maxX, float _minY, float _maxY){
  minX = _minX;
  maxX = _maxX;
  minY = _minY;
  maxY = _maxY;
}

void MotionInfo::getBoundingBox(float & _minX, float & _maxX, float & _minY, float & _maxY) const {
  _minX = minX;
  _maxX = maxX;
  _minY = minY;
  _maxY = maxY;
}

const Vector2D & MotionInfo::getMidBounds() const {
  return midBounds;
}

void MotionInfo::setOdometryChanges(float _odometryX, float _odometryY, float _odometryTheta) {
  odometryX = _odometryX;
  odometryY = _odometryY;
  odometryTheta = _odometryTheta;
}

void MotionInfo::getOdometryChanges(Vector2D & odometryTranslation, float & _odometryTheta) const {
  odometryTranslation.x = odometryX;
  odometryTranslation.y = odometryY;
  _odometryTheta = odometryTheta;
}

void MotionInfo::setMotionEffects(float _resultR, float _resultTheta, float _deviationResultR, float _deviationResultTheta) {
  resultR = _resultR;
  resultTheta = _resultTheta;
  deviationResultR = _deviationResultR;
  deviationResultTheta = _deviationResultTheta;
}

void MotionInfo::getMotionEffects(float & _resultR, float & _resultTheta, float & _deviationResultR, float & _deviationResultTheta) const {
  _resultR = resultR;
  _resultTheta = resultTheta;
  _deviationResultR = deviationResultR;
  _deviationResultTheta = deviationResultTheta;
}

void MotionInfo::setTimeToExecute(float _timeToExecute) {
  timeToExecute = _timeToExecute;
}

void MotionInfo::getTimeToExecute(float & _timeToExecute) const {
  _timeToExecute = timeToExecute;
}

void MotionInfo::setMotionType(int _motionType) {
  motionType = _motionType;
}

void MotionInfo::getMotionType(int & _motionType) const {
  _motionType = motionType;
}

bool MotionInfo::isAForwardKick(bool & isLeftKick) const{
  if (motionType == 1) {
    isLeftKick = true;
    return true;
  } else if (motionType == -1){
    isLeftKick = false;
    return true;
  }// else all are not forward kicks
  isLeftKick = false;
  return false;
}

bool MotionInfo::isAngleGoodForKick(const Vector2D & relTarget) const {
  float angleDiff = fabs(norm_angle(resultTheta - relTarget.angle()));
  return (angleDiff <= ANGLE_THRESH);
}

bool MotionInfo::isAngleGoodForKick(float minAngleValue, float maxAngleValue) const {
//  if (minAngleValue > maxAngleValue) {
//    float temp = minAngleValue;
//    minAngleValue = maxAngleValue;
//    maxAngleValue = temp;
//  }
  if (minAngleValue <= maxAngleValue) {
    return (resultTheta <= maxAngleValue) && (resultTheta >= minAngleValue);
  }
  else {
    // We can't kick backwards
    return false;
  }
}

bool MotionInfo::isBallInKickRange(const Vector2D & ballPosition) const {
  if ( (ballPosition.x >= minX) &&
       (ballPosition.x <= maxX) &&
       (ballPosition.y >= minY) &&
       (ballPosition.y <= maxY) ){
    return true;
  }
  else {
    return false;
  }
}


/*
 * Returns Vector2D stating the relative coordinates of the estimated ball position after the kick
 */
Vector2D MotionInfo::getKickEffect(const Vector2D & ballPosition) const {
  return (ballPosition + kickEffect);
}

/*
 * Returns score of the kick
 * relTarget target's relative coordinates
 */
float MotionInfo::getKickScore(const Vector2D & relTarget, const Vector2D & ballPosition, bool through, bool useAngle, float minAngle, float maxAngle) const {
  float angleDiff(0.0f), distDiff(0.0f), ballDistDiff(0.0f);

  if (useAngle) {
    // If the angle matches our bounds, we shouldn't take any penalty
    if (isAngleGoodForKick(minAngle, maxAngle)) {
      angleDiff = 0;
    }
    // If the angle doesn't match our bounds, we should take penalty for how far
    // we are from the bounds
    else {
      angleDiff = min(fabs(norm_angle(resultTheta - minAngle)), fabs(norm_angle(resultTheta - maxAngle)));
    }
  }
  // We take a penalty based on how far we are from the target
  else {
    angleDiff = fabs(norm_angle(resultTheta - relTarget.angle()));
  }

  // If we can't kick the ball currently, we take a penalty for how much we have
  // to walk
  if (!isBallInKickRange(ballPosition)) {
    LOG_DEBUG("Kick %d, (%.1f, %.1f)-(%.1f, %.1f)", getType(), minX, maxX, minY, maxY);
    ballDistDiff = (ballPosition - midBounds).length();
  }

  // If we want to kick through the target, we take a penalty if we kick shorter
  // than it
  if (through) {
    distDiff = max(relTarget.length() - resultR, 0.0f);
  }
  // Otherwise, we take a penalty based on how far we are from the target, both
  // shorter and longer distances
  else {
    distDiff = fabs(relTarget.length() - resultR);
  }

  LOG_DEBUG("Kick %d: distDiff = %.1f, angleDiff = %.1f, ballDistDiff = %.1f",
            getType(), distDiff, angleDiff, ballDistDiff);

  return (-distDiff * distWeight - angleDiff * angleWeight - ballDistDiff * ballDistWeight);

}

float MotionInfo::getResultTheta() const {
  return resultTheta;
}
