#include "NullComm.h"

NullComm::NullComm() {
}

NullComm::~NullComm() {
}

bool NullComm::run(Messages & messages, Feedback & feedback) {
  return false;
}
