#ifndef __LQ_ELCDIF_H
#define __LQ_ELCDIF_H


/*******************************************************************************
* Definitions
******************************************************************************/ 
#define APP_LCD_WIDTH  480//液晶屏宽度
#define APP_LCD_HEIGHT 272//液晶屏高度
#define APP_HSW 41  //表示水平同步信号的宽度，用VCLK计算
#define APP_HFP 4   //表示一行的有效数据结束到下一个水平同步信号开始之间的VCLK的个数
#define APP_HBP 8   //表示从水平同步信号开始到一行的有效数据开始之间的VCLK的个数
#define APP_VSW 10  //表示垂直同步脉冲的宽度，用行数计算
#define APP_VFP 4   //表示在一帧图像结束后，垂直同步信号以前的无效的行数
#define APP_VBP 2   //表示在一帧图像开始时，垂直同步信号以后的无效的行数
#define APP_LCD_POL_FLAGS (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)
#define APP_LCDIF_DATA_BUS kELCDIF_DataBus16Bit



/* Frame buffer data alignment. */
#define FRAME_BUFFER_ALIGN 64  //帧缓冲区分配 








extern void LQ_TFT480_272_Init(void);
extern void Test_TFT_480_272(void);

extern uint8_t counter;
extern volatile uint8_t LQ_LCD;  
extern int OFFSET2;
extern uint32_t lcdFrameBuf[4][LCD_HEIGHT][LCD_WIDTH];


#endif