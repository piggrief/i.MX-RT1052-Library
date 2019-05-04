/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2018年11月13日
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
================================================
下载及调试模式：0010
下载      模式：0001
运行      模式：0010
------------------------------------------------
用JLINKV9/OB及以上版本可以下载SDRAM和QSPI FLASH;
用LQDAPLINKV3可以下载SDRAM和QSPI FLASH;
------------------------------------------------
部分函数尚未验证完毕，默认测试OV7725模块和配套4.3吋彩屏
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "AHRS.h"    //使用姿态解算时需要添加该头文件
//==========================================================================================  
//测试函数名称        测试内部模块及功能           智能车及应用开发               完成状况
//----------------------------------------------------------------------------------------- 
//Test_LED();        //测试GPIO输出口               LED显示及外设控制              已完成
//Test_OLED();       //测试模拟SPI功能              OLED模块功能                   已完成
//Test_GPIO_KEY();   //测试GPIO输入                 按键检测功能                   已完成 
//Test_GPIO_EXINT(); //测试GPIO输入及外部中断       按键、中断检测功能             已完成     
//LQ_UART_Test();    //测试UART及中断               蓝牙、USB转TTL调试及通信       已完成
//Test_ADC();        //测试ADC采集功能              电磁传感器及电源电压监控       已完成
//Test_PIT();        //测试PIT定时中断功能                                         已完成
//Test_PWM_nCH();    //测试PWM                      PWM1，PWM2，PWM4 产生PWM波     已完成
//Test_Servo();      //测试用PWM功能                数字舵机驱动控制               已完成
//Test_Motor();      //测试用PWM功能                电机驱动控制                   已完成  
//Test_OLED_Camera();//测试OLED和MT9V034功能        OLED显示屏及面阵摄像头动图     已完成
//Test_ENCoder();    //编码器采集                                                  已完成
//Text_TFT_480_272();//测试480 * 272 屏                                            已完成
//Test_TFT_Camera(); //测试OV7725 or 神眼和TFT4.3 + 触摸屏                         已完成
//Test_WDG();        //测试看门狗功能               程序监控                       已完成
//Test_9AX();        //测试I2C及龙邱九轴            陀螺仪及加速度功能             已完成 
//Test_MPU6050();    //测试I2C及6轴陀螺仪功能       陀螺仪及加速度功能             已完成 
//Test_EEPROM();     //测试EEPROM功能               内部EEPROM数据存贮             待完成 
//-----------------------------------------------------------------------------------------  
//========================================================================================== 
/*      更新说明                                                                    时间
 *      1.优化 Test_OLED_Camera() 
 *      2.增加systick计时器功能 
 *      3.优化延时函数                                                             2018/11/08
 *
 *      1.优化 PID
 *      2.优化 Scheduler                                                           2018/11/09
 *
 *      1.增加串口+DMA 例程                                                        
 *      2.增加QTMR模块的PWM功能例程                                                2018/11/19
 *
 *      1.增加QTMR模块的正交解码例程                                               
 *      2.优化神眼摄像头SCCB                                                       2018/11/21
 *
 *      1.优化神眼摄像头帧率设置                                                   2018/11/27
 *
 *      1.优化7725摄像头帧率配置
 *      2.增加上位机显示图像功能                                                   2018/11/28
 *
 *      1.解决神眼188*120分辨率不出图问题
 *      2.解决图像抖动问题                                                         2019/01/15
 */

void delayms(uint32_t ms)
{
    uint32_t i,j;
    
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 200000; j++)//600M--1ms
        __NOP();//asm("NOP");   
    }
    return ;
}
void delayus(uint32_t us) //延时1us 实测为1.18us   延时10us 实测为10.02us   延时100us 实测为98.4us
{
    uint32_t i,j;
    
    for (i = 0; i < us; i++)
    {
        for (j = 0; j < 290; j++)//600M--1us
        __NOP();//asm("NOP");   
    }
    return ;
} 

void DelayTest(void)
{
    _systime.delay_ms(900);
}

uint32_t fullCameraBufferAddr;     
unsigned char * image;
uint64_t time;
uint64_t now;

//#define DMA0_DMA16_DriverIRQHandler DMA_CH_0_16_DriverIRQHandler
volatile bool g_Transfer_Done = false;
    edma_handle_t g_EDMA_Handle;
    edma_config_t userConfig;

    edma_transfer_config_t transferConfig;

/* User callback function for EDMA transfer. */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}
void TMR2_IRQHandler(void);
AT_NONCACHEABLE_SECTION_INIT(uint32_t databuff[4]) = {0x01, 0x02, 0x03, 0x04};
AT_NONCACHEABLE_SECTION_INIT(uint32_t databuff2[4]) = {0x00, 0x00, 0x00, 0x00};
int main(void)
{        
    uint8_t count = 0;

    BOARD_ConfigMPU();                   /* 初始化内存保护单元 */      
    BOARD_InitSDRAMPins();               /* SDRAM初始化 */
    BOARD_BootClockRUN();                /* 初始化开发板时钟 */   
    BOARD_InitDEBUG_UARTPins();          //UART调试口管脚复用初始化 
    BOARD_InitDebugConsole();            //UART调试口初始化 可以使用 PRINTF函数          
    LED_Init();                          //初始化核心板和开发板上的LED接口
    LQ_UART_Init(LPUART1, 256000);       //串口1初始化 可以使用 printf函数
    _systime.init();                     //开启systick定时器
    NVIC_SetPriorityGrouping(2);/*设置中断优先级组  0: 0个抢占优先级16位个子优先级 
                                 *1: 2个抢占优先级 8个子优先级 2: 4个抢占优先级 4个子优先级 
                                 *3: 8个抢占优先级 2个子优先级 4: 16个抢占优先级 0个子优先级
                                 */
    
    //Test_PIT();

    /****************打印系统时钟******************/
    PRINTF("\r\n");
    PRINTF("*****LQ_1052*****\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
    PRINTF("Video:           %d Hz\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));
    PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
    PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
    PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
    PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
    PRINTF("USB1PLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
    PRINTF("USB1PLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
    PRINTF("USB1PLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
    PRINTF("USB1PLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
    PRINTF("USB1PLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
    
//-----------------------------------------------------------------------------------------  
//  测试函数都是死循环，每次只能开启一个，综合应用需自行调用各模块初始化函数
//-----------------------------------------------------------------------------------------
//    Test_ADC();          //测试ADC并显示在TFT1.8上               电磁传感器及电源电压监控  
//    Test_UART();         //测试UART及中断                      蓝牙、USB转TTL调试及通信  
//    Test_LED();          //测试GPIO输出口     LED显示及外设控制             
//    Test_GPIO_KEY();     //测试GPIO输入+TFT1.8                   按键检测功能并显示在TFT1.8上              
//    Test_GPIO_ExInt();   //测试GPIO输入及外部中断              按键、中断检测功能    
//    Test_PIT();          //测试PIT定时中断功能  
//    Test_OLED();         //测试OLED
//    Test_RNG();          //随机数产生      
//    Test_9AX();          //九轴测试 
//    Test_MPU6050();      //测试I2C及6轴陀螺仪功能              陀螺仪及加速度功能 
//    Test_ahrs();         //测试六轴姿态解算   
//    Test_ANO_DT();       //匿名上位机观察数据波形  不使用DMA模式，请将宏定义#define USE_DMA  0  
//    Test_UART_DMA();     //匿名上位机观察波形，使用DMA传输，不占用CPU时间，点击匿名上位机的陀螺仪校准，核心板红灯亮
//    Test_PWM_nCH();      //测试PWM                             PWM1， 产生PWM波  用示波器观察
//    Test_QTMR_PWM();     //测试QTMR 产生的PWM                  示波器观察
//    Test_QTMR_QD();      //测试QTMR正交解码功能
//    Test_Servo();        //数字舵机测试      注意 初次使用此函数时，舵机不要装舵盘，防止舵机卡死，设置中值后再装车，可以自行设置中值
//    Test_Motor();        //直流电机驱动测试，                  用龙邱全桥驱动板  注意： 电机1： 使用 L5  M5   电机2：使用A8  A9  电机3：使用 C9 B9  电机4：A10 J1
//    Test_ENCoder();      //编码器采集
//    Test_Camera_Reprot();  //上位机看图  7725RGB565 / 7725 灰度 / 神眼灰度
//    Test_SGP18_OV7725();   //测试OV7725RGB 和TFT1.8
    //Test_SGP18_Camera();   //测试神眼 Or 7725 二值化 + TFT1.8  注意，7725使用灰度图像时使用YUYV格式 需要配置 cameraConfig = { .pixelFormat = kVIDEO_PixelFormatYUYV }
//----------------------------------------------------------------------------------------- 
    PRINTF("经过了 %d \r\n", time);
    time = MeasureRunTime_ms(DelayTest);

    const qtmr_config_t QuadTimer_1_Channel_0_config = {
        .primarySource = kQTMR_ClockDivide_2,
        .secondarySource = kQTMR_Counter0InputPin,
        .enableMasterMode = false,
        .enableExternalForce = false,
        .faultFilterCount = 0,
        .faultFilterPeriod = 0,
        .debugMode = kQTMR_RunNormalInDebug
    };
    /*QTMR输入捕捉*/
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */
    CLOCK_EnableClock(kCLOCK_Xbar1);            /* xbar1 clock (xbar1_clk_enable): 0x03U */

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_14_XBAR1_INOUT12,        /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
        (~(IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
        | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL(0x01U) /* QTIMER2 TMR0 input select: input from XBAR */
        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U) /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
        );
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputQtimer2Tmr0Input); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT90 output assigned to QTIMER2_TMR0_INPUT */

    //QTMR_GetDefaultConfig(&qtmrcpatureconfig);
    QTMR_Init(TMR2, kQTMR_Channel_0, &QuadTimer_1_Channel_0_config);
    QTMR_SetupInputCapture(TMR2, kQTMR_Channel_0, kQTMR_Counter0InputPin, false, true, kQTMR_FallingEdge);
    QTMR_EnableDma(TMR2, kQTMR_Channel_0, kQTMR_InputEdgeFlagDmaEnable);
    //PWM_StartTimer(PWM1, 1u << kPWM_Module_0); //开启定时器
    /*eDMA初始化*/
    DMAMUX_Init(DMAMUX);
    //DMAMUX_EnableAlwaysOn(DMAMUX, 0, true);
    DMAMUX_SetSource(DMAMUX, 0, kDmaRequestMuxQTIMER2CaptTimer0);
    DMAMUX_EnableChannel(DMAMUX, 0);



    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(DMA0, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, DMA0, 0);
    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);

    EnableIRQ(TMR2_IRQn);
    QTMR_StartTimer(TMR2, kQTMR_Channel_0, kQTMR_PriSrcRiseEdge);
    /*eDMA传输*/

    EDMA_PrepareTransfer(&transferConfig, databuff, sizeof(databuff[0]), databuff2, sizeof(databuff[2])
        , sizeof(databuff[0]), sizeof(databuff), kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    EDMA_StartTransfer(&g_EDMA_Handle);

    /* Wait for EDMA transfer finish */
    while (g_Transfer_Done != true)
    {
    }

    TFTSPI_Init();                 //TFT1.8初始化  
    TFTSPI_CLS(u16BLUE);           //清屏
    LQ_PWM_Init(PWM2, kPWM_Module_0, kPWM_PwmA_B, 12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     A8 A9
    LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmA_B, 12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     B9 C9
    //camera_init_1();
    _systime.delay_ms(200);
    //LQ_Camera_Init();
    uint16_t color = 0;
    LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_0, 2000, 0);
    LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_1, 4000, 0);

    while(1)
    {                                                 

    }        
    
    
    
    
    
    
}

void TMR2_IRQHandler(void)
{
    int a = 0;
    return;
}