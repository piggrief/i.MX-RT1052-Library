/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技RT1052核心板
【编    写】Z
【备    注】
【软件版本】V1.0
【最后更新】2018年11月27日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
----------------------------------------------------------------
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

/*******************************************************************************
*  SDK提供了两种在Noncacheable区定义缓冲区和变量的方法：
*  AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes)
*  AT_NONCACHEABLE_SECTION(var)
******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH], FRAME_BUFFER_ALIGN); //定义摄像头数据缓存区
static void BOARD_PullCameraPowerDownPin(bool pullUp)  //闪光灯
{
//    if (pullUp)
//    {
//        GPIO_PinWrite(GPIO1, 4, 1);
//    }
//    else
//    {
//        GPIO_PinWrite(GPIO1, 4, 0);
//    }
}
static void BOARD_PullCameraResetPin(bool pullUp)  //摄像头复位引脚
{
    /* Reset pin is connected to DCDC_3V3. */
    return;
}

#ifdef LQMT9V034 //LQMT9V034模块
static LQMT9V034_resource_t LQMT9V034Resource = {   //摄像头初始化结构体
    .sccbI2C = LPI2C1,
 
    .inputClockFreq_Hz = 27000000,
};

camera_device_handle_t cameraDevice = {           //摄像头驱动配置结构体
    .resource = &LQMT9V034Resource,
    .ops = &LQMT9V034_ops,
};
#else //LQOV7725模块
static ov7725_resource_t ov7725Resource = {       //摄像头初始化结构体
    .sccbI2C = LPI2C1,
    .pullResetPin = BOARD_PullCameraResetPin,
    .pullPowerDownPin = BOARD_PullCameraPowerDownPin,
    .inputClockFreq_Hz = 24000000,                 //修改这里可以修改帧率 具体看static const ov7725_clock_config_t ov7725ClockConfigs[]
};
camera_device_handle_t cameraDevice = {            //摄像头驱动配置结构体
    .resource = &ov7725Resource,
    .ops = &ov7725_ops,
};
#endif

/* connect to CSI. */
static csi_resource_t csiResource = {   //指向csi基地址
    .csiBase = CSI,
};

static csi_private_data_t csiPrivateData;  //csi私有数据

camera_receiver_handle_t cameraReceiver = {//摄像头接收配置结构体
    .resource = &csiResource, 
    .ops = &csi_ops, 
    .privateData = &csiPrivateData,
};


extern void CSI_DriverIRQHandler(void);

void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】摄像头CSI接口和I2C接口时钟配置
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void BOARD_InitCameraResource(void)
{
    LPI2C_Init(LPI2C1, 400000);
    /* CSI MCLK select 24M. */
    /*
    * CSI clock source: 
    * 00 derive clock from osc_clk (24M)
    * 01 derive clock from PLL2 PFD2
    * 10 derive clock from pll3_120M
    * 11 derive clock from PLL3 PFD1
    */
    CLOCK_SetMux(kCLOCK_CsiMux, 2);
    /*
    * CSI clock divider:
    *
    * 000 divide by 1
    * 001 divide by 2
    * 010 divide by 3
    * 011 divide by 4
    * 100 divide by 5
    * 101 divide by 6
    * 110 divide by 7
    * 111 divide by 8
    */
    CLOCK_SetDiv(kCLOCK_CsiDiv, 2);
    
    /*
    * For RT1050, there is not dedicate clock gate for CSI MCLK, it use CSI
    * clock gate.
    */
    
    /* Set the pins for CSI reset and power down. */
//    gpio_pin_config_t pinConfig = {
//        kGPIO_DigitalOutput, 1,
//    };
//    
//    GPIO_PinInit(GPIO1, 4, &pinConfig);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】摄像头初始化
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
camera_config_t cameraConfig;   //摄像头配置结构体
#ifdef LQMT9V034 //LQMT9V034模块
    // Configure camera device and receiver.
    camera_config_t cameraConfig = {
        .pixelFormat   = kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatBGR565,
        .bytesPerPixel = APP_BPP,//   每个像素点几个数据
        .resolution = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT), //分辨率
        .frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,                //行间隔
        .interface     = kCAMERA_InterfaceGatedClock,                            //摄像机接口类型
        .controlFlags = APP_CAMERA_CONTROL_FLAGS,
        .framePerSec   = 60,                                                     //fps 修改需要修改曝光时间 和 分辨率 配合
    };
#else            //LQOV7725模块
    camera_config_t cameraConfig = {
        .pixelFormat = kVIDEO_PixelFormatRGB565,//kVIDEO_PixelFormatRGB565,//kVIDEO_PixelFormatYUYV,
        .bytesPerPixel = APP_BPP,
        .resolution = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT),  //分辨率
        .frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,                 //行间隔
        .interface =  kCAMERA_InterfaceCCIR656,                                   //摄像机接口类型
        .controlFlags = APP_CAMERA_CONTROL_FLAGS,
        .framePerSec = 75,                                                        //fps 修改需要修改plck 和 分辨率 配合
    };
#endif
void LQ_Camera_Init(void)  
{
    BOARD_InitCSIPins();      //摄像头CSI管脚复用  
    BOARD_InitLPI2C1Pins();   //摄像头 I2C1管脚复用  
    BOARD_InitCameraResource();//csi and I2C clock config   
    
     /*
    * Configure the camera.
    */
    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);  //初始化csi
    
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);                  //初始化相机配置
    
    CAMERA_DEVICE_Start(&cameraDevice);                                //启动相机
    
    /* Submit the empty frame buffers to buffer queue. */
    for (uint32_t i = 0; i < APP_CAMERA_FRAME_BUFFER_COUNT; i++) //将空帧缓冲区提交到缓冲区队列
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(csiFrameBuf[i]));
    }
    
    CAMERA_RECEIVER_Start(&cameraReceiver);   // 启动接收camera数据
    delayms(200);        //延时200毫秒  摄像头不是重新上电 可以不要延时
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】上位机看图 不要用DAPlink的串口，容易卡死 使用山外上位机 使用神眼时上位机设置为灰度 7725时，上位机设置为RGB565 小端模式
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_Camera_Reprot(void)
{
    uint32_t fullCameraBufferAddr;  
#if (defined LQOV7725) && (defined LQOV7725YUV)
    cameraConfig.pixelFormat = kVIDEO_PixelFormatYUYV;
#endif
    LQ_Camera_Init();
    delayms(200);        //延时200毫秒   
    uint8_t count = 0;
    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR)) {   //注意，使用csiFrameBuf数组时，最好关闭Cache 不然上次数据可能会存放在cache里面，造成数据错乱
        SCB_DisableICache();
    }
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_DisableDCache();
    }
    while (1)
    {     
        // Wait to get the full frame buffer to show. 
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //摄像头CSI缓存区已满
        {
        }       
        CAMERA_RECEIVER_Stop(&cameraReceiver);//停止csi接收
        for(count = 0; count < 4; count++)    //获取当前缓冲满的数组索引
        {
            if(fullCameraBufferAddr == (uint32_t )csiFrameBuf[count])
                break;
        }
        LQ_UART_PutChar(LPUART1, 0x01);  //帧头
        LQ_UART_PutChar(LPUART1, 0xfe);  //帧头
        for(int i = 0; i < APP_CAMERA_HEIGHT; i++)  //分辨率越高 ，出图越慢
        {
#if  (defined LQMT9V034) || (defined LQOV7725RGB)   //上位机看 7725 RGB565图像 和 神眼灰度图像  7725使用RGB565格式 cameraConfig = { .pixelFormat = kVIDEO_PixelFormatRGB565 }
            for(int j = 0; j < APP_CAMERA_WIDTH * 2; j++)
            {
                if(*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j) == 0xfe )  //防止错误发送帧尾
                {
                    *((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j) = 0xff;  
                }
                LQ_UART_PutChar(LPUART1, *((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j)); //发送数据
            }
//            for(int j = 0; j < APP_CAMERA_WIDTH; j++)  //觉得上面的指针不好理解，可以用下面的
//            {
//                if(csiFrameBuf[count][i][j]<<8  == 0xfe00)  //防止错误发送帧尾
//                {
//                    LQ_UART_PutChar(LPUART1, 0xff); //发送数据 
//                }
//                else
//                {
//                    LQ_UART_PutChar(LPUART1, (uint8_t)csiFrameBuf[count][i][j]); //发送数据
//                }
                
//                if(csiFrameBuf[count][i][j]>>8  == 0xfe )  //防止错误发送帧尾
//                {
//                    LQ_UART_PutChar(LPUART1, 0xff); //发送数据 
//                }
//                else
//                {
//                    LQ_UART_PutChar(LPUART1, (csiFrameBuf[count][i][j]>>8)); //发送数据
//                }
//                
//            }
#else     //上位机看 7725 的灰度图像  注意，看灰度图像时，7725使用YUYV格式 cameraConfig = { .pixelFormat = kVIDEO_PixelFormatYUYV }
            for(int j = 1; j < APP_CAMERA_WIDTH * 2; j+=2)  //UYVY 格式图像 ，Y是灰度，看灰度图像只输出Y值
            {
                if(*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j) == 0xfe )  //防止错误发送帧尾
                {
                    *((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j) = 0xff;  
                }
                LQ_UART_PutChar(LPUART1, *((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j)); //发送数据
            }
#endif
        }  
        LQ_UART_PutChar(LPUART1, 0xfe);  //帧尾
        LQ_UART_PutChar(LPUART1, 0x01);  //帧尾
        
         // Return the camera buffer to camera queue. 
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列
        CAMERA_RECEIVER_Start(&cameraReceiver);   // 启动接收camera数据
        LED_Ctrl(LED_R, RVS); //EVK LED闪烁  
    }
}



