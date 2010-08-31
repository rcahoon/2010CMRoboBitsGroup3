#ifndef LOGTOTEXT_H_
#define LOGTOTEXT_H_

#include <string>

#include "Log/Log.h"

/**
 * This class logs directly to the console (as text).
 */
class LogToText: public Log {
public:
  LogToText(ConfigFile & configFile);
  virtual ~LogToText();

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
  enum TextColor {
    Default = 0,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    BoldBlack,
    BoldRed,
    BoldGreen,
    BoldYellow,
    BoldBlue,
    BoldMagenta,
    BoldCyan,
    BoldWhite,
    numColors // this should always be last!
  };

  const int TRACE_COLOR, DEBUG_COLOR, INFO_COLOR, WARN_COLOR, ERROR_COLOR;

  std::string colors[numColors];
  char const * logLevelColors[LOG_LEVEL_NONE + 1];
};

#endif /* LOGTOTEXT_H_ */
