#ifndef __LQ_CSI_H
#define __LQ_CSI_H


#define LQMT9V034    //选择使用神眼摄像头模块
//#define LQOV7725   //两者选其一

#ifdef LQMT9V034 //LQMT9V034模块
#define APP_CAMERA_WIDTH  (IMAGEW)
#define APP_CAMERA_HEIGHT (IMAGEH/2)  //一个uint16_t里面装了两个像素， 一行IMAGEW 里面其实装了两行的像素点，所以高度/2
#define APP_BPP 2 //像素格式，

#else            //LQOV7725模块
//#define LQOV7725RGB   //使用7725RGB   使用上位机看图时，选择RGB还是灰度
#define LQOV7725YUV   //使用7725灰度  使用上位机看图时，选择RGB还是灰度
#define APP_CAMERA_WIDTH  320
#define APP_CAMERA_HEIGHT 240
/* Pixel format YUV422, bytesPerPixel is 2. */
#define APP_BPP 2 //像素格式
#endif

#define APP_CAMERA_FRAME_BUFFER_COUNT 4 
#define FRAME_BUFFER_ALIGN 64  //对齐 
/*使用TFT1.8寸屏时*/
#define  Use_ROWS  160    //使用图像高度
#define  Use_Line  128   //使用图像宽度

#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_VsyncActiveLow| kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge) //使用摄像头的上升沿
extern void LQ_Camera_Init(void);
extern camera_device_handle_t cameraDevice;    
extern camera_receiver_handle_t cameraReceiver;
extern uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH];
extern void Test_Camera_Reprot(void);
extern camera_config_t cameraConfig;   //摄像头配置结构体
#endif