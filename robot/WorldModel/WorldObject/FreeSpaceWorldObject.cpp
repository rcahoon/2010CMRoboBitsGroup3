#include "FreeSpaceWorldObject.h"

// Define the logging constants
#define COMPONENT WORLD_MODEL
#define CLASS_LOG_LEVEL LOG_LEVEL_DEBUG
#include "Log/LogSettings.h"
#include <iostream>

FreeSpaceWorldObject::FreeSpaceWorldObject(Log & _log,
                                         WorldObject::Type _type)
  : WorldObject(_log, _type){
      clear();
}

FreeSpaceWorldObject::FreeSpaceWorldObject(const FreeSpaceWorldObject & other)
  : WorldObject(other) {
      
    for ( int i = 0; i < NUM_FREE_SPACE_SLOTS; i++ )
    {
        distances[i] = other.distances[i];
    }
}

FreeSpaceWorldObject::~FreeSpaceWorldObject() {
}


void FreeSpaceWorldObject::setDistance(int slot, float distance)
{
    if ( slot >= 0 && slot < NUM_FREE_SPACE_SLOTS )
    {
        distances[slot] = distance;
    }
}

float FreeSpaceWorldObject::getDistance(int slot) const
{
    
    if ( slot >= 0 && slot < NUM_FREE_SPACE_SLOTS )
    {
        return distances[slot];
    }
    else
    {
        return 0;
    }
}

float FreeSpaceWorldObject::getAngleDistance(float angle) const
{
    int slot = 0;
    float currAngle = -M_PI/2;
    while (currAngle < angle )
    { 
        currAngle += SLOT_ANGLE;
        slot++;
    }
    if ( slot > 0 )
    {
        slot--;
        currAngle -= SLOT_ANGLE;
    }
//    std::cout << " angle: " << angle << " slot id: " << slot << std::endl;
   
    if ( slot < NUM_FREE_SPACE_SLOTS && slot >= 0 )
    {
        return distances[slot];
    }
    else
    {
        return -1;
    }
}


void FreeSpaceWorldObject::clear() {
  WorldObject::clear();
    for ( int i = 0; i < NUM_FREE_SPACE_SLOTS; i++ )
    {
        distances[i] = MAX_FREE_SPACE_DISTANCE;
    }

}

void FreeSpaceWorldObject::copyFrom(const WorldObject & other) {
  
    WorldObject::copyFrom(other);

    // Try to cast the other object as a world object with velocity
    FreeSpaceWorldObject const *otherFreeSpace = dynamic_cast<FreeSpaceWorldObject const *>(&other);
    if (otherFreeSpace != NULL) 
    {
        for (int i = 0; i < NUM_FREE_SPACE_SLOTS ; i++ )
        { 
            distances[i] = otherFreeSpace->distances[i];
        }
    }
    // Default values, but give a warning
    else 
    {
        for ( int i = 0; i < NUM_FREE_SPACE_SLOTS; i++ )
        {
            distances[i] = 0;
        }

        LOG_WARN("Couldn't cast world object as a world object with distance array.");
    }
}

int FreeSpaceWorldObject::getAngleSlot(float angle) const
{
    int slot = 0;
    float currAngle = -M_PI/2;
    while (currAngle < angle )
    { 
        currAngle += SLOT_ANGLE;
        slot++;
    }
    if ( slot > 0 )
    {
        slot--;
        currAngle -= SLOT_ANGLE;
    }

    return slot;
}

float FreeSpaceWorldObject::getSlotAngle(int slot) const
{
    if ( slot < 0 || slot >= NUM_FREE_SPACE_SLOTS ) return 0;

    return (( slot * SLOT_ANGLE + (slot + 1 ) * SLOT_ANGLE ) / 2) - M_PI/2;
}


int FreeSpaceWorldObject::getMaxDistanceSlot(float startAngle, float endAngle ) const
{
    if ( (startAngle < -M_PI / 2) || (startAngle > M_PI / 2) || (endAngle < -M_PI / 2) || (endAngle > M_PI / 2) ) return -1;
    
    float maxDist = 0;
    int maxSlot = -1;

    int startSlot = getAngleSlot(startAngle);
    int endSlot = getAngleSlot(endAngle);

    for ( int i = startSlot; i <= endSlot; i++ )
    {
        if ( distances[i] > maxDist )
        {
            maxDist = distances[i];
            maxSlot = i;
        }

    }

    return maxSlot;
}

Vector2D FreeSpaceWorldObject::getMaxFreeSpacePoint(float startAngle, float endAngle) const
{
    Vector2D ret(-1,-1);
    if ( (startAngle < -M_PI / 2) || (startAngle > M_PI / 2) || (endAngle < -M_PI / 2) || (endAngle > M_PI / 2) ) return ret;

    int slot = getMaxDistanceSlot(startAngle, endAngle);
    ret.heading( getSlotAngle(slot) );

    ret *= distances[slot];

    return ret;
}

bool FreeSpaceWorldObject::isOccupied(float startAngle, float endAngle, float distanceThreshold) const
{
    if ( (startAngle < -M_PI / 2) || (startAngle > M_PI / 2) || (endAngle < -M_PI / 2) || (endAngle > M_PI / 2) ) return false;

#ifdef LOG_DEBUG_ACTIVE
    for ( float i = startAngle; i < endAngle; i += fabs(endAngle - startAngle)/10 )
    {
        LOG_DEBUG("dist diff for %.1f deg is %.1f", i / M_PI * 180.0f, getAngleDistance(i));
    }
#endif

    for ( float i = startAngle; i < endAngle; i += fabs(endAngle - startAngle)/10 )
    {
        //std::cout << "dist diff for " << i << " is " << fabs( goalDist - freeSpace->getAngleDistance(i) ) << std::endl;
        if ( getAngleDistance(i) < distanceThreshold )
        {
            return true;
        }

    }
    return false;

}


