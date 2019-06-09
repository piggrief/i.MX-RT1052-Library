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
#ifndef _LQ_INLUDE_FILES_H_
#define _LQ_INLUDE_FILES_H_
    #include "stdio.h"
    #include "math.h"
//SDK底层驱动库
    #include "fsl_adc.h"
    #include "fsl_cache.h"
    #include "fsl_common.h"
    #include "fsl_clock.h"
    #include "fsl_debug_console.h"
    #include "fsl_elcdif.h"
    #include "fsl_enc.h"    
    #include "fsl_gpio.h"
    #include "fsl_gpt.h"
    #include "fsl_iomuxc.h"
    #include "fsl_lpuart.h"
    #include "fsl_lpi2c.h"
    #include "fsl_pwm.h"
    #include "fsl_pit.h"
    #include "fsl_qtmr.h"
    #include "fsl_trng.h"
    #include "fsl_usdhc.h"
    #include "fsl_wdog.h"
    #include "fsl_xbara.h"
    #include "board.h"
    #include "pin_mux.h"
    #include "fsl_flexspi.h"
    #include "fsl_lpuart_edma.h"
    #include "fsl_edma.h"
    #include "fsl_dmamux.h"
    #include "fsl_lpspi.h"
    #include "fsl_sccb.h"

#define delayms(x) _systime.delay_ms(x)
#define delayus(x) _systime.delay_us(x)
#define uint16 uint16_t
#define int16 int16_t
#define uint8 uint8_t
#define int8 int8_t


//用户自定义文件    
    #include "LQ_IIC.h"
    #include "LQ_GPIOmacro.h" //GPIO口51操作格式 
    #include "LQ_SGP18T.h"    //TFT1.8吋彩屏模块
    #include "LQ_LED.h"       //LED指示
    #include "LQ_KEY.h"       //独立按键
    #include "LQ_PWM.h"       //电机，舵机PWM控制 XBARA
    #include "LQ_UART.h"       //UART串口
    #include "LQ_ADC.h"       //ADC转换采集
    #include "LQ_PIT.h"       //PIT定时
    #include "LQ_TRNG.h"      //随机数发生器
    #include "LQ_Encoder.h"   //编码器正交解码数据采集
    #include "m_systime.h"    //systick 定时器，用于计时
    #include "status.h"       //标志位
    #include "LQ_QTMR.h"      //计时器可用于生成PWM  和 正交解码
    #include "LQ_SPI.h"

    #include "Pig_edma.h"
    #include "PigMT9V032.h"
    #include "DebugFunction.h"
    #include "MecanumWheelControl.h" 

    #include "Pig_SPI.h"
    #include "TFTDriver.h"
    #include "LQ_MPU6050.h"
//    void LQ_Get_CameraBuff(void);//得到摄像头数据 
//    void LQ_Camera_0_1_Handle(void);  //二值化并显示
//    void Seek_Road(void);
//    void Test_TFT_Camera(void);    //摄像头+彩屏例程

    extern void Test_OLED_Camera(void);//摄像头+oled例程
#endif //_LQ_INLUDE_FILES_H_
