/*
 * BehaviorsTest.h
 *
 *  Created on: Sep 8, 2010
 *      Author: labrot1
 */

#ifndef BEHAVIORSTEST_H_
#define BEHAVIORSTEST_H_

#include "../Behaviors.h"
#include "Behaviors/shared/BehaviorBase.h"

class ConfigFile;
class Log;
class Field;

namespace RoboCup2010 {

class BehaviorsTest : public Behaviors {
public:
  BehaviorsTest(ConfigFile & configFile, Log & log, Field & field);

  virtual ~BehaviorsTest();

  virtual bool run(BEHAVIOR_PARAMS);

private:
  ConfigFile & configFile;
  Log & log;
  Field & field;
};

}


#endif /* BEHAVIORSTEST_H_ */
