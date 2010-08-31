#include "FreeSpaceVisionObject.h"

// Define the logging constants
#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"


FreeSpaceVisionObject::FreeSpaceVisionObject(Log & _log)  : VisionObject(_log,
                                                            VisionObject::FreeSpace) {    
}

FreeSpaceVisionObject::FreeSpaceVisionObject(const FreeSpaceVisionObject & other)
  : VisionObject(other){
}

FreeSpaceVisionObject::~FreeSpaceVisionObject() {
}

void FreeSpaceVisionObject::clear() {
  VisionObject::clear();

}

void FreeSpaceVisionObject::copyFrom(const VisionObject & other) {
  VisionObject::copyFrom(other);

}


void FreeSpaceVisionObject::setStartRelative(Vector2D _startRelative)
{
    startRelative = _startRelative;
}
void FreeSpaceVisionObject::setEndRelative(Vector2D _endRelative)
{
    endRelative = _endRelative;
}

Vector2D FreeSpaceVisionObject::getStartRelative()
{
    return startRelative;
}

Vector2D FreeSpaceVisionObject::getEndRelative()
{
    return endRelative;
}
