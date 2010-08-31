#include "gl_draw.h"

#include "../CMVEdit.h"

#include "camera_view.h"


CameraView::CameraView(int _x,int _y,int _w,int _h) :
  ZoomWindow(_x,_y,_w,_h,NULL,2)
{
  cmvedit=0;
  setCMapAlpha(0.25);
  mzero(m_push);
  mzero(m_cur);
  end();

  if(false){
    CalibPoint p;
    for(int i=0; i<11; i++){
      p.world.set(i*100,0,0);
      p.screen.set(i*10,0);
      pts.push_back(p);
    }
  }
  drag_idx = -1;

  logfile_count = 0;
}

void CameraView::setCMapAlpha(float alpha)
{
  for(int i=0; i<num_layers; i++) blend[i] = 0.0;
  blend[0] = 1.0;
  blend[1] = alpha;
  if(cmvedit){
    cmvedit->image_alpha_slider->value(1-alpha);
  }
  redraw();
}

int CameraView::pickPoint(const vector2f &loc)
{
  int idx = -1;
  double md = sq(10.0);

  for(unsigned i=0; i<pts.size(); i++){
    double d = sqdist(loc,pts[i].screen);
    if(d < md){
      idx = i;
      md = d;
    }
  }

  return(idx);
}

void CameraView::draw()
{
  ZoomWindow::draw();

  if(false){
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0,1.0,1.0,0.5);

    glBegin(GL_LINES);
    glVertex3f( 0, m_cur.sy, 0);
    glVertex3f(sw, m_cur.sy, 0);
    glVertex3f(m_cur.sx,  0, 0);
    glVertex3f(m_cur.sx, sh, 0);
    glEnd();
  }

  if(pts.size() != 0){
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0,1.0,0.0,0.75);

    glBegin(GL_LINE_STRIP);
    for(unsigned i=0; i<pts.size(); i++){
      glVertex(pts[i].screen);
    }
    glEnd();

    vector2f r1(0.5,0.25);
    vector2f r2 = r1*5;

    for(unsigned i=0; i<pts.size(); i++){
      vector3f c(pts[i].screen.x,pts[i].screen.y,0);
      GlDrawWireSquare(c,r1);
      GlDrawWireSquare(c,r2);
    }
  }
}

int CameraView::handle(int event)
{
  int ret = 0;

  switch(event){
    case FL_ENTER:
      Fl::focus(this);
      break;

    case FL_FOCUS:
    case FL_UNFOCUS:
      ret = 1;
      break;

    case FL_MOVE:
      m_cur = getMouseLoc();
      cmvedit->updateColorBoxes((int)m_cur.sx,(int)m_cur.sy);
      break;

    case FL_PUSH:
    case FL_DRAG: {
      if(event == FL_PUSH){
        m_last = m_push = m_cur = getMouseLoc();
        drag_idx = pickPoint(vector2f(m_cur.sx,m_cur.sy));
      }else{
        m_last = m_cur;
        m_cur = getMouseLoc();
      }

      if(isInsideImage(m_cur)){
        cmvedit->updateColorBoxes(m_cur.ix,m_cur.iy);

        yuv c = cmvedit->vision.getImagePixel(m_cur.ix,m_cur.iy);
        int tm_y = c.y >> (8-bits_y);
        int tm_u = c.u >> (8-bits_u);
        int tm_v = c.v >> (8-bits_v);
        cmap_t cl = cmvedit->vision.getClassPixel(m_cur.ix,m_cur.iy);

        switch(Fl::event_buttons()){
          case FL_BUTTON1:
            if(drag_idx >= 0){
              pts[drag_idx].screen.set(m_cur.sx,m_cur.sy);
              redraw();
            }else if(cmvedit->vision.setTMap(tm_y,tm_u,tm_v,
                                             cmvedit->current_color,
                                             cmvedit->current_size)){
              if(!cmvedit->setYLevel(tm_y)){
                cmvedit->tm_view->redraw();
              }
            }
            break;

          case FL_BUTTON2:
            dragView(m_push,m_cur);
            redraw();
            break;

          case FL_BUTTON3:
            cmvedit->setYLevel(tm_y);
            cmvedit->setCurrentColor(cl);
            break;
        }
      }

      ret = 1;
    } break;

    case FL_RELEASE:
      drag_idx = -1;
      break;

    case FL_MOUSEWHEEL: {
      int d = calcProportionalZoomDelta(1.1,-Fl::event_dy());
      // if(zoom(d)) redraw();
      if(zoomCentered(d,m_cur.wx,m_cur.wy)) redraw();
      ret = 1;
    } break;

    case FL_KEYDOWN:
      ret = 1;
      switch(Fl::event_key()){
        // TODO: move these navigation keys somewhere generic
        case 'a':
          cmvedit->toggleAutoUpdate(16);
	  break;

        case ' ':
	  cmvedit->processFrame();
	  break;
        case '>':
        case '.':
	  cmvedit->processFrame(1);
	  break;
        case '<':
        case ',':
	  cmvedit->processFrame(-1);
	  break;

        case 'u':
	  cmvedit->processFrame(0);
	  break;

        case 'c':
          setCMapAlpha(1.0);
	  redraw();
	  // cmvedit->mode_choice->value(cmvedit->b->mode);
	  break;
        case 'v':
          setCMapAlpha(0.0);
	  redraw();
	  // cmvedit->mode_choice->value(cmvedit->b->mode);
	  break;
        case 'z':
          zoomFull();
          redraw();
          break;

        /*
        case 'o':
        case 'p':
	  printf("Saving [cmap%03d.ppm] [img%03d.ppm]\n",
		 saved_images,saved_images);
	  sprintf(buf,"cmap%03d.ppm",saved_images);
	  cmvedit->b->cmap_img.save(buf);
	  sprintf(buf,"img%03d.ppm",saved_images);
	  cmvedit->b->video_img.save(buf);
	  saved_images++;
	  break;
        */
        case 'l': {
          CharString logfile;
          logfile.printf("logs/cmvedit%02d.vraw",logfile_count);
	  printf("Saving %s [",logfile()); fflush(stdout);
	  /*NOT SAVING TO LOG EDITED BY JUNYUN*/
//          cmvedit->cap.saveLog(logfile(),120,1);
          printf("]\n");
          logfile_count++;
        } break;

        /*
        case 'e':{
	  cmvedit->vision.expandTMap(cmvedit->current_color);

          int ofs = cmvedit->process_frame_step;
          cmvedit->process_frame_step = 0;
	  cmvedit->processFrame();
          cmvedit->process_frame_step = ofs;
	} break;
        */

        default:
          ret = 0;
      }
      break;
  }

  return(ret? ret : ZoomWindow::handle(event));
}
