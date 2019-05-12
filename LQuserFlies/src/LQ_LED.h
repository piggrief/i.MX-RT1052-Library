/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年2月1日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.20.1及以上版本
【Target 】 i.MX RT1052
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 528MHz
【USB PLL】 480MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_LED_H__
#define __LQ_LED_H__

#include "include.h"

extern gpio_pin_config_t GPIO_Output_Config;  
/*******************************************************************************
* Definitions
******************************************************************************/
extern gpio_pin_config_t GPIO_Output_Config;

//定义模块号
typedef enum
{
  LED_G=1,
  LED_R=2,   
  LED_B=3,
  LEDALL=5,//全部   
} LEDn_e;
typedef enum
{
  white = 0,  //白色
  black = 1,  //黑色
  red   = 2,  //红
  green = 3,  //绿
  blue  = 4,  //蓝色 
  yellow= 5,  //黄色
  violet= 6,  //紫色
  cyan  = 7,  //青色
}LED_t;

typedef enum
{
  ON=0,  //亮
  OFF=1, //灭
  RVS=2, //反转  
}LEDs_e;
extern void LED_Color(LED_t color);
extern void LED_Color_Reverse(LED_t color);
extern void LED_Init(void);
extern void LED_Ctrl(LEDn_e ledno, LEDs_e sta);
extern void Test_LED(void);

#endif 
