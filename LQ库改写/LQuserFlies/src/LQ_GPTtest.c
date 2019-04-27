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
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define GPT_IRQ_ID GPT2_IRQn
#define EXAMPLE_GPT GPT2
#define EXAMPLE_GPT_IRQHandler GPT2_IRQHandler
   
/* Select IPG Clock as PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_DIVIDER_SELECT (0U)
/* Get source clock for GPT driver (GPT prescaler = 0) */
#define EXAMPLE_GPT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (EXAMPLE_GPT_CLOCK_DIVIDER_SELECT + 1U)) 

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint16_t pitIsrCnt3 = 0;
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【功能说明】GPT中断服务函数
【软件版本】V1.0
【最后更新】2018年11月24日 
【函数名】
【返回值】无
【参数值】无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void EXAMPLE_GPT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_OutputCompare1Flag);
    pitIsrCnt3++;
    LED_Ctrl(LED0, RVS); 
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
    __DSB();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【功能说明】测试GPI定时中断功能
【软件版本】V1.0
【最后更新】2018年11月24日 
【函数名】
【返回值】无
【参数值】无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_GPT_Timer(void)
{
    uint16_t pitIsrCnt2 = 0;
    uint32_t gptFreq;
    gpt_config_t gptConfig;
    
    /*Clock setting for GPT*/
    CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);

    GPT_GetDefaultConfig(&gptConfig);

    /* Initialize GPT module */
    GPT_Init(EXAMPLE_GPT, &gptConfig);

    /* Divide GPT clock source frequency by 3 inside GPT module */
    GPT_SetClockDivider(EXAMPLE_GPT, 3);

    /* Get GPT clock frequency */
    gptFreq = EXAMPLE_GPT_CLK_FREQ;

    /* GPT frequency is divided by 3 inside module */
    gptFreq /= 3;

    /* Set both GPT modules to 1 second duration */
    GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);

    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_OutputCompare1InterruptEnable);

    /* Enable at the Interrupt */
    EnableIRQ(GPT_IRQ_ID);

    /* Start Timer */
    PRINTF("\r\nStarting GPT timer ...");
    GPT_StartTimer(EXAMPLE_GPT);

    while (true)
    {
        /* Check whether occur interupt and toggle LED */
        if (pitIsrCnt3>pitIsrCnt2)
        {
            PRINTF("\r\n GPT中断已发生%d次!",pitIsrCnt3);
            LED_Ctrl(LED1, RVS); 
            pitIsrCnt2=pitIsrCnt3;
        }    
       
        //else{__WFI();}
    }
}
