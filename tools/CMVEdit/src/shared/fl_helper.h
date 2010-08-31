#ifndef __FL_HELPER_H__
#define __FL_HELPER_H__

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Browser.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Scroll.H>

#include <FL/fl_draw.H>
#include <FL/fl_message.H>

#include "colors.h"
#include "util.h"
#include "sstring.h"

//==== Helpful functions =============================================//

static inline Fl_Color fl_rgb_color(rgb c)
{
  return(fl_rgb_color(c.red, c.green, c.blue));
}

static inline void fl_color(rgb c)
{
  fl_color(c.red, c.green, c.blue);
}

static inline void fl_color(rgb c,float scale)
{
  fl_color(bound((int)rint(c.red   * scale), 0, 255),
           bound((int)rint(c.green * scale), 0, 255),
           bound((int)rint(c.blue  * scale), 0, 255));
}

const char *fl_event_str(int ev);

//--------------------------------------------------------------------//

class MouseLoc{
public:
  int wx,wy; // window location in display pixels
public:
  void set(int _wx,int _wy)
    {wx=_wx; wy=_wy;}
  void update()
    {set(Fl::event_x(),Fl::event_y());}
};

//==== Simple Widget Extensions ======================================//

class ShowWinButton : public Fl_Button{
public:
  int act_id; // user-set action id#
  Fl_Window *show_win; // the window to toggle the visibility of
public:
  ShowWinButton(int x,int y,int w,int h,const char *label);
  int handle(int ev);
};

//--------------------------------------------------------------------//

class ValueInput : public Fl_Value_Input{
  const char *output_format;
public:
  ValueInput(int x,int y,int w,int h,const char *label);
  int format(char* buffer);

  void setFormat(const char *_output_format)
    {output_format = _output_format;}
};

//--------------------------------------------------------------------//

class BarGraph : public Fl_Box {
  static const int b = 1;
  float v_min,v_max,v;
  int x0;
protected:
  int getx(float v)
  {return((int)((w()-2*b) * (v - v_min) / (v_max - v_min)));}
public:
  BarGraph(int x,int y,int w,int h,const char *label,
           float _v_min,float _v_max);
  void update(float nv);
  void draw();
};

//==== Bit Selection Widgets =========================================//

struct BitBrowserItem{
  const char *str;
  unsigned mask;
  bool default_value;
};

class SelectBitBrowser : public Fl_Multi_Browser {
public:
  typedef BitBrowserItem Item;
protected:
  unsigned mask;

  void recalcMask();
  static void cb(Fl_Widget *w, void *p);
public:
  SelectBitBrowser(int x,int y,int w,int h,const char *label=0);
  int add(Item *item,int num);
  int add(Item *item) {return(add(item,1));}

  unsigned getMask() {return(mask);}
  void setMask(unsigned _mask);
};

class CheckBitBrowser : public Fl_Check_Browser {
public:
  typedef BitBrowserItem Item;
protected:
  Item *items;
  int num;
  unsigned mask;

  void recalcMask();
  static void cb(Fl_Widget *w, void *p);
public:
  CheckBitBrowser(int x,int y,int w,int h,const char *label=0);
  int setItems(Item *_items,int _num);
  unsigned getMask() {return(mask);}
};

//==== Improved Scroller =============================================//

class CrossScroll : public Fl_Scroll {
  int cross_x,cross_y;
  bool cross_visible;

  float view_x,view_y;
public:
  void draw();
  int handle(int event);

  void updateCross(int nx,int ny,bool erase_old,bool draw_new);
  void moveCross(int nx,int ny);
  void showCross();
  void hideCross();

  void saveView();
  void centerView();
public:
  CrossScroll(int x,int y,int w,int h);
};

//==== Text Information Box ==========================================//

class InfoBox : public Fl_Box {
public:
  CharString str;
public:
  InfoBox(int x,int y,int w,int h,const char *initial_label);
  void update();
};

#endif
