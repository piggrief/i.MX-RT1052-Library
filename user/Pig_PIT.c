# include "include.h"
# include "Pig_PIT.h"

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
extern void PIT0Inter();
void PIT_IRQHandler(void)
{
    
    if((PIT_GetStatusFlags(PIT,kPIT_Chnl_0)&kPIT_TimerFlag)==kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/     
        PIT0Inter();
//        LED_Ctrl(LED0, RVS);
    }
    if((PIT_GetStatusFlags(PIT,kPIT_Chnl_1)&kPIT_TimerFlag)==kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/
    }
    if((PIT_GetStatusFlags(PIT,kPIT_Chnl_2)&kPIT_TimerFlag)==kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/        
    }
    if((PIT_GetStatusFlags(PIT,kPIT_Chnl_3)&kPIT_TimerFlag)==kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/        
    }
    
}