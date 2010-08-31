#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#include "Log.h"
#include "shared/ConfigFile/ConfigFile.h"

Log::Log(ConfigFile & configFile)
  : maxTextLength(configFile.getInt("log/maxTextLength", 255)),
    textBuffer(new char[maxTextLength]),
    connectedToClient(false) {
}

Log::~Log() {
  if (textBuffer != NULL) {
    delete [] textBuffer;
  }
}

void Log::logText(int logLevel,
                  int component,
                  char const * className,
                  char const * functionName,
                  char const * format, ...) {
  // Create the message
  va_list valist;
  va_start(valist, format);
  int length = vsnprintf(textBuffer, maxTextLength, format, valist);
  va_end(valist);

  // Send it to the subclass
  logTextFormatted(logLevel, component, className, functionName, textBuffer, length);
}

unsigned long Log::getTimestamp() {
  timeval startTime;
  gettimeofday(&startTime,NULL);
  return startTime.tv_sec*(1000) + (long int)(startTime.tv_usec/1000);
}

void Log::setConnectedToClient(bool _connectedToClient) {
  connectedToClient = _connectedToClient;
}

bool Log::isConnectedToClient() const {
  return connectedToClient;
}
