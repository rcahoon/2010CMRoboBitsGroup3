#include <stdlib.h>
#include <string.h>
#include <fstream>

#include "LogToFile.h"
#include "Comm/Messages/Messages.h"
#include "GameController/GameState.h"
#include "shared/ConfigFile/ConfigFile.h"

LogToFile::LogToFile(ConfigFile & configFile)
  : Log(configFile),
    FILENAME(configFile.getPath("log/logToFile/filename", "logToFile.txt")),
    MAX_BUFFER_SIZE(configFile.getInt("log/logToFile/maxBufferSize", 128) * 1024 * 1024),
    TIME_TO_SLEEP(configFile.getMillisecondsFromSeconds("log/logToFile/timeToSleep", 2000) * 1000),
    startLogging(false),
    buffer(NULL),
    offset(0) {
  if (MAX_BUFFER_SIZE > 0) {
    buffer = (char *)malloc(MAX_BUFFER_SIZE);
  }
  if (buffer == NULL) {
    printf("Error creating buffer for LogToFile.\n");
    exit(0);
  }
}

LogToFile::~LogToFile() {
  if (buffer != NULL) {
    free(buffer);
  }
}

bool LogToFile::run(Messages & messages) {
  return false;
}

void LogToFile::logBegin(unsigned long timestamp) {
}

void LogToFile::logEnd() {
  // Should we start logging?
  if (!startLogging) {
    return;
  }
  // Put a carriage-return
  if (offset + 1 < MAX_BUFFER_SIZE) {
    buffer[offset++] = '\n';
  }
}

void LogToFile::logTextFormatted(int logLevel,
                                 int component,
                                 char const * className,
                                 char const * functionName,
                                 char const * text,
                                 int textLength) {
  // Should we start logging?
  if (!startLogging) {
    return;
  }

  // Do we have enough space to store everything?
  int classLength = strlen(className);
  int functionLength = strlen(functionName);
  int length = classLength + functionLength + textLength + 4;
  if (offset + length > MAX_BUFFER_SIZE) {
    return;
  }

  // Save the log level and component id
  buffer[offset++] = '0' + (char)logLevel;
  buffer[offset++] = '0' + (char)component;
  buffer[offset++] = ' ';

  // Save the class name
  memcpy(buffer + offset, className, classLength);
  offset += classLength;
  buffer[offset++] = ' ';

  // Save the function name
  memcpy(buffer + offset, functionName, functionLength);
  offset += functionLength;
  buffer[offset++] = ' ';

  // Save the text
  memcpy(buffer + offset, text, textLength);
  offset += textLength;
  buffer[offset++] = '\n';
}

void LogToFile::logRobotState(const RobotState & robotState) {
}

void LogToFile::logSegmentedImage(const SegmentedImage & segmentedImage) {
}

void LogToFile::logShape(Screen screen, const Shape & shape) {
}

void LogToFile::logTeamColor(bool isBlue) {
}

void LogToFile::logGameState(const GameState & gameState) {
  // We only write the log file if the gameState is finished
  if (gameState.getStateOfGame() == GameState::finished) {
    if (offset > 0) {
      // Create the file
      std::ofstream fileStream(FILENAME.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

      // Write the data and close the file
      printf("Writing the log file ... \n");
      fileStream.write(buffer, offset);
      printf("Done!\n");
      fileStream.close();

      // Sleep for 2 seconds so we sit down
      if (TIME_TO_SLEEP > 0) {
        usleep(TIME_TO_SLEEP);
      }

      offset = 0;
    }
    startLogging = false;
  }
  // We start logging if the gameState is not initial
  else if (!startLogging && (gameState.getStateOfGame() != GameState::initial)) {
    startLogging = true;
  }
}
