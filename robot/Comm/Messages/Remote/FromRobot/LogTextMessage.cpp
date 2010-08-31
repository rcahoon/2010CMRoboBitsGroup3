#include <stdint.h>
#include <string.h>

#include "LogTextMessage.h"

LogTextMessage::LogTextMessage(int logLevel,
                               int component,
                               char const * className,
                               char const * functionName,
                               char const * text)
  : RemoteMessageFromRobot(RemoteMessageFromRobot::logText, 0, NULL) {
  createData(logLevel, component, className, functionName, text);
}

LogTextMessage::~LogTextMessage() {
}

void LogTextMessage::createData(int logLevel,
                                int component,
                                char const * className,
                                char const * functionName,
                                char const * text) {

  // How many bytes do we need?
  // 1 for logLevel
  // 1 for component
  // className's length + \0 terminator
  // functionName's length + \0 terminator
  // text's length + \0 terminator
  unsigned int classNameLength    = strlen(className);
  unsigned int functionNameLength = strlen(functionName);
  unsigned int textLength         = strlen(text);
  size = 5 + classNameLength + functionNameLength + textLength;

  // Generate the data in bytes
  data = new char[size];

  if (data == NULL) {
    size = 0;
    return;
  }

  // The first byte is the log level
  data[0] = (uint8_t)logLevel;
  // The second byte is the component id
  data[1] = (uint8_t)component;

  // The next chunk of bytes are for className
  // (the \0 terminator is not copied)
  if (classNameLength > 0) {
    memcpy(data + 2, className, classNameLength);
  }
  // Add the \0
  data[2 + classNameLength] = 0;

  // The next chunk of bytes are for functionName
  if (functionNameLength > 0) {
    memcpy(data + 3 + classNameLength, functionName, functionNameLength);
  }
  // Add the \0
  data[3 + classNameLength + functionNameLength] = 0;

  // The next chunk of bytes are for text
  if (textLength > 0) {
    memcpy(data + 4 + classNameLength + functionNameLength, text, textLength);
  }
  // Add the \0
  data[size - 1] = 0;
}
