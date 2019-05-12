#ifndef __AP_MATH_H
#define __AP_MATH_H
#include "include.h"

#define M_2PI 6.283185307179586
# define PI 3.141592653589793f
#define M_PI_2 1.570796326794897f

#define DEG_TO_RAD 0.017453292519943295769236907684886f
#define RAD_TO_DEG 57.295779513082320876798154814105f

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

extern	float invSqrt(float x); 
extern	float fast_atan(float v);
extern  float constrain_float(float amt, float low, float high);
extern  int16_t constrain_int16(int16_t amt, int16_t low, int16_t high);
extern  float wrap_180_cd(float error);
#endif //

