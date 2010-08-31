#ifndef _INCLUDED_POSE_H_
#define _INCLUDED_POSE_H_

#define POSE_TEM \
  template <class vector,class real>
#define POSE_FUN \
  DynamicPose2D<vector,real>

#include <stdio.h>

//==== Pose Class Definition =========================================//

POSE_TEM
class DynamicPose2D{
public:
  // pose specified in world coordinates
  vector loc;
  vector vel;
  real angle;
  real ang_vel;
public:
  vector heading; // unit vector
public:
  void setup();

  // methods for transforming a point
  vector toLocal(const vector &wp) const
    {return(heading.project_in(wp - loc));}
  vector toWorld(const vector &lp) const
    {return(loc + heading.project_out(lp));}

  // methods for transforming a point and a velocity
  void toLocal(vector &wp,vector &wv) const;
  void toWorld(vector &lp,vector &lv) const;
};

//==== Pose Class Implementation =====================================//

POSE_TEM
void POSE_FUN::setup()
{
  heading.heading(angle);
}

POSE_TEM
void POSE_FUN::toLocal(vector &wp,vector &wv) const
{
  wp = heading.project_in(wp - loc);
  wv = heading.project_in(wv - vel) - wp.perp()*ang_vel;
}

POSE_TEM
void POSE_FUN::toWorld(vector &lp,vector &lv) const
{
  lv = vel + heading.project_out(lv + lp.perp()*ang_vel);
  lp = loc + heading.project_out(lp);
}

#endif
