#ifndef __LQ_CSI_H
#define __LQ_CSI_H


#define LQMT9V034    //选择使用神眼摄像头模块

#define APP_CAMERA_WIDTH  (188)
#define APP_CAMERA_HEIGHT (120/2)  //一个uint16_t里面装了两个像素， 一行IMAGEW 里面其实装了两行的像素点，所以高度/2
#define APP_BPP 2 //像素格式，

#define APP_CAMERA_FRAME_BUF  FER_COUNT 4 
#define FRAME_BUFFER_ALIGN 64  //对齐 64
/*使用TFT1.8寸屏时*/
#define  Use_ROWS  160    //使用图像高度
#define  Use_Line  128   //使用图像宽度

#define APP_CAMERA_CONTROL_FLAGS  (kCAMERA_VsyncActiveLow| kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)//(kCAMERA_DataLatchOnRisingEdge | kCAMERA_VsyncActiveLow) //使用摄像头的上升沿
extern void LQ_Camera_Init(void);
extern camera_device_handle_t cameraDevice;    
extern camera_receiver_handle_t cameraReceiver;
//extern uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH];
extern void Test_Camera_Reprot(void);
extern camera_config_t cameraConfig;   //摄像头配置结构体
#endif