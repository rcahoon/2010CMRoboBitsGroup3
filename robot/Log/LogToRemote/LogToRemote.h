#ifndef LOGTOREMOTE_H_
#define LOGTOREMOTE_H_

#include <vector>

#include "Log/Log.h"

class RemoteMessageFromRobot;

/**
 * This class logs to a remote client (on a PC), via RemoteMessageFromRobot.
 */
class LogToRemote: public Log {
public:
  LogToRemote(ConfigFile & configFile);
  virtual ~LogToRemote();

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
  const int & LOG_ROBOTSTATE_PERIOD;
  const int & LOG_ORIGINAL_IMAGE_PERIOD;
  const int & LOG_SEGMENTED_IMAGE_PERIOD;

  int logRobotStateCounter, logOriginalImageCounter, logSegmentedImageCounter;

  std::vector<RemoteMessageFromRobot const *> remoteMessagesFromRobot;
};

#endif /* LOGTOREMOTE_H_ */
