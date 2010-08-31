#ifndef _INCLUDED_ZOOM_WINDOW_H_
#define _INCLUDED_ZOOM_WINDOW_H_

#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>

#include "image.h"
#include "gl_texture.h"


class ZoomWindow : public Fl_Gl_Window {
public:
  static const int MaxZoom = 64;

  struct MouseLoc{
    int   wx,wy; // window location in display pixels
    float sx,sy; // location in source image coordinates
    int   ix,iy; // integer location in source image coordinates
    float vx,vy; // current viewport
  };
protected:
  int num_layers;    // number of layers
  GlTexture *tex;    // textures for layers
  float *blend;      // layer blending coefficients

  int z,zx,zy;       // zoom factors
  int sw,sh;         // texture source dimensions

  float vx,vy;       // center of viewport
  float vw,vh;       // size of viewport
public:
  ZoomWindow(int _x,int _y,int _w,int _h,const char *_label,int _num_layers);
  ~ZoomWindow();
  void setSourceSize(int _w,int _h);

  bool setZoom(int _z,int _zx,int _zy);
  bool setZoom(int _z)
    {return(setZoom(_z,zx,zy));}
  bool zoom(int delta)
    {return(setZoom(z+delta,zx,zy));}
  bool zoomCentered(int delta,int tx,int ty);
  int calcProportionalZoomDelta(float factor,int dir) const;

  void zoomFull();
  void centerView(float _vx,float _vy);
  void shiftView(float dx,float dy);
  void dragView(const MouseLoc &m0,const MouseLoc &m1);
  void getViewableArea(ImageArea &a) const;

  void shiftViewW(float dwx,float dwy)
    {shiftView(dwx/(z*zx),dwy/(z*zy));}

  bool update(int layer,const SImage &img);
  void setBlend(int layer,float alpha);

  void resize(int _x,int _y,int _w,int _h);
protected:
  void glSetup();
  void setView();
public:
  void draw();
  MouseLoc getMouseLoc() const;
  bool isInsideImage(const MouseLoc &m) const;
  int handle(int event);
};

#endif
