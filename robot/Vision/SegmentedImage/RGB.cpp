#include "RGB.h"

RGB::RGB(unsigned char _red, unsigned char _green, unsigned char _blue)
  : red(_red),
    green(_green),
    blue(_blue) {
}

RGB::~RGB() {
}

void RGB::set(unsigned char _red, unsigned char _green, unsigned char _blue) {
  red   = _red;
  green = _green;
  blue  = _blue;
}

int RGB::getColor() const {
  return (((int)red   << 16) & 0xFF0000)
       | (((int)green << 8 ) & 0x00FF00)
       | (((int)blue       ) & 0x0000FF);
}

unsigned char RGB::getRed() const {
  return red;
}

unsigned char RGB::getGreen() const {
  return green;
}

unsigned char RGB::getBlue() const {
  return blue;
}

