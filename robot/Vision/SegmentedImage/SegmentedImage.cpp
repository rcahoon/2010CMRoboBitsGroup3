#include "SegmentedImage.h"
#include "shared/ConfigFile/ConfigFile.h"

SegmentedImage::SegmentedImage(ConfigFile & configFile)
  : IMAGE_WIDTH (configFile.getInt("vision/imageWidth", 320)),
    IMAGE_HEIGHT(configFile.getInt("vision/imageHeight", 240)),
    indices(NULL),
    colors(NULL),
    numColors(0) {
}

SegmentedImage::~SegmentedImage() {
}

void SegmentedImage::setIndices(char const * _indices) {
  indices = _indices;
}

void SegmentedImage::setRGB(RGB const * _colors, int _numColors) {
  colors    = _colors;
  numColors = _numColors;
}

int SegmentedImage::getImageWidth() const {
  return IMAGE_WIDTH;
}

int SegmentedImage::getImageHeight() const {
  return IMAGE_HEIGHT;
}

char const * SegmentedImage::getIndices() const {
  return indices;
}

RGB const * SegmentedImage::getColors() const {
  return colors;
}

int SegmentedImage::getNumColors() const {
  return numColors;
}

