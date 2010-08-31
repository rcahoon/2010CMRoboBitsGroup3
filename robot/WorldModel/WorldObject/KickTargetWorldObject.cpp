#include "KickTargetWorldObject.h"

// Define the logging constants
#define COMPONENT WORLD_MODEL
#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"
#include <iostream>

KickTargetWorldObject::KickTargetWorldObject(Log & _log,
                                         WorldObject::Type _type)
  : WorldObject(_log, _type){
      clear();
}

KickTargetWorldObject::KickTargetWorldObject(const KickTargetWorldObject & other)
  : WorldObject(other) {
      
}

KickTargetWorldObject::~KickTargetWorldObject() {
}


void KickTargetWorldObject::clear() {
  WorldObject::clear();

}

void KickTargetWorldObject::copyFrom(const WorldObject & other) {
  
    WorldObject::copyFrom(other);
}

