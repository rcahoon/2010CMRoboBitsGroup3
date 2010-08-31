#ifndef _INCLUDED_TMAP_VIEW_H_
#define _INCLUDED_TMAP_VIEW_H_

#include "runqueue.h"

#include <FL/Fl.H>
#include <FL/Fl_Box.H>

//#include "vision/vision.h"


class CMVEdit;

class TMapUndoEntry{
public:
  int y_level,size;
  cmap_t *data;
public:
  TMapUndoEntry()
    {y_level=size=0; data=NULL;}
  TMapUndoEntry(const TMapUndoEntry &e)
    {y_level=size=0; data=NULL; copy(e);}
  ~TMapUndoEntry()
    {reset();}

  TMapUndoEntry &operator=(const TMapUndoEntry &e)
    {copy(e); return(*this);}

  bool init(int _size);
  void reset();
  bool copy(const TMapUndoEntry &e);

  bool set(const cmap_t *tmap,int yi,int num_u,int num_v);
  bool get(      cmap_t *tmap,       int num_u,int num_v) const;
  int diff(const cmap_t *tmap,int yi,int num_u,int num_v) const;
};

class TMapBox : public Fl_Box {
public:
  int zoom;
  int y_level;
  int last_iu,last_iv;
  bool redraw_all;

  int cen_u; // center "white" value for U dimension of color
  int cen_v; // center "white" value for V dimension of color

  enum ViewMode{
    ModeTMap     = 0,
    ModeTMapHist = 1,
    ModeHistOnly = 2,
  };
  ViewMode mode;

  // undo information
  static const int MaxUndoLevels = 64;
  RunningQueue<TMapUndoEntry,MaxUndoLevels> undo_data;
  int undo_level;
  bool changed;

  // gui parent
  CMVEdit *cmvedit;

public:
  TMapBox(int x,int y,int w,int h);

  bool isHistogramVisible()
    {return(mode != ModeTMap);}
  void redrawDirty()
    {redraw_all=false; redraw();}

protected:
  int drawLine(int y1,int u1,int v1,
               int y2,int u2,int v2,
               int dest_color,int rad);
  int drawOutwardLine(int y0,int u0,int v0,
                      int dest_color,int rad);

  int paint(int y,int u,int v,cmap_t src_color,cmap_t dest_color);
  int paint(int y,int u,int v,cmap_t dest_color);

  void saveUndoInfo();
  void setUndo(int _undo_level);
public:
  void undo()
    {setUndo(undo_level+1);}
  void redo()
    {setUndo(undo_level-1);}
  void clearUndo();

public:
  void draw();
  int handle(int event);
};

#endif
