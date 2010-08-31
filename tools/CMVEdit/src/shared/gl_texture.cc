// (C) 2004-2007 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <FL/Fl_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>


#include "gl_texture.h"


static const bool Debug = false;


void GlTexture::setup(unsigned flags)
{
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  // Set texture filtering
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if(flags & TexMipMap){
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
  }else{
    int f = (flags & TexNearest)? GL_NEAREST : GL_LINEAR;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);
  }

  // Repeating texture
  int r = (flags & TexNoRepeat)? GL_CLAMP : GL_REPEAT;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, r);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, r);
}

bool GlTexture::set(const uchar *data,
                    int width,int height,int depth,int format,
                    unsigned flags)
{
  if(Debug){
    printf("set: %d w=%d h=%d d=%d f=%d\n",
           !data,width,height,depth,format);
  }
  if(!data || width<=0 || height<=0 || depth<=0 || format<=0) return(false);

  GLint max_tex_dim;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_dim);
  if(Debug) printf("max=%d\n",max_tex_dim);
  if(width>max_tex_dim || height>max_tex_dim) return(false);

  destroy();
  glGenTextures(1,&id);

  if(Debug) printf("id=%d\n",id);

  glBindTexture(GL_TEXTURE_2D,id);

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  if(flags & TexMipMap){
    gluBuild2DMipmaps(GL_TEXTURE_2D, depth, width, height,
                      format, GL_UNSIGNED_BYTE, data);
  }else{
    glTexImage2D(GL_TEXTURE_2D, 0, depth, width, height, 0,
                 format, GL_UNSIGNED_BYTE, data);
  }

  setup(flags);

  return(true);
}

bool GlTexture::update(const uchar *data,int width,int height,int format)
{
  glBindTexture(GL_TEXTURE_2D,id);
  glTexSubImage2D(GL_TEXTURE_2D,0,
                  0,0,width,height,
                  format, GL_UNSIGNED_BYTE, data);
  return(true);
}

bool GlTexture::load(const char *filename,unsigned flags)
{
  // load image
  Fl_RGB_Image *img = NULL;
  if(strstr(filename,".jpg")){
    img = new Fl_JPEG_Image(filename);
  }else if(strstr(filename,".png")){
    img = new Fl_PNG_Image(filename);
  }
  if(!img) return(false);

  // determine pixel format
  int format = 0;
  switch(img->d()){
    case 3: format = GL_RGB;  break;
    case 4: format = GL_RGBA; break;
  }

  if(format != 0){
    // try to set texture
    const uchar *data = (const uchar*) *(img->data());
    if(Debug) printf("tex %s (id=%d)\n",filename,id);
    if(!set(data, img->w(), img->h(), img->d(), format, flags)){
      format = -1;
    }
  }

  delete(img);
  return(format > 0);
}

void GlTexture::buildSolidColor(uchar red,uchar green,uchar blue)
{
  rgb buf[4];

  for(int i=0; i<4; i++){
    buf[i].red   = red;
    buf[i].green = green;
    buf[i].blue  = blue;
  }

  set(buf,2,2,0);
}

void GlTexture::destroy()
{
  if(id){
    glDeleteTextures(1,&id);
    id = 0;
  }
}

void GlTexture::select(int texture_unit) const
{
  if(id){
    glActiveTexture((GLenum)texture_unit);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,id);
  }
}

void GlTexture::select() const
{
  if(id){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,id);
  }
}
