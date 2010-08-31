#include "CommConfigurable.h"
#include "Comm/Player/CommPlayer.h"
#include "Comm/Remote/CommRemote.h"
#include "shared/ConfigFile/ConfigFile.h"

CommConfigurable::CommConfigurable(ConfigFile & configFile, Log & log)
  : comm() {
  // Should we use CommPlayer?
  if (configFile.getBool("configurable/comm/useCommPlayer", true)) {
    comm.push_back(new CommPlayer(configFile, log));
  }
  // Should we use CommRemote?
  if (configFile.getBool("configurable/comm/useCommRemote", true)) {
    comm.push_back(new CommRemote(configFile, log));
  }
}

CommConfigurable::~CommConfigurable() {
  // Delete any subclasses we have
  std::vector<Comm *>::iterator iter;
  for (iter = comm.begin(); iter != comm.end(); iter++) {
    delete (*iter);
  }
  comm.clear();
}

bool CommConfigurable::run(Messages & messages, Feedback & feedback) {
  // Run all the subclasses
  bool result = false;
  std::vector<Comm *>::iterator iter;
  for (iter = comm.begin(); iter != comm.end(); iter++) {
    Comm *subclass = (Comm *)(*iter);
    if (subclass->run(messages, feedback)) {
      result = true;
    }
  }

  return result;
}
