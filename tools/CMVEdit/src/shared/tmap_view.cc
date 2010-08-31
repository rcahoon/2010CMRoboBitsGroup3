#include "../CMVEdit.h"

#include "tmap_view.h"


static const bool DebugUndo = false;


//====================================================================//

bool TMapUndoEntry::init(int _size)
{
  if(size != _size){
    reset();
    data = new cmap_t[_size];
    if(data) size = _size;
  }
  return(size == _size);
}

void TMapUndoEntry::reset()
{
  delete[](data);
  data = NULL;
  y_level = size = 0;
}

bool TMapUndoEntry::copy(const TMapUndoEntry &e)
{
  if(!init(e.size)) return(false);
  y_level = e.y_level;
  memcpy(data,e.data,size*sizeof(cmap_t));
  return(true);
}

bool TMapUndoEntry::set(const cmap_t *tmap,int yi,int num_u,int num_v)
{
  if(!tmap || !init(num_u * num_v)) return(false);
  memcpy(data, tmap+yi*size, size*sizeof(cmap_t));
  y_level = yi;
  return(true);
}

bool TMapUndoEntry::get(cmap_t *tmap,int num_u,int num_v) const
{
  if(!tmap || size!=num_u*num_v) return(false);
  memcpy(tmap+y_level*size, data, size*sizeof(cmap_t));
  return(true);
}

int TMapUndoEntry::diff(const cmap_t *tmap,int yi,int num_u,int num_v) const
{
  if(!tmap || size!=num_u*num_v) return(-1);
  int nd = 0;
  tmap += yi*size;
  for(int i=0; i<size; i++){
    if(tmap[i] != data[i]) nd++;
  }
  return(nd);
}

//====================================================================//

TMapBox::TMapBox(int x,int y,int w,int h)
  : Fl_Box(x,y,w,h)
{
  zoom = 2;
  y_level = 0;
  redraw_all = true;
  mode = ModeTMapHist;

  zoom = 2;
  y_level = 0;
  last_iu = last_iv = 0;
  redraw_all = true;

  cen_u = -1;
  cen_v = -1;

  mode = ModeTMapHist;

  undo_data.init();
  undo_level = 0;
  changed = false;

  cmvedit = NULL;
}

int TMapBox::drawLine(int y1,int u1,int v1,
                      int y2,int u2,int v2,
                      int dest_color,int rad)
{
  int my,mu,mv;
  int num = 0;

  num += cmvedit->vision.setTMap(y1,u1,v1,dest_color|128,rad);
  num += cmvedit->vision.setTMap(y2,u2,v2,dest_color|128,rad);
  if(num) changed = true;

  if(abs(y1-y2)<2 &&
     abs(u1-u2)<2 &&
     abs(v1-v2)<2) return(num);

  my = (y1 + y2) / 2;
  mu = (u1 + u2) / 2;
  mv = (v1 + v2) / 2;

  num += drawLine(y1,u1,v1,my,mu,mv,dest_color,rad);
  num += drawLine(my,mu,mv,y2,u2,v2,dest_color,rad);

  return(num);
}

int TMapBox::drawOutwardLine(int y0,int u0,int v0,
                             int dest_color,int rad)
{
  int nu = cmvedit->vision.tmap_u_size;
  int nv = cmvedit->vision.tmap_v_size;

  if(cen_u < 0) cen_u = nu/2;
  if(cen_v < 0) cen_v = nv/2;

  // get delta
  int du = u0 - cen_u;
  int dv = v0 - cen_v;

  int u1 = u0;
  int v1 = v0;

  if(du || dv){
    // walk out until one dimension is off the tmap
    while(u1>=0 && u1<=nu &&
          v1>=0 && v1<=nv){
      u1 += du;
      v1 += dv;
    }
  }

  int num = drawLine(y0,u0,v0, y0,u1,v1, dest_color,rad);

  if(num) changed = true;
  return(num);
}

int TMapBox::paint(int y,int u,int v,
                   cmap_t src_color,cmap_t dest_color)
{
  int v1,v2,i;
  int num;

  // find current line extents
  v1 = v2 = v;
  while(v1>=0 && cmvedit->vision.getTMapFast(y,u,v1)==src_color) v1--;
  while(v2<cmvedit->vision.tmap_v_size && cmvedit->vision.getTMapFast(y,u,v2)==src_color) v2++;
  v1++;

  // paint it
  for(i=v1; i<v2; i++) cmvedit->vision.setTMap(y,u,i,dest_color);
  num = v2 - v1;

  // recurse above
  if(u > 0){
    for(i=v1; i<v2; i++){
      if(cmvedit->vision.getTMapFast(y,u-1,i) == src_color){
        num += paint(y,u-1,i,src_color,dest_color);
      }
    }
  }

  // recurse below
  if(u+1 < cmvedit->vision.tmap_u_size){
    for(i=v1; i<v2; i++){
      if(cmvedit->vision.getTMapFast(y,u+1,i) == src_color){
        num += paint(y,u+1,i,src_color,dest_color);
      }
    }
  }

  if(num) changed = true;
  return(num);
}

int TMapBox::paint(int y,int u,int v,cmap_t dest_color)
{
  cmap_t src_color;

  src_color = cmvedit->vision.getTMap(y,u,v);

  if(y>=0 && y<cmvedit->vision.tmap_y_size &&
     u>=0 && u<cmvedit->vision.tmap_u_size &&
     v>=0 && v<cmvedit->vision.tmap_v_size &&
     src_color != dest_color){
    return(paint(y,u,v,src_color,dest_color));
  }else{
    return(0);
  }
}

void TMapBox::saveUndoInfo()
{
  if(!changed) return;

  if(undo_level){
    undo_data.truncate(-undo_level-1);
    undo_level = 0;
    if(DebugUndo){
      printf("truncate undo info (n=%d)\n",undo_data.size());
    }
  }

  const LowVision &v = cmvedit->vision;
  TMapUndoEntry e;
  e.set(v.tmap,y_level, v.tmap_u_size, v.tmap_v_size);
  undo_data.add(e);

  if(DebugUndo){
    printf("saved undo (y=%d,n=%d)\n",e.y_level,undo_data.size());
  }

  changed = false;
}

void TMapBox::setUndo(int _undo_level)
{
  if(undo_data.empty()) return;
  _undo_level = bound(_undo_level,0,undo_data.size()-1);
  if(_undo_level == undo_level) return;

  if(DebugUndo) printf("setUndo: %d -> %d\n",undo_level,_undo_level);
  undo_level = _undo_level;
  const LowVision &v = cmvedit->vision;
  const TMapUndoEntry &e = undo_data.get(-undo_level);
  e.get(v.tmap,v.tmap_u_size,v.tmap_v_size);

  cmvedit->setYLevel(e.y_level);
  redraw();
  changed = true;
}

void TMapBox::clearUndo()
{
  undo_data.clear();
  undo_level = 0;
  changed = true;
  if(DebugUndo) printf("clear undo.\n");
}

void TMapBox::draw()
{
  int u,v,w,i;
  int nu,nv;
  cmap_t p;
  yuv yuv_c;
  rgb c;
  int num = 0;

  nu = cmvedit->vision.tmap_u_size;
  nv = cmvedit->vision.tmap_v_size;

  // draw box using one rectangle per run
  for(u=0; u<nu; u++){
    if(mode != ModeHistOnly){
      v = 0;
      while(v < nv){
        p = cmvedit->vision.getTMapFast(y_level,u,v);
        w = 1;
        while(v+w<nv && cmvedit->vision.getTMapFast(y_level,u,v+w)==p) w++;
        if(redraw_all || (p&128)){
          if(p&128){
            p &= ~128;
            for(i=0; i<w; i++) cmvedit->vision.clearDirtyTMap(y_level,u,v+i);
          }
          c = cmvedit->vision.color[p].color;
          fl_rectf(x()+v*zoom,y()+u*zoom,w*zoom,zoom,
                   c.red,c.green,c.blue);
          num++;
        }
        v += w;
      }
    }else{
      c = cmvedit->vision.color[0].color;
      fl_rectf(x(),y()+u*zoom,nv*zoom,zoom,
               c.red,c.green,c.blue);
    }

    if(mode != ModeTMap){
      // draw histogram grid
      int gsz = 2;
      int ofs = (zoom-gsz)/2;

      yuv_c.y = (y_level << (8-bits_y)) + (1 << (8 - bits_y))/2;
      //Som: added parenthesis
      yuv_c.u = (u<<(8-bits_u)) + ((1 << (8 - bits_u))/2);
      i = (y_level << (bits_u+bits_v)) + (u << bits_v);

      for(v=0; v<nv; v++){
        if(cmvedit->tmap_used[i + v]){
          //Som: added parenthesis
          yuv_c.v = (v<<(8-bits_v)) + ((1 << (8 - bits_v))/2);
          c = YuvToRgb(yuv_c);
          fl_rectf(x()+v*zoom+ofs,y()+u*zoom+ofs,gsz,gsz,
                   c.red,c.green,c.blue);
        }
      }
    }
  }
  // printf("ra=%d n=%d\n",redraw_all,num);
  redraw_all = true;
}

int TMapBox::handle(int event)
{
  int bret,ret=0;
  int c;

  switch(event){
    case FL_ENTER:
      Fl::focus(this);
      break;

    case FL_MOVE:
    case FL_PUSH:
    case FL_DRAG: {
      int cur_color = cmvedit->current_color;
      int cur_size  = cmvedit->current_size;

      int iv = (Fl::event_x()-1 - x()) / zoom;
      int iu = (Fl::event_y()-1 - y()) / zoom;

      if(iv!=last_iv || iu!=last_iu){
        cmvedit->tmap_loc->str.printf("TMap[%2u,%3u,%3u]",y_level,iu,iv);
	cmvedit->tmap_loc->update();

        yuv c;
        c.y = y_level << (8-bits_y);
        c.u = iu      << (8-bits_u);
        c.v = iv      << (8-bits_v);
        cmvedit->updateColorBoxes(c);
      }

      if(event == FL_PUSH){
	last_iv = iv;
	last_iu = iu;
        saveUndoInfo();
      }
      switch(Fl::event_buttons()){
        case FL_BUTTON1:
          if(Fl::event_state(FL_SHIFT)){
            // todo: draw line from last click
          }else if(Fl::event_state(FL_CTRL)){
            // draw line from current position to border
            if(drawOutwardLine(y_level,iu,iv,
                               cur_color,cur_size)){
              redrawDirty();
            }
          }else{
            // draw line from last mouse position
            if(drawLine(y_level,last_iu,last_iv,y_level,iu,iv,
                        cur_color,cur_size)){
              redrawDirty();
            }
          }
	  // printf("(%d,%d)->(%d,%d)\n",last_iv,last_iu,iv,iu);
	  break;
        case FL_BUTTON2:
	  if(paint(y_level,iu,iv,cur_color)){
            saveUndoInfo();
	    redraw();
	  }
	  break;
        case FL_BUTTON3:
	  c = cmvedit->vision.getTMap(y_level,iu,iv);
	  cmvedit->setCurrentColor(c);
	  break;
      }
      last_iv = iv;
      last_iu = iu;
      ret = 1;
    } break;

    case FL_RELEASE:
      saveUndoInfo();
      ret = 1;
      break;
    case FL_MOUSEWHEEL:
      cmvedit->setYLevel(y_level+Fl::event_dy());
      changed = true;
      ret = 1;
      break;
    case FL_FOCUS:
    case FL_UNFOCUS:
      ret = 1;
      break;
    case FL_KEYDOWN:
      ret = 1;
      switch(Fl::event_key()){
        case ' ':
	  cmvedit->processFrame();
	  break;

        case 'h':
          mode = (ViewMode)!mode;
          cmvedit->color_hist_choice->value(mode);
          redraw();
          break;

        case FL_Page_Up:
          changed = true;
	  cmvedit->setYLevel(y_level-1);
	  break;
        case FL_Page_Down:
          changed = true;
	  cmvedit->setYLevel(y_level+1);
	  break;

        case '1': cmvedit->current_size = 0; break;
        case '2': cmvedit->current_size = 1; break;
        case '3': cmvedit->current_size = 2; break;

        case 'z':
          cen_u = last_iu;
          cen_v = last_iv;
          break;

        default:
          ret = 0;
      }
      break;
  }

  bret = Fl_Box::handle(event);
  return((ret != 0)? ret : bret);
}
