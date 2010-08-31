#ifndef LOGTOFILE_H_
#define LOGTOFILE_H_

#include <string>

#include "Log/Log.h"

/**
 * This class logs data to file. This is meant to be run during competitions,
 * so that we have some logs to process and see what went wrong.
 */
class LogToFile : public Log {
public:

  LogToFile(ConfigFile & configFile);
  virtual ~LogToFile();

  virtual bool run(Messages & messages);

  virtual void logBegin(unsigned long timestamp);

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

private:
  const std::string FILENAME;

  const int MAX_BUFFER_SIZE;
  const unsigned long TIME_TO_SLEEP;

  bool startLogging;

  char *buffer;
  int offset;
};

#endif /* LOGTOFILE_H_ */
