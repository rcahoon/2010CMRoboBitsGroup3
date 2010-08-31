#include "BehaviorBase.h"

// Define the logging constants
#define COMPONENT BEHAVIORS
//#define CLASS_LOG_LEVEL LOG_LEVEL_NONE
#include "Log/LogSettings.h"

BehaviorBase::BehaviorBase()
  : status(0) {
}

BehaviorBase::~BehaviorBase() {
}

int BehaviorBase::getStatus() {
  return status;
}
