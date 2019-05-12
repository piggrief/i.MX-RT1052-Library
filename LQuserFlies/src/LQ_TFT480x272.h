/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱RT1052核心板套件
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年1月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.20.1及以上
【Target  】i.MX RT1052DVLA
【Crystal 】 24.000Mhz
【busclock】264.500MHz
【pllclock】528.000MHz  
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef __LQ_TFT480x272_H__
#define __LQ_TFT480x272_H__

#include "fsl_common.h"

extern  uint32_t MYCOLOR[10];
 
/*******************接口定义******************************/
/* LCD definition. */
#define LCDIF_DATA_BUS              kELCDIF_DataBus16Bit //TFT屏数据线宽度
#define LCD_WIDTH   480
#define LCD_HEIGHT  272
#define LCD_HSW 41
#define LCD_HFP 4
#define LCD_HBP 8
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 2

#define LCD_POL_FLAGS (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

/* Display. */
#define LCD_DISP_GPIO GPIO1
#define LCD_DISP_GPIO_PIN   2

/* Back light. */
#define LCD_BL_GPIO GPIO2
#define LCD_BL_GPIO_PIN     31

#define APP_IMG_HEIGHT 272
#define APP_IMG_WIDTH  480

#define	RED		0xFF0000
#define	GREEN		0x00FF00
#define	BLUE		0x0000FF
#define	PURPLE		0xFF00FF
#define	YELLOW		0xFFFF00
#define	CYAN		0x00FFFF //蓝绿色
#define	ORANGE		0xFF1080
#define	BLACK		0x000000
#define	WHITE		0xFFFFFF

extern uint16_t logo[];	
extern void delay(uint32_t t);								 //延时函数
extern void TFT_CLS(uint32_t fcolor);					                 //全屏显示某种颜色
extern void TFT_Draw_Part(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint32_t fcolor,uint8_t clr);     //填充矩形区域，行起始、终止坐标，列起始、终止坐标，颜色
extern void TFT_Draw_Line(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint32_t fcolor,uint8_t clr);     //画线，行起始、终止坐标，列起始、终止坐标，颜色
extern void TFT_Draw_Rectangle(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint32_t fcolor,uint8_t clr);//画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
extern void TFT_Draw_Circle(uint16_t x,uint16_t y,uint16_t r,uint32_t fcolor,uint8_t clr);                 //画圆形边框，圆心横坐标、纵坐标，半径，颜色
extern void TFT_Draw_CircleCake(uint16_t x,uint16_t y,uint16_t r,uint32_t fcolor,uint8_t clr);
extern void TFT_Draw_Dot(uint16_t x,uint16_t y,uint32_t fcolor);	                         //画点，横坐标，纵坐标，颜色
extern void TFT_P6X8(uint16_t x, uint16_t y, uint8_t c_dat, uint32_t fcolor, uint32_t bcolor, uint8_t clr);
extern void TFT_P6X8Str(uint16_t x, uint16_t y, uint8_t *s_dat,uint32_t fcolor, uint32_t bcolor, uint8_t clr);
extern void TFT_P8X8(uint16_t x, uint16_t y, uint8_t c_dat,uint32_t fcolor, uint32_t bcolor, uint8_t clr);
extern void TFT_P8X8Str(uint16_t x, uint16_t y, uint8_t *s_dat,uint32_t fcolor, uint32_t bcolor, uint8_t clr);
extern void TFT_P8X16(uint16_t x, uint16_t y, uint8_t c_dat,uint32_t fcolor, uint32_t bcolor, uint8_t clr);
extern void TFT_P8X16Str(uint16_t x, uint16_t y, uint8_t *s_dat,uint32_t fcolor, uint32_t bcolor, uint8_t clr);
extern void TFT_P16x16Str(uint16_t x,uint16_t y,uint8_t *s_dat,uint32_t fcolor, uint32_t bcolor, uint8_t clr);
extern void TFT_Show_Pic(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint8_t *PicArray);
extern void TFT_Show_Logo(uint16_t xs,uint16_t ys);
extern void TFT_Test(void);

#endif 