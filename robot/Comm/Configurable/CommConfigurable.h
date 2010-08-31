#ifndef COMMCONFIGURABLE_H_
#define COMMCONFIGURABLE_H_

#include <vector>
#include "Comm/Comm.h"

class ConfigFile;
class Log;

/**
 * This class reads the config file and decides which Comm subclasses to use
 */
class CommConfigurable : public Comm {
public:
  CommConfigurable(ConfigFile & configFile, Log & log);
  virtual ~CommConfigurable();

  virtual bool run(Messages & messages, Feedback & feedback);

private:
  std::vector<Comm *> comm;
};

#endif /* COMMCONFIGURABLE_H_ */
