/*
 * CMVEdit.h
 *
 *  Created on: Apr 2, 2009
 *      Author: junyun
 */

#ifndef CMVEDIT_H_
#define CMVEDIT_H_

#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Window.H>
#include "shared/vision_constants.h"

#include "shared/fl_helper.h"
#include "shared/image.h"
#include "shared/vision.h"

#include "NaoImageCapture.h"

#include "shared/camera_view.h"
#include "shared/tmap_view.h"

class CMVEdit : public Fl_Window {
public:
  LowVision vision;
  // Nao Image Capture helper
  NaoImageCapture* naoImageCapture;
//  const Capture::Image *cap_img;

  cmap_t *tmap_used;
  ImageArea active_area;

  cmap_t current_color;
  int current_size;
  int process_frame_step;
  int auto_update_ms;

  pixel *yuv_img;
  SImage video_img;
  SImage cmap_img;
  SImage diff_img;

public:
  CameraView *camera_view;

  // Fl_Window *cw;
  // ZoomBox *b;
  // CrossScroll *sc;
  Fl_Shared_Image *img;

  Fl_Group *tmap_win;
  TMapBox *tm_view;
  Fl_Scrollbar *tm_scroll;
  Fl_Box *mouse_color_box;
  Fl_Box *mouse_class_box;

  Fl_Choice *color_choice;
  Fl_Choice *mode_choice;
  Fl_Choice *color_hist_choice;
  Fl_Slider *image_alpha_slider;

  InfoBox *tmap_loc;
  InfoBox *frame_cnt;
  InfoBox *region_info;
  vision_image *prev_img;
public:
  CMVEdit(int x,int y,int w,int h);
protected:
  bool joyInit(int joy_idx);
  void setColorChoices();
public:
  bool init(int video_device_id,int input_idx,
            const char *config_dir,const char *log_file,
            int joy_idx, std::string naoIP);
  void close();

  void setCurrentColor(int color_id);
  bool setYLevel(int yi);
  void processFrame(int step);
  void processFrameNao(int step);
  void processFrame(){
    processFrameNao(process_frame_step);
  }
  void handleJoystick();
  void toggleAutoUpdate(int ms);
  void loadTMap();
  void saveTMap();
  void loadFile(const char *name);

  void clearTMapUsed();
  void markTMapUsed(const vision_image &cmv_img,
                    const ImageArea &area);

  void updateColorBoxes(int ix,int iy);
  void updateColorBoxes(yuv c);
  std::string naoIP;
  static const int IMAGE_WIDTH;
  static const int IMAGE_HEIGHT;
  static const int IMAGE_PITCH;
};
#endif /* CMVEDIT_H_ */
