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
#include "include.h"

volatile uint16_t pitIsrCnt0 = 0;   //测试用的  volatile易变的，在中断里面会改变的变量定义时最好加上，告诉编译器不要优化，每次读取都从该变量的地址读取
volatile uint16_t pitIsrCnt1 = 0;  //测试用的  volatile易变的，在中断里面会改变的变量定义时最好加上，告诉编译器不要优化，每次读取都从该变量的地址读取
volatile uint16_t pitIsrCnt2 = 0;  //测试用的  volatile易变的，在中断里面会改变的变量定义时最好加上，告诉编译器不要优化，每次读取都从该变量的地址读取
volatile uint16_t pitIsrCnt3 = 0; //测试用的  volatile易变的，在中断里面会改变的变量定义时最好加上，告诉编译器不要优化，每次读取都从该变量的地址读取

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【功能说明】PIT中断服务函数
【软件版本】V1.0
【最后更新】2018年11月24日 
【函数名】
【返回值】无
【参数值】无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void PIT_IRQHandler(void)
{
    
    if((PIT_GetStatusFlags(PIT,kPIT_Chnl_0)&kPIT_TimerFlag)==kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/     
        pitIsrCnt0++;
//        LED_Ctrl(LED0, RVS);
    }
    if((PIT_GetStatusFlags(PIT,kPIT_Chnl_1)&kPIT_TimerFlag)==kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/
        pitIsrCnt1++;
    }
    if((PIT_GetStatusFlags(PIT,kPIT_Chnl_2)&kPIT_TimerFlag)==kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/        
        pitIsrCnt2++;
    }
    if((PIT_GetStatusFlags(PIT,kPIT_Chnl_3)&kPIT_TimerFlag)==kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/        
        pitIsrCnt3++;
    }
    
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【功能说明】测试PIT定时中断
【软件版本】V1.0
【最后更新】2018年11月24日 
【函数名】
【返回值】无
【参数值】无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_PIT(void)
{ 
    LQ_PIT_Init(kPIT_Chnl_2, 1000000);    //1000 000us 一次中断
    LQ_PIT_Init(kPIT_Chnl_3, 2000000);    //2000 000us 一次中断
    while (true)
    {
        /* Check whether occur interupt and toggle LED */
         if (pitIsrCnt2)
        {
            printf("\r\n PIT通道2中断已发生!");
            LED_Color(red);   //绿灯 
            pitIsrCnt2=0;
        }
         if (pitIsrCnt3)
        {
            printf("\r\n PIT通道3中断已发生!");
            LED_Color(blue);   //蓝灯 
            pitIsrCnt3=0;
        }
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】PIT定时中断初始化
【软件版本】V1.0
【最后更新】2018年11月24日 
【函数名】
【返回值】无
【参数值】无    LQ_PIT_Init(kPIT_Chnl_0, 100000);  //开启通道0定时器中断，每100000us中断一次
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_PIT_Init(pit_chnl_t channel, uint32_t count)
{
     /* PIT初始化结构体 */
    pit_config_t pitConfig;  
    
    /* Set PERCLK_CLK source to OSC_CLK*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    /* Set PERCLK_CLK divider to 1 */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);

    /*
     * pitConfig.enableRunInDebug = false;  //默认参数
     */
    PIT_GetDefaultConfig(&pitConfig);   //得到PIT默认参数

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);    //初始化PIT

    /* Set timer period for channel  */
    PIT_SetTimerPeriod(PIT, channel, USEC_TO_COUNT(count, CLOCK_GetFreq(kCLOCK_OscClk))); //设置PIT通道和定时时间

    /* Enable timer interrupts for channel  */
    PIT_EnableInterrupts(PIT, channel, kPIT_TimerInterruptEnable);  //使能通道中断

    //优先级配置 抢占优先级0  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(PIT_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,2));
    
    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQn);   //使能中断

    /* Start channel  */
    PRINTF("\r\nStarting channel ...");
    
    PIT_StartTimer(PIT, channel);   //开启定时器

}

