#ifndef ROBOCUP2010_GLOBALLOCALIZE_H_
#define ROBOCUP2010_GLOBALLOCALIZE_H_

#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/shared/FSM.h"
#include "shared/UsefulMacros.h"

#define SCAN_INCREMENT  0.04f

class ConfigFile;
class Log;

namespace RoboCup2010 {

class GlobalLocalize : public BehaviorBase {
public:
	GlobalLocalize(ConfigFile & configFile, Log & log);

	virtual ~GlobalLocalize();

	virtual bool run(BEHAVIOR_PARAMS);

private:
	PREVENT_COPY_AND_ASSIGNMENT(GlobalLocalize);
	
	Log & log;
	
	float headAngle;
	float scan_dir;
};

} // end namespace

#endif /* ROBOCUP2010_GLOBALLOCALIZE_H_ */

