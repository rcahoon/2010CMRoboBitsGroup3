#include "StandNeutral.h"

StandNeutral::StandNeutral()
  : whichLeg(StandNeutral::Any) {
}

StandNeutral::~StandNeutral() {
}

void StandNeutral::setWhichLeg(StandNeutral::WhichLeg _whichLeg) {
  whichLeg = _whichLeg;
}

StandNeutral::WhichLeg StandNeutral::getWhichLeg() const {
  return whichLeg;
}

/*
BodyCommand::Type StandNeutral::getType() const {
  return BodyCommand::StandNeutral;
}
*/
