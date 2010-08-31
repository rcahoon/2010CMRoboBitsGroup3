#include <stdio.h>

#include <FL/Fl_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_PNM_Image.H>

#include <jpeglib.h>
#include <png.h>
#include <setjmp.h>

#include "image_io.h"


enum Format{
  FormatUnknown = 0,
  FormatPPM,
  FormatPNG,
  FormatJPEG
};

Format GetFormat(const char *filename)
{
  const char *ext = strrchr(filename,'.');
  if(!ext) return(FormatUnknown);

  if(!strcmp(ext,".ppm")) return(FormatPPM);
  if(!strcmp(ext,".png")) return(FormatPNG);
  if(!strcmp(ext,".jpg")) return(FormatJPEG);

  return(FormatUnknown);
}

uchar *ReadGrayscale(int &width,int &height, const char *filename)
{
  uchar *imgbuf = NULL;
  width = height = 0;

  // load image
  Format fmt = GetFormat(filename);
  Fl_RGB_Image *img;
  switch(fmt){
    case FormatPPM:  img = new Fl_PNM_Image(filename);  break;
    case FormatPNG:  img = new Fl_PNG_Image(filename);  break;
    case FormatJPEG: img = new Fl_JPEG_Image(filename); break;
    default:         img = NULL;
  }
  if(!img) return(NULL);

  // make sure its the right format
  int sz = img->w() * img->h();
  if(!img->data() || img->d()!=1 || sz==0) goto end;

  // TODO: handle 2,3,4 channel images

  // allocate an image buffer
  imgbuf = new uchar[sz];
  if(!imgbuf) goto end;

  // copy the data
  width  = img->w();
  height = img->h();
  memcpy(imgbuf,*(img->data()),width*height*sizeof(uchar));

 end:
  delete(img);
  return(imgbuf);
}

rgb *ReadRGB(int &width,int &height, const char *filename)
{
  rgb *imgbuf = NULL;
  width = height = 0;

  // load image
  Format fmt = GetFormat(filename);
  Fl_RGB_Image *img;
  switch(fmt){
    case FormatPPM:  img = new Fl_PNM_Image(filename);  break;
    case FormatPNG:  img = new Fl_PNG_Image(filename);  break;
    case FormatJPEG: img = new Fl_JPEG_Image(filename); break;
    default:         img = NULL;
  }
  if(!img) return(NULL);

  // make sure its the right format
  if(!img->data() || img->d()!=3) goto end;

  // allocate an image buffer
  imgbuf = new rgb[img->w() * img->h()];
  if(!imgbuf) goto end;

  // copy the data
  width  = img->w();
  height = img->h();
  memcpy(imgbuf,*(img->data()),width*height*sizeof(rgb));

 end:
  delete(img);
  return(imgbuf);
}

bool WriteRGB(rgb *imgbuf, int width, int height, const char *filename,
              int quality)
{
  Format fmt = GetFormat(filename);
  // printf("fmt=%d\n",fmt);

  switch(fmt){
    case FormatPPM:  return(WritePPM (imgbuf,width,height,filename));
    case FormatPNG:  return(WritePNG (imgbuf,width,height,filename));
    case FormatJPEG: return(WriteJPEG(imgbuf,width,height,filename,quality));

    default:
      printf("WriteRGB: Unknown format for \"%s\"\n",filename);
      return(false);
  }
}

bool WritePPM(rgb *imgbuf, int width, int height, const char *filename)
{
  // open output file
  FILE *out = fopen(filename,"wb");
  if(!out) return(false);

  // write the image
  fprintf(out,"P6\n%d %d\n%d\n",width,height,255);
  fwrite(imgbuf,3,width*height,out);

  return(fclose(out) == 0);
}

bool WriteJPEG(rgb *imgbuf, int width, int height, const char *filename,
               int quality)
{
  jpeg_compress_struct cinfo;
  jpeg_error_mgr jerr;
  JSAMPROW *row = NULL;
  bool ok = false;

  // open output file
  FILE *out = fopen(filename,"wb");
  if(!out) return(false);

  // set up error handler
  cinfo.err = jpeg_std_error(&jerr);

  // create compression structure
  jpeg_create_compress(&cinfo);

  // set output stream
  jpeg_stdio_dest(&cinfo, out);

  // set image information
  cinfo.image_width  = width;
  cinfo.image_height = height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo,quality,TRUE);
  jpeg_start_compress(&cinfo,TRUE);

  // set up row pointers
  row = new JSAMPROW[height];
  if(!row) goto end;
  for(int i=0; i<height; i++){
    row[i] = (JSAMPROW)(&imgbuf[i * width]);
  }

  // write out the image
  jpeg_write_scanlines(&cinfo,row,height);
  jpeg_finish_compress(&cinfo);

  // clean up and exit
  ok = true;
 end:
  delete[](row);
  jpeg_destroy_compress(&cinfo);
  if(out) fclose(out);
  return(ok);
}

bool WritePNG(rgb *imgbuf, int width, int height, const char *filename)
{
  png_structp png = NULL;
  png_infop info = NULL;
  png_bytep *row = NULL;
  bool ok = false;

  // open the output file
  FILE *out = fopen(filename,"wb");
  if(!out) return(false);

  // create writing and info header structures
  png = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
  if(!png) goto end;
  info = png_create_info_struct(png);
  if(!info) goto end;

  // set the error handler
  if(setjmp(png_jmpbuf(png))) goto end;

  // Set up the output stream
  png_init_io(png,out);

  // Set image information
  png_set_IHDR(png, info, width, height, 8,
               PNG_COLOR_TYPE_RGB, PNG_INTERLACE_ADAM7,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  // Write the file header information.
  png_write_info(png,info);

  // swap location of alpha bytes from ARGB to RGBA
  // png_set_swap_alpha(png);

  // Get rid of filler byte if present (4->3 channels)
  // png_set_filler(png, 0, PNG_FILLER_BEFORE);

  // set up row pointers
  row = new png_bytep[height];
  if(!row) goto end;
  for(int i=0; i<height; i++){
    row[i] = (png_bytep)(&imgbuf[i * width]);
  }

  // write out the image
  png_write_image(png,row);
  png_write_end(png,info);

  // clean up and exit
  ok = true;
 end:
  delete[](row);
  if(png) png_destroy_write_struct(&png, &info);
  fclose(out);
  return(ok);
}
