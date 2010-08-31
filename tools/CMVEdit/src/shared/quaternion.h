// (C) 2005 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "gvector.h"
#include "random.h"


template <class num>
class Quaternion{
public:
  num s,x,y,z;
public:
  Quaternion()
    {}
  Quaternion(const GVector::vector3d<num> &axis,num angle)
    {set(axis,angle);}

  void set(const GVector::vector3d<num> &axis,num angle);
  void random(Random &rng);

  void zero()
    {s=1; x=0; y=0; z=0;}
  num length() const
    {return(sqrt(s*s + x*x + y*y + z*z));}
  Quaternion norm() const;

  Quaternion operator*(num scalar) const;
  Quaternion operator*(const Quaternion &q) const;

  void toMat3x3(num *mat) const;
};

template <class num>
void Quaternion<num>::set(const GVector::vector3d<num> &axis,num angle)
{
  double cv,sv;
  sincos(angle*0.5,&sv,&cv);

  s = cv;
  x = axis.x * sv;
  y = axis.y * sv;
  z = axis.z * sv;
}

template <class num>
void Quaternion<num>::random(Random &rng)
{
  num l;

  do{
    x = rng.real32();
    y = rng.real32();
    z = rng.real32();
    l = x*x + y*y + z*z;
  }while(l > 1.0);

  num a  = M_PI * rng.real32();
  num sf = sin(a) / sqrt(l);

  s = cos(a);
  x *= sf;
  y *= sf;
  z *= sf;
}

template <class num>
Quaternion<num> Quaternion<num>::norm() const
{
  Quaternion q;
  double f = 1.0 / length();
  q.s = s * f;
  q.x = x * f;
  q.y = y * f;
  q.z = z * f;
  return(q);
}

template <class num>
Quaternion<num> Quaternion<num>::operator*(num scalar) const
{
  Quaternion r;
  r.s = s * scalar;
  r.x = x * scalar;
  r.y = y * scalar;
  r.z = z * scalar;
  return(r);
}

template <class num>
static inline Quaternion<num> operator*(num scalar,const Quaternion<num> &q)
{
  Quaternion<num> r;
  r.s = q.s * scalar;
  r.x = q.x * scalar;
  r.y = q.y * scalar;
  r.z = q.z * scalar;
  return(r);
}

template <class num>
Quaternion<num> Quaternion<num>::operator*(const Quaternion &q) const
{
  Quaternion r;
  r.s = s*q.s - (x*q.x + y*q.y + z*q.z);
  r.x = (s*q.x + q.s*x) + (y*q.z - z*q.y);
  r.y = (s*q.y + q.s*y) + (z*q.x - x*q.z);
  r.z = (s*q.z + q.s*z) + (x*q.y - y*q.x);
  return(r);
}

template <class num>
void Quaternion<num>::toMat3x3(num *mat) const
{
  mat[3*0 + 0] = 1 - 2*y*y - 2*z*z;
  mat[3*0 + 1] =     2*x*y - 2*z*s;
  mat[3*0 + 2] =     2*x*z + 2*y*s;

  mat[3*1 + 0] =     2*x*y + 2*z*s;
  mat[3*1 + 1] = 1 - 2*x*x - 2*z*z;
  mat[3*1 + 2] =     2*y*z - 2*x*s;

  mat[3*2 + 0] =     2*x*z - 2*y*s;
  mat[3*2 + 1] =     2*y*z + 2*x*s;
  mat[3*2 + 2] = 1 - 2*x*x - 2*y*y;
}

#endif
