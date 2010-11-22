#include "Pose.h"
#include "shared/num_util.h"
#include <assert.h>

// Define the logging constants
#define COMPONENT LOCALIZATION
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "../Log/LogSettings.h"

Pose::Pose(const Vector2D & _position,
           const float _angle,
           const Matrix& _covariance,
           const bool _lost)
         : position(_position),
           angle(_angle),
           covariance(_covariance),
           lost(_lost) {
  
  assert(_covariance.rows()==3 && _covariance.cols()==3);
}

Pose::Pose(const Pose & other)
         : position(other.position),
           angle(other.angle),
           covariance(other.covariance),
           lost(other.lost) {
}

Pose::~Pose() {
}

const Vector2D Pose::getPosition() const{
  return position;
}

void Pose::setPosition(const Vector2D & _position){
  position = _position;
}

void Pose::setPosition(const float x, const float y) {
  position.x = x;
  position.y = y;
}

float Pose:: getAngle() const{
  return angle;
}

void Pose::setAngle(const float _angle){
  angle = _angle;
}

const Matrix& Pose::getCovariance() const{
  return covariance;
}

void Pose::setCovariance(const Matrix& _covariance){
  covariance = _covariance;
}

bool Pose::isLost() const{
  return lost;
}

void Pose::setLost(const bool _lost){
  lost = _lost;
}

Vector2D Pose::convertRelativeToGlobal(const Vector2D & relativeCoords) const {
  float s = sin(angle);
  float c = cos(angle);
  float x = relativeCoords.x;
  float y = relativeCoords.y;

  Vector2D globalCoords = position;
  globalCoords.x += c * x - s * y;
  globalCoords.y += s * x + c * y;

  return globalCoords;
}

Vector2D Pose::convertGlobalToRelative(const Vector2D & globalCoords) const {
  float s = sin(-angle);
  float c = cos(-angle);
  float x = globalCoords.x - position.x;
  float y = globalCoords.y - position.y;

  return Vector2D(c * x - s * y, s * x + c * y);
}

float Pose::convertRelativeAngleToGlobal(float relativeAngle) const {
  return norm_angle(angle + relativeAngle);
}

float Pose::convertGlobalAngleToRelative(float globalAngle) const {
  return norm_angle(globalAngle - angle);
}

Pose & Pose::operator=(const Pose & other) {
  position = other.position;
  angle = other.angle;
  covariance = other.covariance;
  lost = other.lost;

  return *this;
}

