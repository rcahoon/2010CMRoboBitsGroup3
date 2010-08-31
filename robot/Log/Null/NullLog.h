#ifndef NULLLOG_H_
#define NULLLOG_H_

#include "Log/Log.h"

class ConfigFile;

class NullLog : public Log {
public:
  NullLog(ConfigFile & configFile);

  virtual ~NullLog();

  virtual bool run(Messages & messages);

  virtual void logBegin(unsigned long timestamp);

  virtual void logEnd();

  virtual void logRobotState(const RobotState & robotState);

  virtual void logTextFormatted(int logLevel,
                                int component,
                                char const * className,
                                char const * functionName,
                                char const * text,
                                int textLength);

  virtual void logSegmentedImage(const SegmentedImage & segmentedImage);

  virtual void logShape(Screen screen, const Shape & shape);

  virtual void logTeamColor(bool isBlue);

  virtual void logGameState(const GameState & gameState);
};

#endif /* NULLLOG_H_ */
