/*
   C++ class definition for use with random.cc This class implements a
   Mersenne Twister RNG, which is not only fast but has very good
   independence properties.  See the following for details:
     http://www.math.keio.ac.jp/~matumoto/emt.html

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above
        copyright notice, this list of conditions and the following
        disclaimer in the documentation and/or other materials
        provided with the distribution.

     3. The names of its contributors may not be used to endorse or
        promote products derived from this software without specific
        prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
   COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
   STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
   OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdint.h>
#include <algorithm>

class Random{
  static const int N = 624;
  static const int M = 397;

  // state
  uint32_t state[N];
  uint32_t *next;
  int left;

  double gstate; //  __attribute__((__aligned__(8)));
  int gleft;

  void next_state();
public:
  Random(){left=-1; gleft=0;}

  void randomize();
  void seed(uint32_t s);
  void seed(const uint32_t *init_key,int key_length);

  // uniform random numbers
  unsigned uint32();
  int int32()
    {return((int)(uint32()));}
  int int31()
    {return((int)(uint32()>>1));}

  unsigned uint32(unsigned max) // unsigned integer in [0,max)
    {return(uint32() % max);}

  unsigned uint32(unsigned min,unsigned max) // unsigned integer in [min,max)
    {return(min + uint32() % (max-min));}

  double real32() // real number in [0,1)
    {return((double)uint32() * (1.0/4294967296.0));}
  double sreal32() // real number in [-1,1]
    {return((double)uint32() * (2.0/4294967295.0) - 1.0);}
  double real32(double a,double b) // real number in [a,b)
    {return(a + real32()*(b-a));}

  double real32o() // real number in (0,1) "(o)pen interval"
    {return(((double)uint32() + 1.0) * (1.0/4294967297.0));}
  double real53() // full precision double in [0,1)
    {unsigned a=uint32()>>5,b=uint32()>>6;
     return(a*67108864.0+b)*(1.0/9007199254740992.0);}

  // returns true with probability <prob>
  bool pbool(double prob)
    {return(uint32() >= (unsigned)(prob*4294967297.0 + 0.5));}

  // non-uniform distributions
  double gaussian32(); // zero mean, unit variance gaussian deviate
  double exp32();      // unit mean, positive exponential deviate

  template <class int_t>
  void permutation(int_t *arr,int len);
};

template <class int_t>
void Random::permutation(int_t *arr,int len)
{
  int i;

  for(i=0; i<len; i++) arr[i] = i;

  for(i=len; i>0; i--){
    swap(arr[i-1],arr[uint32(i)]);
  }
}

#endif /*__RANDOM_H__*/
