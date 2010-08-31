#ifndef _INCLUDED_CAMERA_VIEW_H_
#define _INCLUDED_CAMERA_VIEW_H_

#include <vector>

#include <FL/Fl.H>

#include "geometry.h"

#include "vision.h"
#include "zoom_win.h"


class CMVEdit;


class CameraView : public ZoomWindow {
public:
  MouseLoc m_push,m_cur,m_last;

  struct CalibPoint{
    vector3f world;
    vector2f screen;
  };
  std::vector<CalibPoint> pts;
  int drag_idx;

  CMVEdit *cmvedit; // gui parent

  int logfile_count;
public:
  CameraView(int _x,int _y,int _w,int _h);

  void setCMapAlpha(float alpha);

  int pickPoint(const vector2f &loc);

  void draw();
  int handle(int event);
};

#endif
