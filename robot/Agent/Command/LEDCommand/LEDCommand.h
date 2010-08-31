#ifndef LEDCOMMAND_H
#define LEDCOMMAND_H

#include "LEDType.h"
#include "shared/UsefulMacros.h"

class LEDCommand{
public:
  LEDCommand();
  ~LEDCommand();
  void clear();

  void setLeftEar(unsigned int numLEDs, unsigned char intensity);
  void setLeftEarLED(LED::earType led, unsigned char intensity);
  void setLeftEarPattern(unsigned int pattern, unsigned char intensity);
  void setRightEar(unsigned int numLEDs, unsigned char intensity);
  void setRightEarLED(LED::earType led, unsigned char intensity);
  void setRightEarPattern(unsigned int pattern, unsigned char intensity);
  void setBothEars(unsigned int numLEDs, unsigned char intensity);
  void setBothEarsLED(LED::earType led, unsigned char intensity);
  void setBothEarsPattern(unsigned int pattern, unsigned char intensity);
  void setLeftEye(int rgbColor);
  void setLeftEyeInner(int rgbColor);
  void setLeftEyeOuter(int rgbColor);
  void setLeftEyeLED(LED::eyeType led, int rgbColor);
  void setRightEye(int rgbColor);
  void setRightEyeInner(int rgbColor);
  void setRightEyeOuter(int rgbColor);
  void setRightEyeLED(LED::eyeType led, int rgbColor);
  void setEyesInner(int rgbColor);
  void setEyesOuter(int rgbColor);
  void setChest(unsigned int rgbColor);
  void setLeftFoot(unsigned int rgbColor);
  void setRightFoot(unsigned int rgbColor);
  void setLED(LED::ledType led, unsigned char intensity);

  unsigned char const * getLEDs() const;
  unsigned int const * getChest() const;
  unsigned int const * getLeftFoot() const;
  unsigned int const * getRightFoot() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(LEDCommand);

  unsigned char ledList[LED::numLEDs];
  unsigned int chestLED;
  unsigned int leftFootLED;
  unsigned int rightFootLED;

};

#endif /* LEDCOMMAND_H_ */
