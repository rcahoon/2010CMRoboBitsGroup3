#ifndef SEGMENTEDIMAGE_H_
#define SEGMENTEDIMAGE_H_

#include "shared/UsefulMacros.h"

class ConfigFile;
class RGB;

class SegmentedImage {
public:
  SegmentedImage(ConfigFile & configFile);

  virtual ~SegmentedImage();

  void setIndices(char const * indices);

  void setRGB(RGB const * colors, int numColors);

  int getImageWidth() const;

  int getImageHeight() const;

  char const * getIndices() const;

  RGB const * getColors() const;

  int getNumColors() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(SegmentedImage);

  const int IMAGE_WIDTH, IMAGE_HEIGHT;

  char const *indices;

  RGB const *colors;
  int        numColors;
};

#endif /* SEGMENTEDIMAGE_H_ */
