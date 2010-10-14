#include "HeadStiffness.h"

HeadStiffness::HeadStiffness(float _stiffness)
  : stiffness(_stiffness) {
}

HeadStiffness::~HeadStiffness() {
}

float HeadStiffness::getStiffness() const {
  return stiffness;
}

void HeadStiffness::setStiffness(float _stiffness) {
  stiffness = _stiffness;
}
