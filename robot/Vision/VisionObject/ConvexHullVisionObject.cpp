#include "ConvexHullVisionObject.h"

// Define the logging constants
#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"


ConvexHullVisionObject::ConvexHullVisionObject(Log & _log)  : VisionObject(_log,
                                                            VisionObject::ConvexHull) {    
}

ConvexHullVisionObject::ConvexHullVisionObject(const ConvexHullVisionObject & other)
  : VisionObject(other){
}

ConvexHullVisionObject::~ConvexHullVisionObject() {
}

void ConvexHullVisionObject::clear() {
  VisionObject::clear();
  points.clear();
}

void ConvexHullVisionObject::copyFrom(const VisionObject & other) {
  VisionObject::copyFrom(other);

}

unsigned int ConvexHullVisionObject::getSize()
{
    return points.size();
}


std::vector<Vector2D> ConvexHullVisionObject::getPoints()
{
    return points;
}

void ConvexHullVisionObject::set(std::vector<Vector2D> source)
{
    points.assign(source.begin(), source.end());
}
