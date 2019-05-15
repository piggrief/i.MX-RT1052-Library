# ifndef _PIGMT9V032_H_
# define _PIGMT9V032_H_

#define COL     188//图像宽度   范围1-752     K60采集不允许超过188
#define ROW     120//图像高度	范围1-480


//摄像头命令枚举
typedef enum
{
    INIT = 0,               //摄像头初始化命令
    AUTO_EXP,               //自动曝光命令
    EXP_TIME,               //曝光时间命令
    FPS,                    //摄像头帧率命令
    SET_COL,                //图像列命令
    SET_ROW,                //图像行命令
    LR_OFFSET,              //图像左右偏移命令
    UD_OFFSET,              //图像上下偏移命令
    GAIN,                   //图像偏移命令
    CONFIG_FINISH,          //非命令位，主要用来占位计数

    SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
    GET_STATUS,             //获取摄像头配置命令
    GET_VERSION,            //固件版本号命令

    SET_ADDR = 0XFE,        //寄存器地址命令
    SET_DATA                //寄存器数据命令
}CMD;

extern unsigned char mt9v032_finish_flag;  //一场图像采集完成标志位
extern unsigned char image_1[ROW][COL];      //内部使用


//引脚配置

#define MT9V032_COF_UART_1    LPUART5 //配置摄像头所使用到的串口     
#define MT9V032_COF_UART_2    LPUART3 //配置摄像头所使用到的串口     

typedef enum
{
    Image_Collecting,
    Image_CollectFinish,
    Image_Dealing,
    Image_DealingFinsh
}ImageDealState;

extern ImageDealState Image_1_DealState_Now;
extern ImageDealState Image_2_DealState_Now;

void   mt9v032_cof_uart_interrupt(void);
void   VSYNC_1(void);
void   VSYNC_2(void);
void   camera_init_1(void);
void   camera_init_2(void);
void   seekfree_sendimg_032(void);
void   row_finished(void);
void sendimg(void);

# endif