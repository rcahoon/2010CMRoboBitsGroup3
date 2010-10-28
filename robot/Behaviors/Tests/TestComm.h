#ifndef ROBOCUPTESTS_TESTCOMM_H_
#define ROBOCUPTESTS_TESTCOMM_H_

#include "Behaviors/shared/BehaviorBase.h"
#include "Behaviors/shared/FSM.h"
#include "shared/UsefulMacros.h"

class ConfigFile;
class Log;

namespace RoboCupTests {

class TestComm : public BehaviorBase {
public:
  TestComm(ConfigFile & configFile, Log & log);

  virtual ~TestComm();

  virtual bool run(BEHAVIOR_PARAMS);

  void reset();

private:
  PREVENT_COPY_AND_ASSIGNMENT(TestComm);

  void init(unsigned long timestamp);

  ConfigFile & configFile;
  Log & log;
};

} // end namespace

#endif /* ROBOCUPTESTS_TESTCOMM_H_ */

