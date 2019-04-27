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

#define WDOG_WCT_INSTRUCITON_COUNT (256U)

static WDOG_Type *wdog_base = WDOG1;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void WaitWctRefresh()
{
    for (uint32_t i = 0; i < WDOG_WCT_INSTRUCITON_COUNT; i++)
    {
    }
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【功能说明】测试看门狗功能
【软件版本】V1.0
【最后更新】2018年11月24日 
【函数名】
【返回值】无
【参数值】无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_WDOG(void)
{
    wdog_config_t config;

    printf("--- Start Wdog test ---\r\n");
    /*
    * wdogConfig->enableWdog = true;
    * wdogConfig->workMode.enableWait = true;
    * wdogConfig->workMode.enableStop = false;
    * wdogConfig->workMode.enableDebug = false;
    * wdogConfig->enableInterrupt = false;
    * wdogConfig->enablePowerdown = false;
    * wdogConfig->resetExtension = flase;
    * wdogConfig->timeoutValue = 0xFFU;
    * wdogConfig->interruptTimeValue = 0x04u;
    */
    WDOG_GetDefaultConfig(&config);
    config.timeoutValue = 0xffu;
    config.interruptTimeValue = 0x08u;
    WDOG_Init(wdog_base, &config);
    printf("--- wdog Init done---\r\n");

    for (uint32_t i = 0; i < 10; i++)
    {
        WDOG_Refresh(wdog_base);
        printf("--- Refresh wdog %d time ---\r\n", i + 1);
        WaitWctRefresh();
    }

    printf("\r\nEnd of Wdog example!\r\n");
    while (1)
    {
    }
}
