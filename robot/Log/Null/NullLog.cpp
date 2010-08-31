#include "NullLog.h"

NullLog::NullLog(ConfigFile & configFile)
  : Log(configFile) {
}

NullLog::~NullLog() {
}

bool NullLog::run(Messages & messages) {
  return false;
}

void NullLog::logBegin(unsigned long timestamp) {
}

void NullLog::logEnd() {
}

void NullLog::logRobotState(const RobotState & robotState) {
}

void NullLog::logTextFormatted(int logLevel,
                               int component,
                               char const * className,
                               char const * functionName,
                               char const * text,
                               int textLength) {
}

void NullLog::logSegmentedImage(const SegmentedImage & segmentedImage) {
}

void NullLog::logShape(Screen screen, const Shape & shape) {
}

void NullLog::logTeamColor(bool isBlue) {
}

void NullLog::logGameState(const GameState & gameState) {
}
