#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Tabs.H>
#include <FL/fl_draw.H>

#include <string.h>
#include <errno.h>
#include <getopt.h>

#include "shared/fl_helper.h"
#include "shared/sstring.h"


#include "NaoImageCapture.h"
#include "shared/colors.h"
#include "shared/cmv_types.h"
#include "ConfigFile.h"



#include "shared/yuv2rgb.h"
#include "shared/zoom_win.h"

#include "CMVEdit.h"

const int CMVEdit::IMAGE_WIDTH(ConfigFile::getSingleton().getInt("vision/imageWidth"));
const int CMVEdit::IMAGE_HEIGHT(ConfigFile::getSingleton().getInt("vision/imageHeight"));
const int CMVEdit::IMAGE_PITCH(ConfigFile::getSingleton().getInt("vision/imagePitch"));
//====================================================================//

CharString tmap_file;
CMVEdit *cmvedit;

//====================================================================//

void CMVEdit::setCurrentColor(int color_id)
{
  if(color_id!=current_color &&
     color_id>=0 && color_id<vision.num_colors){
    color_choice->value(color_id);
    current_color = color_id;
  }
}

bool CMVEdit::setYLevel(int yi)
{
  if(yi!=tm_view->y_level && yi>=0 && yi<vision.tmap_y_size){
    tm_view->y_level = yi;
    tm_view->redraw();
    tm_scroll->scrollvalue(yi,1,0,vision.tmap_y_size);
    return(true);
  }else{
    return(false);
  }
}

void color_cb(Fl_Widget *w,int color_id)
{
  cmvedit->setCurrentColor(color_id);
}

void CMVEdit::toggleAutoUpdate(int ms)
{
  if(auto_update_ms){
    auto_update_ms = 0;
  }else{
    auto_update_ms = ms;
    processFrame();
  }
}

void ProcessFrame()
{
  cmvedit->processFrame();
}

void CMVEdit::processFrame(int step)
{
}

void CMVEdit::processFrameNao(int step)
{
  if(step) process_frame_step = step;

    vision_image *cmv_img = new vision_image();

    char const *rawValues = naoImageCapture->getImage();
    cmv_img->buf = (pixel *)rawValues;
    cmv_img->width = IMAGE_WIDTH;
    cmv_img->height = IMAGE_HEIGHT;
    cmv_img->pitch = IMAGE_PITCH;
    cmv_img->field = 0;


    int pixels = cmv_img->width * cmv_img->height;
    int size   = cmv_img->pitch * cmv_img->height;


    // copy out frame
    if(yuv_img == NULL){
      yuv_img = new pixel[pixels];
    }

    if(yuv_img){
      memcpy(yuv_img,cmv_img->buf,size);
    }

    camera_view->getViewableArea(active_area);

    markTMapUsed(*cmv_img,active_area);
    // run low level vision
    if (!vision.processFrame(*cmv_img)) {
      printf("CMVision failed to process frame.\n");
      return;
    }
    if (!vision.getThresholdImage(cmap_img)) {
      printf("CMVision failed to return the threshold image.\n");
      return;
    }
    if (!vision.makeRgbImage(*cmv_img,video_img)) {
      printf("CMVision failed to make the rgb image.\n");
      return;
    }

    if(camera_view){
      camera_view->update(0,video_img);
      camera_view->update(1,cmap_img);
      camera_view->redraw();
    }

  handleJoystick();

  // camera_view->redraw();
  // b->redraw();
  if(tm_view->visible() && tm_view->isHistogramVisible()){
    tm_view->redraw();
  }

  updateColorBoxes(camera_view->m_cur.ix,
                   camera_view->m_cur.iy);

  if(auto_update_ms){
    Fl::add_timeout(auto_update_ms/1000.0,
        (Fl_Timeout_Handler)ProcessFrame,NULL);
  }

    if(prev_img)
      delete prev_img;
    prev_img = cmv_img;

}

void CMVEdit::handleJoystick()
{
}

void CMVEdit::loadTMap()
{
  printf("loading..."); fflush(stdout);
  if(vision.loadTMap(tmap_file())){
    tm_view->clearUndo();
    printf("ok\n");
  }else{
    printf("FAILED!\n");
  }
}

void CMVEdit::saveTMap()
{
  printf("saving..."); fflush(stdout);
  if(vision.saveTMap(tmap_file())){
    printf("ok\n");
  }else{
    printf("FAILED!\n");
  }
}

//====================================================================//

void ProcessFrameCB(Fl_Widget *w,void *p)
{
  ProcessFrame();
}

void ClearUsedCB(Fl_Widget *w,void *p)
{
  if(cmvedit){
    cmvedit->clearTMapUsed();
    cmvedit->tm_view->redraw();
  }
}

void SaveTMapCB(Fl_Widget *w,void *p)
{
  cmvedit->saveTMap();
}

void RevertTMapCB(Fl_Widget *w,void *p)
{
  cmvedit->loadTMap();
  cmvedit->processFrameNao(0);
}

void UndoTMapCB(Fl_Widget *w,void *p)
{
  cmvedit->tm_view->undo();
}

void RedoTMapCB(Fl_Widget *w,void *p)
{
  cmvedit->tm_view->redo();
}

void ImageAlphaCB(Fl_Widget *w,void *p)
{
  float a = cmvedit->image_alpha_slider->value();
  cmvedit->camera_view->setCMapAlpha(1-a);
}

//====================================================================//

void tmap_scroll_cb(Fl_Widget *w,TMapBox *b)
{
  int v;
  v = (int)(((Fl_Valuator*)w)->value());
  b->y_level = v;
  b->redraw();
}

void TMapGridCB(Fl_Widget *w,TMapBox::ViewMode view_mode)
{
  cmvedit->tm_view->mode = view_mode;
  cmvedit->tm_view->redraw();
}

Fl_Menu_Item tmap_view_choices[] = {
  {"Class"     , 0, (Fl_Callback*)TMapGridCB, (void*)TMapBox::ModeTMap},
  {"Class+Hist", 0, (Fl_Callback*)TMapGridCB, (void*)TMapBox::ModeTMapHist},
  {"Histogram" , 0, (Fl_Callback*)TMapGridCB, (void*)TMapBox::ModeHistOnly},
  {0}
};

//====================================================================//

void CMVEdit::loadFile(const char *name)
{
}

static const char *options_str = "v:i:l:j:h";
static struct option options[] = {
  // name arg flag val
  {"video"   , 1, 0, 'v'},
  {"input"   , 1, 0, 'i'},
  {"log"     , 1, 0, 'l'},
  {"joystick", 1, 0, 'j'},
  {"help"    , 0, 0, 'h'},
  {0, 0, 0, 0}
};

CMVEdit::CMVEdit(int x,int y,int w,int h) :
  Fl_Window(x,y,w,h,"CMVEdit")
{
  //initialize prev_img
  prev_img = NULL;
  naoIP = "";
//  cap_img = 0x00;
  yuv_img = 0x00;
  // Camera view area
  camera_view = new CameraView(0,0,w-(512+32),h);
  camera_view->setSourceSize(IMAGE_WIDTH,IMAGE_HEIGHT);
  camera_view->setZoom(1,1,2);
  camera_view->cmvedit = this;

  // Tabbed area
  int tmap_zoom = 2;
  int tmap_l = tmap_zoom * 256;
  int tab_w = tmap_l+16+16;
  int tab_h = 24;

  Fl_Tabs *tabs = new Fl_Tabs(w-tab_w,0,tab_w,h);

  // Threshold view tab
  int xo = w - tab_w + 8;
  int yo = tab_h + 16;

  tmap_win = new Fl_Group(xo,tab_h,tab_w,h-tab_h,"Classification");

  static const int bw = tmap_l/6;
  static const int bh = 32;
  int bx = xo;
  Fl_Button *revert_button = new Fl_Button(bx,yo,bw,bh,"Revert");     bx+=bw;
  Fl_Button *save_button   = new Fl_Button(bx,yo,bw,bh,"Save");       bx+=bw;
  bx += bw/2;
  Fl_Button *undo_button   = new Fl_Button(bx,yo,bw,bh,"Undo");       bx+=bw;
  Fl_Button *redo_button   = new Fl_Button(bx,yo,bw,bh,"Redo");       bx+=bw;
  bx += bw/2;
  Fl_Button *clear_button  = new Fl_Button(bx,yo,bw,bh,"Clear Hist"); bx+=bw;

  save_button->callback(SaveTMapCB);
  revert_button->callback(RevertTMapCB);
  undo_button->callback(UndoTMapCB);
  redo_button->callback(RedoTMapCB);
  clear_button->callback(ClearUsedCB);

  yo += bh+1;

  tm_view = new TMapBox(xo,yo,tmap_l,tmap_l);
  tm_view->box(FL_NO_BOX);
  tm_view->cmvedit = this;

  tm_scroll = new Fl_Scrollbar(xo+tmap_l,yo,16,tmap_l,"");
  tm_scroll->step(1);
  tm_scroll->scrollvalue(0,1,0,vision.tmap_y_size);
  tm_scroll->callback((Fl_Callback*)tmap_scroll_cb,tm_view);
  yo += tmap_l;

  color_choice = new Fl_Choice(xo+208,yo+16,200,24,"Color:");

  color_hist_choice = new Fl_Choice(xo+272,yo+80,136,24,"TMap view:");
  color_hist_choice->menu(tmap_view_choices);
  color_hist_choice->value(1);

  image_alpha_slider = new Fl_Slider(xo+272,yo+112,128, 24,"Image alpha");
  image_alpha_slider->type(3);
  image_alpha_slider->align(FL_ALIGN_LEFT);
  image_alpha_slider->callback((Fl_Callback*)ImageAlphaCB);

  tmap_loc = new InfoBox(xo+8,yo+16,136,24,"");

  frame_cnt = new InfoBox(xo+432,yo+16,80,24,"F 100/100");

  region_info = new InfoBox(xo+8,yo+144,144,112,"Region");
  region_info->align(FL_ALIGN_LEFT | FL_ALIGN_TOP | FL_ALIGN_INSIDE);

  mouse_color_box = new Fl_Box(FL_DOWN_BOX,xo+ 8,yo+48,64,64,"");
  mouse_class_box = new Fl_Box(FL_DOWN_BOX,xo+80,yo+48,64,64,"");

  tmap_win->end();
  tabs->add(tmap_win);

  // end tabs and make camera area resizable
  tabs->end();
  resizable(camera_view);
  end();
}


bool CMVEdit::joyInit(int joy_idx)
{
  return(false);
}

bool CMVEdit::init(int video_device_id,int input_idx,
                   const char *config_dir,const char *log_file,
                   int joy_idx, std::string naoIP)
{
  CharString video_device;
  video_device.printf("/dev/video%d",video_device_id);
  tmap_file.printf("%s/thresh%d.tmap.gz",config_dir,video_device_id);

  // initialize vision
  CharString colors_file;
  colors_file.printf("%s/colors.txt",config_dir);
  if(!vision.init(colors_file(),tmap_file(),IMAGE_WIDTH,IMAGE_HEIGHT)){
    printf("Vision init failed.\n");
    return(false);
  }

  try
  {
    std::cout<<naoIP<<std::endl;
    naoImageCapture = new NaoImageCapture(naoIP,9559);
  }catch(AL::ALError &alerror)
  {
    std::cout<<"Error in opening camera: "<<alerror.toString()<<std::endl;
    return false;
  }

  tmap_used = NULL;

  current_color = 0;
  current_size  = 1;
  process_frame_step = 1;
  auto_update_ms = 0;

  video_img.allocate(IMAGE_WIDTH,IMAGE_HEIGHT);
  cmap_img.copy_geometry(video_img);
  diff_img.copy_geometry(video_img);

  setColorChoices();

  if(joy_idx >= 0){
    joyInit(joy_idx);
  }

  return(true);
}

void CMVEdit::setColorChoices()
{
  Fl_Menu_Item *color_choices = new Fl_Menu_Item[vision.num_colors+1];
  Fl_Menu_Item item;

  mzero(item);
  color_choices[vision.num_colors] = item; // list terminator
  item.callback_ = (Fl_Callback*)color_cb;

  for(int i=0; i<vision.num_colors; i++){
    item.text = vision.color[i].name;
    item.user_data_ = (void*)i;
    color_choices[i] = item;
  }

  color_choice->menu(color_choices);
  color_choice->value(0);
}

void CMVEdit::close()
{
  vision.close();

  delete[](tmap_used);
  delete[](yuv_img);
  tmap_used = NULL;
  yuv_img = NULL;
}

void CMVEdit::clearTMapUsed()
{
  if(tmap_used){
    int sz = 1 << (bits_y + bits_u + bits_v);
    memset(tmap_used,0,sz);
  }
}

void CMVEdit::markTMapUsed(const vision_image &cmv_img,
                           const ImageArea &area)
{
  if(false){
    printf("%d %d %d %d\n",
           area.x,area.y,
           area.w,area.h);
  }

  if(tmap_used == NULL){
    int sz = 1 << (bits_y + bits_u + bits_v);
    tmap_used = new cmap_t[sz];
    memset(tmap_used,0,sz);
  }

//  if(tmap_used){
//#ifdef YUVPIXEL
//    CMVision::MarkTMapUsedYUV<pixel,cmap_t,vision_image,bits_y,bits_u,bits_v>
//      (tmap_used, cmv_img, 1,
//       area.x, area.y, area.w, area.h);
//#else
//      CMVision::MarkTMapUsed<pixel,cmap_t,vision_image,bits_y,bits_u,bits_v>
//        (tmap_used, cmv_img, 1,
//         area.x, area.y, area.w, area.h);
//#endif
//  }
}

void CMVEdit::updateColorBoxes(int ix,int iy)
{
  if(iy<0 || iy>=cmvedit->vision.height ||
     ix<0 || ix>=cmvedit->vision.width) return;

  yuv c = vision.getImagePixel(ix,iy);
  rgb rc = YuvToRgb(c);
  rgb rcb = rc;

  if(mouse_color_box){
    mouse_color_box->color(fl_rgb_color(rc.red,rc.green,rc.blue));
    mouse_color_box->redraw();
  }

  cmap_t color_class = vision.getClassPixel(ix,iy);
  rc = vision.color[color_class].color;

  if(mouse_class_box){
    mouse_class_box->color(fl_rgb_color(rc.red,rc.green,rc.blue));
    mouse_class_box->redraw();
  }

  if(region_info){
    region_info->str.printf("loc=<%3d %3d>\nyuv=(%3d %3d %3d)\nrgb=(%3d %3d %3d)",
                            ix,iy,
                            c.y,c.u,c.v,
                            rcb.red,rcb.green,rcb.blue);

    if(!cmvedit->auto_update_ms){
      CMVision::region *r = cmvedit->vision.findRegion(ix,iy);
      if(r){
        CharString reg_str(128);
        c = cmvedit->vision.getAverageColor(r,cmvedit->yuv_img);
        reg_str.printf("avg=(%3u %3u %3u)\ncen=<%0.1f %0.1f>\ndim=%dx%d\narea=%d\n",
                       c.y,c.u,c.v,
                       r->cen_x,r->cen_y,
                       r->x2-r->x1+1,
                       r->y2-r->y1+1,
                       r->area);
        region_info->str += reg_str;
      }
    }
    region_info->update();
  }
}

void CMVEdit::updateColorBoxes(yuv c)
{
  rgb rc = YuvToRgb(c);

  if(mouse_color_box){
    mouse_color_box->color(fl_rgb_color(rc.red,rc.green,rc.blue));
    mouse_color_box->redraw();
  }

  cmap_t color_class = vision.getTMap(c);
  rc = vision.color[color_class].color;

  if(mouse_class_box){
    mouse_class_box->color(fl_rgb_color(rc.red,rc.green,rc.blue));
    mouse_class_box->redraw();
  }
}

//====================================================================//

// int dvisual = 0;
int arg(int, char **argv, int &i)
{
  // if (argv[i][1] == '8') {dvisual = 1; i++; return 1;}
  return 0;
}

int main(int argc, char **argv)
{
  if(argc < 2){
	std::cout<<"Usage: ./cmvedit <NAO_IP>"<<std::endl;
	return -1;
  }


  int i = 1;

  const char *log_file = NULL;
  int video_device_id = VIDEO_DEVICE_ID;
  int input_idx = IMAGE_CAPTURE_INPUT;
  int joy_idx = -1;

  int c,opt_idx;
  while((c = getopt_long(argc, argv,options_str,options,&opt_idx)) >= 0){
    switch(c){
      case 'v':
        video_device_id = atoi(optarg);
        break;

      case 'i':
        input_idx = atoi(optarg);
        break;

      case 'l':
        log_file = optarg;
        break;

      case 'j':
        joy_idx = atoi(optarg);
        break;

      case 'h':
        // Usage(argv[0]);
        break;
    }
  }

  const char *config_dir = "config";

  YuvToRgbInit();
  fl_register_images();

  Fl::args(argc,argv,i,arg);

  // if(!dvisual)
  Fl::visual(FL_RGB);

//  if(false){
//    cmvedit = new CMVEdit(0,0,1280+512+32,960);
//  }else{
    cmvedit = new CMVEdit(0,0,1280,960);
//  }
    cmvedit->naoIP = argv[1];

  int ret;

  if(cmvedit->init(video_device_id,input_idx,
                   config_dir,log_file,joy_idx, cmvedit->naoIP)){
    cmvedit->processFrame();
    cmvedit->show(argc,argv);

    ret = Fl::run();
  }else{
    ret = 1;
  }

  cmvedit->close();
  delete(cmvedit->naoImageCapture);
  delete(cmvedit);
  return(ret);
}
