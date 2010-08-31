#ifndef _INCLUDED_RANGE_H_
#define _INCLUDED_RANGE_H_

namespace Range {

#define RANGE_TEM template <class num,bool open_min,bool open_max>
#define RANGE_FUN Range<num,open_min,open_max>

RANGE_TEM
class Range{
public:
  num min;
  num max;
public:
  void set(num _min,num _max)
    {min=_min; max=_max;}
  void set(num _min_max)
    {min=_min_max; max=_min_max;}
  void expand(num e)
    {min-=e; max+=e;}
  bool inside(num v) const;
};

RANGE_TEM
bool RANGE_FUN::inside(num v) const
{
  return((open_min? v>min : v>=min) &&
         (open_max? v<max : v<=max));
}

#undef RANGE_TEM
#undef RANGE_FUN
};
#endif
