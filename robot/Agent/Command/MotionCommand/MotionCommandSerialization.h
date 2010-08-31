#ifndef MOTIONCOMMANDSERIALIZATION_H_
#define MOTIONCOMMANDSERIALIZATION_H_

#include "MotionCommandList.h"

// Register the classes so that serializing derived classes will work properly
BOOST_CLASS_EXPORT_GUID(HeadAngles, "HeadAngles");
BOOST_CLASS_EXPORT_GUID(NoHeadCommand, "NoHeadCommand");

BOOST_CLASS_EXPORT_GUID(StandNeutral, "StandNeutral");
BOOST_CLASS_EXPORT_GUID(NoBodyCommand, "NoBodyCommand");
BOOST_CLASS_EXPORT_GUID(Walk, "Walk");

#endif /* MOTIONCOMMANDSERIALIZATION_H_ */
