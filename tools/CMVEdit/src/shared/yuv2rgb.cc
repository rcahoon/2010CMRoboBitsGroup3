/* LICENSE: */

#include "yuv2rgb.h"


rgb YuvToRgb(yuv p)
{
  using namespace YuvToRgbConstants;
  rgb c;

  int y = CY * (p.y - 16) + 32768;
  int u = p.u - 128;
  int v = p.v - 128;

  c.red   = bound((y + CRV*v        ) >> 16,0,255);
  c.green = bound((y + CGU*u + CGV*v) >> 16,0,255);
  c.blue  = bound((y + CBU*u        ) >> 16,0,255);

  return(c);
}
