
// Header:
// File Name: 
// Author: Z
// Date:2018/08/03

#ifndef __M_SYSTIME_H
#define __M_SYSTIME_H

#include "include.h"

typedef struct
{
		
	void (* init) (void);  
	uint64_t (* get_time_us) (void);
	uint32_t (* get_time_ms) (void);
	void (* delay_us)(uint64_t);//延时1us 实测为1.23us   延时10us 实测为10.23us   延时100us 实测为100.23us
	void (* delay_ms)(uint32_t);

}systime_t;

extern systime_t  _systime;
/*!位置：m_systime.h
* @brief 测量程序运行时间，us级别
*
* @param *RunFuction 待测量运行时间的函数指针，要定义一个void输入，void输出的函数
* @example uint64_t time = MeasureRunTime_us(DelayTest);
*/
uint64_t MeasureRunTime_us(void(*RunFuction)(void));
/*!位置：m_systime.h
* @brief 测量程序运行时间，ms级别
*
* @param *RunFuction 待测量运行时间的函数指针，要定义一个void输入，void输出的函数
* @example uint64_t time = MeasureRunTime_ms(DelayTest);
*/
uint32_t MeasureRunTime_ms(void(*RunFuction)(void));


#endif //__M_SYSTIME_H




