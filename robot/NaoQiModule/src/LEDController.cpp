#include "LEDController.h"

#include "Log/Log.h"
#include "shared/ConfigFile/ConfigFile.h"

using namespace LED;

LEDController::LEDController(AL::ALPtr<AL::ALBroker> broker, ConfigFile & configFile, Log & _log)
  : STARTUP_TIME(configFile.getMillisecondsFromSeconds("ledController/startupTime", 15000)),
    RESET_TIME  (configFile.getMillisecondsFromSeconds("ledController/resetTime", 10000)),
    log(_log),
    startupTimestamp(_log.getTimestamp()),
    resetTimestamp(startupTimestamp),
    startedUp(false),
    initialized(false) {
  // Create a proxy to the LEDs module
  try {
    leds = AL::ALPtr<AL::ALLedsProxy>(new AL::ALLedsProxy(broker));
  }
  catch(const AL::ALError&) {
    std::cout << "could not create a proxy to LED" << std::endl;
  }

}

LEDController::~LEDController(){

}

void LEDController::executeLEDCommand(const LEDCommand & ledCommand){
  if (!startedUp) {
    if (log.getTimestamp() - startupTimestamp > STARTUP_TIME) {
      startedUp = true;
    }
    return;
  }
  else {
    if (log.getTimestamp() - resetTimestamp > RESET_TIME) {
      initialized = false;
    }
  }

  int i;
  unsigned char const *newLedList = ledCommand.getLEDs();
  unsigned int const *newChestLED = ledCommand.getChest();
  unsigned int const *newLeftFootLED = ledCommand.getLeftFoot();
  unsigned int const *newRightFootLED = ledCommand.getRightFoot();

  if (initialized) {
    for (i=0; i<numLEDs; i++){
      if (newLedList[i] != ledList[i]){
        ledList[i] = newLedList[i];
        leds->setIntensity(ledStringValue[i].c_str(), (float)ledList[i] / 255.);
      }
    }
    //check Chest LED
    if (*newChestLED != chestLED){
      chestLED = *newChestLED;
      leds->fadeRGB("ChestLeds", chestLED, 0);
    }

    //check left foot LED
    if (*newLeftFootLED != leftFootLED){
      leftFootLED = *newLeftFootLED;
      leds->fadeRGB("LeftFootLeds", leftFootLED, 0);
    }

    //check right foot LED
    if (*newRightFootLED != rightFootLED){
      rightFootLED = *newRightFootLED;
      leds->fadeRGB("RightFootLeds", rightFootLED, 0);
    }
  }
  else {
    initialized = true;
    resetTimestamp = log.getTimestamp();

    for (i=0; i<numLEDs; i++){
      ledList[i] = newLedList[i];
      leds->setIntensity(ledStringValue[i].c_str(), (float)ledList[i] /255.);
    }
    //chest
    chestLED = *newChestLED;
    leds->fadeRGB("ChestLeds", chestLED, 0);
    //left foot
    leftFootLED = *newLeftFootLED;
    leds->fadeRGB("LeftFootLeds", leftFootLED, 0);
    //right foot
    rightFootLED = *newRightFootLED;
    leds->fadeRGB("RightFootLeds", rightFootLED, 0);
  }
}

const std::string LEDController::ledStringValue[] = {
      "Ears/Led/Left/0Deg/Actuator/Value",
      "Ears/Led/Left/36Deg/Actuator/Value",
      "Ears/Led/Left/72Deg/Actuator/Value",
      "Ears/Led/Left/108Deg/Actuator/Value",
      "Ears/Led/Left/144Deg/Actuator/Value",
      "Ears/Led/Left/180Deg/Actuator/Value",
      "Ears/Led/Left/216Deg/Actuator/Value",
      "Ears/Led/Left/252Deg/Actuator/Value",
      "Ears/Led/Left/288Deg/Actuator/Value",
      "Ears/Led/Left/324Deg/Actuator/Value",
      "Ears/Led/Right/0Deg/Actuator/Value",
      "Ears/Led/Right/36Deg/Actuator/Value",
      "Ears/Led/Right/72Deg/Actuator/Value",
      "Ears/Led/Right/108Deg/Actuator/Value",
      "Ears/Led/Right/144Deg/Actuator/Value",
      "Ears/Led/Right/180Deg/Actuator/Value",
      "Ears/Led/Right/216Deg/Actuator/Value",
      "Ears/Led/Right/252Deg/Actuator/Value",
      "Ears/Led/Right/288Deg/Actuator/Value",
      "Ears/Led/Right/324Deg/Actuator/Value",
      "Face/Led/Red/Left/0Deg/Actuator/Value",
      "Face/Led/Red/Left/45Deg/Actuator/Value",
      "Face/Led/Red/Left/90Deg/Actuator/Value",
      "Face/Led/Red/Left/135Deg/Actuator/Value",
      "Face/Led/Red/Left/180Deg/Actuator/Value",
      "Face/Led/Red/Left/225Deg/Actuator/Value",
      "Face/Led/Red/Left/270Deg/Actuator/Value",
      "Face/Led/Red/Left/315Deg/Actuator/Value",
      "Face/Led/Green/Left/0Deg/Actuator/Value",
      "Face/Led/Green/Left/45Deg/Actuator/Value",
      "Face/Led/Green/Left/90Deg/Actuator/Value",
      "Face/Led/Green/Left/135Deg/Actuator/Value",
      "Face/Led/Green/Left/180Deg/Actuator/Value",
      "Face/Led/Green/Left/225Deg/Actuator/Value",
      "Face/Led/Green/Left/270Deg/Actuator/Value",
      "Face/Led/Green/Left/315Deg/Actuator/Value",
      "Face/Led/Blue/Left/0Deg/Actuator/Value",
      "Face/Led/Blue/Left/45Deg/Actuator/Value",
      "Face/Led/Blue/Left/90Deg/Actuator/Value",
      "Face/Led/Blue/Left/135Deg/Actuator/Value",
      "Face/Led/Blue/Left/180Deg/Actuator/Value",
      "Face/Led/Blue/Left/225Deg/Actuator/Value",
      "Face/Led/Blue/Left/270Deg/Actuator/Value",
      "Face/Led/Blue/Left/315Deg/Actuator/Value",
      "Face/Led/Red/Right/0Deg/Actuator/Value",
      "Face/Led/Red/Right/45Deg/Actuator/Value",
      "Face/Led/Red/Right/90Deg/Actuator/Value",
      "Face/Led/Red/Right/135Deg/Actuator/Value",
      "Face/Led/Red/Right/180Deg/Actuator/Value",
      "Face/Led/Red/Right/225Deg/Actuator/Value",
      "Face/Led/Red/Right/270Deg/Actuator/Value",
      "Face/Led/Red/Right/315Deg/Actuator/Value",
      "Face/Led/Green/Right/0Deg/Actuator/Value",
      "Face/Led/Green/Right/45Deg/Actuator/Value",
      "Face/Led/Green/Right/90Deg/Actuator/Value",
      "Face/Led/Green/Right/135Deg/Actuator/Value",
      "Face/Led/Green/Right/180Deg/Actuator/Value",
      "Face/Led/Green/Right/225Deg/Actuator/Value",
      "Face/Led/Green/Right/270Deg/Actuator/Value",
      "Face/Led/Green/Right/315Deg/Actuator/Value",
      "Face/Led/Blue/Right/0Deg/Actuator/Value",
      "Face/Led/Blue/Right/45Deg/Actuator/Value",
      "Face/Led/Blue/Right/90Deg/Actuator/Value",
      "Face/Led/Blue/Right/135Deg/Actuator/Value",
      "Face/Led/Blue/Right/180Deg/Actuator/Value",
      "Face/Led/Blue/Right/225Deg/Actuator/Value",
      "Face/Led/Blue/Right/270Deg/Actuator/Value",
      "Face/Led/Blue/Right/315Deg/Actuator/Value",
  };
