#include "LogConfigurable.h"
#include "Log/LogToRemote/LogToRemote.h"
#include "Log/LogToText/LogToText.h"
#include "Log/LogToFile/LogToFile.h"
#include "shared/ConfigFile/ConfigFile.h"

LogConfigurable::LogConfigurable(ConfigFile & configFile)
  : Log(configFile),
    log() {
  // Should we use LogToRemote?
  if (configFile.getBool("configurable/log/useLogToRemote", true)) {
    log.push_back(new LogToRemote(configFile));
  }
  // Should we use LogToText?
  if (configFile.getBool("configurable/log/useLogToText", true)) {
    log.push_back(new LogToText(configFile));
  }
  // Should we use LogToFile?
  if (configFile.getBool("configurable/log/useLogToFile", true)) {
    log.push_back(new LogToFile(configFile));
  }
}

LogConfigurable::~LogConfigurable() {
  // Delete any subclasses we have
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    delete (*iter);
  }
  log.clear();
}

bool LogConfigurable::run(Messages & messages) {
  // Run all the subclasses
  bool result = false;
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    Log *subclass = (Log *)(*iter);
    if (subclass->run(messages)) {
      result = true;
    }
  }

  return result;
}

void LogConfigurable::logBegin(unsigned long timestamp) {
  // Run all the subclasses
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    Log *subclass = (Log *)(*iter);
    subclass->logBegin(timestamp);
  }
}

void LogConfigurable::logEnd() {
  // Run all the subclasses
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    Log *subclass = (Log *)(*iter);
    subclass->logEnd();
  }
}

void LogConfigurable::logTextFormatted(int logLevel,
                                       int component,
                                       char const * className,
                                       char const * functionName,
                                       char const * text,
                                       int textLength) {
  // Run all the subclasses
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    Log *subclass = (Log *)(*iter);
    subclass->logTextFormatted(logLevel, component, className, functionName, text, textLength);
  }
}

void LogConfigurable::logRobotState(const RobotState & robotState) {
  // Run all the subclasses
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    Log *subclass = (Log *)(*iter);
    subclass->logRobotState(robotState);
  }
}

void LogConfigurable::logSegmentedImage(const SegmentedImage & segmentedImage) {
  // Run all the subclasses
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    Log *subclass = (Log *)(*iter);
    subclass->logSegmentedImage(segmentedImage);
  }
}

void LogConfigurable::logShape(Screen screen, const Shape & shape) {
  // Run all the subclasses
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    Log *subclass = (Log *)(*iter);
    subclass->logShape(screen, shape);
  }
}

void LogConfigurable::logTeamColor(bool isBlue) {
  // Run all the subclasses
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    Log *subclass = (Log *)(*iter);
    subclass->logTeamColor(isBlue);
  }
}

void LogConfigurable::logGameState(const GameState & gameState) {
  // Run all the subclasses
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    Log *subclass = (Log *)(*iter);
    subclass->logGameState(gameState);
  }
}

void LogConfigurable::setConnectedToClient(bool connectedToClient) {
  Log::setConnectedToClient(connectedToClient);

  // Inform all the subclasses
  std::vector<Log *>::iterator iter;
  for (iter = log.begin(); iter != log.end(); iter++) {
    Log *subclass = (Log *)(*iter);
    subclass->setConnectedToClient(connectedToClient);
  }
}
