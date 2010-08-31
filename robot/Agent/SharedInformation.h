#ifndef SHAREDINFORMATION_H_
#define SHAREDINFORMATION_H_

#include "RobotState.h"
#include "Command/Command.h"
#include "Localization/Pose.h"

#define SHAREDMEMORYNAME "/cmurfshm"
#define SEMAPHORENAME "/cmurfsem"

class SharedInformation 
{
public:
    SharedInformation():lastUpdateTime(0)
    { 
    }
    virtual ~SharedInformation()
    {
    }

    RobotState state;
    Command command;
   
    Pose currPosition;
    Pose lastPosition;
    unsigned long lastUpdateTime;
};

#endif /* SHAREDINFORMATION_H_ */
