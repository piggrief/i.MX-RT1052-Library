/*!
* @file       Pig_UART.c
* @brief      根据官方的fsl_lpuart添加一些方便的接口
* @details    关于UART的引脚、时钟需要用官方配置工具先生成
* @author     pig's grief
* @version    v1.0
* @date       2019-4-10
* @to do      增加UART的eDMA模式
*/
# include "include.h"
# include "Pig_UART.h"

#define LPUART_Default_CLOCK_SOURCE 80000000UL;
//UART的默认配置
const lpuart_config_t LPUART_Default_config = {
    .baudRate_Bps = 256000,                     //波特率，按需配置
    .parityMode = kLPUART_ParityDisabled,       //是否有奇偶校验，一般不奇偶校验
    .dataBitsCount = kLPUART_EightDataBits,     //数据传输位数，一般为8位
    .isMsb = false,                             //是否先传MSB，一般为否
    .stopBitCount = kLPUART_OneStopBit,         //设定停止位有几位，一般为1位
    .txFifoWatermark = 0,                       //tx水印值
    .rxFifoWatermark = 1,                       //rx水印值
    .enableRxRTS = false,                       //是否使用RTS，一般为否
    .enableTxCTS = false,                       //是否使用CTS，一般为否
    .txCtsSource = kLPUART_CtsSourcePin,        //一般保持默认
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //一般保持默认
    .rxIdleType = kLPUART_IdleTypeStartBit,     //一般保持默认
    .rxIdleConfig = kLPUART_IdleCharacter1,     //一般保持默认
    .enableTx = true,                           //是否使能Tx,按需求设定
    .enableRx = true                            //是否使能Rx,按需求设定
};

uint32_t LPUART1_Clock = LPUART_Default_CLOCK_SOURCE;//UART1的时钟频率
lpuart_config_t LPUART1_config = {
    .baudRate_Bps = 256000,                     //波特率，按需配置
    .parityMode = kLPUART_ParityDisabled,       //是否有奇偶校验，一般不奇偶校验
    .dataBitsCount = kLPUART_EightDataBits,     //数据传输位数，一般为8位
    .isMsb = false,                             //是否先传MSB，一般为否
    .stopBitCount = kLPUART_OneStopBit,         //设定停止位有几位，一般为1位
    .txFifoWatermark = 0,                       //tx水印值
    .rxFifoWatermark = 1,                       //rx水印值
    .enableRxRTS = false,                       //是否使用RTS，一般为否
    .enableTxCTS = false,                       //是否使用CTS，一般为否
    .txCtsSource = kLPUART_CtsSourcePin,        //一般保持默认
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //一般保持默认
    .rxIdleType = kLPUART_IdleTypeStartBit,     //一般保持默认
    .rxIdleConfig = kLPUART_IdleCharacter1,     //一般保持默认
    .enableTx = true,                           //是否使能Tx,按需求设定
    .enableRx = true                            //是否使能Rx,按需求设定
};//UART1的配置

uint32_t LPUART2_Clock = LPUART_Default_CLOCK_SOURCE;//UART2的时钟频率
lpuart_config_t LPUART2_config = {
    .baudRate_Bps = 256000,                     //波特率，按需配置
    .parityMode = kLPUART_ParityDisabled,       //是否有奇偶校验，一般不奇偶校验
    .dataBitsCount = kLPUART_EightDataBits,     //数据传输位数，一般为8位
    .isMsb = false,                             //是否先传MSB，一般为否
    .stopBitCount = kLPUART_OneStopBit,         //设定停止位有几位，一般为1位
    .txFifoWatermark = 0,                       //tx水印值
    .rxFifoWatermark = 1,                       //rx水印值
    .enableRxRTS = false,                       //是否使用RTS，一般为否
    .enableTxCTS = false,                       //是否使用CTS，一般为否
    .txCtsSource = kLPUART_CtsSourcePin,        //一般保持默认
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //一般保持默认
    .rxIdleType = kLPUART_IdleTypeStartBit,     //一般保持默认
    .rxIdleConfig = kLPUART_IdleCharacter1,     //一般保持默认
    .enableTx = true,                           //是否使能Tx,按需求设定
    .enableRx = true                            //是否使能Rx,按需求设定
};//UART2的配置

uint32_t LPUART3_Clock = LPUART_Default_CLOCK_SOURCE;//UART3的时钟频率
lpuart_config_t LPUART3_config = {
    .baudRate_Bps = 256000,                     //波特率，按需配置
    .parityMode = kLPUART_ParityDisabled,       //是否有奇偶校验，一般不奇偶校验
    .dataBitsCount = kLPUART_EightDataBits,     //数据传输位数，一般为8位
    .isMsb = false,                             //是否先传MSB，一般为否
    .stopBitCount = kLPUART_OneStopBit,         //设定停止位有几位，一般为1位
    .txFifoWatermark = 0,                       //tx水印值
    .rxFifoWatermark = 1,                       //rx水印值
    .enableRxRTS = false,                       //是否使用RTS，一般为否
    .enableTxCTS = false,                       //是否使用CTS，一般为否
    .txCtsSource = kLPUART_CtsSourcePin,        //一般保持默认
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //一般保持默认
    .rxIdleType = kLPUART_IdleTypeStartBit,     //一般保持默认
    .rxIdleConfig = kLPUART_IdleCharacter1,     //一般保持默认
    .enableTx = true,                           //是否使能Tx,按需求设定
    .enableRx = true                            //是否使能Rx,按需求设定
};//UART3的配置

uint32_t LPUART4_Clock = LPUART_Default_CLOCK_SOURCE;//UART4的时钟频率
lpuart_config_t LPUART4_config = {
    .baudRate_Bps = 256000,                     //波特率，按需配置
    .parityMode = kLPUART_ParityDisabled,       //是否有奇偶校验，一般不奇偶校验
    .dataBitsCount = kLPUART_EightDataBits,     //数据传输位数，一般为8位
    .isMsb = false,                             //是否先传MSB，一般为否
    .stopBitCount = kLPUART_OneStopBit,         //设定停止位有几位，一般为1位
    .txFifoWatermark = 0,                       //tx水印值
    .rxFifoWatermark = 1,                       //rx水印值
    .enableRxRTS = false,                       //是否使用RTS，一般为否
    .enableTxCTS = false,                       //是否使用CTS，一般为否
    .txCtsSource = kLPUART_CtsSourcePin,        //一般保持默认
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //一般保持默认
    .rxIdleType = kLPUART_IdleTypeStartBit,     //一般保持默认
    .rxIdleConfig = kLPUART_IdleCharacter1,     //一般保持默认
    .enableTx = true,                           //是否使能Tx,按需求设定
    .enableRx = true                            //是否使能Rx,按需求设定
};//UART4的配置

uint32_t LPUART5_Clock = LPUART_Default_CLOCK_SOURCE;//UART5的时钟频率
lpuart_config_t LPUART5_config = {
    .baudRate_Bps = 256000,                     //波特率，按需配置
    .parityMode = kLPUART_ParityDisabled,       //是否有奇偶校验，一般不奇偶校验
    .dataBitsCount = kLPUART_EightDataBits,     //数据传输位数，一般为8位
    .isMsb = false,                             //是否先传MSB，一般为否
    .stopBitCount = kLPUART_OneStopBit,         //设定停止位有几位，一般为1位
    .txFifoWatermark = 0,                       //tx水印值
    .rxFifoWatermark = 1,                       //rx水印值
    .enableRxRTS = false,                       //是否使用RTS，一般为否
    .enableTxCTS = false,                       //是否使用CTS，一般为否
    .txCtsSource = kLPUART_CtsSourcePin,        //一般保持默认
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //一般保持默认
    .rxIdleType = kLPUART_IdleTypeStartBit,     //一般保持默认
    .rxIdleConfig = kLPUART_IdleCharacter1,     //一般保持默认
    .enableTx = true,                           //是否使能Tx,按需求设定
    .enableRx = true                            //是否使能Rx,按需求设定
};//UART5的配置

uint32_t LPUART6_Clock = LPUART_Default_CLOCK_SOURCE;//UART6的时钟频率
lpuart_config_t LPUART6_config = {
    .baudRate_Bps = 256000,                     //波特率，按需配置
    .parityMode = kLPUART_ParityDisabled,       //是否有奇偶校验，一般不奇偶校验
    .dataBitsCount = kLPUART_EightDataBits,     //数据传输位数，一般为8位
    .isMsb = false,                             //是否先传MSB，一般为否
    .stopBitCount = kLPUART_OneStopBit,         //设定停止位有几位，一般为1位
    .txFifoWatermark = 0,                       //tx水印值
    .rxFifoWatermark = 1,                       //rx水印值
    .enableRxRTS = false,                       //是否使用RTS，一般为否
    .enableTxCTS = false,                       //是否使用CTS，一般为否
    .txCtsSource = kLPUART_CtsSourcePin,        //一般保持默认
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //一般保持默认
    .rxIdleType = kLPUART_IdleTypeStartBit,     //一般保持默认
    .rxIdleConfig = kLPUART_IdleCharacter1,     //一般保持默认
    .enableTx = true,                           //是否使能Tx,按需求设定
    .enableRx = true                            //是否使能Rx,按需求设定
};//UART6的配置

uint32_t LPUART7_Clock = LPUART_Default_CLOCK_SOURCE;//UART7的时钟频率
lpuart_config_t LPUART7_config = {
    .baudRate_Bps = 256000,                     //波特率，按需配置
    .parityMode = kLPUART_ParityDisabled,       //是否有奇偶校验，一般不奇偶校验
    .dataBitsCount = kLPUART_EightDataBits,     //数据传输位数，一般为8位
    .isMsb = false,                             //是否先传MSB，一般为否
    .stopBitCount = kLPUART_OneStopBit,         //设定停止位有几位，一般为1位
    .txFifoWatermark = 0,                       //tx水印值
    .rxFifoWatermark = 1,                       //rx水印值
    .enableRxRTS = false,                       //是否使用RTS，一般为否
    .enableTxCTS = false,                       //是否使用CTS，一般为否
    .txCtsSource = kLPUART_CtsSourcePin,        //一般保持默认
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //一般保持默认
    .rxIdleType = kLPUART_IdleTypeStartBit,     //一般保持默认
    .rxIdleConfig = kLPUART_IdleCharacter1,     //一般保持默认
    .enableTx = true,                           //是否使能Tx,按需求设定
    .enableRx = true                            //是否使能Rx,按需求设定
};//UART7的配置

uint32_t LPUART8_Clock = LPUART_Default_CLOCK_SOURCE;//UART8的时钟频率
lpuart_config_t LPUART8_config = {
    .baudRate_Bps = 256000,                     //波特率，按需配置
    .parityMode = kLPUART_ParityDisabled,       //是否有奇偶校验，一般不奇偶校验
    .dataBitsCount = kLPUART_EightDataBits,     //数据传输位数，一般为8位
    .isMsb = false,                             //是否先传MSB，一般为否
    .stopBitCount = kLPUART_OneStopBit,         //设定停止位有几位，一般为1位
    .txFifoWatermark = 0,                       //tx水印值
    .rxFifoWatermark = 1,                       //rx水印值
    .enableRxRTS = false,                       //是否使用RTS，一般为否
    .enableTxCTS = false,                       //是否使用CTS，一般为否
    .txCtsSource = kLPUART_CtsSourcePin,        //一般保持默认
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //一般保持默认
    .rxIdleType = kLPUART_IdleTypeStartBit,     //一般保持默认
    .rxIdleConfig = kLPUART_IdleCharacter1,     //一般保持默认
    .enableTx = true,                           //是否使能Tx,按需求设定
    .enableRx = true                            //是否使能Rx,按需求设定
};//UART8的配置


void UART_Init(LPUART_Type *UARTn, uint32_t bound)
{
    lpuart_config_t UartConfig = LPUART_Default_config;
    uint32_t UartClock = LPUART_Default_CLOCK_SOURCE;

    if (UARTn == LPUART1)
    {
        UartConfig = LPUART1_config;
        UartClock = LPUART1_Clock;
    }
    else if (UARTn == LPUART2)
    {
        UartConfig = LPUART2_config;
        UartClock = LPUART2_Clock;
    }
    else if (UARTn == LPUART3)
    {
        UartConfig = LPUART3_config;
        UartClock = LPUART3_Clock;
    }
    else if (UARTn == LPUART4)
    {
        UartConfig = LPUART4_config;
        UartClock = LPUART4_Clock;
    }
    else if (UARTn == LPUART5)
    {
        UartConfig = LPUART5_config;
        UartClock = LPUART5_Clock;
    }
    else if (UARTn == LPUART6)
    {
        UartConfig = LPUART6_config;
        UartClock = LPUART6_Clock;
    }
    else if (UARTn == LPUART7)
    {
        UartConfig = LPUART7_config;
        UartClock = LPUART7_Clock;
    }
    else if (UARTn == LPUART8)
    {
        UartConfig = LPUART8_config;
        UartClock = LPUART8_Clock;
    }
    UartConfig.baudRate_Bps = bound;

    LPUART_Init(UARTn, &UartConfig, UartClock);
}

uint8 UART_GetChar(LPUART_Type *UARTn)
{
    return LPUART_ReadByte(UARTn);
}

void UART_Send(LPUART_Type *UARTn, uint8 * databuff, uint8 datasize)
{
    LPUART_WriteBlocking(UARTn, databuff, datasize);
}
