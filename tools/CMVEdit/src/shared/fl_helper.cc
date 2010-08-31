#include <FL/x.H>

#include "fl_helper.h"


const char *fl_event_str(int ev)
{
  static const int MaxEvent = 24;
  const char *EventString[MaxEvent] = {
    "FL_NO_EVENT",
    "FL_PUSH",
    "FL_RELEASE",
    "FL_ENTER",
    "FL_LEAVE",
    "FL_DRAG",
    "FL_FOCUS",
    "FL_UNFOCUS",
    "FL_KEYDOWN",
    "FL_KEYUP",
    "FL_CLOSE",
    "FL_MOVE",
    "FL_SHORTCUT",
    "FL_DEACTIVATE",
    "FL_ACTIVATE",
    "FL_HIDE",
    "FL_SHOW",
    "FL_PASTE",
    "FL_SELECTIONCLEAR",
    "FL_MOUSEWHEEL",
    "FL_DND_ENTER",
    "FL_DND_DRAG",
    "FL_DND_LEAVE",
    "FL_DND_RELEASE"
  };

  if(ev>=0 && ev<MaxEvent){
    return(EventString[ev]);
  }else{
    return(NULL);
  }
}

//====================================================================//

ShowWinButton::ShowWinButton(int x,int y,int w,int h,const char *label)
  : Fl_Button(x,y,w,h,label)
{
  act_id = 0;
  show_win = NULL;
}

int ShowWinButton::handle(int ev)
{
  static const bool debug = false;
  Fl_Window *w = show_win;

  if(ev==FL_RELEASE && Fl::event_inside(this) && w!=NULL){
    if(w->visible()){
      if(debug) printf("ShowWinButton::hide\n");
      w->hide();
    }else{
      if(debug) printf("ShowWinButton::show\n");
      if(w->x()==0 && w->y()==0){
        Fl_Window *p = window();
        static const int border = 4;
        w->position(p->x()+p->w()+2*border,p->y());
      }
      w->show();
    }
  }

  return(Fl_Button::handle(ev));
}

//--------------------------------------------------------------------//

ValueInput::ValueInput(int x,int y,int w,int h,const char *label)
  : Fl_Value_Input(x,y,w,h,label)
{
  output_format = "%g";
}

int ValueInput::format(char* buffer)
{
  double v = value();
  return(sprintf(buffer,output_format,v));
}

//--------------------------------------------------------------------//

BarGraph::BarGraph(int x,int y,int w,int h,const char *label,
                         float _v_min,float _v_max)
  : Fl_Box(x,y,w,h,label)
{
  box(FL_FLAT_BOX);
  align(FL_ALIGN_LEFT);
  color(fl_rgb_color(0));
  v_min = _v_min;
  v_max = _v_max;
  v = 0.0;
  x0 = getx(0.0);
}

void BarGraph::update(float nv)
{
  v = nv;
  draw();
}

void BarGraph::draw()
{
  Fl_Box::draw();

  if(v>0){
    fl_color(0,0,255);
  }else{
    fl_color(255,0,0);
  }

  int xv = getx(v);
  fl_rectf(x()+b + min(x0,xv),y()+b,abs(x0-xv),h()-2*b);
}

//====================================================================//

void SelectBitBrowser::recalcMask()
{
  mask = 0;
  for(int i=1; i<=size(); i++){
    if(selected(i)) mask |= ((Item*)data(i))->mask;
  }
}

void SelectBitBrowser::cb(Fl_Widget *w, void *p)
{
  ((SelectBitBrowser*)w)->recalcMask();
}

SelectBitBrowser::SelectBitBrowser(int x,int y,int w,int h,const char *label)
  : Fl_Multi_Browser(x,y,w,h,label)
{
  type(FL_MULTI_BROWSER);
  mask = 0;
  callback(cb);
}

int SelectBitBrowser::add(Item *item,int num)
{
  if(!item) return(0);

  for(int i=0; i<num; i++){
    Fl_Multi_Browser::add(item[i].str,(void*)(&item[i]));
    select(size(),item[i].default_value);
    if(item[i].default_value) mask |= (1 << (size()-1));
  }
  return(num);
}

void SelectBitBrowser::setMask(unsigned _mask)
{
  mask = _mask;
  for(int i=0; i<size(); i++){
    bool sel = (mask >> i) & 1;
    select(i+1,sel);
  }
}

//--------------------------------------------------------------------//

void CheckBitBrowser::recalcMask()
{
  mask = 0;
  for(int i=0; i<num; i++){
    if(checked(i+1)) mask |= items[i].mask;
  }
  // printf("%X\n",mask);
}

void CheckBitBrowser::cb(Fl_Widget *w, void *p)
{
  ((CheckBitBrowser*)w)->recalcMask();
}

//CheckBitBrowser::CheckBitBrowser(int x,int y,int w,int h,const char *label)
//  : Fl_Check_Browser(x,y,w,h,label)
//{
//  type(FL_MULTI_BROWSER);
//  when(FL_WHEN_CHANGED);
//
//  items = NULL;
//  num = 0;
//  mask = 0;
//  callback(cb);
//}

int CheckBitBrowser::setItems(BitBrowserItem *_items,int _num)
{
  items = _items;
  num = _num;

  for(int i=0; i<num; i++){
    int val = (items[i].default_value != 0);
    if(val) mask |= items[i].mask;
    Fl_Check_Browser::add(items[i].str,val);
  }

  return(num);
}

//====================================================================//

static const bool UseCrosshair = false;

CrossScroll::CrossScroll(int x,int y,int w,int h)
  : Fl_Scroll(x,y,w,h)
{
  cross_visible = false;
  cross_x = cross_y = 0;
  view_x = view_y = 0.0;
  box(FL_NO_BOX);
}

void CrossScroll::draw()
{
  bool cross_update = cross_visible;

  if(cross_update){
    fl_push_no_clip();
    hideCross();
    fl_pop_clip();
  }

  Fl_Scroll::draw();

  if(cross_update){
    fl_push_no_clip();
    showCross();
    fl_pop_clip();
  }
}

int CrossScroll::handle(int event)
{
  int mx,my;
  int bret,ret=0;

  switch(event){
    case FL_ENTER:
    case FL_MOVE:
    case FL_DRAG:
      if(UseCrosshair){
        mx = Fl::event_x();
        my = Fl::event_y();
        window()->make_current();
        if((mx < x()+w()-16) && (my < y()+h()-16)){
          moveCross(mx,my);
          fl_cursor(FL_CURSOR_NONE,FL_WHITE,FL_BLACK);
        }else{
          hideCross();
          fl_cursor(FL_CURSOR_DEFAULT,FL_WHITE,FL_BLACK);
        }
      }
      break;
    case FL_LEAVE:
      if(UseCrosshair){
        window()->make_current();
        hideCross();
        fl_cursor(FL_CURSOR_DEFAULT,FL_WHITE,FL_BLACK);
      }
      break;
    case FL_PUSH:
    case FL_RELEASE:
      ret = 1;
      break;
  }

  bret = Fl_Scroll::handle(event);
  return((ret != 0)? ret : bret);
}

void CrossScroll::updateCross(int nx,int ny,bool erase_old,bool show_new)
{
  XGCValues values;
  values.function = GXinvert;
  XChangeGC(fl_display,fl_gc,GCFunction,&values);
  int t;

  if(cross_visible && erase_old){
    t = cross_y;
    XDrawLine(fl_display,fl_window,fl_gc,x(),t,x()+w()-16,t);
    t = cross_x;
    XDrawLine(fl_display,fl_window,fl_gc,t,y(),t,y()+h()-16);
    cross_visible = false;
  }

  cross_x = nx;
  cross_y = ny;

  if(show_new){
    t = cross_y;
    XDrawLine(fl_display,fl_window,fl_gc,x(),t,x()+w()-16,t);
    t = cross_x;
    XDrawLine(fl_display,fl_window,fl_gc,t,y(),t,y()+h()-16);
    cross_visible = true;
  }

  values.function = GXcopy;
  XChangeGC(fl_display,fl_gc,GCFunction,&values);
}

void CrossScroll::moveCross(int nx,int ny)
{
  updateCross(nx,ny,true,true);
}

void CrossScroll::showCross()
{
  if(!cross_visible) updateCross(cross_x,cross_y,false,true);
}

void CrossScroll::hideCross()
{
  if(cross_visible) updateCross(cross_x,cross_y,true,false);
}

void CrossScroll::saveView()
{
  if(children() == 0) return;
  Fl_Widget *c = child(0);
  if(!c) return;

  // get fractional position of mouse in client area
  view_x = (float)(xposition() + Fl::event_x() - x()) / c->w();
  view_y = (float)(yposition() + Fl::event_y() - y()) / c->h();
}

void CrossScroll::centerView()
{
  if(children() == 0) return;
  Fl_Widget *c = child(0);
  if(!c) return;

  // calculate desired position to center fractional view on mouse
  int des_x = (int)rint(view_x * c->w()) - (Fl::event_x() - x());
  int des_y = (int)rint(view_y * c->h()) - (Fl::event_y() - y());

  // bound position to show only valid client area
  int ScrollBarSize = 16; // not sure how to get this properly
  if(c->w() > w()-ScrollBarSize){
    des_x = bound(des_x, 0, c->w() - w() + ScrollBarSize);
  }else{
    des_x = 0;
  }
  if(c->h() > h()-ScrollBarSize){
    des_y = bound(des_y, 0, c->h() - h() + ScrollBarSize);
  }else{
    des_y = 0;
  }
  position(des_x,des_y);
}

//==== Text Information Box ==========================================//

InfoBox::InfoBox(int x,int y,int w,int h,const char *initial_label)
  : Fl_Box(FL_DOWN_BOX,x,y,w,h,NULL)
{
  str.copy(initial_label);
  align(FL_ALIGN_INSIDE);
  labelfont(FL_COURIER);
  label(str());
}

void InfoBox::update()
{
  label(str());
  redraw();
}
