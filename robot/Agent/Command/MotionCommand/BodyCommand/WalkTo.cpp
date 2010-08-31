#include "WalkTo.h"

WalkTo::WalkTo(float _pX, float _pY, float _pTheta)
  : pX(_pX),
    pY(_pY),
    pTheta(_pTheta) {
}

WalkTo::~WalkTo() {
}

float WalkTo::getPositionX() const {
  return pX;
}

float WalkTo::getPositionY() const {
  return pY;
}

float WalkTo::getPositionTheta() const {
  return pTheta;
}

void WalkTo::setPosition(float _pX, float _pY, float _pTheta) {
  pX = _pX;
  pY = _pY;
  pTheta = _pTheta;
}
