#ifndef __AHRS_H
#define __AHRS_H
#include "Inertial_Sensor.h"
typedef struct 
{
	float q0;
	float q1;
	float q2;
	float q3;
}Q4_t;

typedef struct
{
	vector3f_t a;
	vector3f_t b;
	vector3f_t c;
}Matrix_t;


typedef struct
{
	void (* update)(void);

}AHRS;

extern AHRS _AHRS;

extern void Test_ahrs(void);    //≤‚ ‘◊ÀÃ¨Ω‚À„
extern void Test_ahrs9(void);

#endif //__AHRS_H

