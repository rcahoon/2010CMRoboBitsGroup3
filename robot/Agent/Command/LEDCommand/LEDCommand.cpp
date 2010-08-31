#include "LEDCommand.h"

#include <string.h>

using namespace LED;

LEDCommand::LEDCommand(){
  clear();
}

LEDCommand::~LEDCommand(){

}

void LEDCommand::clear(){
  memset(ledList, 0, numLEDs * sizeof(unsigned char));
  chestLED = 0;
  leftFootLED = 0;
  rightFootLED = 0;
}

void LEDCommand::setLeftEar(unsigned int numLEDs, unsigned char intensity){
  switch ( numLEDs ){
    case 10:
      ledList[EarsLedLeft324Deg] = intensity;
    case 9:
      ledList[EarsLedLeft288Deg] = intensity;
    case 8:
      ledList[EarsLedLeft252Deg] = intensity;
    case 7:
      ledList[EarsLedLeft216Deg] = intensity;
    case 6:
      ledList[EarsLedLeft180Deg] = intensity;
    case 5:
      ledList[EarsLedLeft144Deg] = intensity;
    case 4:
      ledList[EarsLedLeft108Deg] = intensity;
    case 3:
      ledList[EarsLedLeft72Deg] = intensity;
    case 2:
      ledList[EarsLedLeft36Deg] = intensity;
    case 1:
      ledList[EarsLedLeft0Deg] = intensity;
    case 0:
      break;

    default:
      break;
  }


  switch ( numLEDs ){
    case 0:
      ledList[EarsLedLeft0Deg] = 0;
    case 1:
      ledList[EarsLedLeft36Deg] = 0;
    case 2:
      ledList[EarsLedLeft72Deg] = 0;
    case 3:
      ledList[EarsLedLeft108Deg] = 0;
    case 4:
      ledList[EarsLedLeft144Deg] = 0;
    case 5:
      ledList[EarsLedLeft180Deg] = 0;
    case 6:
      ledList[EarsLedLeft216Deg] = 0;
    case 7:
      ledList[EarsLedLeft252Deg] = 0;
    case 8:
      ledList[EarsLedLeft288Deg] = 0;
    case 9:
      ledList[EarsLedLeft324Deg] = 0;
    default:
      break;
  }
}

void LEDCommand::setLeftEarLED(earType led, unsigned char intensity){

  switch ( led ){
    case EarLed324Deg:
      ledList[EarsLedLeft324Deg] = intensity;
      break;
    case EarLed288Deg:
      ledList[EarsLedLeft288Deg] = intensity;
      break;
    case EarLed252Deg:
      ledList[EarsLedLeft252Deg] = intensity;
      break;
    case EarLed216Deg:
      ledList[EarsLedLeft216Deg] = intensity;
      break;
    case EarLed180Deg:
      ledList[EarsLedLeft180Deg] = intensity;
      break;
    case EarLed144Deg:
      ledList[EarsLedLeft144Deg] = intensity;
      break;
    case EarLed108Deg:
      ledList[EarsLedLeft108Deg] = intensity;
      break;
    case EarLed72Deg:
      ledList[EarsLedLeft72Deg] = intensity;
      break;
    case EarLed36Deg:
      ledList[EarsLedLeft36Deg] = intensity;
      break;
    case EarLed0Deg:
      ledList[EarsLedLeft0Deg] = intensity;
      break;
    default:
      break;
  }
}

void LEDCommand::setLeftEarPattern(unsigned int pattern, unsigned char intensity) {
  for (int i = 1; i <= 10; i++) {
    if (pattern & 1) {
      setLeftEarLED((earType)i, intensity);
    }
    else {
      setLeftEarLED((earType)i, 0);
    }

    pattern = pattern >> 1;
  }
}

void LEDCommand::setRightEar(unsigned int numLEDs, unsigned char intensity){

  switch ( numLEDs ){
    case 10:
      ledList[EarsLedRight324Deg] = intensity;
    case 9:
      ledList[EarsLedRight288Deg] = intensity;
    case 8:
      ledList[EarsLedRight252Deg] = intensity;
    case 7:
      ledList[EarsLedRight216Deg] = intensity;
    case 6:
      ledList[EarsLedRight180Deg] = intensity;
    case 5:
      ledList[EarsLedRight144Deg] = intensity;
    case 4:
      ledList[EarsLedRight108Deg] = intensity;
    case 3:
      ledList[EarsLedRight72Deg] = intensity;
    case 2:
      ledList[EarsLedRight36Deg] = intensity;
    case 1:
      ledList[EarsLedRight0Deg] = intensity;
    case 0:
      break;

    default:
      break;
  }


  switch ( numLEDs ){
    case 0:
      ledList[EarsLedRight0Deg] = 0;
    case 1:
      ledList[EarsLedRight36Deg] = 0;
    case 2:
      ledList[EarsLedRight72Deg] = 0;
    case 3:
      ledList[EarsLedRight108Deg] = 0;
    case 4:
      ledList[EarsLedRight144Deg] = 0;
    case 5:
      ledList[EarsLedRight180Deg] = 0;
    case 6:
      ledList[EarsLedRight216Deg] = 0;
    case 7:
      ledList[EarsLedRight252Deg] = 0;
    case 8:
      ledList[EarsLedRight288Deg] = 0;
    case 9:
      ledList[EarsLedRight324Deg] = 0;
    default:
      break;
  }
}

void LEDCommand::setRightEarLED(earType led, unsigned char intensity){

  switch ( led ){
    case EarLed324Deg:
      ledList[EarsLedRight324Deg] = intensity;
      break;
    case EarLed288Deg:
      ledList[EarsLedRight288Deg] = intensity;
      break;
    case EarLed252Deg:
      ledList[EarsLedRight252Deg] = intensity;
      break;
    case EarLed216Deg:
      ledList[EarsLedRight216Deg] = intensity;
      break;
    case EarLed180Deg:
      ledList[EarsLedRight180Deg] = intensity;
      break;
    case EarLed144Deg:
      ledList[EarsLedRight144Deg] = intensity;
      break;
    case EarLed108Deg:
      ledList[EarsLedRight108Deg] = intensity;
      break;
    case EarLed72Deg:
      ledList[EarsLedRight72Deg] = intensity;
      break;
    case EarLed36Deg:
      ledList[EarsLedRight36Deg] = intensity;
      break;
    case EarLed0Deg:
      ledList[EarsLedRight0Deg] = intensity;
      break;
    default:
      break;
  }
}

void LEDCommand::setRightEarPattern(unsigned int pattern, unsigned char intensity) {
  for (int i = 1; i <= 10; i++) {
    if (pattern & 1) {
      setRightEarLED((earType)i, intensity);
    }
    else {
      setRightEarLED((earType)i, 0);
    }

    pattern = pattern >> 1;
  }
}

void LEDCommand::setBothEars(unsigned int numLEDs, unsigned char intensity) {
  setLeftEar(numLEDs, intensity);
  setRightEar(numLEDs, intensity);
}

void LEDCommand::setBothEarsLED(earType led, unsigned char intensity) {
  setLeftEarLED(led, intensity);
  setRightEarLED(led, intensity);
}

void LEDCommand::setBothEarsPattern(unsigned int pattern, unsigned char intensity) {
  setLeftEarPattern(pattern, intensity);
  setRightEarPattern(pattern, intensity);
}

void LEDCommand::setLeftEye(int rgbColor){
  unsigned char red = (unsigned char)((rgbColor & 0xFF0000) >> 16);
  unsigned char green = (unsigned char)((rgbColor & 0x00FF00) >> 8);
  unsigned char blue = (unsigned char)(rgbColor & 0x0000FF);

  ledList[FaceLedRedLeft0Deg] = red;
  ledList[FaceLedRedLeft45Deg] = red;
  ledList[FaceLedRedLeft90Deg] = red;
  ledList[FaceLedRedLeft135Deg] = red;
  ledList[FaceLedRedLeft180Deg] = red;
  ledList[FaceLedRedLeft225Deg] = red;
  ledList[FaceLedRedLeft270Deg] = red;
  ledList[FaceLedRedLeft315Deg] = red;

  ledList[FaceLedGreenLeft0Deg] = green;
  ledList[FaceLedGreenLeft45Deg] = green;
  ledList[FaceLedGreenLeft90Deg] = green;
  ledList[FaceLedGreenLeft135Deg] = green;
  ledList[FaceLedGreenLeft180Deg] = green;
  ledList[FaceLedGreenLeft225Deg] = green;
  ledList[FaceLedGreenLeft270Deg] = green;
  ledList[FaceLedGreenLeft315Deg] = green;

  ledList[FaceLedBlueLeft0Deg] = blue;
  ledList[FaceLedBlueLeft45Deg] = blue;
  ledList[FaceLedBlueLeft90Deg] = blue;
  ledList[FaceLedBlueLeft135Deg] = blue;
  ledList[FaceLedBlueLeft180Deg] = blue;
  ledList[FaceLedBlueLeft225Deg] = blue;
  ledList[FaceLedBlueLeft270Deg] = blue;
  ledList[FaceLedBlueLeft315Deg] = blue;
}

void LEDCommand::setLeftEyeInner(int rgbColor) {
  unsigned char red = (unsigned char)((rgbColor & 0xFF0000) >> 16);
  unsigned char green = (unsigned char)((rgbColor & 0x00FF00) >> 8);
  unsigned char blue = (unsigned char)(rgbColor & 0x0000FF);

  ledList[FaceLedRedLeft315Deg] = red;
  ledList[FaceLedRedLeft0Deg] = red;
  ledList[FaceLedRedLeft45Deg] = red;
  ledList[FaceLedRedLeft90Deg] = red;

  ledList[FaceLedGreenLeft315Deg] = green;
  ledList[FaceLedGreenLeft0Deg] = green;
  ledList[FaceLedGreenLeft45Deg] = green;
  ledList[FaceLedGreenLeft90Deg] = green;

  ledList[FaceLedBlueLeft315Deg] = blue;
  ledList[FaceLedBlueLeft0Deg] = blue;
  ledList[FaceLedBlueLeft45Deg] = blue;
  ledList[FaceLedBlueLeft90Deg] = blue;
}

void LEDCommand::setLeftEyeOuter(int rgbColor) {
  unsigned char red = (unsigned char)((rgbColor & 0xFF0000) >> 16);
  unsigned char green = (unsigned char)((rgbColor & 0x00FF00) >> 8);
  unsigned char blue = (unsigned char)(rgbColor & 0x0000FF);

  ledList[FaceLedRedLeft135Deg] = red;
  ledList[FaceLedRedLeft180Deg] = red;
  ledList[FaceLedRedLeft225Deg] = red;
  ledList[FaceLedRedLeft270Deg] = red;

  ledList[FaceLedGreenLeft135Deg] = green;
  ledList[FaceLedGreenLeft180Deg] = green;
  ledList[FaceLedGreenLeft225Deg] = green;
  ledList[FaceLedGreenLeft270Deg] = green;

  ledList[FaceLedBlueLeft135Deg] = blue;
  ledList[FaceLedBlueLeft180Deg] = blue;
  ledList[FaceLedBlueLeft225Deg] = blue;
  ledList[FaceLedBlueLeft270Deg] = blue;
}

void LEDCommand::setRightEyeInner(int rgbColor) {
  unsigned char red = (unsigned char)((rgbColor & 0xFF0000) >> 16);
  unsigned char green = (unsigned char)((rgbColor & 0x00FF00) >> 8);
  unsigned char blue = (unsigned char)(rgbColor & 0x0000FF);

  ledList[FaceLedRedRight225Deg] = red;
  ledList[FaceLedRedRight270Deg] = red;
  ledList[FaceLedRedRight315Deg] = red;
  ledList[FaceLedRedRight0Deg] = red;

  ledList[FaceLedGreenRight225Deg] = green;
  ledList[FaceLedGreenRight270Deg] = green;
  ledList[FaceLedGreenRight315Deg] = green;
  ledList[FaceLedGreenRight0Deg] = green;

  ledList[FaceLedBlueRight225Deg] = blue;
  ledList[FaceLedBlueRight270Deg] = blue;
  ledList[FaceLedBlueRight315Deg] = blue;
  ledList[FaceLedBlueRight0Deg] = blue;
}

void LEDCommand::setRightEyeOuter(int rgbColor) {
  unsigned char red = (unsigned char)((rgbColor & 0xFF0000) >> 16);
  unsigned char green = (unsigned char)((rgbColor & 0x00FF00) >> 8);
  unsigned char blue = (unsigned char)(rgbColor & 0x0000FF);

  ledList[FaceLedRedRight45Deg] = red;
  ledList[FaceLedRedRight90Deg] = red;
  ledList[FaceLedRedRight135Deg] = red;
  ledList[FaceLedRedRight180Deg] = red;

  ledList[FaceLedGreenRight45Deg] = green;
  ledList[FaceLedGreenRight90Deg] = green;
  ledList[FaceLedGreenRight135Deg] = green;
  ledList[FaceLedGreenRight180Deg] = green;

  ledList[FaceLedBlueRight45Deg] = blue;
  ledList[FaceLedBlueRight90Deg] = blue;
  ledList[FaceLedBlueRight135Deg] = blue;
  ledList[FaceLedBlueRight180Deg] = blue;
}

void LEDCommand::setEyesInner(int rgbColor) {
  setLeftEyeInner(rgbColor);
  setRightEyeInner(rgbColor);
}

void LEDCommand::setEyesOuter(int rgbColor) {
  setLeftEyeOuter(rgbColor);
  setRightEyeOuter(rgbColor);
}

void LEDCommand::setLeftEyeLED(eyeType led, int rgbColor){
  unsigned char red = (unsigned char)((rgbColor & 0xFF0000) >> 16);
  unsigned char green = (unsigned char)((rgbColor & 0x00FF00) >> 8);
  unsigned char blue = (unsigned char)(rgbColor & 0x0000FF);

  switch ( led ){
    case EyeLed0Deg:
      ledList[FaceLedRedLeft0Deg] = red;
      ledList[FaceLedGreenLeft0Deg] = green;
      ledList[FaceLedBlueLeft0Deg] = blue;
      break;
    case EyeLed45Deg:
      ledList[FaceLedRedLeft45Deg] = red;
      ledList[FaceLedGreenLeft45Deg] = green;
      ledList[FaceLedBlueLeft45Deg] = blue;
      break;
    case EyeLed90Deg:
      ledList[FaceLedRedLeft90Deg] = red;
      ledList[FaceLedGreenLeft90Deg] = green;
      ledList[FaceLedBlueLeft90Deg] = blue;
      break;
    case EyeLed135Deg:
      ledList[FaceLedRedLeft135Deg] = red;
      ledList[FaceLedGreenLeft135Deg] = green;
      ledList[FaceLedBlueLeft135Deg] = blue;
      break;
    case EyeLed180Deg:
      ledList[FaceLedRedLeft180Deg] = red;
      ledList[FaceLedGreenLeft180Deg] = green;
      ledList[FaceLedBlueLeft180Deg] = blue;
      break;
    case EyeLed225Deg:
      ledList[FaceLedRedLeft225Deg] = red;
      ledList[FaceLedGreenLeft225Deg] = green;
      ledList[FaceLedBlueLeft225Deg] = blue;
      break;
    case EyeLed270Deg:
      ledList[FaceLedRedLeft270Deg] = red;
      ledList[FaceLedGreenLeft270Deg] = green;
      ledList[FaceLedBlueLeft270Deg] = blue;
      break;
    case EyeLed315Deg:
      ledList[FaceLedRedLeft315Deg] = red;
      ledList[FaceLedGreenLeft315Deg] = green;
      ledList[FaceLedBlueLeft315Deg] = blue;
      break;
    default:
      break;
  }
}

void LEDCommand::setRightEye(int rgbColor){
  unsigned char red = (unsigned char)((rgbColor & 0xFF0000) >> 16);
  unsigned char green = (unsigned char)((rgbColor & 0x00FF00) >> 8);
  unsigned char blue = (unsigned char)(rgbColor & 0x0000FF);

  ledList[FaceLedRedRight0Deg] = red;
  ledList[FaceLedRedRight45Deg] = red;
  ledList[FaceLedRedRight90Deg] = red;
  ledList[FaceLedRedRight135Deg] = red;
  ledList[FaceLedRedRight180Deg] = red;
  ledList[FaceLedRedRight225Deg] = red;
  ledList[FaceLedRedRight270Deg] = red;
  ledList[FaceLedRedRight315Deg] = red;

  ledList[FaceLedGreenRight0Deg] = green;
  ledList[FaceLedGreenRight45Deg] = green;
  ledList[FaceLedGreenRight90Deg] = green;
  ledList[FaceLedGreenRight135Deg] = green;
  ledList[FaceLedGreenRight180Deg] = green;
  ledList[FaceLedGreenRight225Deg] = green;
  ledList[FaceLedGreenRight270Deg] = green;
  ledList[FaceLedGreenRight315Deg] = green;

  ledList[FaceLedBlueRight0Deg] = blue;
  ledList[FaceLedBlueRight45Deg] = blue;
  ledList[FaceLedBlueRight90Deg] = blue;
  ledList[FaceLedBlueRight135Deg] = blue;
  ledList[FaceLedBlueRight180Deg] = blue;
  ledList[FaceLedBlueRight225Deg] = blue;
  ledList[FaceLedBlueRight270Deg] = blue;
  ledList[FaceLedBlueRight315Deg] = blue;
}

void LEDCommand::setRightEyeLED(eyeType led, int rgbColor){
  unsigned char red = (unsigned char)((rgbColor & 0xFF0000) >> 16);
  unsigned char green = (unsigned char)((rgbColor & 0x00FF00) >> 8);
  unsigned char blue = (unsigned char)(rgbColor & 0x0000FF);

  switch ( led ){
    case EyeLed0Deg:
      ledList[FaceLedRedRight0Deg] = red;
      ledList[FaceLedGreenRight0Deg] = green;
      ledList[FaceLedBlueRight0Deg] = blue;
      break;
    case EyeLed45Deg:
      ledList[FaceLedRedRight45Deg] = red;
      ledList[FaceLedGreenRight45Deg] = green;
      ledList[FaceLedBlueRight45Deg] = blue;
      break;
    case EyeLed90Deg:
      ledList[FaceLedRedRight90Deg] = red;
      ledList[FaceLedGreenRight90Deg] = green;
      ledList[FaceLedBlueRight90Deg] = blue;
      break;
    case EyeLed135Deg:
      ledList[FaceLedRedRight135Deg] = red;
      ledList[FaceLedGreenRight135Deg] = green;
      ledList[FaceLedBlueRight135Deg] = blue;
      break;
    case EyeLed180Deg:
      ledList[FaceLedRedRight180Deg] = red;
      ledList[FaceLedGreenRight180Deg] = green;
      ledList[FaceLedBlueRight180Deg] = blue;
      break;
    case EyeLed225Deg:
      ledList[FaceLedRedRight225Deg] = red;
      ledList[FaceLedGreenRight225Deg] = green;
      ledList[FaceLedBlueRight225Deg] = blue;
      break;
    case EyeLed270Deg:
      ledList[FaceLedRedRight270Deg] = red;
      ledList[FaceLedGreenRight270Deg] = green;
      ledList[FaceLedBlueRight270Deg] = blue;
      break;
    case EyeLed315Deg:
      ledList[FaceLedRedRight315Deg] = red;
      ledList[FaceLedGreenRight315Deg] = green;
      ledList[FaceLedBlueRight315Deg] = blue;
      break;
    default:
      break;
  }
}

void LEDCommand::setChest(unsigned int rgbColor){
  chestLED = rgbColor;
}

void LEDCommand::setLeftFoot(unsigned int rgbColor){
  leftFootLED = rgbColor;
}

void LEDCommand::setRightFoot(unsigned int rgbColor){
  rightFootLED = rgbColor;
}

void LEDCommand::setLED(ledType led, unsigned char intensity){
  ledList[led] = intensity;
}

unsigned char const * LEDCommand::getLEDs() const {
  return ledList;
}

unsigned int const * LEDCommand::getChest() const{
  return &chestLED;
}

unsigned int const * LEDCommand::getLeftFoot() const{
  return &leftFootLED;
}

unsigned int const * LEDCommand::getRightFoot() const{
  return &rightFootLED;
}
