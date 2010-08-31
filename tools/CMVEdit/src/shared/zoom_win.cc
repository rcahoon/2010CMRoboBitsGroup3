#include <FL/Fl.H>
#include <FL/fl_message.H>

#include "zoom_win.h"

#include <stdio.h>


ZoomWindow::ZoomWindow(int _x,int _y,int _w,int _h,const char *_label,
                       int _num_layers)
  : Fl_Gl_Window(_x,_y,_w,_h,_label)
{
  num_layers = _num_layers;
  tex = new GlTexture[num_layers];
  blend = new float[num_layers];
  if(!tex || !blend){
    delete[](tex);
    delete[](blend);
    tex = NULL;
    blend = NULL;
    return;
  }

  for(int i=0; i<num_layers; i++){
    blend[i] = 0.0;
  }

  z = zx = zy = 1;
  sw = sh = 0;
  vx = vy = 0;
  vw = vh = 0;
}

ZoomWindow::~ZoomWindow() 
{
  delete[](tex);
  delete[](blend);
  tex = NULL;
  blend = NULL;
}

void ZoomWindow::setSourceSize(int _w,int _h)
{
  sw = _w;
  sh = _h;

  vx = _w/2;
  vy = _h/2;
  vw = _w;
  vh = _h;
}

bool ZoomWindow::setZoom(int _z,int _zx,int _zy)
{
  if(_z<1 || _zx<1 || _zy<1) return(false);
  if(_z>MaxZoom) return(false);

  z  = _z;
  zx = _zx;
  zy = _zy;

  vw = w() / (float)(z*zx);
  vh = h() / (float)(z*zy);

  return(true);
}

bool ZoomWindow::zoomCentered(int delta,int tx,int ty)
{
  /*
  m(vx,vw) = vx + vw*(tx/w() - 0.5)
  m(vx,vw) = m(vx0,vw0)

  given: vw,vx0,vw0
  solve for: vx

  m(vx,vw) = m(vx0,vw0)
  vx + vw*(tx/w() - 0.5) = vx0 + vw0*(tx/w() - 0.5)
  vx = vx0 + vw0*(tx/w() - 0.5) - vw*(tx/w() - 0.5)
  vx = vx0 + (vw0 - vw)*(tx/w() - 0.5)
  */

  float vx0 = vx;
  float vy0 = vy;
  float vw0 = vw;
  float vh0 = vh;

  if(zoom(delta)){
    vx = vx0 + (vw0 - vw)*((float)tx/w() - 0.5);
    vy = vy0 + (vh0 - vh)*((float)ty/h() - 0.5);
    return(true);
  }else{
    return(false);
  }
}

int ZoomWindow::calcProportionalZoomDelta(float factor,int dir) const
{
  if(dir > 0){
    return((int)ceil(z*factor) - z);
  }else if(dir < 0){
    return((int)floor(z/factor) - z);
  }else{
    return(0);
  }
}

void ZoomWindow::zoomFull()
{
  int z_full = min(w() / (sw*zx),
                   h() / (sh*zy));
  vx = 0.5*sw;
  vy = 0.5*sh;
  setZoom(z_full);
}

void ZoomWindow::centerView(float _vx,float _vy)
{
  vx = bound(_vx,0,sw);
  vy = bound(_vy,0,sh);

  /*
  vx = bound(_vx,vw*0.5,sw-vw*0.5);
  vy = bound(_vy,vh*0.5,sh-vh*0.5);

  if(vw > sw) vx = vw*0.5;
  if(vh > sh) vy = vh*0.5;
  */
}

void ZoomWindow::shiftView(float dx,float dy)
{
  centerView(vx+dx,vy+dy);
}

void ZoomWindow::dragView(const MouseLoc &m0,const MouseLoc &m1)
{
  float dx = (float)(m1.wx - m0.wx) / (z*zx);
  float dy = (float)(m1.wy - m0.wy) / (z*zy);
  centerView(m0.vx-dx, m0.vy-dy);
}

void ZoomWindow::getViewableArea(ImageArea &a) const
{
  a.x = bound((int)floor(vx - vw*0.5),0,sw-1);
  a.y = bound((int)floor(vy - vh*0.5),0,sh-1);
  a.w = bound((int)ceil(vw),0,sw-a.x);
  a.h = bound((int)ceil(vh),0,sh-a.y);
}

bool ZoomWindow::update(int layer,const SImage &img)
{
  if(!valid()) return(false);
  if(layer<0 || layer>=num_layers) return(false);

  const unsigned char *d = (const unsigned char*)img.img;
  if(!tex[layer].valid()){
    return(tex[layer].set(d,img.width(),img.height(),3,GL_RGB,
                          TexNoRepeat|TexNearest));
  }else{
    return(tex[layer].update(d,img.width(),img.height(),GL_RGB));
  }
}

void ZoomWindow::setBlend(int layer,float alpha)
{
  if(layer<0 || layer>=num_layers) return;
  blend[layer] = bound(alpha,0.0,1.0);
}

void ZoomWindow::resize(int _x,int _y,int _w,int _h)
{
  Fl_Gl_Window::resize(_x,_y,_w,_h);

  vw = w() / (float)(z*zx);
  vh = h() / (float)(z*zy);
}

void ZoomWindow::glSetup()
{
  valid(1);
  glViewport(0,0,w(),h());

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
}

void ZoomWindow::setView()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(vx - vw*0.5, vx + vw*0.5,
          vy + vh*0.5, vy - vh*0.5,
          1.0, -1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void ZoomWindow::draw()
{
  if(!valid()) glSetup();

  setView();
  glClear(GL_COLOR_BUFFER_BIT);

  for(int i=0; i<num_layers; i++){
    tex[i].select();
    glColor4f(1.0,1.0,1.0,blend[i]);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex3f( 0,sh,0);
    glTexCoord2f(1.0, 1.0); glVertex3f(sw,sh,0);
    glTexCoord2f(1.0, 0.0); glVertex3f(sw, 0,0);
    glTexCoord2f(0.0, 0.0); glVertex3f( 0, 0,0);
    glEnd();
  }
}

ZoomWindow::MouseLoc ZoomWindow::getMouseLoc() const
{
  MouseLoc m;
  mzero(m);
  m.wx = Fl::event_x();
  m.wy = Fl::event_y();

  m.sx = vx + vw*((float)m.wx/w() - 0.5);
  m.sy = vy + vh*((float)m.wy/h() - 0.5);

  m.ix = (int)floor(m.sx);
  m.iy = (int)floor(m.sy);

  m.vx = vx;
  m.vy = vy;

  if(false){
    printf("loc: w(%d %d) : s(%0.1f %0.1f)\n",m.wx,m.wy,m.sx,m.sy);
  }

  return(m);
}

bool ZoomWindow::isInsideImage(const MouseLoc &m) const
{
  return(m.ix>0 && m.ix<sw &&
         m.iy>0 && m.iy<sh);
}

int ZoomWindow::handle(int event)
{
  int ret = 0;

  switch(event){
    case FL_MOVE:
      break;

    case FL_PUSH:
    case FL_DRAG:
      switch(Fl::event_buttons()){
        case FL_BUTTON1:
        case FL_BUTTON2:
        case FL_BUTTON3:
	  break;
      }

      ret = 1;
      break;

    case FL_MOUSEWHEEL: {
      int d = calcProportionalZoomDelta(1.1,-Fl::event_dy());
      if(zoom(d)) redraw();
      ret = 1;
    } break;
  }

  if(ret){
    return(ret);
  }else{
    return(Fl_Gl_Window::handle(event));
  }
}
