#ifndef COLORSPACECONVERTER_H_
#define COLORSPACECONVERTER_H_

#include "colors.h"

class colorSpaceConverter
{

	public:
		static yuv* YUYVtoYUV(const yuyv* YUYVimg, int width, int height);
		static rgb* YUVtoRGB(const yuv* YUVimg, int width, int height);
//		static bgr* YUVtoBGR(const yuv* YUVimg, int width, int height);
//		static bgr* RGBtoBGR(const rgb* RGBimg, int width, int height);

		static rgb YUVtoRGB(yuv YUV);

		//This is untested !!!
		static rgb* YUYVtoRGB(const yuyv* YUYVimg, int width, int height);

		static int WritePPM(const rgb *img,int width,int height, const char *filename);

};

#endif /*COLORSPACECONVERTER_H_*/
