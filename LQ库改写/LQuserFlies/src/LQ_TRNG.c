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
#define TRNG_EXAMPLE_RANDOM_NUMBER 10  //每次生成10个32位随机数

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【功能说明】测试硬件随机数发生功能
【软件版本】V1.0
【最后更新】2018年11月24日 
【函数名】
【返回值】无
【参数值】无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_RNG(void)
{
    uint32_t i;
    trng_config_t trngConfig;
    status_t status;
    uint32_t data[TRNG_EXAMPLE_RANDOM_NUMBER];


    printf("TRNG Peripheral Driver Example\r\n");

    /* Initialize TRNG configuration structure to default.*/
    /*
     * trngConfig.lock = TRNG_USER_CONFIG_DEFAULT_LOCK;
     * trngConfig.clockMode = kTRNG_ClockModeRingOscillator;
     * trngConfig.ringOscDiv = TRNG_USER_CONFIG_DEFAULT_OSC_DIV;
     * trngConfig.sampleMode = kTRNG_SampleModeRaw;
     * trngConfig.entropyDelay = TRNG_USER_CONFIG_DEFAULT_ENTROPY_DELAY;
     * trngConfig.sampleSize = TRNG_USER_CONFIG_DEFAULT_SAMPLE_SIZE;
     * trngConfig.sparseBitLimit = TRNG_USER_CONFIG_DEFAULT_SPARSE_BIT_LIMIT;
     * trngConfig.retryCount = TRNG_USER_CONFIG_DEFAULT_RETRY_COUNT;
     * trngConfig.longRunMaxLimit = TRNG_USER_CONFIG_DEFAULT_RUN_MAX_LIMIT;
     * trngConfig.monobitLimit.maximum = TRNG_USER_CONFIG_DEFAULT_MONOBIT_MAXIMUM;
     * trngConfig.monobitLimit.minimum = TRNG_USER_CONFIG_DEFAULT_MONOBIT_MINIMUM;
     * trngConfig.runBit1Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT1_MAXIMUM;
     * trngConfig.runBit1Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT1_MINIMUM;
     * trngConfig.runBit2Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT2_MAXIMUM;
     * trngConfig.runBit2Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT2_MINIMUM;
     * trngConfig.runBit3Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT3_MAXIMUM;
     * trngConfig.runBit3Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT3_MINIMUM;
     * trngConfig.runBit4Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT4_MAXIMUM;
     * trngConfig.runBit4Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT4_MINIMUM;
     * trngConfig.runBit5Limit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT5_MAXIMUM;
     * trngConfig.runBit5Limit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT5_MINIMUM;
     * trngConfig.runBit6PlusLimit.maximum = TRNG_USER_CONFIG_DEFAULT_RUNBIT6PLUS_MAXIMUM;
     * trngConfig.runBit6PlusLimit.minimum = TRNG_USER_CONFIG_DEFAULT_RUNBIT6PLUS_MINIMUM;
     * trngConfig.pokerLimit.maximum = TRNG_USER_CONFIG_DEFAULT_POKER_MAXIMUM;
     * trngConfig.pokerLimit.minimum = TRNG_USER_CONFIG_DEFAULT_POKER_MINIMUM;
     * trngConfig.frequencyCountLimit.maximum = TRNG_USER_CONFIG_DEFAULT_FREQUENCY_MAXIMUM;
     * trngConfig.frequencyCountLimit.minimum = TRNG_USER_CONFIG_DEFAULT_FREQUENCY_MINIMUM;
     */
    TRNG_GetDefaultConfig(&trngConfig);//获取默认设置
    //采样模式设置为Von Neumann,获得较佳数据，也可不用设置   
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;

    //初始化TRNG
    status = TRNG_Init(TRNG, &trngConfig);
    if (kStatus_Success == status)
    {
        while (1)
        {
            printf("\r\n生成%d个32位随机数，如下: \r\n", TRNG_EXAMPLE_RANDOM_NUMBER);

            //获得随机数
            status = TRNG_GetRandomData(TRNG, data, sizeof(data));
            if (status == kStatus_Success)
            {
                //输出随机数到串口助手
                for (i = 0; i < TRNG_EXAMPLE_RANDOM_NUMBER; i++)
                {
                    printf("Random[%d] = 0x%X\r\n", i, data[i]);
                }
            }
            else
            {
                printf("TRNG failed!\r\n");
            }            
        LED_Ctrl(LED_B, RVS); //EVK LED闪烁  
        delayms(1000);        //延时1秒
        }
    }
    else
    {
        printf("TRNG initialize failed!\r\n");
    }
    return ;
}
