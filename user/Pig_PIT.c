# include "include.h"
# include "Pig_PIT.h"

void PIT_Init_us(pit_chnl_t PITn, long TimeInterval, bool enableRunInDebug)
{
    pit_config_t config;
    config.enableRunInDebug = enableRunInDebug;

    /* 初始化PIT */
    PIT_Init(PIT, &config);
    /* 设置PITn的计时间隔为TimeInterval微秒 */
    PIT_SetTimerPeriod(PIT, PITn, 75 * TimeInterval);
    /* 使能PIT0中断 */
    PIT_EnableInterrupts(PIT, PITn, kPIT_TimerInterruptEnable);
    //优先级配置 抢占优先级0  子优先级2   越小优先级越高  抢占优先级可打断别的中断
    NVIC_SetPriority(PIT_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 2));
    /* 在中断向量表中使能 PIT 中断*/
    EnableIRQ(PIT_IRQN);
    /* 开启PITn计时 */
    PIT_StartTimer(PIT, PITn);
}

extern void PIT0Inter();
void PIT_IRQHANDLER(void)
{
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_0)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/
        PIT0Inter();
    }
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_1)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/
    }
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_2)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/
    }
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_3)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*用户可自行添加代码*/
    }
}