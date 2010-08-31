// (C) 2002-2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#ifndef __VISION_H__
#define __VISION_H__

#include "cmvision.h"
#include "geometry.h"
#include "image.h"

// Edited by Som
#ifdef YUVPIXEL
  typedef yuv pixel;
#else
  #ifdef USE_METEOR
    typedef uyvy pixel;
  #else
    typedef yuyv pixel;
  #endif
#endif

typedef uchar cmap_t;
typedef CMVision::image<pixel> vision_image;
typedef CMVision::color_class_state color_class_state;
typedef CMVision::run<cmap_t> Run;

// Edited by Som
#ifdef YUVPIXEL
  const int bits_y = 4;
  const int bits_u = 6;
  const int bits_v = 6;
#else
  #if 1
    const int bits_y = 4;
    const int bits_u = 8;
    const int bits_v = 8;
  #else
    const int bits_y = 4;
    const int bits_u = 6;
    const int bits_v = 6;
  #endif
#endif

int WritePPM(const rgb *img,int width,int height,
             const char *filename);

#define MIN_EXP_REGION_SIZE  32
#define MIN_EXP_RUN_LENGTH    6

// Must match indicies in colors.txt!!
enum Colors{
  ColorBackground = 0,
  ColorYellow,
  ColorBrightBlue,
  ColorOrange,
  ColorPink,
  ColorWhite,
  ColorGreen,
  ColorRed,
  NumColors
};

rgb YuvToRgb(yuv p);

class LowVision{
public:
  pixel *buf;
  cmap_t *cmap,*tmap;
  Run *rmap;
  CMVision::region *reg;

  color_class_state color[NumColors];

  int width,height;
  int max_width,max_height;
  int max_runs,max_regions;
  int num_colors,num_runs,num_regions;
  int frame,field;

public:
  bool init(const char *colorfile,const char *tmapfile,
            int _width,int _height);
  bool close();

  bool processFrame(vision_image &img);
  bool saveThresholdImage(const char *filename);
  bool saveColorizedImage(const char *filename,rgb *reg_color);
  bool getThresholdImage(SImage &img);

  const CMVision::region *getRegions(int c)
    {return(color[c].list);}
  yuv getAverageColor(const CMVision::region *reg)
    {return(CMVision::AverageColor(buf,width,height,rmap,reg->run_start));}
  int getNumRegions(int c)
    {return(color[c].num);}
  int getRegionID(const CMVision::region *r)
    {return(r - reg);}
  CMVision::region *findRegion(int x,int y);
  int addToHistogram(int x1,int y1,int x2,int y2,int *hist_count)
    {return(CMVision::AddToHistogram(cmap,width,height,
				     x1,y1,x2,y2,hist_count));}

  cmap_t getClassPixel(int x,int y)
    {return(cmap[y*width+x]);}
  yuv getImagePixel(int x,int y);
  int getField()
    {return(field);}

  // Threshold Map Editing Functions
  static const int tmap_y_size = 1<<bits_y;
  static const int tmap_u_size = 1<<bits_u;
  static const int tmap_v_size = 1<<bits_v;
  cmap_t getTMap(int yi,int ui,int vi);
  cmap_t getTMap(yuv c);
  cmap_t getTMapFast(int yi,int ui,int vi)
    {return(tmap[(yi << (bits_u+bits_v)) + (ui << bits_v) + vi]);}
  bool setTMap(int yi,int ui,int vi,cmap_t val);
  int  setTMap(int yi,int ui,int vi,cmap_t val,int uv_radius);
  bool clearDirtyTMap(int yi,int ui,int vi);
  bool loadTMap(const char *tmapfile) const;
  bool saveTMap(const char *tmapfile) const;
  yuv getAverageColor(CMVision::region *reg,pixel *buf)
    {return(CMVision::AverageColor(buf,width,height,rmap,reg->run_start));}

  int countCMapPixels(int x,int y,int w,int h,cmap_t c,int &total_num);
  int countTMapVoxels(int yi0,int yi1,
                      int ui0,int ui1,
                      int vi0,int vi1,
                      cmap_t c,int &total_num);

  void expandTMap(int x,int y,cmap_t c);
  void expandTMap(cmap_t c);

  bool makeRgbImage(vision_image &src,SImage &dest);
  // Debugging
  uint32_t getStateHash();
  void dump();
};

//==== Some Simple Vision Processors =================================//

/*class RangeInt{
public:
  int min;
  int max;

public:
  void set(int _min,int _max)
    {min=_min; max=_max;}
  bool check(int v) const
    {return(v>=min && v<max);}
};*/

class RegionFilter{
public:
  const CMVision::region *reg;
  int w,h;
public:
  RangeInt area;
  RangeInt width;
  RangeInt height;
public:
  RegionFilter() {reg=NULL;}
  void init(const CMVision::region *reg_list);
  const CMVision::region *getNext();

  bool operator()(const CMVision::region &reg) const;
};

class ConfMask{
  uchar *mask; // mask data
  uchar def;   // default value
  uchar xs,ys; // x-shift (log2[vw/w]), y-shift (log2[vh/h])
  int w,h;     // mask pixel dimensions
  int vw,vh;   // vision image resolution
public:
  ConfMask()
    {mask=NULL; reset();}
  ~ConfMask()
    {reset();}

  // set all queries to the following value
  void clear(float _def);

  // clear memory usage
  void reset();

  // load an image mask
  bool load(const char *file,int _vw,int _vh);

  int getMaskWidth()  {return(w);}
  int getMaskHeight() {return(h);}

  // get the confidence for a vision image location
  float get(float x,float y);
};

#endif
