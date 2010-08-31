// (C) 2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
// or alternately by a specific written agreement.

#ifndef _INCLUDED_YUV_TO_RGB_H_
#define _INCLUDED_YUV_TO_RGB_H_

#include "util.h"
#include "colors.h"

/*
  The "official" conversion from YUV to RGB is given in the MPEG
  ISO/IEC 13818-2 standard, Section 6.3.6 as:
    R = 76309*(Y-16) + crv*(V-128)               + 32768 >> 16
    G = 76309*(Y-16) - cgu*(U-128) - cgv*(V-128) + 32768 >> 16
    B = 76309*(Y-16) + cbu*(U-128)               + 32768 >> 16

  Where the c__ values are given by the following table:
                                    crv     cbu     cgu    cgv
    No sequence display extension  117504  138453  13954  34903
    ITU-R Rec.709(1990)            117504  138453  13954  34903
    Unspecified                    104597  132201  25675  53279
    Reserved                       104597  132201  25675  53279
    FCC                            104448  132798  24759  53109
    ITU-R Rec.624-4 System B, G    104597  132201  25675  53279
    SMPTE 170M                     104597  132201  25675  53279
    SMPTE 240M(1987)               117579  136230  16907  35559

  The entries for {Unspecified, Reserved, ITU-R Rec.624-4, SMPTE 170M}
  are all equal, and most closely follow the other conversion formulas
  I've seen, so those are the constants which are used here.
*/

namespace YuvToRgbConstants{
  static const int CY  =  76309;
  static const int CRV = 104597;
  static const int CBU = 132201;
  static const int CGU = -25675;
  static const int CGV = -53279;
};

static inline void YuvToRgbInit()
{
  // An old version of this code used lookup tables; The function is
  // left here for compatibility.  It should still be called before
  // calling YuvToRgb, as future implementations may start use lookup
  // tables again.
}

template <class pixel_t>
rgb YuvToRgb(pixel_t p)
{
  using namespace YuvToRgbConstants;
  rgb c;
  int y,u,v;

  y = (CY * (p.y1 + p.y2 - 16*2))/2 + 32768;
  // FIXME: swapped u,v
  u = p.v - 128;
  v = p.u - 128;

  c.red   = bound((y + CRV*v        ) >> 16,0,255);
  c.green = bound((y + CGU*u + CGV*v) >> 16,0,255);
  c.blue  = bound((y + CBU*u        ) >> 16,0,255);

  return(c);
}

template <class pixel_t>
void YuvToRgb(rgb *dest,const pixel_t *src,int width,int height)
{
  using namespace YuvToRgbConstants;
  int size = width * height / 2;
  rgb c0,c1;

  for(int i=0; i<size; i++){
    pixel_t p = src[i];
    // FIXME: swapped u,v
    int u = p.u - 128;
    int v = p.v - 128;

    int r = CRV*v;
    int g = CGU*u + CGV*v;
    int b = CBU*u;

    int y0 = CY * (p.y1 - 16) + 32768;
    int y1 = CY * (p.y2 - 16) + 32768;

    c0.red   = bound((y0 + r) >> 16,0,255);
    c0.green = bound((y0 + g) >> 16,0,255);
    c0.blue  = bound((y0 + b) >> 16,0,255);

    c1.red   = bound((y1 + r) >> 16,0,255);
    c1.green = bound((y1 + g) >> 16,0,255);
    c1.blue  = bound((y1 + b) >> 16,0,255);

    dest[2*i+0] = c0;
    dest[2*i+1] = c1;
  }
}

rgb YuvToRgb(yuv p);

#endif
