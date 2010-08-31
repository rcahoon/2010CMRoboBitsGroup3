#ifndef MOTIONCOMMANDLIST_H_
#define MOTIONCOMMANDLIST_H_

// When adding a new Motion Command,
// remember to update MotionCommandSerialiation.h

#include "MotionCommand.h"

#include "HeadCommand/HeadAngles.h"
#include "HeadCommand/NoHeadCommand.h"

#include "BodyCommand/StandNeutral.h"
#include "BodyCommand/StopWalk.h"
#include "BodyCommand/NoBodyCommand.h"
#include "BodyCommand/Walk.h"
#include "BodyCommand/WalkTo.h"
#include "BodyCommand/StaticAction.h"

enum BodyCommand {
    bcNoBodyCommand = 0,  // - No params

    bcStandNeutral,       // - No params

    bcStopWalk,          // - No params

    bcWalk,               // - Walk with x, y, theta velocity
    bcWalkTo,             // - Walk to a specific relative position x,y,theta w.r.t. the current position of the robot
    bcStaticAction        // - Perform a static action (i.e., a kick)
};

enum HeadCommand {
    hcNoHeadCommand = 0, // - No params
    hcHeadAngles,        // - Directly set the head angles (pan, tilt) (in radians)
  };
#endif /* MOTIONCOMMANDLIST_H_ */
