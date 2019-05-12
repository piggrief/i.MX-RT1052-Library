#include "AP_Math.h"
#include "Inertial_Sensor.h"
#include "math.h"
/**************************实现函数*********************************************************************
函  数：static float invSqrt(float x) 
功　能: 快速计算 1/Sqrt(x) 	
参  数：要计算的值
返回值：结果
备  注：比普通Sqrt()函数要快四倍See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
*********************************************************************************************************/
float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}


// a faster varient of atan.  accurate to 6 decimal places for values between -1 ~ 1 but then diverges quickly
float fast_atan(float v)
{
    float v2 = v*v;
    return (v*(1.6867629106f + v2*0.4378497304f)/(1.6867633134f + v2));
}


// constrain a value
float constrain_float(float amt, float low, float high) 
{
	// the check for NaN as a float prevents propogation of
	// floating point errors through any function that uses
	// constrain_float(). The normal float semantics already handle -Inf
	// and +Inf
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

// constrain a int16_t value
int16_t constrain_int16(int16_t amt, int16_t low, int16_t high) {
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

// constrain a int32_t value
int32_t constrain_int32(int32_t amt, int32_t low, int32_t high) {
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

// degrees -> radians
float radians(float deg) {
	return deg * DEG_TO_RAD;
}

// radians -> degrees
float degrees(float rad) {
	return rad * RAD_TO_DEG;
}

// square
float sq(float v) {
	return v*v;
}

// 2D vector length
float pythagorous2(float a, float b) {
	return sqrtf(sq(a)+sq(b));
}

// 3D vector length
float pythagorous3(float a, float b, float c) {
	return sqrtf(sq(a)+sq(b)+sq(c));
}


/*
  wrap an angle in centi-degrees to 0..36000
 */
int32_t wrap_360_cd(int32_t error)
{
    while (error > 360) error -= 360;
    while (error < 0) error += 360;
    return error;
}

/*
  wrap an angle in centi-degrees to -18000..18000
 */
float wrap_180_cd(float error)
{
    while (error > 180) error -= 360;
    while (error < -180) error += 360;
    return error;
}





