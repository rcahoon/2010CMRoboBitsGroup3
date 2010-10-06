#include <stdlib.h>
#include <math.h>

int randomInt(int min, int max)
{
	int range = max - min;
	int inv_range = RAND_MAX/range;
	int upper = range * inv_range;
	int x;
	do {
		x = lrand48();
	} while (x >= upper);
	x /= inv_range;
	return x;
}
double randomDbl()
{
	return drand48();
}
double randomDbl(int min, int max)
{
	return (max-min)*randomDbl()+min;
}

/*
* Code extraced from:
*
**************************** stoc1.cpp **********************************
* Author:        Agner Fog
* Date created:  2002-01-04
* Last modified: 2008-11-30
* Project:       stocc.zip
* Source URL:    www.agner.org/random
*
* Description:
* Non-uniform random number generator functions.
*
* This file contains source code for the class StochasticLib1 defined in stocc.h.
*
* Documentation:
* ==============
* The file stocc.h contains class definitions.
* The file stocc.htm contains further instructions.
* The file distrib.pdf contains definitions of the statistic distributions.
* The file sampmet.pdf contains theoretical descriptions of the methods used
* for sampling from these distributions.
* The file ran-instructions.pdf contains general instructions.
*
* Copyright 2002-2008 by Agner Fog. 
* GNU General Public License http://www.gnu.org/licenses/gpl.html
*****************************************************************************/

static double normal_x2;
static int normal_x2_valid = 0;

double randomGaussian(double m, double s) {
   // normal distribution with mean m and standard deviation s
   double normal_x1;                   // first random coordinate (normal_x2 is member of class)
   double w;                           // radius
   if (normal_x2_valid) {              // we have a valid result from last call
      normal_x2_valid = 0;
      return normal_x2 * s + m;
   }    
   // make two normally distributed variates by Box-Muller transformation
   do {
      normal_x1 = 2. * randomDbl() - 1.;
      normal_x2 = 2. * randomDbl() - 1.;
      w = normal_x1*normal_x1 + normal_x2*normal_x2;
   }
   while (w >= 1. || w < 1E-30);
   w = sqrt(log(w)*(-2./w));
   normal_x1 *= w;  normal_x2 *= w;    // normal_x1 and normal_x2 are independent normally distributed variates
   normal_x2_valid = 1;                // save normal_x2 for next call
   return normal_x1 * s + m;
}
