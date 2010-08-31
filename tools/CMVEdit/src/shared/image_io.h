#ifndef __IMAGE_IO_H__
#define __IMAGE_IO_H__

#include "colors.h"

// automatic format image reading and writing functions
uchar *ReadGrayscale(int &width,int &height, const char *filename);
rgb *ReadRGB(             int &width,int &height,const char *filename);
bool WriteRGB(rgb *imgbuf,int  width,int  height,const char *filename,
              int quality = 85);

// manually selected format image writers
bool WritePPM( rgb *imgbuf, int width, int height, const char *filename);
bool WriteJPEG(rgb *imgbuf, int width, int height, const char *filename,
               int quality = 85);
bool WritePNG( rgb *imgbuf, int width, int height, const char *filename);

#endif
