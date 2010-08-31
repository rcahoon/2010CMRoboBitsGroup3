#include "Walk.h"

Walk::Walk(float _vX, float _vY, float _vTheta)
  : vX(_vX),
    vY(_vY),
    vTheta(_vTheta) {
}

Walk::~Walk() {
}

/*
BodyCommand::Type Walk::getType() const {
  return BodyCommand::Walk;
}
*/

float Walk::getVelocityX() const {
  return vX;
}

float Walk::getVelocityY() const {
  return vY;
}

float Walk::getVelocityTheta() const {
  return vTheta;
}

void Walk::setVelocity(float _vX, float _vY, float _vTheta) {
  vX = _vX;
  vY = _vY;
  vTheta = _vTheta;
}
