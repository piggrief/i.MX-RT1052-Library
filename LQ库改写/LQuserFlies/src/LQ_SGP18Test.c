
#include "include.h"
/*******************************************************************************
*  SDK提供了两种在Noncacheable区定义缓冲区和变量的方法：
*  AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes)
*  AT_NONCACHEABLE_SECTION(var)
******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint16_t lcdFrameBuf[2][LCD_HEIGHT][LCD_WIDTH], FRAME_BUFFER_ALIGN);               //LCD数据缓存区

uint8_t counter;       //LCD有两个缓冲区，一个当前显示用，一个缓冲用

int OFFSET0=0;      //最远处，赛道中心值综合偏移量
int OFFSET1=0;      //第二格
int OFFSET2=0;      //最近，第三格


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】oled + camera test
【软件版本】V1.0
【最后更新】2018年11月7日 
【函数名】
【返回值】无
【参数值】无
【实例】 TFT1.8显示OV7725 RGB图像测试  7725分辨率设置为320*240
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_SGP18_OV7725(void)
{
    TFTSPI_Init();               //TFT1.8初始化     
    uint32_t fullCameraBufferAddr;     
    LQ_Camera_Init();
//    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR)) {   
//        SCB_DisableICache();
//    }
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {//注意，使用csiFrameBuf数组时，最好关闭Cache 不然上次数据可能会存放在cache里面，造成数据错乱
        SCB_DisableDCache();
    }
    delayms(200);        //延时200毫秒     
    while (1)
    {     
        // Wait to get the full frame buffer to show. 
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //摄像头CSI缓存区已满
        {
        }   
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列        
        TFTSPI_Set_Pos(0,0,APP_CAMERA_WIDTH/2,APP_CAMERA_HEIGHT/2);
        for(int i = 0; i < APP_CAMERA_HEIGHT; i+=2)  //隔一行取一行
        {
            for(int j = 0; j < APP_CAMERA_WIDTH ; j+=2)//隔一列取一列
            {
                TFTSPI_Write_Word (*((uint16_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH + j)); //显示数据
            }
        }
        
        LED_Color_Reverse(red); //EVK LED闪烁    
    }
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】oled + camera test
【软件版本】V1.0
【最后更新】2018年11月7日 
【函数名】
【返回值】无
【参数值】无
【实例】 TFT1.8显示神眼二值化 图像测试  神眼分辨率设置为752*480
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_SGP18_Camera(void)
{
    TFTSPI_Init();               //TFT1.8初始化  
    TFTSPI_CLS(u16WHITE);           //清屏
    uint32_t fullCameraBufferAddr;   
#ifdef LQOV7725
    cameraConfig.pixelFormat = kVIDEO_PixelFormatYUYV;
#endif
    LQ_Camera_Init();
    delayms(200);        //延时200毫秒  
    uint16_t color = 0;
//    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR)) {   
//        SCB_DisableICache();
//    }
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {//注意，使用csiFrameBuf数组时，最好关闭D-Cache 不然上次数据可能会存放在cache里面，造成数据错乱
        SCB_DisableDCache();
    }
    while (1)
    {     
        // Wait to get the full frame buffer to show. 
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //摄像头CSI缓存区已满
        {
        }   
        
#ifdef LQMT9V034   
        TFTSPI_Set_Pos(0,0,(uint8_t)(APP_CAMERA_WIDTH/2-1) ,APP_CAMERA_HEIGHT);//注意 设置显示大小要与下面的实际显示大小相等，不然会显示不出来或者花屏
        for(int i = 0; i < APP_CAMERA_HEIGHT; i+=2)  //  480/4/2/2 = 30
        {
            for(int j = 0; j < APP_CAMERA_WIDTH*2; j+=2)//隔2列取一列  752*2/4/4 = 188   //两行数据 一行94像素
            {
                //灰度显示
                color = 0;
                color=(((*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j))>>3))<<11;
                color=color|((((*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j))>>2))<<5);
                color=color|(((*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j))>>3));
                TFTSPI_Write_Word(color);
                //二值化显示
//                if(*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j) > 0x60)  //阈值0x60 二值化显示
//                  TFTSPI_Write_Word (0xffff); //显示数据
//                else
//                  TFTSPI_Write_Word (0x0000); //显示数据
            }
        }
#else  // 7725 的灰度图像  注意，看灰度图像时，7725使用YUYV格式 cameraConfig = { .pixelFormat = kVIDEO_PixelFormatYUYV }
//        TFTSPI_Set_Pos(0,0,APP_CAMERA_WIDTH/2,APP_CAMERA_HEIGHT/2);
//        for(int i = 0; i < APP_CAMERA_HEIGHT; i+=2)  //隔2行取一行 240 / 2 = 120
//        {
//            for(int j = 1; j < APP_CAMERA_WIDTH *2 ; j+=4)//隔4列取一列 摄像头UYVY格式  Y是灰度，320/2=160
//            {
//                if(*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j) > 0x60)  //阈值0x60 二值化显示
//                  TFTSPI_Write_Word (0xffff); //显示数据
//                else
//                  TFTSPI_Write_Word (0x0000); //显示数据
//            }
//        }
        TFTSPI_Set_Pos(0,0,APP_CAMERA_WIDTH/4-1,APP_CAMERA_HEIGHT/4);
        for(int i = 0; i < APP_CAMERA_HEIGHT; i+=4)  //隔4行取一行 240 / 4 = 60
        {
            for(int j = 1; j < APP_CAMERA_WIDTH *2 ; j+=8)//隔8列取一列 摄像头UYVY格式  Y是灰度，320/4=80
            {
                if(*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j) > 0x60)  //阈值0x60 二值化显示
                  TFTSPI_Write_Word (0xffff); //显示数据
                else
                  TFTSPI_Write_Word (0x0000); //显示数据
            }
        }
#endif
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列  
        LED_Color_Reverse(red); //EVK LED闪烁  
    }
}



