#include <stdint.h>
#include <string.h>

#include "SegmentedImageMessage.h"
#include "Vision/SegmentedImage/SegmentedImage.h"
#include "Vision/SegmentedImage/RGB.h"

#include <stdio.h>

SegmentedImageMessage::SegmentedImageMessage(const SegmentedImage & segmentedImage)
  : RemoteMessageFromRobot(RemoteMessageFromRobot::segmentedImage, 0, NULL) {
  createData(segmentedImage);
}

SegmentedImageMessage::~SegmentedImageMessage() {
}

void SegmentedImageMessage::createData(const SegmentedImage & segmentedImage) {
  int imageWidth  = segmentedImage.getImageWidth();
  int imageHeight = segmentedImage.getImageHeight();
  int imageSize   = imageWidth * imageHeight;
  int numColors   = segmentedImage.getNumColors();

  // How many bytes do we need?
  // 4 for the width of image
  // 4 for the height of image
  // the segmented image (1 byte per pixel)
  // 1 for the number of colors
  // 3 for each color
  size = 9 + imageSize + 3 * numColors;

  // Generate the data in bytes
  data = new char[size];

  if (data == NULL) {
    size = 0;
    return;
  }

  unsigned int offset = 0;

  // The first 4 bytes are the width of the image
  writeInt(data + offset, imageWidth);
  offset += 4;

  // The next 4 bytes are the height of the image
  writeInt(data + offset, imageHeight);
  offset += 4;

  // The next section is the segmented image itself
  memcpy(data + offset, segmentedImage.getIndices(), imageSize);
  offset += imageSize;

  // The next byte is the number of colors
  data[offset] = (uint8_t)numColors;
  offset++;

  RGB const * colors = segmentedImage.getColors();
  for (int i = 0; i < numColors; i++) {
    data[offset    ] = colors[i].getRed();
    data[offset + 1] = colors[i].getGreen();
    data[offset + 2] = colors[i].getBlue();

    offset += 3;
  }
}
