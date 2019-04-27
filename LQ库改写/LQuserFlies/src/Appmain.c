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
int main(void)
{        
  
    BOARD_ConfigMPU();                   /* 初始化内存保护单元 */      
    BOARD_InitSDRAMPins();               /* SDRAM初始化 */
    BOARD_BootClockRUN();                /* 初始化开发板时钟 */   
    BOARD_InitDEBUG_UARTPins();          //UART调试口管脚复用初始化 
    BOARD_InitDebugConsole();            //UART调试口初始化 可以使用 PRINTF函数          
    LED_Init();                          //初始化核心板和开发板上的LED接口
    LQ_UART_Init(LPUART1, 115200);       //串口1初始化 可以使用 printf函数
    _systime.init();                     //开启systick定时器
    NVIC_SetPriorityGrouping(2);/*设置中断优先级组  0: 0个抢占优先级16位个子优先级 
                                 *1: 2个抢占优先级 8个子优先级 2: 4个抢占优先级 4个子优先级 
                                 *3: 8个抢占优先级 2个子优先级 4: 16个抢占优先级 0个子优先级
                                 */
    Test_PIT();

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
//    Test_SGP18_Camera();   //测试神眼 Or 7725 二值化 + TFT1.8  注意，7725使用灰度图像时使用YUYV格式 需要配置 cameraConfig = { .pixelFormat = kVIDEO_PixelFormatYUYV }
//----------------------------------------------------------------------------------------- 
    PRINTF("经过了 %d \r\n", time);
    time = MeasureRunTime_ms(DelayTest);
//综合测试    
    while(1)
    {
        //LED_Init();
        //LQ_KEY_Init(); 
        TFTSPI_Init();                 //TFT1.8初始化  
        TFTSPI_CLS(u16BLUE);           //清屏
        
        ///*GPIO 外部中断配置开始*/
        //CLOCK_EnableClock(kCLOCK_Iomuxc);                         // IO口时钟使能
        //IOMUXC_SetPinMux(IOMUXC_SNVS_WAKEUP_GPIO5_IO00,0U);       // 设置管脚复用功能                          
        //IOMUXC_SetPinConfig(IOMUXC_SNVS_WAKEUP_GPIO5_IO00,0xF080);// 配置管脚
        //
        //gpio_pin_config_t GPIO_Input_Config =                     // GPIO初始化结构体
        //{
        //    kGPIO_DigitalInput,                                   // GPIO为输入方向
        //    1,                                                    // 高电平
        //    kGPIO_IntFallingEdge,                                 // 下降沿触发中断
        //};
        //GPIO_PinInit(GPIO5, 0, &GPIO_Input_Config);               // GPIO输入口，中断
        //
        //GPIO_PortEnableInterrupts(GPIO5,1<<0);			          // GPIO5_00中断使能
        ////优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断
        //NVIC_SetPriority(GPIO5_Combined_0_15_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
        //
        //EnableIRQ(GPIO5_Combined_0_15_IRQn);			          //使能GPIO5_0~15IO的中断  
        //
      
        //IOMUXC_SetPinMux(IOMUXC_SNVS_WAKEUP_GPIO5_IO00,0U);          //管脚L6
        //IOMUXC_SetPinMux(IOMUXC_SNVS_PMIC_STBY_REQ_GPIO5_IO02,0U);   //管脚L7
        //IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_GPIO2_IO28,0U);           //管脚D13
        //IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_03_GPIO1_IO03,0U);        //管脚G11
        //
        //IOMUXC_SetPinConfig(IOMUXC_SNVS_WAKEUP_GPIO5_IO00,0xF080); 
        //IOMUXC_SetPinConfig(IOMUXC_SNVS_PMIC_STBY_REQ_GPIO5_IO02,0xF080);
        //IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_12_GPIO2_IO28,0xF080);
        //IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_03_GPIO1_IO03,0xF080);
        //
        //gpio_pin_config_t GPIO_Config = {kGPIO_DigitalInput,    //GPIO为输入方向
        //                           1,                    //高电平
        //                           kGPIO_NoIntmode,      //不触发中断
        //                           };
        //
        //GPIO_PinInit(GPIO5,  0, &GPIO_Config);        // GPIO输入口，非中断
        //GPIO_PinInit(GPIO5, 02, &GPIO_Config);        // GPIO输入口，非中断
        //GPIO_PinInit(GPIO1, 03, &GPIO_Config);        // GPIO输入口，非中断
        //GPIO_PinInit(GPIO2, 28, &GPIO_Config);        // GPIO输入口，非中断
        //
        //LQ_PWM_Init(PWM2, kPWM_Module_3, kPWM_PwmA_B,  200);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz //默认是M3  M4，
        //LQ_PWM_Init(PWM2, kPWM_Module_0, kPWM_PwmA_B, 12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     A8 A9
        //LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmA_B, 12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     B9 C9
        //LQ_PWM_Init(PWM2, kPWM_Module_2, kPWM_PwmB,   12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     A10
        //LQ_PWM_Init(PWM1, kPWM_Module_1, kPWM_PwmA,   12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     J1
        //LQ_PWM_Init(PWM1, kPWM_Module_3, kPWM_PwmA_B, 12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     L5 M5
        //
        //LQ_ENC_Init(ENC1);   //正交解码初始化
        //LQ_ENC_Init(ENC2);
        //LQ_ENC_Init(ENC3);   //正交解码初始化
        //LQ_ENC_Init(ENC4);

        LQ_Camera_Init();
        short velocity1, velocity2, velocity3, velocity4;
        uint16_t color = 0;
        while(1)
        {                                          
         
          
          
        }        
    }
    
    
    
    
    
    
}
