#include <stdio.h>

#include "colorSpaceConverter.h"
#include <iostream>

yuv* colorSpaceConverter::YUYVtoYUV(const yuyv* YUYVimg, int width, int height)
{
	yuv *YUVimg= new yuv[width*height];
	for(int i=0,j=0;i<width*height;i+=2, j++)
	{
		int Y1 = YUYVimg[j].y1;
		int U = YUYVimg[j].u;
		int Y2 = YUYVimg[j].y2;
		int V = YUYVimg[j].v;

		YUVimg[i].y = (uchar)Y1;
		YUVimg[i].u = (uchar)U;
		YUVimg[i].v = (uchar)V;

		YUVimg[i+1].y = (uchar)Y2;
		YUVimg[i+1].u = (uchar)U;
		YUVimg[i+1].v = (uchar)V;
	}
	return YUVimg;
}

// Yuv to RGB
rgb* colorSpaceConverter::YUVtoRGB(const yuv* YUVimg, int width, int height)
{
	rgb *RGBimg= new rgb[width*height];

	if(!RGBimg)
	{
		std::cout<<"[YUVtoRGB] Cannot create RGB object\n";
		return NULL;
	}

	for(int j=0;j<width*height;j++)
	{
		int Y = YUVimg[j].y;
		int U = YUVimg[j].u;
		int V = YUVimg[j].v;

		int B = (int)(1.164*(Y-16) + 2.018*(U-128));
		int G = (int)(1.164*(Y-16) - 0.813*(V-128) - 0.391*(U-128));
		int R = (int)(1.164*(Y-16) + 1.596*(V-128));

			if(B<0)	B=0;
			if(G<0) G=0;
			if(R<0)	R=0;
			if(B>255)	B=255;
			if(G>255)	G=255;
			if(R>255)	R=255;

		RGBimg[j].red = (uchar)R;
		RGBimg[j].blue = (uchar)B;
		RGBimg[j].green = (uchar)G;
	}

	return RGBimg;
}

// Yuv to RGB
rgb colorSpaceConverter::YUVtoRGB(yuv yuvpixel)
{
	rgb rgbpixel;
		int B = (int)(1.164*(yuvpixel.y-16) + 2.018*(yuvpixel.u-128));
		int G = (int)(1.164*(yuvpixel.y-16) - 0.813*(yuvpixel.v-128) - 0.391*(yuvpixel.u-128));
		int R = (int)(1.164*(yuvpixel.y-16) + 1.596*(yuvpixel.v-128));

			if(B<0)	B=0;
			if(G<0) G=0;
			if(R<0)	R=0;
			if(B>255)	B=255;
			if(G>255)	G=255;
			if(R>255)	R=255;

			rgbpixel.red = (uchar)R;
			rgbpixel.blue = (uchar)B;
			rgbpixel.green = (uchar)G;
	return rgbpixel;
}

//bgr* colorSpaceConverter::RGBtoBGR(const rgb* RGBimg, int width, int height)
//{
//	bgr *BGRimg= new bgr[width*height];
//
//	for(int j=0;j<width*height;j++)
//	{
//		BGRimg[j].red = RGBimg[j].red;
//		BGRimg[j].blue = RGBimg[j].blue;
//		BGRimg[j].green = RGBimg[j].green;
//	}
//	return BGRimg;
//}
//
//bgr* colorSpaceConverter::YUVtoBGR(const yuv* YUVimg, int width, int height)
//{
//	bgr *BGRimg= new bgr[width*height];
//
//	for(int j=0;j<width*height;j++)
//	{
//		int Y = YUVimg[j].y;
//		int U = YUVimg[j].u;
//		int V = YUVimg[j].v;
//
//		int B = (int)(1.164*(Y-16) + 2.018*(U-128));
//		int G = (int)(1.164*(Y-16) - 0.813*(V-128) - 0.391*(U-128));
//		int R = (int)(1.164*(Y-16) + 1.596*(V-128));
//
//			if(B<0)	B=0;
//			if(G<0) G=0;
//			if(R<0)	R=0;
//			if(B>255)	B=255;
//			if(G>255)	G=255;
//			if(R>255)	R=255;
//
//		BGRimg[j].red = (uchar)R;
//		BGRimg[j].blue = (uchar)B;
//		BGRimg[j].green = (uchar)G;
//	}
//	return BGRimg;
//}

//Warning!!! Untested.
rgb* colorSpaceConverter::YUYVtoRGB(const yuyv* YUYVimg, int width, int height)
{
	rgb *RGBimg= new rgb[width*height];
	for(int i=0,j=0;i<width*height;i+=2, j++)
	{
		int Y1 = YUYVimg[j].y1;
		int U = YUYVimg[j].u;
		int Y2 = YUYVimg[j].y2;
		int V = YUYVimg[j].v;

		int B = (int)(1.164*(Y1-16) + 2.018*(U-128));
		int G = (int)(1.164*(Y1-16) - 0.813*(V-128) - 0.391*(U-128));
		int R = (int)(1.164*(Y1-16) + 1.596*(V-128));

			if(B<0)	B=0;
			if(G<0) G=0;
			if(R<0)	R=0;
			if(B>255)	B=255;
			if(G>255)	G=255;
			if(R>255)	R=255;

		RGBimg[i].red = (uchar)R;
		RGBimg[i].blue = (uchar)B;
		RGBimg[i].green = (uchar)G;

		B = (int)(1.164*(Y2-16) + 2.018*(U-128));
		G = (int)(1.164*(Y2-16) - 0.813*(V-128) - 0.391*(U-128));
		R = (int)(1.164*(Y2-16) + 1.596*(V-128));

			if(B<0)	B=0;
			if(G<0) G=0;
			if(R<0)	R=0;
			if(B>255)	B=255;
			if(G>255)	G=255;
			if(R>255)	R=255;

		RGBimg[i+1].red = (uchar)R;
		RGBimg[i+1].blue = (uchar)B;
		RGBimg[i+1].green = (uchar)G;
	}

	return RGBimg;
}


int colorSpaceConverter::WritePPM(const rgb *img,int width,int height, const char *filename)
{
  FILE *out;
  int wrote;

  out = fopen(filename,"wb");
  if(!out) return(0);

  fprintf(out,"P6\n%d %d\n%d\n",width,height,255);
  wrote = fwrite(img,sizeof(rgb),width*height,out);
  fclose(out);

  return(wrote);
}
