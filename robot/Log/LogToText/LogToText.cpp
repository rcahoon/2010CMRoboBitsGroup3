#include <stdio.h>

#include "LogToText.h"
#include "Comm/Messages/Messages.h"
#include "shared/ConfigFile/ConfigFile.h"

LogToText::LogToText(ConfigFile & configFile)
  : Log(configFile),
    TRACE_COLOR(configFile.getInt("logToText/traceColor", (int)LogToText::Cyan)),
    DEBUG_COLOR(configFile.getInt("logToText/debugColor", (int)LogToText::Green)),
    INFO_COLOR(configFile.getInt ("logToText/infoColor", (int)LogToText::Blue)),
    WARN_COLOR(configFile.getInt ("logToText/warnColor", (int)LogToText::Red)),
    ERROR_COLOR(configFile.getInt("logToText/errorColor", (int)LogToText::BoldRed)) {
  // Initialize the colors
  colors[0]  = "\033[0m";
  colors[1]  = "\033[30m";
  colors[2]  = "\033[31m";
  colors[3]  = "\033[32m";
  colors[4]  = "\033[33m";
  colors[5]  = "\033[34m";
  colors[6]  = "\033[35m";
  colors[7]  = "\033[36m";
  colors[8]  = "\033[37m";
  colors[9]  = "\033[30;1m";
  colors[10] = "\033[31;1m";
  colors[11] = "\033[32;1m";
  colors[12] = "\033[33;1m";
  colors[13] = "\033[34;1m";
  colors[14] = "\033[35;1m";
  colors[15] = "\033[36;1m";
  colors[16] = "\033[37;1m";

  logLevelColors[LOG_LEVEL_TRACE] = colors[TRACE_COLOR].c_str();
  logLevelColors[LOG_LEVEL_DEBUG] = colors[DEBUG_COLOR].c_str();
  logLevelColors[LOG_LEVEL_INFO]  = colors[INFO_COLOR].c_str();
  logLevelColors[LOG_LEVEL_WARN]  = colors[WARN_COLOR].c_str();
  logLevelColors[LOG_LEVEL_ERROR] = colors[ERROR_COLOR].c_str();
  logLevelColors[LOG_LEVEL_NONE]  = colors[LogToText::Default].c_str();
}

LogToText::~LogToText() {
}

bool LogToText::run(Messages & messages) {
  return false;
}

void LogToText::logBegin(unsigned long timestamp) {
}

void LogToText::logEnd() {
}

void LogToText::logTextFormatted(int logLevel,
                                 int component,
                                 char const * className,
                                 char const * functionName,
                                 char const * text,
                                 int textLength) {
  // Warning: if log level is invalid, we can access the logLevelColors array
  // out of bounds

  // Print it to the screen
  printf("%s%s::%s - %s%s\n",
         logLevelColors[logLevel],
         className, functionName, text,
         logLevelColors[LOG_LEVEL_NONE]);
}

void LogToText::logRobotState(const RobotState & robotState) {
}

void LogToText::logSegmentedImage(const SegmentedImage & segmentedImage) {
}

void LogToText::logShape(Screen screen, const Shape & shape) {
}

void LogToText::logTeamColor(bool isBlue) {
}

void LogToText::logGameState(const GameState & gameState) {
}
