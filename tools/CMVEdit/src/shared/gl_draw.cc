// (C) 2004-2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#include "image_io.h"

#include "gl_draw.h"

#if !COLOR_MATERIAL_WORKS
void glColor(rgb c)
{
  float color[4];
  color[0] = c.red   / 255.0;
  color[1] = c.green / 255.0;
  color[2] = c.blue  / 255.0;
  color[3] = 1.0;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT , color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
}
#endif

void GlDrawSquare(vector3f cen,vector2f rad)
{
  glNormal3f(0,0,1);

  glBegin(GL_QUADS);
  glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z);
  glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z);
  glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z);
  glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z);

  glEnd();
}

void GlDrawWireSquare(vector3f cen,vector2f rad)
{
  glNormal3f(0,0,1);

  glBegin(GL_LINE_LOOP);
  glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z);
  glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z);
  glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z);
  glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z);
  glEnd();
}

void GlDrawBox(vector3f cen,vector3f rad)
{
  glBegin(GL_QUADS);

  // sides
  glNormal3f(-1, 0, 0);
  glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z-rad.z);
  glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z-rad.z);
  glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z+rad.z);
  glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z+rad.z);

  glNormal3f( 0,-1, 0);
  glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z-rad.z);
  glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z-rad.z);
  glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z+rad.z);
  glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z+rad.z);

  glNormal3f( 1, 0, 0);
  glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z-rad.z);
  glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z-rad.z);
  glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z+rad.z);
  glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z+rad.z);

  glNormal3f( 0, 1, 0);
  glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z-rad.z);
  glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z-rad.z);
  glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z+rad.z);
  glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z+rad.z);

  // top and bottom
  glNormal3f( 0, 0, 1);
  glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z+rad.z);
  glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z+rad.z);
  glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z+rad.z);
  glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z+rad.z);

  glNormal3f( 0, 0, -1);
  glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z-rad.z);
  glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z-rad.z);
  glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z-rad.z);
  glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z-rad.z);

  glEnd();
}

void GlDrawBoxTex(vector3f cen,vector3f rad)
{
  glBegin(GL_QUADS);
  double s = 0.001;

  // sides
  glNormal3f(-1, 0, 0);
  glTexCoord2f(s*(cen.y+rad.y), s*(cen.z-rad.z)); glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.y-rad.y), s*(cen.z-rad.z)); glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.y-rad.y), s*(cen.z+rad.z)); glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z+rad.z);
  glTexCoord2f(s*(cen.y+rad.y), s*(cen.z+rad.z)); glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z+rad.z);

  glNormal3f( 0,-1, 0);
  glTexCoord2f(s*(cen.x-rad.x), s*(cen.z-rad.z)); glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.x+rad.x), s*(cen.z-rad.z)); glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.x+rad.x), s*(cen.z+rad.z)); glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z+rad.z);
  glTexCoord2f(s*(cen.x-rad.x), s*(cen.z+rad.z)); glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z+rad.z);

  glNormal3f( 1, 0, 0);
  glTexCoord2f(s*(cen.y-rad.y), s*(cen.z-rad.z)); glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.y+rad.y), s*(cen.z-rad.z)); glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.y+rad.y), s*(cen.z+rad.z)); glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z+rad.z);
  glTexCoord2f(s*(cen.y-rad.y), s*(cen.z+rad.z)); glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z+rad.z);

  glNormal3f( 0, 1, 0);
  glTexCoord2f(s*(cen.x+rad.x), s*(cen.z-rad.z)); glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.x-rad.x), s*(cen.z-rad.z)); glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.x-rad.x), s*(cen.z+rad.z)); glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z+rad.z);
  glTexCoord2f(s*(cen.x+rad.x), s*(cen.z+rad.z)); glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z+rad.z);

  // top and bottom
  glNormal3f( 0, 0, 1);
  glTexCoord2f(s*(cen.x-rad.x), s*(cen.y-rad.y)); glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z+rad.z);
  glTexCoord2f(s*(cen.x+rad.x), s*(cen.y-rad.y)); glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z+rad.z);
  glTexCoord2f(s*(cen.x+rad.x), s*(cen.y+rad.y)); glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z+rad.z);
  glTexCoord2f(s*(cen.x-rad.x), s*(cen.y+rad.y)); glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z+rad.z);

  glNormal3f( 0, 0, -1);
  glTexCoord2f(s*(cen.x-rad.x), s*(cen.y-rad.y)); glVertex3f(cen.x-rad.x, cen.y-rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.x-rad.x), s*(cen.y+rad.y)); glVertex3f(cen.x-rad.x, cen.y+rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.x+rad.x), s*(cen.y+rad.y)); glVertex3f(cen.x+rad.x, cen.y+rad.y, cen.z-rad.z);
  glTexCoord2f(s*(cen.x+rad.x), s*(cen.y-rad.y)); glVertex3f(cen.x+rad.x, cen.y-rad.y, cen.z-rad.z);

  glEnd();
}

void GlDrawWireBox(vector3f cen,vector3f rad)
{
  vector3f pts[8];
  static const double s = sqrt(1.0 / 3.0);

  pts[0].set(cen.x - rad.x, cen.y - rad.y, cen.z - rad.z);
  pts[1].set(cen.x + rad.x, cen.y - rad.y, cen.z - rad.z);
  pts[2].set(cen.x + rad.x, cen.y + rad.y, cen.z - rad.z);
  pts[3].set(cen.x - rad.x, cen.y + rad.y, cen.z - rad.z);

  pts[4].set(cen.x - rad.x, cen.y - rad.y, cen.z + rad.z);
  pts[5].set(cen.x + rad.x, cen.y - rad.y, cen.z + rad.z);
  pts[6].set(cen.x + rad.x, cen.y + rad.y, cen.z + rad.z);
  pts[7].set(cen.x - rad.x, cen.y + rad.y, cen.z + rad.z);

  glBegin(GL_LINE_STRIP);
    glNormal3f(-s,-s,-s); glVertex3f(pts[0]);
    glNormal3f( s,-s,-s); glVertex3f(pts[1]);
    glNormal3f( s, s,-s); glVertex3f(pts[2]);
    glNormal3f(-s, s,-s); glVertex3f(pts[3]);
    glNormal3f(-s,-s,-s); glVertex3f(pts[0]);

    glNormal3f(-s,-s, s); glVertex3f(pts[4]);
    glNormal3f( s,-s, s); glVertex3f(pts[5]);
    glNormal3f( s, s, s); glVertex3f(pts[6]);
    glNormal3f(-s, s, s); glVertex3f(pts[7]);
    glNormal3f(-s,-s, s); glVertex3f(pts[4]);
  glEnd();

  glBegin(GL_LINES);
    glNormal3f( s,-s,-s); glVertex3f(pts[1]);
    glNormal3f( s,-s, s); glVertex3f(pts[5]);

    glNormal3f( s, s,-s); glVertex3f(pts[2]);
    glNormal3f( s, s, s); glVertex3f(pts[6]);

    glNormal3f(-s, s,-s); glVertex3f(pts[3]);
    glNormal3f(-s, s, s); glVertex3f(pts[7]);
  glEnd();
}

void GlDrawTrapezoid(vector3f cen,vector3f bot,vector3f top)
{
  glBegin(GL_QUADS);

  // FIXME: normals are wrong

  // sides
  glNormal3f(-1, 0, 0);
  glVertex3f(cen.x-bot.x, cen.y+bot.y, cen.z-bot.z);
  glVertex3f(cen.x-bot.x, cen.y-bot.y, cen.z-bot.z);
  glVertex3f(cen.x-top.x, cen.y-top.y, cen.z+top.z);
  glVertex3f(cen.x-top.x, cen.y+top.y, cen.z+top.z);

  glNormal3f( 0,-1, 0);
  glVertex3f(cen.x-bot.x, cen.y-bot.y, cen.z-bot.z);
  glVertex3f(cen.x+bot.x, cen.y-bot.y, cen.z-bot.z);
  glVertex3f(cen.x+top.x, cen.y-top.y, cen.z+top.z);
  glVertex3f(cen.x-top.x, cen.y-top.y, cen.z+top.z);

  glNormal3f( 1, 0, 0);
  glVertex3f(cen.x+bot.x, cen.y-bot.y, cen.z-bot.z);
  glVertex3f(cen.x+bot.x, cen.y+bot.y, cen.z-bot.z);
  glVertex3f(cen.x+top.x, cen.y+top.y, cen.z+top.z);
  glVertex3f(cen.x+top.x, cen.y-top.y, cen.z+top.z);

  glNormal3f( 0, 1, 0);
  glVertex3f(cen.x+bot.x, cen.y+bot.y, cen.z-bot.z);
  glVertex3f(cen.x-bot.x, cen.y+bot.y, cen.z-bot.z);
  glVertex3f(cen.x-top.x, cen.y+top.y, cen.z+top.z);
  glVertex3f(cen.x+top.x, cen.y+top.y, cen.z+top.z);

  // top and bottom
  glNormal3f( 0, 0, 1);
  glVertex3f(cen.x-top.x, cen.y-top.y, cen.z+top.z);
  glVertex3f(cen.x+top.x, cen.y-top.y, cen.z+top.z);
  glVertex3f(cen.x+top.x, cen.y+top.y, cen.z+top.z);
  glVertex3f(cen.x-top.x, cen.y+top.y, cen.z+top.z);

  glNormal3f( 0, 0, -1);
  glVertex3f(cen.x-bot.x, cen.y-bot.y, cen.z-bot.z);
  glVertex3f(cen.x-bot.x, cen.y+bot.y, cen.z-bot.z);
  glVertex3f(cen.x+bot.x, cen.y+bot.y, cen.z-bot.z);
  glVertex3f(cen.x+bot.x, cen.y-bot.y, cen.z-bot.z);

  glEnd();
}

void GlSetup()
{
  if(false){
    // enable backface culling.
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
  }

  // enable other options
  if(false){
    glEnable(GL_DEPTH_TEST);
  }else{
    glDisable(GL_DEPTH_TEST);
  }

  // material
  float front_mat_ambient[]   = {1.0, 1.0, 1.0, 1.0};
  float front_mat_diffuse[]   = {1.0, 1.0, 1.0, 1.0};
  float front_mat_specular[]  = {0.0, 0.0, 0.0, 1.0};
  float front_mat_shininess[] = {1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT  , front_mat_ambient  );
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , front_mat_diffuse  );
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR , front_mat_specular );
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, front_mat_shininess);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  if(COLOR_MATERIAL_WORKS) glEnable(GL_COLOR_MATERIAL);

  float lmodel_twoside[] = {GL_TRUE};
  glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE,lmodel_twoside);

  glLineWidth(2);
}

void GlDrawTriangle(const vector3d &a,
                    const vector3d &b,
                    const vector3d &c)
{
  vector3d n = (b-a).cross(c-a).norm();
  glNormal3dv((double*)&n);
  glVertex3dv((double*)&a);
  glVertex3dv((double*)&b);
  glVertex3dv((double*)&c);
}

void GlDrawWireBox(const vector2f &cen,const vector2f &rad)
{
  glBegin(GL_LINE_LOOP);
  glVertex2f(cen.x-rad.x, cen.y-rad.y);
  glVertex2f(cen.x+rad.x, cen.y-rad.y);
  glVertex2f(cen.x+rad.x, cen.y+rad.y);
  glVertex2f(cen.x-rad.x, cen.y+rad.y);
  glEnd();
}

static void FlipImage(rgb *img,int width,int height)
{
  int i,j;

  for(i=0; i<height/2; i++){
    for(j=0; j<width; j++){
      swap(img[i*width + j],img[(height-i-1)*width + j]);
    }
  }
}

bool GlSaveScreenShot(const char *filename,int quality)
{
  int dim[4];
  glGetIntegerv(GL_VIEWPORT,dim);
  int x      = dim[0];
  int y      = dim[1];
  int width  = dim[2];
  int height = dim[3];

  rgb *buf = new rgb[width*height];
  if(!buf) return(false);

  glReadPixels(x,y,width,height,GL_RGB,GL_UNSIGNED_BYTE,buf);
  FlipImage(buf,width,height);
  bool ok = WriteRGB(buf,width,height,filename,quality);
  delete[](buf);

  return(ok);
}
