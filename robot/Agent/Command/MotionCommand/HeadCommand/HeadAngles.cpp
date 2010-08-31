#include "HeadAngles.h"

HeadAngles::HeadAngles(float _pan, float _tilt, float _speed)
  : pan(_pan),
    tilt(_tilt),
    speed(_speed) {
}

HeadAngles::~HeadAngles() {
}

/*
HeadCommand::Type HeadAngles::getType() const {
  return HeadCommand::HeadAngles;
}
*/

float HeadAngles::getPan() const {
  return pan;
}

float HeadAngles::getTilt() const {
  return tilt;
}

float HeadAngles::getSpeed() const {
  return speed;
}

void HeadAngles::setPan(float _pan) {
  pan = _pan;
}

void HeadAngles::setTilt(float _tilt) {
  tilt = _tilt;
}

void HeadAngles::setSpeed(float _speed) {
  if (_speed < 0) {
    _speed = 0;
  }
  else if (_speed > 1) {
    _speed = 1;
  }
  speed = _speed;
}

void HeadAngles::setAngles(float _pan, float _tilt) {
  pan  = _pan;
  tilt = _tilt;
}
