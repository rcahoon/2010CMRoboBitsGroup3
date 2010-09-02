// (C) 2002-2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#include "Vision/SegmentedImage/RGB.h"

// Define the logging constants
#define COMPONENT VISION
//#define CLASS_LOG_LEVEL LOG_LEVEL_WARN
#include "Log/LogSettings.h"

#include "shared/crc32.h"

#include "vision.h"
#include <iostream>
#include <assert.h>

#define ODD(x) ((x) % 2)

#define CLASSIFY(pos) \
        if ( pos < IMAGESIZE ) \
        { \
            p = buf[(pos) / 2]; \
            m = ( p.u << 8) + p.v; \
            if ( ODD((pos)) ) \
            { \
                cmap[ (pos) ] = tmap[m + ((p.y1 >> 4) << 16)]; \
                /*cmap[ (pos) - 1 ] = tmap[m + ((p.y2 >> 4) << 16)];*/ \
                cmap[ (pos)-1 ] = cmap[ (pos) ]; \
            }\
            else \
            { \
                cmap[ (pos) ] = tmap[m + ((p.y1 >> 4) << 16)]; \
                /*cmap[ (pos) + 1] = tmap[m + ((p.y2 >> 4) << 16)];*/ \
                cmap[(pos)+1] = cmap[(pos)]; \
            } \
        } \

namespace RoboCup2010 {

rgb YuvToRgb(yuv p)
{
  rgb r;
  int y,u,v;

  y = p.y;
  u = p.v*2 - 255;
  v = p.u*2 - 255;

  r.red   = bound(y + u                     ,0,255);
  r.green = bound((int)(y - 0.51*u - 0.19*v),0,255);
  r.blue  = bound(y + v                     ,0,255);

  return(r);
}


LowVision::LowVision(ConfigFile & configFile, Log & _log)
  : log(_log),
    buf(NULL),
    cmap(NULL), tmap(NULL),
    rmap(NULL),
    reg(NULL),
    segmentedImage(configFile),
    segmentedColors(NULL) {
}

LowVision::~LowVision() {
  if (cmap != NULL) {
    delete [] cmap;
    cmap = NULL;
  }
  if (tmap != NULL) {
    delete [] tmap;
    tmap = NULL;
  }
  if (rmap != NULL) {
    delete [] rmap;
    rmap = NULL;
  }
  if (reg != NULL) {
    delete [] reg;
    reg = NULL;
  }
  for (int i = 0; i < NumColors; i++) {
    if (color[i].name != NULL) {
      free(color[i].name);
      color[i].name = NULL;
    }
  }
  if (segmentedColors != NULL) {
    delete [] segmentedColors;
    segmentedColors = NULL;
  }
}

bool LowVision::init(const char *colorfile,const char *tmapfile,
                     int _width,int _height)
{
  int num_y,num_u,num_v;
  int size;
  bool ok = true;

  max_width  = width  = _width;
  max_height = height = _height;

  // Load color information
  num_colors = CMVision::LoadColorInformation(color,NumColors,colorfile);
  if(num_colors > 0){
    LOG_INFO("Loaded %d colors.", num_colors)
  }else{
    LOG_ERROR("Could not load color information \"%s\".", colorfile)
    ok = false;
  }

  // Save color information to colorRgb
  memset(colorRgb, 0, NumColors * sizeof(rgb));
  for (int i = 0; i < num_colors; i++) {
    colorRgb[i] = color[i].color;
  }

  // Set up the colors for segmented image
  setupSegmentedImage();

  // Set up threshold
  size = 1 << (bits_y + bits_u + bits_v);
  num_y = 1 << bits_y;
  num_u = 1 << bits_u;
  num_v = 1 << bits_v;

  tmap = CMVision::NewThresholdMap<cmap_t>(num_y,num_u,num_v);
  // tmap = new cmap_t[size];
  // memset(tmap,0,size*sizeof(cmap_t));

  // CMVision::SaveThresholdFile(tmap,num_y,num_u,num_v,tmapfile);

  if(CMVision::LoadThresholdFile(tmap,num_y,num_u,num_v,tmapfile)){
    LOG_INFO("Loaded thresholds.")
  }else{
    LOG_ERROR("Could not load thresholds \"%s\".", tmapfile)
    ok = false;
  }
  CMVision::CheckTMapColors(tmap,num_y,num_u,num_v,num_colors,0);

  // Allocate map structures
  max_width  = width;
  max_height = height;
  size = width * height;
  max_runs = size / MIN_EXP_RUN_LENGTH;
  max_regions = size / MIN_EXP_REGION_SIZE;
  size = max_width * max_height;
  cmap = new cmap_t[size+1]; // extra needed for EncodeRuns terminator
  rmap = new Run[max_runs];
  reg  = new Region[max_regions];

  mzero(cmap,size+1);
  mzero(rmap,max_runs);
  mzero(reg,max_regions);

  frame = -1;
  field = 0;


  return(ok);
}

bool LowVision::close()
{
  delete[](tmap);
  delete[](cmap);
  delete[](rmap);
  delete[](reg);

  tmap = NULL;
  cmap = NULL;
  rmap = NULL;
  reg  = NULL;

  max_width  = 0;
  max_height = 0;

  return(true);
}

unsigned checksum(char *buf,int len)
{
  unsigned u;
  int i;

  u = 0;
  for(i=0; i<len; i++){
    u = ((u << 5) | (u >> (32-5))) ^ buf[i];
  }

  return(u);
}


bool LowVision::processFrame(vision_image &img)
{
    int max_area;
    //unsigned long t1,t2,t3,t4,t5;

    if ( img.buf == NULL )
    {
        return false;
    }
    
     
    //t1 = log.getTimestamp();

    //memcpy(rawImage, (unsigned char*)img.buf, 153600);
    //bcopy((unsigned char*)img.buf,rawImage, 153600);

    //std::cout << "internal image copy took " << t2 - t1 << " milliseconds" << std::endl;
    
    //buf    = (pixel*)rawImage;
    
    //t2 = log.getTimestamp();
    
    buf    = img.buf;
    width  = img.width;
    height = img.height;
    field  = img.field;
    frame++;
    //perform color thresholding

  
    //do run length encoding


    pixel p;
    int m;
    cmap_t mp,save;
    int x,y,j,l;
    Run r;

    r.next = 0;
    //int pos = 0;

    //memset(cmap,0,IMAGESIZE+1);
    // initialize terminator restore

//    pthread_mutex_lock(&mutex);

    CLASSIFY(0)


    save = cmap[0];

    j = 0;
    int index = 0;
    for(y=0; y<IMAGEHEIGHT; y++)
    {
        index = y * IMAGEWIDTH;

        // restore previous terminator and store next
        // one in the first pixel on the next row
        
        cmap[ index ] = save;

        CLASSIFY( index + IMAGEWIDTH )
        save = cmap[ index + IMAGEWIDTH ];
        
        cmap[index + IMAGEWIDTH] = 255;
        r.y = y;

        x = 0;
        while(x < IMAGEWIDTH)
        {
            CLASSIFY( index + x )
            mp = cmap[index + x];
            r.x = x;

            l = x;
            #define RUNSTEP 2 
            while(x < IMAGEWIDTH && cmap[index + x] == mp)
            {
                //instead of going 1 by 1, we go 2 by 2 and when the run ends, we go one back and check if its color is different. 
                //
                x+=RUNSTEP; 
                CLASSIFY( index + x )
                
                //memset(&cmap[index + x - RUNSTEP + 1],mp, RUNSTEP-1); //needed when RUNSTEP > 2
            }
                        
            //CLASSIFY( index + x - 1 )
            if ( cmap[index + x - 1] != mp )
            {
                x--;
            }
            

            /*
            //needed when RUNSTEP > 2
            if ( x < IMAGEWIDTH )
            {
                int backend = x - RUNSTEP + 1;
                if ( backend < IMAGEWIDTH )
                {
                    int backstart = x-1;
                    if ( backstart >= IMAGEWIDTH )
                    {
                        backstart = IMAGEWIDTH-1;
                    }
                    for ( int kkk = backstart; backend > x-(RUNSTEP-1); kkk-- )
                    {
                        CLASSIFY( index + kkk )
                        if ( cmap[index + kkk] != mp )
                        {
                            x = kkk;
                        }
                    }
                }
            }
            */
            
            if (((mp >= 1) && (mp <= 3)) || (x>=IMAGEWIDTH))
            {
	            r.color = mp;
	            r.width = x - l;
	            r.parent = j;
	            rmap[j++] = r;

//#ifdef LOGGING
//        LOG_DEBUG("run (%d,%d):%d %d\n",r.x,r.y,r.width,r.color)
//#endif

	            if(j >= max_runs)
                {
                    cmap[index + IMAGEWIDTH] = save;
                    num_runs = j;
                }
            }
        }
    }

    num_runs = j;

    if(num_runs == max_runs)
    {
        LOG_WARN("Exceeded maximum number of runs in CMVision::EncodeRuns.")
    }

    //t3 = log.getTimestamp();
    //std::cout << "RLE took " << t3 - t2 << " milliseconds" << std::endl;
    //connected components
    CMVision::ConnectComponents(rmap,num_runs);

    //t4 = log.getTimestamp();
    //std::cout << "connected components took " << t4 - t3 << " milliseconds" << std::endl;

    //region extraction
    num_regions = CMVision::ExtractRegions(reg,max_regions,rmap,num_runs);

    for(int i=0; i<num_colors; i++)
    {
        color[i].list = NULL;
        color[i].num  = 0;
    }

    if(num_regions == max_regions)
    {
        LOG_WARN("Exceeded maximum number of regions in CMVision::ExtractRegions.")
        return(false);
    }

    LOG_DEBUG("runs:%6d (%6d) regions:%6d (%6d)\n",
            num_runs, max_runs,
            num_regions, max_regions)

    //Separate regions
    max_area = CMVision::SeparateRegions(color,num_colors,reg,num_regions);

    //Sort regions
    CMVision::SortRegions(color,num_colors,max_area);
    
    //t5 = log.getTimestamp();
    //std::cout << "region building took " << t5 - t4 << " milliseconds" << std::endl;
    // CMVision::CreateRunIndex(yindex,rmap,num_runs);
    return(true);
}

int WritePPM(const rgb *img,int width,int height,
             const char *filename)
{
  FILE *out;
  int wrote;

  out = fopen(filename,"wb");
  if(!out) return(0);

  fprintf(out,"P6\n%d %d\n%d\n",width,height,255);
  wrote = fwrite(img,sizeof(rgb),width*height,out);
  fclose(out);

  return(wrote);
}

bool LowVision::saveThresholdImage(const char *filename)
{
  rgb *buf;
  int wrote;

  buf = new rgb[width * height];
  if(!buf) return(false);

  IndexToRgb(buf,cmap,width,height,color,num_colors);
  wrote = WritePPM(buf,width,height,filename);
  delete [] buf;

  return(wrote > 0);
}

bool LowVision::saveColorizedImage(const char *filename,rgb *reg_color)
{
  rgb *buf;
  int wrote;

  buf = new rgb[width * height];
  if(!buf) return(false);
  mzero(buf,width*height);

  RegionToRgbImage(buf,width,height,rmap,num_runs,reg_color);
  wrote = WritePPM(buf,width,height,filename);
  delete[](buf);

  return(wrote > 0);
}

yuv LowVision::getImagePixel(int x,int y)
{
#ifdef YUVPIXEL
  pixel p2 = buf[(y*width + x)];
  yuv p;
  p.y = p2.y;
#else
  pixel p2 = buf[(y*width + x) / 2];
  yuv p;
  p.y = (x&1)? p2.y2 : p2.y1;
#endif
  p.u = p2.u;
  p.v = p2.v;
  return(p);
}

Region *LowVision::findRegion(int x,int y)
{
  int r = FindRun(rmap,0,num_runs-1,x,y);
  return((r >= 0)? &reg[rmap[r].parent] : NULL);
}

cmap_t LowVision::getTMap(int yi,int ui,int vi)
{
  if(yi>=0 && yi<tmap_y_size &&
     ui>=0 && ui<tmap_u_size &&
     vi>=0 && vi<tmap_v_size){
    return(tmap[(yi << (bits_u+bits_v)) + (ui << bits_v) + vi]);
  }else{
    return(0);
  }
}

bool LowVision::setTMap(int yi,int ui,int vi,cmap_t val)
{
  int l;
  if(yi>=0 && yi<tmap_y_size &&
     ui>=0 && ui<tmap_u_size &&
     vi>=0 && vi<tmap_v_size){
    l = (yi << (bits_u+bits_v)) + (ui << bits_v) + vi;
    if((tmap[l]|128) == (val|128)) return(false);
    tmap[l] = val;
    return(true);
  }else{
    return(false);
  }
}

int LowVision::setTMap(int yi,int ui,int vi,cmap_t val,int uv_radius)
{
  int n = 0;
  for(int u=ui-uv_radius; u<ui+uv_radius+1; u++){
    for(int v=vi-uv_radius; v<vi+uv_radius+1; v++){
      n += setTMap(yi,u,v,val);
    }
  }
  return(n);
}

bool LowVision::clearDirtyTMap(int yi,int ui,int vi)
{
  int l;
  if(yi>=0 && yi<tmap_y_size &&
     ui>=0 && ui<tmap_u_size &&
     vi>=0 && vi<tmap_v_size){
    l = (yi << (bits_u+bits_v)) + (ui << bits_v) + vi;
    if((tmap[l] & 128) == 0) return(false);
    tmap[l] &= ~128;
    return(true);
  }else{
    return(false);
  }
}

bool LowVision::saveTMap(const char *tmapfile) const
{
  return(CMVision::SaveThresholdFile(
         tmap,tmap_y_size,tmap_u_size,tmap_v_size,tmapfile));
}

int LowVision::countCMapPixels(int x,int y,int w,int h,
                               cmap_t c,int &total_num)
{
  // fix bounding area to be on image
  if(x < 0){
    w += x;
    x = 0;
  }

  if(y < 0){
    h += y;
    y = 0;
  }

  if(x+w >= width ) w = width -x;
  if(y+h >= height) h = height-y;

  // count up matching color pixels
  int num = 0;

  for(int i=0; i<h; i++){
    for(int j=0; j<w; j++){
      num += (getClassPixel(x+j,y+i) == c);
    }
  }

  total_num = w*h;
  return(num);
}

int LowVision::countTMapVoxels(int yi0,int yi1,
                               int ui0,int ui1,
                               int vi0,int vi1,
                               cmap_t c,int &total_num)
{
  // fix bounding area to be on valid tmap
  if(yi0 < 0) yi0 = 0;
  if(ui0 < 0) ui0 = 0;
  if(vi0 < 0) vi0 = 0;

  if(yi1 >= tmap_y_size) yi1 = tmap_y_size - 1;
  if(ui1 >= tmap_u_size) ui1 = tmap_u_size - 1;
  if(vi1 >= tmap_v_size) vi1 = tmap_v_size - 1;

  // count up matching entries
  int num = 0;

  for(int y=yi0; y<=yi1; y++){
    for(int u=ui0; u<=ui1; u++){
      for(int v=vi0; v<=vi1; v++){
        num += (getTMapFast(y,u,v) == c);
      }
    }
  }

  total_num =
    max(yi1-yi0+1,0) *
    max(ui1-ui0+1,0) *
    max(vi1-vi0+1,0);

  return(num);
}

void LowVision::expandTMap(int x,int y,cmap_t c)
{
  static const int N = 1;
  static const int M = 2;

  bool match = (getClassPixel(x,y) == c);

  if(!match){
    int cmap_total = 0;
    int cmap_num = countCMapPixels(x-N,y-N,2*N+1,2*N+1,c,cmap_total);

    yuv p = getImagePixel(x,y);
    int yi = p.y >> (8-bits_y);
    int ui = p.u >> (8-bits_u);
    int vi = p.v >> (8-bits_v);

    // drop out early if already set (since last update)
    if(getTMap(yi,ui,vi) == c) return;

    int tmap_total = 0;
    int tmap_num = countTMapVoxels(yi,yi,
                                   ui-M,ui+M,
                                   vi-M,vi+M,
                                   c,tmap_total);

    float cf = (float)(cmap_num-match) / (cmap_total-1);
    float tf = (float)(tmap_num-match) / (tmap_total-1);

    if(cf>0.6 && tf > 0.25){
      //bool b = setTMap(yi,ui,vi,c);
      LOG_DEBUG("(%d %d) [%d %d %d] -> %d (%d)",
                x,y,yi,ui,vi,
                c,b)
    }

    if(false){
      LOG_DEBUG("(%d %d) %d/%d %d/%d",
                x,y, cmap_num,cmap_total, tmap_num,tmap_total)
      LOG_DEBUG("cf=%f tf=%f",
                cf,tf)
    }
  }
}

void LowVision::expandTMap(cmap_t c)
{
  for(int y=1; y<height-1; y++){
    for(int x=1; x<width-1; x++){
      expandTMap(x,y,c);
    }
  }
}

uint32_t LowVision::getStateHash()
{
  uint32_t hv=0;
  int sz = width * height;
  hv = CalcCRC32(hv,buf ,sz/2*sizeof(pixel));
  hv = CalcCRC32(hv,cmap,sz*sizeof(cmap_t));
  hv = CalcCRC32(hv,rmap,num_runs*sizeof(Run));
  // hv = CalcCRC32(hv,reg,num_regions*sizeof(Region));
  return(hv);
}

void LowVision::dump()
{
  LOG_INFO("image: %dx%d field=%d",width,height,field)
  LOG_INFO("runs: %d/%d (%1.2f%%)",
	         num_runs,max_runs,100.0*num_runs/max_runs)
  LOG_INFO("regions: %d/%d (%1.2f%%)",
	         num_regions,max_regions,100.0*num_regions/max_regions)
}

void LowVision::setupSegmentedImage() {
  segmentedColors = new RGB[num_colors];
  for (int i = 0; i < num_colors; i++) {
    segmentedColors[i].set(colorRgb[i].red, colorRgb[i].green, colorRgb[i].blue);
  }

  segmentedImage.setRGB(segmentedColors, num_colors);
}

const SegmentedImage & LowVision::getSegmentedImage() {
  segmentedImage.setIndices((char const *)cmap);
  return segmentedImage;
}

//====================================================================//
//    Region Filter Class Implementation
//====================================================================//

void RegionFilter::init(const Region *reg_list)
{
  reg = reg_list;
  // skip too-large regions in sorted region list
  while(reg!=NULL && reg->area>area.max) reg = reg->next;
}

const Region *RegionFilter::getNext()
{
  // terminate when no regions, or no suitably large ones
  if(!reg) return(NULL);

  // find the next region matching our ranges
  while(reg){
    w = reg->x2 - reg->x1 + 1;
    h = reg->y2 - reg->y1 + 1;

    if(reg->area < area.min) return(NULL);
    if(width.check(w) && height.check(h)){
      const Region *match = reg;
      reg = reg->next;
      return(match);
    }

    reg = reg->next;
  }

  return(NULL);
}

bool RegionFilter::operator()(const Region &reg) const
{
  int w = reg.x2 - reg.x1 + 1;
  int h = reg.y2 - reg.y1 + 1;

  return(area.check(reg.area) && width.check(w) && height.check(h));
}

//====================================================================//
//    Confidence Mask Class Implementation
//====================================================================//

void ConfMask::clear(float _def)
// set all queries to the following value
{
  reset();
  mask = &def;
  def = (int)(255.0 * _def);
  w = h = 1;
  vw = vh = 1;

  // make sure all query coordinates get zeroed
  xs = ys = 30;
}

void ConfMask::reset()
{
  if(mask != &def) delete[](mask);
  mask = NULL;
}

bool ConfMask::load(const char *filename,int _vw,int _vh)
// load an image mask
{
  // int fmt;
  reset();

  // load image and allocate
  vw = _vw;
  vh = _vh;
  mask = NULL; // ReadGrayscale(w,h,filename);
  if(!mask) goto error;

  // set up shift values
  xs = 0; while((vw>>xs) > w) xs++;
  ys = 0; while((vh>>ys) > h) ys++;

  if(vw%(1 << xs)!=0 || vh%(1 << ys)!=0){
//    LOG_ERROR("vw/h and vh/h must be powers of 2")
//    LOG_ERROR("vw=%d vh=%d w=%d h=%d (xs=%d ys=%d)", vw, vh, w, h, xs, ys)
  }

  // delete(img);
  return(true);

error:
  delete(mask); mask=NULL;
  return(false);
}

float ConfMask::get(float x,float y)
{
  int ix,iy;
  uchar val;

  ix = ((int)x) >> xs;
  iy = ((int)y) >> ys;
  val = mask[iy*w + ix];

//  LOG_DEBUG("%d %d %d", ix, iy, val)

  return(val / 255.0);
}

} // end namespace
