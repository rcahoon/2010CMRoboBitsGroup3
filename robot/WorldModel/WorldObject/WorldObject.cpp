#include "WorldObject.h"

// Define the logging constants
#define COMPONENT WORLD_MODEL
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

WorldObject::WorldObject(Log & _log,
                         const Type _type)
  : log(_log),
    type(_type),
    source(WorldObject::UnknownSource),
    localPosition(0, 0),
    globalPosition(0, 0),
    covariance(Matrix::I<2>()*DEFAULT_VARIANCE),
    visible(false),
    valid(false),
    suspicious(false),
    newHypothesis(false) {
}

WorldObject::WorldObject(const WorldObject & other)
  : log(other.log),
    type(other.type),
    source(other.source),
    localPosition(other.localPosition),
    globalPosition(other.globalPosition),
    covariance(other.covariance),
    visible(other.visible),
    valid(other.valid),
    suspicious(other.suspicious),
    newHypothesis(other.newHypothesis) {
}

WorldObject::~WorldObject() {
}

WorldObject::Type WorldObject::getType() const {
  return type;
}

WorldObject::Source WorldObject::getSource() const {
  return source;
}

Vector2D const & WorldObject::getLocalPosition() const {
  return localPosition;
}

Vector2D const & WorldObject::getGlobalPosition() const {
  return globalPosition;
}

const Matrix& WorldObject::getCovariance() const {
  return covariance;
}

bool WorldObject::isVisible() const {
  return visible;
}

bool WorldObject::isValid() const {
  return valid;
}

bool WorldObject::isSuspicious() const {
  return suspicious;
}

bool WorldObject::isNewHypothesis() const {
  return newHypothesis;
}

void WorldObject::setLocalPosition(const Vector2D & _localPosition) {
  localPosition = _localPosition;
}

void WorldObject::setGlobalPosition(const Vector2D & _globalPosition) {
  globalPosition = _globalPosition;
}

void WorldObject::setCovariance(const Matrix& _covariance) {
  covariance = _covariance;
}

void WorldObject::setVisible(const bool _visible) {
  visible = _visible;

  if (visible) {
    valid      = true;
    suspicious = false;
  }
}

void WorldObject::setValid(const bool _valid) {
  valid = _valid;

  if (!valid) {
    visible    = false;
    suspicious = true;
  }
}

void WorldObject::setSuspicious(const bool _suspicious) {
  suspicious = _suspicious;

  if (suspicious) {
    visible = false;
    valid   = true;
  }
}

void WorldObject::setSource(WorldObject::Source _source) {
  source = _source;
}

void WorldObject::setNewHypothesis(bool _newHypothesis) {
  newHypothesis = _newHypothesis;
}

void WorldObject::clear() {
  source = WorldObject::UnknownSource;
  localPosition.x  = localPosition.y  = 0;
  globalPosition.x = globalPosition.y = 0;
  covariance = Matrix::I<2>()*DEFAULT_VARIANCE;
  visible = valid = suspicious = newHypothesis = false;
}

void WorldObject::copyFrom(const WorldObject & other) {
  localPosition  = other.localPosition;
  globalPosition = other.globalPosition;
  covariance     = other.covariance;
  visible        = other.visible;
  valid          = other.valid;
  suspicious     = other.suspicious;
  source         = other.source;
  newHypothesis  = other.newHypothesis;
}

WorldObject & WorldObject::operator=(const WorldObject & other) {
  if (this != &other) {
    copyFrom(other);
  }

  return (*this);
}

