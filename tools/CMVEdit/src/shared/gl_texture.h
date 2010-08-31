// (C) 2004-2007 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#ifndef _INCLUDED_GL_TEXTURE_H_
#define _INCLUDED_GL_TEXTURE_H_

#include "colors.h"


enum GlTextureFlags{
  TexNoRepeat = 1<<0,
  TexMipMap   = 1<<1,
  TexNearest  = 1<<2,
};

class GlTexture{
public:
  unsigned id;
private:
  void setup(unsigned flags);
public:
  GlTexture()
    {id=0;}
  ~GlTexture()
    {destroy();}
  bool valid()
    {return(id != 0);}

  bool set(const uchar *data,int width,int height,int depth,int format,
           unsigned flags);
  bool update(const uchar *data,int width,int height,int format);

  bool set(const rgb *data,int width,int height,unsigned flags)
    {return(set((const uchar*)data,width,height,3,GL_RGB,flags));}
  bool update(const rgb *data,int width,int height)
    {return(update((const uchar*)data,width,height,GL_RGB));}

  bool load(const char *filename,unsigned flags);
  void buildSolidColor(uchar red,uchar green,uchar blue);

  void destroy();
  void select(int texture_unit) const;
  void select() const;
};

#endif
