// (C) 2004-2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#ifndef _INCLUDED_GLDRAW_H_
#define _INCLUDED_GLDRAW_H_

#include <stdio.h>

#include <GL/gl.h>
#include <FL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
// #include <SDL_opengl.h>

#include "geometry.h"
#include "colors.h"

#define COLOR_MATERIAL_WORKS 1

#if COLOR_MATERIAL_WORKS
inline void glColor(rgb c)
  {glColor3ub(c.red,c.green,c.blue);}
inline void glColor(rgb c,unsigned char a)
  {glColor4ub(c.red,c.green,c.blue,a);}
#else
void glColor(rgb c);
#endif

inline void glVertex2f(const vector2f &v) {glVertex2fv((const float* )&v);}
inline void glVertex2d(const vector2d &v) {glVertex2dv((const double*)&v);}

inline void glVertex3f(const vector3f &v) {glVertex3fv((const float* )&v);}
inline void glVertex3d(const vector3d &v) {glVertex3dv((const double*)&v);}

inline void glNormal3f(const vector3f &v) {glNormal3fv((const float* )&v);}
inline void glNormal3d(const vector3d &v) {glNormal3dv((const double*)&v);}

inline void glVertex(const vector2f &v) {glVertex2fv((const float* )&v);}
inline void glVertex(const vector2d &v) {glVertex2dv((const double*)&v);}

inline void glVertex(const vector3f &v) {glVertex3fv((const float* )&v);}
inline void glVertex(const vector3d &v) {glVertex3dv((const double*)&v);}

inline void glNormal(const vector3f &v) {glNormal3fv((const float* )&v);}
inline void glNormal(const vector3d &v) {glNormal3dv((const double*)&v);}

inline void glTexCoord2f(const vector2f &v) {glTexCoord2f(v.x, v.y);}
inline void glTexCoord2d(const vector2d &v) {glTexCoord2d(v.x, v.y);}

void GlSetup();

void GlDrawSquare(vector3f cen,vector2f rad);
void GlDrawWireSquare(vector3f cen,vector2f rad);

void GlDrawBox(vector3f cen,vector3f rad);
void GlDrawBoxTex(vector3f cen,vector3f rad);
void GlDrawWireBox(vector3f cen,vector3f rad);
void GlDrawTrapezoid(vector3f cen,vector3f bot,vector3f top);

void GlDrawTriangle(const vector3d &a,
                    const vector3d &b,
                    const vector3d &c);

void GlDrawWireBox(const vector2f &cen,const vector2f &rad);

bool GlSaveScreenShot(const char *filename,int quality=85);

#endif
