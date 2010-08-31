#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <albroker.h>
#include <alptr.h>
#include <alledsproxy.h>
#include "Agent/Command/LEDCommand/LEDType.h"
#include "Agent/Command/LEDCommand/LEDCommand.h"

class ConfigFile;
class Log;

class LEDController {
public:
  LEDController(AL::ALPtr<AL::ALBroker> broker, ConfigFile & configFile, Log & log);
  ~LEDController();
  void executeLEDCommand(const LEDCommand & ledCommand);
private:
  const float STARTUP_TIME;
  const float RESET_TIME;

  Log & log;
  unsigned long startupTimestamp, resetTimestamp;

  AL::ALPtr<AL::ALLedsProxy> leds;
  unsigned char ledList[LED::numLEDs];
  unsigned int chestLED;
  unsigned int leftFootLED;
  unsigned int rightFootLED;
  bool startedUp, initialized;

  static const std::string ledStringValue[];
};

#endif // LEDCONTROLLER_H
