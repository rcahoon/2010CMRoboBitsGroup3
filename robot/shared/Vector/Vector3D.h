/*========================================================================
    This was taken from GVector.h originally, and split into 2 files:
    Vector2D.h & Vector3D.h

    Somchaya Liemhetcharat, 2009
  ========================================================================*/

/*========================================================================
    GVector.h : Simple vector class for 2D and 3D vectors
  ------------------------------------------------------------------------
    Copyright (C) 1999-2006  James R. Bruce
    School of Computer Science, Carnegie Mellon University
  ------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ========================================================================*/

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include <math.h>
#include <boost/serialization/utility.hpp>

#include "shared/num_util.h"

namespace GVector {

//=====================================================================//
//  Vector3D Class
//=====================================================================//

#define EPSILON (1.0E-10)

#define V3COMP(p) p.x,p.y,p.z

template <class num>
class vector3d{
public:
  num x,y,z;

  vector3d()
    {}
  vector3d(num nx,num ny,num nz)
    {x=nx; y=ny; z=nz;}

  void set(num nx,num ny,num nz)
    {x=nx; y=ny; z=nz;}
  void setAll(num nx)
    {x=y=z=nx;}
  void set(vector3d<num> p)
    {x=p.x; y=p.y; z=p.z;}
  void zero()
    {x=y=z=0;}

  vector3d<num> &operator=(const vector3d<num> p)
    {set(p); return(*this);}

  /// element accessor
  num &operator[](int idx)
    {return(((num*)this)[idx]);}
  const num &operator[](int idx) const
    {return(((num*)this)[idx]);}

  num length() const ;
  num sqlength() const ;
  vector3d<num> norm() const ;
  vector3d<num> norm(const num len) const ;
  void normalize();
  bool nonzero() const
    {return(x!=0 || y!=0 || z!=0);}

  num dot(const vector3d<num> p) const ;
  vector3d<num> cross(const vector3d<num> p) const ;

  vector3d<num> &operator+=(const vector3d<num> p);
  vector3d<num> &operator-=(const vector3d<num> p);
  vector3d<num> &operator*=(const vector3d<num> p);
  vector3d<num> &operator/=(const vector3d<num> p);

  vector3d<num> operator+(const vector3d<num> p) const;
  vector3d<num> operator-(const vector3d<num> p) const;
  vector3d<num> operator*(const vector3d<num> p) const;
  vector3d<num> operator/(const vector3d<num> p) const;

  vector3d<num> operator*(num f) const;
  vector3d<num> operator/(num f) const;
  vector3d<num> &operator*=(num f);
  vector3d<num> &operator/=(num f);

  vector3d<num> operator-() const;

  bool operator==(const vector3d<num> p) const;
  bool operator!=(const vector3d<num> p) const;
  bool operator< (const vector3d<num> p) const;
  bool operator> (const vector3d<num> p) const;
  bool operator<=(const vector3d<num> p) const;
  bool operator>=(const vector3d<num> p) const;

  vector3d<num> rotate(const vector3d<num> r,const float a) const;
  vector3d<num> rotate_x(const float a) const;
  vector3d<num> rotate_y(const float a) const;
  vector3d<num> rotate_z(const float a) const;

  bool finite() const
    {return(::finite(x) && ::finite(y) && ::finite(z));}

  void take_min(const vector3d<num> p);
  void take_max(const vector3d<num> p);

private:
  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & x;
    ar & y;
    ar & z;
  }
};

template <class num>
num vector3d<num>::length() const
{
  return(sqrt(x*x + y*y + z*z));
}

template <class num>
num vector3d<num>::sqlength() const
{
  return(x*x + y*y + z*z);
}

template <class num>
vector3d<num> vector3d<num>::norm() const
{
  vector3d<num> p;
  num l;

  l = sqrt(x*x + y*y + z*z);
  p.x = x / l;
  p.y = y / l;
  p.z = z / l;

  return(p);
}

template <class num>
vector3d<num> vector3d<num>::norm(const num len) const
{
  vector3d<num> p;
  num f;

  f = len / sqrt(x*x + y*y + z*z);
  p.x = x * f;
  p.y = y * f;
  p.z = z * f;

  return(p);
}

template <class num>
void vector3d<num>::normalize()
{
  num l;

  l = sqrt(x*x + y*y + z*z);
  x /= l;
  y /= l;
  z /= l;
}

template <class num>
num vector3d<num>::dot(const vector3d<num> p) const
{
  return(x*p.x + y*p.y + z*p.z);
}

template <class num>
num dot(const vector3d<num> a,const vector3d<num> b)
{
  return(a.x*b.x + a.y*b.y + a.z*b.z);
}

template <class num>
num absdot(const vector3d<num> a,const vector3d<num> b)
{
  return(fabs(a.x*b.x) + fabs(a.y*b.y) + fabs(a.z*b.z));
}

template <class num>
vector3d<num> vector3d<num>::cross(const vector3d<num> p) const
{
  vector3d<num> r;

  // right handed
  r.x = y*p.z - z*p.y;
  r.y = z*p.x - x*p.z;
  r.z = x*p.y - y*p.x;

  return(r);
}

template <class num>
vector3d<num> cross(const vector3d<num> a,const vector3d<num> b)
{
  vector3d<num> r;

  // right handed
  r.x = a.y*b.z - a.z*b.y;
  r.y = a.z*b.x - a.x*b.z;
  r.z = a.x*b.y - a.y*b.x;

  return(r);
}

#define VECTOR3D_EQUAL_BINARY_OPERATOR(opr) \
  template <class num> \
  vector3d<num> &vector3d<num>::operator opr (const vector3d<num> p) \
  {                  \
    x = x opr p.x;   \
    y = y opr p.y;   \
    z = z opr p.z;   \
    return(*this);   \
  }

VECTOR3D_EQUAL_BINARY_OPERATOR(+=)
VECTOR3D_EQUAL_BINARY_OPERATOR(-=)
VECTOR3D_EQUAL_BINARY_OPERATOR(*=)
VECTOR3D_EQUAL_BINARY_OPERATOR(/=)

#define VECTOR3D_BINARY_OPERATOR(opr) \
  template <class num> \
  vector3d<num> vector3d<num>::operator opr (const vector3d<num> p) const \
  {                  \
    vector3d<num> r; \
    r.x = x opr p.x; \
    r.y = y opr p.y; \
    r.z = z opr p.z; \
    return(r);       \
  }

VECTOR3D_BINARY_OPERATOR(+)
VECTOR3D_BINARY_OPERATOR(-)
VECTOR3D_BINARY_OPERATOR(*)
VECTOR3D_BINARY_OPERATOR(/)

#define VECTOR3D_SCALAR_OPERATOR(opr) \
  template <class num> \
  vector3d<num> vector3d<num>::operator opr (const num f) const \
  {                  \
    vector3d<num> r; \
    r.x = x opr f;   \
    r.y = y opr f;   \
    r.z = z opr f;   \
    return(r);       \
  }

VECTOR3D_SCALAR_OPERATOR(*)
VECTOR3D_SCALAR_OPERATOR(/)

#define VECTOR3D_EQUAL_SCALAR_OPERATOR(opr) \
  template <class num> \
  vector3d<num> &vector3d<num>::operator opr (num f) \
  {                \
    x = x opr f;   \
    y = y opr f;   \
    z = z opr f;   \
    return(*this); \
  }

VECTOR3D_EQUAL_SCALAR_OPERATOR(*=)
VECTOR3D_EQUAL_SCALAR_OPERATOR(/=)

#define VECTOR3D_LOGIC_OPERATOR(opr,combine) \
  template <class num> \
  bool vector3d<num>::operator opr (const vector3d<num> p) const \
  {                            \
    return((x opr p.x) combine \
           (y opr p.y) combine \
           (z opr p.z));       \
  }

VECTOR3D_LOGIC_OPERATOR(==,&&)
VECTOR3D_LOGIC_OPERATOR(!=,||)

VECTOR3D_LOGIC_OPERATOR(< ,&&)
VECTOR3D_LOGIC_OPERATOR(> ,&&)
VECTOR3D_LOGIC_OPERATOR(<=,&&)
VECTOR3D_LOGIC_OPERATOR(>=,&&)

template <class num>
vector3d<num> vector3d<num>::operator-() const
{
  vector3d<num> r;

  r.x = -x;
  r.y = -y;
  r.z = -z;

  return(r);
}

template <class num1,class num2>
vector3d<num2> operator*(num1 f,const vector3d<num2> &a)
{
  vector3d<num2> r;

  r.x = f * a.x;
  r.y = f * a.y;
  r.z = f * a.z;

  return(r);
}

template <class num>
inline vector3d<num> abs(vector3d<num> a)
{
  a.x = ::fabs(a.x);
  a.y = ::fabs(a.y);
  a.z = ::fabs(a.z);

  return(a);
}

template <class num>
inline vector3d<num> min(vector3d<num> a,vector3d<num> b)
{
  vector3d<num> v;

  v.x = ::min(a.x,b.x);
  v.y = ::min(a.y,b.y);
  v.z = ::min(a.z,b.z);

  return(v);
}

template <class num>
inline vector3d<num> max(vector3d<num> a,vector3d<num> b)
{
  vector3d<num> v;

  v.x = ::max(a.x,b.x);
  v.y = ::max(a.y,b.y);
  v.z = ::max(a.z,b.z);

  return(v);
}

template <class num>
inline vector3d<num> bound(vector3d<num> v,num low,num high)
{
  v.x = ::bound(v.x,low,high);
  v.y = ::bound(v.y,low,high);
  v.z = ::bound(v.z,low,high);

  return(v);
}

// returns point rotated around axis <r> by <a> radians (right handed)
template <class num>
vector3d<num> vector3d<num>::rotate(const vector3d<num> r,const float a) const
{
  vector3d<num> q;
  float s,c,t;

  s = sin(a);
  c = cos(a);
  t = 1 - c;

  q.x = (t * r.x * r.x + c      ) * x
      + (t * r.x * r.y - s * r.z) * y
      + (t * r.x * r.z + s * r.y) * z;

  q.y = (t * r.y * r.x + s * r.z) * x
      + (t * r.y * r.y + c      ) * y
      + (t * r.y * r.z - s * r.x) * z;

  q.z = (t * r.z * r.x - s * r.y) * x
      + (t * r.z * r.y + s * r.x) * y
      + (t * r.z * r.z + c      ) * z;

  return(q);
}

// returns point rotated around X axis by <a> radians (right handed)
template <class num>
vector3d<num> vector3d<num>::rotate_x(const float a) const
{
  vector3d<num> q;
  float s,c;

  s = sin(a);
  c = cos(a);

  q.x = x;
  q.y = c*y + -s*z;
  q.z = s*y + c*z;

  return(q);
}

// returns point rotated around Y axis by <a> radians (right handed)
template <class num>
vector3d<num> vector3d<num>::rotate_y(const float a) const
{
  vector3d<num> q;
  float s,c;

  s = sin(a);
  c = cos(a);

  q.x = c*x + s*z;
  q.y = y;
  q.z = -s*x + c*z;

  return(q);
}

// returns point rotated around Z axis by <a> radians (right handed)
template <class num>
vector3d<num> vector3d<num>::rotate_z(const float a) const
{
  vector3d<num> q;
  float s,c;

  s = sin(a);
  c = cos(a);

  q.x = c*x + -s*y;
  q.y = s*x + c*y;
  q.z = z;

  return(q);
}

// set the vector to the minimum of its components and p's components
template <class num>
void vector3d<num>::take_min(const vector3d<num> p)
{
  if(p.x < x) x = p.x;
  if(p.y < y) y = p.y;
  if(p.z < z) z = p.z;
}

// set the vector to the maximum of its components and p's components
template <class num>
void vector3d<num>::take_max(const vector3d<num> p)
{
  if(p.x > x) x = p.x;
  if(p.y > y) y = p.y;
  if(p.z > z) z = p.z;
}

// returns distance between two points
template <class num>
num dist(const vector3d<num> a,const vector3d<num> b)
{
  num dx,dy,dz;

  dx = a.x - b.x;
  dy = a.y - b.y;
  dz = a.z - b.z;

  return(sqrt(dx*dx + dy*dy + dz*dz));
}

template <class num>
inline num distance(const vector3d<num> a,const vector3d<num> b)
{
  return(dist(a,b));
}

// returns square of distance between two points
template <class num>
num sqdist(const vector3d<num> a,const vector3d<num> b)
{
  num dx,dy,dz;

  dx = a.x - b.x;
  dy = a.y - b.y;
  dz = a.z - b.z;

  return(dx*dx + dy*dy + dz*dz);
}

template <class num>
inline num sqdistance(const vector3d<num> a,const vector3d<num> b)
{
  return(sqdist(a,b));
}

// returns distance from point p to line x0-x1
template <class num>
num distance_to_line(const vector3d<num> x0,const vector3d<num> x1,const vector3d<num> p)
{
  // FIXME: this is probably broken
  vector3d<num> x;
  num t;

  t = ((p.x - x0.x) + (p.y - x0.y) + (p.z - x0.z)) / (x1.x + x1.y + x1.z);
  x = x0 + (x1 - x0) * t;

  return(distance(x,p));
}

} // namespace GVector

typedef GVector::vector3d<float> vector3d;
typedef GVector::vector3d<float> Vector3D;

#endif /* VECTOR3D_H_ */
