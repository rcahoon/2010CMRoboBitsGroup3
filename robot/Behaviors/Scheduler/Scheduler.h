#ifndef ROBOCUP2010_SCHEDULER_H_
#define ROBOCUP2010_SCHEDULER_H_

#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/shared/FSM.h"
#include "shared/UsefulMacros.h"
//#include "Behaviors/Move2AndKick/Move2AndKick.h"
#include "Behaviors/KickToGoal/KickToGoal.h"
#include "Behaviors/Test/BehaviorsTest.h"
#include "Behaviors/GlobalLocalize/GlobalLocalize.h"

#define POSE_CONFIDENCE_THRESH  0.10

class ConfigFile;
class Log;

namespace RoboCup2010 {

class Scheduler : public BehaviorBase {
public:
	Scheduler(ConfigFile & configFile, Log & log);

	virtual ~Scheduler();

	virtual bool run(BEHAVIOR_PARAMS);

private:
	PREVENT_COPY_AND_ASSIGNMENT(Scheduler);
	
	GlobalLocalize gLoc;
	BehaviorsTest kick2goal;

	Log & log;
};

} // end namespace

#endif /* ROBOCUP2010_SCHEDULER_H_ */

