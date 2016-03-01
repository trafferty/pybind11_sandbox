/*
 * ip_math.h
 *
 *  Created on: Sep 11, 2015
 *      Author: trafferty
 */

#ifndef INCLUDE_IP_MATH_H_
#define INCLUDE_IP_MATH_H_

//#include <math.h>
#include <cmath>

#define PI (4.0*atan(1.0))   /* a constant */
#define TWOPI (8.0*atan(1.0))   /* a constant */
#define Deg2Rad(X) (X*PI/180.0)
#define Rad2Deg(X) (X*180.0/PI)

const double epsilon = 5.0e-08;

/*
 * For floating point variables, they should be initialized to NAN, using the NotAssigned const.  This allows
 * a consistent variable to test against, and in addition, we can therefore use the isnan() function to
 * raise exceptions against variables that are not initialized beyond default initialization.
 *
 * See: http://en.wikipedia.org/wiki/NaN#cite_ref-idl_0-0
 */
const double NotAssigned = NAN;
#define isNotAssigned(x) (isnan(x))

#endif /* INCLUDE_IP_MATH_H_ */
