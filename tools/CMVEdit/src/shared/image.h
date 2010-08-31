#ifndef _INCLUDED_IMAGE_H_
#define _INCLUDED_IMAGE_H_

#include <stdlib.h>
#include <math.h>

#include "colors.h"
#include "util.h"

//==== RGB Utilities =================================================//

rgb blend(rgb a,rgb b,float t);

rgb to_rgb(rgbf c);
rgbf to_rgbf(rgb c);
rgbf clip(rgbf c);
rgbf operator +(const rgbf a,const rgbf b);
rgbf operator *(const rgbf a,const rgbf b);
rgbf operator *(const rgbf a,const double m);

rgbf pow(const rgbf x,const double p);
double sse(rgbf a,rgbf b);

//==== SImage Class ==================================================//

struct ImageArea{
  int x,y,w,h;
};

class SImage{
public:
  rgb *img;
  int w,h;

  SImage()
    {img=NULL; w=h=0;}
  ~SImage()
    {reset();}

  int width() const
    {return(w);}
  int height() const
    {return(h);}
  const rgb *data() const
    {return(img);}

  void reset();
  bool allocate(int nw,int nh);
  bool load(const char *filename);
  bool save(const char *filename);
  bool copy(const SImage &a);
  bool copy_geometry(const SImage &a);

  void clear(rgb c);
  bool blend(const SImage &a,const SImage &b,float t);
  bool samesize(const SImage &a)
    {return(w==a.w && h==a.h);}
  int pixels() const
    {return(w * h);}

  bool decimate(const SImage &src,int pw,int ph);
  bool decimate(const SImage &src,int p)
    {return(decimate(src,p,p));}

  bool blit(const SImage &src,int sx,int sy,int sw,int sh,int dx,int dy);
  bool blit(const SImage &src,int dx,int dy)
    {return(blit(src,0,0,src.w,src.h,dx,dy));}
  void fill_rect(int x,int y,int dw,int dh,rgb c);
  void zoom(const SImage &src,int ox,int oy,int zoom_x,int zoom_y,rgb def);
  void flip();
  void mirror();

  rgb getpixel(int x,int y,rgb def) const;

  rgb getpixel_fast(int x,int y) const
    {return(img[y*w + x]);}
  rgb getpixel_clip(int x,int y) const;
  rgb getpixel_clip(float x,float y) const;

  rgb getpixel_wrap(int x,int y) const
    {return(img[(y % h)*w + (x % w)]);}
  rgb getpixel_wrap(float x,float y) const;

  bool setpixel(int x,int y,rgb c);

  const rgb &operator()(int x,int y) const
    {return(img[y*w + x]);}
  rgb &operator()(int x,int y)
    {return(img[y*w + x]);}

  void drawline(int x1,int y1,int x2,int y2,rgb c);
  void drawbox(int x,int y,int w,int h,rgb c);
};

class Gradient{
  SImage img;
  float x0,x1;
  int row;
public:
  bool load(const char *filename)
    {row=0; return(img.load(filename));}
  void setRange(float _x0,float _x1)
    {x0=_x0; x1=_x1;}
  void select(int _row)
    {row = bound(_row,0,img.height()-1);}

  rgb get(float x);
  rgb operator()(float x)
    {return(get(x));}
};

#endif
