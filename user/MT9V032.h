/*!
* @file       MT9V032.h
* @brief      根据官方的相关摄像头库编写的针对总钻风摄像头的驱动程序
* @details
* @author     pig's grief
* @version    v1.0
* @date       2019-4-22
* @to do
*/
# ifndef _MT9V032_H_
# define _MT9V032_H_
#define IMAGEW 188
#define IMAGEH 120
#define APP_CAMERA_WIDTH  (IMAGEW)
#define APP_CAMERA_HEIGHT (IMAGEH/2)  //一个uint16_t里面装了两个像素， 一行IMAGEW 里面其实装了两行的像素点，所以高度/2
#define APP_BPP 2 //像素格式，
#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge) //使用摄像头的上升沿
#define APP_CAMERA_FRAME_BUFFER_COUNT 4 
#define FRAME_BUFFER_ALIGN 64  //对齐 

extern camera_device_handle_t cameraDevice;
extern camera_receiver_handle_t cameraReceiver;
extern camera_config_t cameraConfig;   //摄像头配置结构体


#endif