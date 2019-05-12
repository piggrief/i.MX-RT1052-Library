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


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】PXP初始化
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
pxp_ps_buffer_config_t psBufferConfig;  //PXP输入配置结构体
pxp_output_buffer_config_t outputBufferConfig;//PXP输出配置结构体
void LQ_PXP_Init(void)
{
       //Configure the PXP for color space conversion.
    PXP_Init(PXP);    //开时钟
    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);
    
    /* Input config. */
#ifdef LQMT9V034 //LQMT9V034模块
        psBufferConfig.pixelFormat =  kPXP_PsPixelFormatRGB888;//kPXP_PsPixelFormatRGB565,//kPXP_PsPixelFormatUYVY1P422,
#else                  //LQOV7725模块
        psBufferConfig.pixelFormat =  kPXP_PsPixelFormatRGB565;//kPXP_PsPixelFormatRGB565,//kPXP_PsPixelFormatUYVY1P422,
#endif
        psBufferConfig.swapByte = false;                 //高8位在前还是低8位在前
        psBufferConfig.bufferAddrU = 0U;
        psBufferConfig.bufferAddrV = 0U;
        psBufferConfig.pitchBytes = APP_CAMERA_WIDTH * APP_BPP;    //一行多少个字节 
        
    /* Output config. */
        outputBufferConfig.pixelFormat = kPXP_OutputPixelFormatARGB8888;//kPXP_OutputPixelFormatRGB888,
        outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
        outputBufferConfig.buffer1Addr = 0U;
        outputBufferConfig.pitchBytes = APP_LCD_WIDTH * 4;
        outputBufferConfig.width = APP_LCD_WIDTH;
        outputBufferConfig.height = APP_LCD_HEIGHT;
     
    
#ifdef LQMT9V034 //LQMT9V034模块
    PXP_EnableCsc1(PXP, false);              //不使能  LQMT9V034模块为单色格式
    PXP_SetProcessSurfaceScaler(PXP,IMAGEW/4, IMAGEH, 376, 272);//缩放图像, 神眼摄像头752 * 480分辨率，数据输出格式为 八位单色彩，也就是一个字节就是一个像素点，但是单片机的csi接口配置为rgb8888格式（就是摄像头本来一个字节表示一个像素点，变成了四个字节表示一个像素点），帧间隔
    PXP_SetProcessSurfacePosition(PXP, 52u, 0u, 428 - 1U, 272 - 1U);                //设置显示位置
#else  //LQOV7725模块
    PXP_SetCsc1Mode(PXP, kPXP_Csc1YCbCr2RGB);//设置 csc1  kPXP_Csc1YCbCr转换为rgb888
    PXP_EnableCsc1(PXP, false);               //当设置7725输出图像格式为yuyv时，使用使能csc1
    PXP_SetProcessSurfaceScaler(PXP, APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT, 376, 272);//缩放图像
    PXP_SetProcessSurfacePosition(PXP, 52u, 0u, 428 - 1U, 272 - 1U);                //设置显示位置
#endif
    
    PXP_SetProcessSurfaceBackGroundColor(PXP, 0u);  //设置背景颜色


    PXP_SetRotateConfig(PXP, kPXP_RotateOutputBuffer, kPXP_Rotate180, kPXP_FlipDisable);//图像旋转多少度，翻转与否
    delayms(200);        
}


