/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】Z
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年11月22日
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
#include "Beacon_Smart_Car.h"




void Test_BeaconSmart_Car(void)
{
    BOARD_ConfigMPU();                   /* 初始化内存保护单元 */      
    BOARD_InitSDRAMPins();               /* SDRAM初始化 */
    BOARD_BootClockRUN();                /* 初始化开发板时钟 */           
    LED_Init();                          //初始化核心板和开发板上的LED接口
    LQ_UART_Init(LPUART1, 115200);       //串口1初始化 可以使用 printf函数
    _systime.init();                     //开启systick定时器
    NVIC_SetPriorityGrouping(2);         /*设置中断优先级组  0: 0个抢占优先级16位个子优先级
                                          *1: 2个抢占优先级 8个子优先级 2: 4个抢占优先级 4个子优先级 
                                          *3: 8个抢占优先级 2个子优先级 4: 16个抢占优先级 0个子优先级
                                          */
    
    LQ_PXP_Init();                       //图像处理初始化
    LQ_Camera_Init();                    //摄像头初始化
    
    LQ_TFT480_272_Init();                //彩屏初始化
    GT9147_Init();                       //初始化触摸板
    
    while(1)
    {
        LQ_Get_CameraBuff();    //得到原始数据

    
    }

}

































