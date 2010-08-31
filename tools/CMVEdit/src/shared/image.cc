#include <stdlib.h>
#include <stdio.h>

#include "image.h"

// Conditional support for loading/saving images
#define USE_IMAGE_IO     1
#define USE_FLTK         0
#define USE_IMAGE_MAGICK 0

#if USE_IMAGE_IO
#include "image_io.h"
#endif

#if USE_FLTK
#include <FL/Fl_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_PNM_Image.H>
#endif

#if USE_IMAGE_MAGICK
#include <Magick++.h>
using namespace Magick;
#endif

//==== RGB Utilities =================================================//

rgb blend(rgb a,rgb b,float t)
{
  rgb r;

  r.red   = (int)((a.red   * (1.0 - t)) + (b.red   * t));
  r.green = (int)((a.green * (1.0 - t)) + (b.green * t));
  r.blue  = (int)((a.blue  * (1.0 - t)) + (b.blue  * t));

  return(r);
}

rgb to_rgb(rgbf c)
{
  rgb r;

  r.red   = bound((int)(255.0 * c.red  ),0,255);
  r.green = bound((int)(255.0 * c.green),0,255);
  r.blue  = bound((int)(255.0 * c.blue ),0,255);

  return(r);
}

rgbf to_rgbf(rgb c)
{
  rgbf r;

  r.red   = c.red   / 255.0;
  r.green = c.green / 255.0;
  r.blue  = c.blue  / 255.0;

  return(r);
}

rgbf clip(rgbf c)
{
  rgbf r;

  r.red   = bound(c.red  ,0.0,255.0);
  r.green = bound(c.green,0.0,255.0);
  r.blue  = bound(c.blue ,0.0,255.0);

  return(r);
}

rgbf operator +(const rgbf a,const rgbf b)
{
  rgbf r;
  r.red   = a.red   + b.red;
  r.green = a.green + b.green;
  r.blue  = a.blue  + b.blue;
  return(r);
}

rgbf operator *(const rgbf a,const rgbf b)
{
  rgbf r;
  r.red   = a.red   * b.red;
  r.green = a.green * b.green;
  r.blue  = a.blue  * b.blue;
  return(r);
}

rgbf operator *(const rgbf a,const double m)
{
  rgbf r;
  r.red   = a.red   * m;
  r.green = a.green * m;
  r.blue  = a.blue  * m;
  return(r);
}

rgbf pow(const rgbf x,const double p)
{
  rgbf r;
  r.red   = ::pow(x.red  ,p);
  r.green = ::pow(x.green,p);
  r.blue  = ::pow(x.blue ,p);
  return(r);
}

double sse(rgbf a,rgbf b)
{
  double dr,dg,db;

  dr = a.red   - b.red;
  dg = a.green - b.green;
  db = a.blue  - b.blue;

  // this is an unweighted difference, which should be better than a
  // luminance comparison, according to the the ColorFAQ
  // (http://www.inforamp.net/~poynton/ColorFAQ.html)
  return(dr*dr + dg*dg + db*db);
}

//==== Image IO Routines =============================================//

#if USE_FLTK
Fl_RGB_Image *LoadImageFltk(const char *filename)
{
  Fl_RGB_Image *img = NULL;

  if(strstr(filename,".png")){
    img = new Fl_PNG_Image(filename);
  }else if(strstr(filename,".ppm") ||
           strstr(filename,".pgm")){
    img = new Fl_PNM_Image(filename);
  }else if(strstr(filename,".jpg") ||
           strstr(filename,".jpeg")){
    img = new Fl_JPEG_Image(filename);
  }
  /*
  else if(strstr(filename,".bmp")){
   img = new Fl_BMP_Image(filename);
  }
  */

  return(img);
}
#endif

//==== SImage Class ==================================================//

void SImage::reset()
{
  delete[](img);
  img = NULL;
  w = h = 0;
}

bool SImage::allocate(int nw,int nh)
{
  int size,nsize;

  size = w * h;
  nsize = nw * nh;

  // allocate new buffer if different size
  if(size != nsize){
    delete[](img);
    img = new rgb[nsize];
  }

  if(img){
    w = nw;
    h = nh;
    return(true);
  }else{
    w = 0;
    h = 0;
    return(false);
  }
}

bool SImage::load(const char *filename)
{
  bool ok = false;

#if USE_IMAGE_IO
  delete[](img);
  img = ReadRGB(w,h,filename);
  ok = (img != NULL);
#endif

#if USE_FLTK
  if(!ok){
    Fl_RGB_Image *fimg = LoadImageFltk(filename);

    allocate(fimg->w(),fimg->h());
    int size = fimg->w() * fimg->h() * fimg->d();
    memcpy(img,*(fimg->data()),size);

    // might need stride too
    // copyRaw(img->data(),img->w(),img->h(),img->d());
    delete(fimg);
  }
#endif

#if USE_IMAGE_MAGICK
  if(!ok){
    try{
      Image image(filename);
      allocate(image.columns(),image.rows());
      image.write(0,0,w,h,"RGB",CharPixel,img);
    }

    catch(Exception &error_){
      ok = false;
    }
  }
#endif

  return(ok);
}

bool SImage::save(const char *filename)
{
  bool ok = false;

#if USE_IMAGE_IO
  ok =  WriteRGB(img,w,h,filename);
#endif

#if USE_IMAGE_MAGICK
  try{
    Image image(w,h,"RGB",CharPixel,img);
    image.write(filename);
  }

  catch(Exception &error_){
    ok = false;
  }
#endif

  return(ok);
}

bool SImage::copy(const SImage &a)
{
  if(!allocate(a.w,a.h)) return(false);

  memcpy(img,a.img,sizeof(rgb)*w*h);

  return(true);
}

bool SImage::copy_geometry(const SImage &a)
// copies the geometry (dimensions, size, allocation) of image a, but
// not its pixel data
{
  if(!allocate(a.w,a.h)) return(false);

  return(true);
}

void SImage::clear(rgb c)
{
  int i,s;

  s = w * h;

  for(i=0; i<s; i++){
    img[i] = c;
  }
}

bool SImage::blend(const SImage &a,const SImage &b,float t)
{
  int i,s;
  rgb r,r_a,r_b,*img_a,*img_b;

  if(a.w!=b.w || a.h!=b.h) return(false);
  if(!allocate(a.w,b.h)) return(false);

  s = w * h;
  img_a = a.img;
  img_b = b.img;

  for(i=0; i<s; i++){
    r_a = img_a[i];
    r_b = img_b[i];

    r.red   = (int)((r_a.red   * (1.0 - t)) + (r_b.red   * t));
    r.green = (int)((r_a.green * (1.0 - t)) + (r_b.green * t));
    r.blue  = (int)((r_a.blue  * (1.0 - t)) + (r_b.blue  * t));

    img[i] = r;
  }

  return(true);
}

bool SImage::decimate(const SImage &src,int pw,int ph)
{
  int x,y,i,j;
  rgb s,d;
  rgb_int sum;
  int div = pw*ph,div_h = div/2;

  if(&src == this) return(false);
  if(pw==1 && ph==1) return(copy(src));

  if(!allocate(src.w/pw,src.h/ph)) return(false);

  for(y=0; y<h; y++){
    for(x=0; x<w; x++){
      // sum up source square
      sum.set(0,0,0);
      for(i=0; i<ph; i++){
        for(j=0; j<pw; j++){
          s = src.img[(y*ph + i)*src.w + (x*pw + j)];
          sum.r += s.red;
          sum.g += s.green;
          sum.b += s.blue;
        }
      }

      // calculate average value and set destination
      d.red   = (sum.r + div_h) / div;
      d.green = (sum.g + div_h) / div;
      d.blue  = (sum.b + div_h) / div;
      img[y*w + x] = d;
    }
  }

  return(true);
}

bool SImage::blit(const SImage &src,int sx,int sy,int sw,int sh,int dx,int dy)
{
  int i,j;

  if(sx<0 || sy<0 || sx+sw>src.w || sy+sh>src.h) return(false);

  if(dx < 0){
    sw += dx;
    dx = 0;
  }

  if(dy < 0){
    sh += dy;
    dy = 0;
  }

  if(dx + sw > w){
    sw = w - dx;
  }

  if(dy + sh > h){
    sh = h - dy;
  }

  if(sw<=0 || sh<=0) return(true);

  for(i=0; i<sh; i++){
    for(j=0; j<sw; j++){
      img[dx+j + w*(dy+i)] = src.img[sx+j + src.w*(sy+i)];
    }
  }

  return(true);
}

void SImage::fill_rect(int x,int y,int dw,int dh,rgb c)
{
  int i,j;

  if(x < 0){
    dw += x;
    x = 0;
  }

  if(y < 0){
    dh += y;
    y = 0;
  }

  if(x + dw > w){
    dw = w - x;
  }

  if(y + dh > h){
    dh = h - y;
  }

  if(dw<=0 || dh<=0) return;

  for(i=0; i<dh; i++){
    for(j=0; j<dw; j++){
      img[x+j + w*(y+i)] = c;
    }
  }
}

void SImage::zoom(const SImage &src,int ox,int oy,
                  int zoom_x,int zoom_y,rgb def)
{
  int x,y,yl;
  int dx,dy,dw,dh;

  if(false){
    dx = 0;
    dy = 0;
    dw = w;
    dh = h;

    if(ox < 0){
      fill_rect(0,0,-ox,h,def);
      dx -= ox;
      ox = 0;
    }

    if(oy < 0){
      fill_rect(0,0,w,-oy,def);
      dy -= oy;
      oy = 0;
    }

    if(dw+ox > src.w*zoom_x){
      dw = src.w*zoom_x - ox;
      fill_rect(dw,0,w-dw,h,def);
    }

    if(dh+oy > src.h*zoom_y){
      dh = src.h*zoom_y - oy;
      fill_rect(0,dh,w,h-dh,def);
    }

    for(y=dy; y<dh; y++){
      yl = ((oy + y) / zoom_y) * src.w;
      for(x=dx; x<dw; x++){
        img[y*w + x] = src.img[yl + (ox + x)/zoom_x];
      }
    }
  }else{
    for(y=0; y<h; y++){
      if(oy+y>=0 && oy+y<src.h*zoom_y){
        yl = (oy + y)/zoom_y*src.w;
        for(x=0; x<w; x++){
          if(ox+x>=0 && ox+x<src.w*zoom_x){
            img[y*w + x] = src.img[yl + (ox + x)/zoom_x];
          }else{
            img[y*w + x] = def;
          }
        }
      }else{
        for(x=0; x<w; x++) img[y*w + x] = def;
      }
    }
  }
}

void SImage::flip()
{
  for(int i=0; i<h/2; i++){
    for(int j=0; j<w; j++){
      swap(img[w*i + j],img[w*(h-1 - i) + j]);
    }
  }
}

void SImage::mirror()
{
  for(int i=0; i<h; i++){
    for(int j=0; j<w/2; j++){
      swap(img[w*i + j],img[w*i + (w-1 - j)]);
    }
  }
}

rgb SImage::getpixel(int x,int y,rgb def) const
{
  if(x>=0 && y>=0 && x<w && y<h){
    return(img[y*w + x]);
  }else{
    return(def);
  }
}

rgb SImage::getpixel_clip(int x,int y) const
// get pixel in image (or neareast pixel if outside image)
{
  return(img[bound(x,0,w-1) + w*bound(y,0,h-1)]);
}

rgb SImage::getpixel_clip(float x,float y) const
// get pixel using linear interpolation, with clipping to edges
{
  rgb tl,tr,bl,br;
  rgb t,b,r;
  int ix,iy;

  ix = (int)x;
  iy = (int)y;

  if(ix>0 && iy>0 && ix<w-1 && iy<h-1){
    // all neighbors in image, common case
    tl = img[(iy + 0)*w + ix + 0];
    tr = img[(iy + 0)*w + ix + 1];
    bl = img[(iy + 1)*w + ix + 0];
    br = img[(iy + 1)*w + ix + 1];
  }else{
    // neighbors fall off of edge, uncommon case
    tl = getpixel_clip(ix + 0,iy + 0);
    tr = getpixel_clip(ix + 1,iy + 0);
    bl = getpixel_clip(ix + 0,iy + 1);
    br = getpixel_clip(ix + 1,iy + 1);
  }

  t = ::blend(tl,tr,x-ix);
  b = ::blend(bl,br,x-ix);
  r = ::blend( t, b,y-iy);

  return(r);
}

rgb SImage::getpixel_wrap(float x,float y) const
// get pixel using linear interpolation, with clipping to edges
{
  rgb tl,tr,bl,br;
  rgb t,b,r;
  int ix,iy;

  ix = (int)x;
  iy = (int)y;

  // find four neighbors for interpolation
  tl = img[((iy + 0) % h)*w + ((ix + 0) % w)];
  tr = img[((iy + 0) % h)*w + ((ix + 1) % w)];
  bl = img[((iy + 1) % h)*w + ((ix + 0) % w)];
  br = img[((iy + 1) % h)*w + ((ix + 1) % w)];

  t = ::blend(tl,tr,x-ix);
  b = ::blend(bl,br,x-ix);
  r = ::blend( t, b,y-iy);

  return(r);
}

bool SImage::setpixel(int x,int y,rgb c)
{
  if(x<0 || y<0 || x>=w || y>=h) return(false);

  img[y*w + x] = c;
  return(true);
}

void SImage::drawline(int x1,int y1,int x2,int y2,rgb c)
{
  int i;

  if(y1 == y2){
    if(x1 > x2) swap(x1,x2);
    x1 = max(x1,  0);
    x2 = min(x2,w-1);
    for(i=x1; i<=x2; i++) img[y1*w + i] = c;
  }else if(x1 == x2){
    if(y1 > y2) swap(y1,y2);
    y1 = max(y1,  0);
    y2 = min(y2,w-1);
    for(i=y1; i<=y2; i++) img[i*w + x1] = c;
  }else{
    // todo, fill this in
  }
}

void SImage::drawbox(int x,int y,int w,int h,rgb c)
{
  if(w<=0 || h<=0) return;

  drawline(x,y, x+w,y  , c);
  drawline(x,y, x  ,y+h, c);

  drawline(x,y+h-1, x+w,y+h-1, c);
  drawline(x+w-1,y, x+w-1,y+h, c);
}

rgb Gradient::get(float x)
{
  if(x <= x0) return(img.getpixel_fast(            0,row));
  if(x >= x1) return(img.getpixel_fast(img.width()-1,row));

  x = (img.width()-1) * (x - x0) / (x1 - x0);
  int ix = (int)floor(x);

  return(blend(img.getpixel_fast(ix+0,row),
               img.getpixel_fast(ix+1,row),x-ix));
}
