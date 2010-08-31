#ifndef LOGCONFIGURABLE_H_
#define LOGCONFIGURABLE_H_

#include <vector>
#include "Log/Log.h"

class ConfigFile;

class LogConfigurable: public Log {
public:
  LogConfigurable(ConfigFile & configFile);

  virtual ~LogConfigurable();

  virtual bool run(Messages & messages);

  virtual void logBegin(unsigned long timstamp);

  virtual void logEnd();

  virtual void logTextFormatted(int logLevel,
                                int component,
                                char const * className,
                                char const * functionName,
                                char const * text,
                                int textLength);

  virtual void logRobotState(const RobotState & robotState);

  virtual void logSegmentedImage(const SegmentedImage & segmentedImage);

  virtual void logShape(Screen screen, const Shape & shape);

  virtual void logTeamColor(bool isBlue);

  virtual void logGameState(const GameState & gameState);

  virtual void setConnectedToClient(bool connectedToClient);

private:
  std::vector<Log *> log;
};

#endif /* LOGCONFIGURABLE_H_ */
