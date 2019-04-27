#include "LQ_control.h"

#include "status.h"
#include "Scheduler.h"



_pid_param_t  Motor_pid =      //电机PID参数结构体
{
     ._kp             =  1.5 ,         //P
     ._ki             =  0.5 ,         //I
     ._kd             =  0.1 ,         //D
     ._imax           = 1000 ,          //积分限幅
                                       
     ._pid_out_p      =   0,           //KP输出
     ._pid_out_i      =   0,           //KI输出
     ._pid_out_d      =   0,           //KD输出
     ._pid_out        =   0,           //pid输出
     
     ._integrator     =   0,           //积分值
     ._last_error     =   0,           //上一次误差
     ._last_derivative=   0,           //上次微分
     ._last_t         =   0,           //上次时间
     .scaler          =   100,           //缩放比

};
_pid_param_t  Servo_pid =    //舵机PID参数结构体
{
     ._kp             =  1.5 ,
     ._ki             =  0.5 ,
     ._kd             =  0.1 ,
     ._imax           =  500 , 
     
     ._pid_out_p      =   0,  
     ._pid_out_i      =   0,  
     ._pid_out_d      =   0,  
     ._pid_out        =   0,
     
     ._integrator     =   0, 
     ._last_error     =   0, 
     ._last_derivative=   0,
     ._last_t         =   0,  
     .scaler          =   1,  

};

uint8_t Left[Use_ROWS], Right[Use_ROWS];  //存放边线位置
/*找到赛道边缘黑线 未测试慎用 */
void LQ_Line_Hunting(uint8_t Pixle [Use_ROWS][Use_Line])
{

    uint8_t i,j;

    for(j = Use_Line/2; j > 0; j--)              //左边
    {
        if(!Pixle[Use_ROWS - 1][j] && !Pixle[Use_ROWS - 1][j-1])  //最近处一行   连续两个点黑点判定为边线
        {
            Left[Use_ROWS - 1] = j;                               //把左边边线位置存放数组中
        }
        
    }
    for(j = Use_Line/2; j < Use_Line; j++)       //右边
    {
        if(!Pixle[Use_ROWS - 1][j] && !Pixle[Use_ROWS - 1][j-1])  //最近处一行   连续两个点黑点判定为边线
        {
            Right[Use_ROWS - 1] = j;                               //把右边边线位置存放数组中
        }
    }
    if(Right[Use_ROWS - 1] - Left[Use_ROWS - 1] < 30)              // 右边线 - 左边线  < 30 个像素点  判定为斑马线
    {
        _status.stop = 1;                                            //停车 
        return;
    }
    if((Right[Use_ROWS - 1] == Use_Line/2 && Left[Use_ROWS - 1]) == Use_Line/2)              // 中线就是黑点  判定为断路区，不使用摄像头
    {
        _status.camera_run = 0;                                            //不跑摄像头 
        return;
    }
    else
    {
        _status.camera_run = 1;
    }
    
    for(i = Use_ROWS - 2; i > 0; i--)
    {
        for(j = Left[i+1] + 5; j > 0; j--)              //左边   边线应该连着  从上一行边线位置右边5个像素点处开始找
        {
            if(!Pixle[i][j] && !Pixle[i][j-1])  //连续两个点黑点判定为边线
            {
                Left[i] = j;
                break;
            }        
        }
        
        if(j == 0)    //没有黑边， 设置最左为黑边
        {
            Left[i] = j;
        }
        
        for(j = Right[i+1] - 5; j < Use_Line; j++)       //右边  从上一行边线位置左边5个像素点处开始找
        {
            if(!Pixle[i][j] && !Pixle[i][j-1])  //连续两个点黑点判定为边线
            {
                Right[i] = j;
                break;
            }   
        }
        
        if(j == Use_Line)    //没有黑边， 设置最右为黑边
        {
            Left[i] = j;
        }

    }
    
}
extern int OFFSET0;      //最远处，赛道中心值综合偏移量
extern int OFFSET1;      //第二格
extern int OFFSET2;      //最近，第三格
void seek_road(void)     //得到路径偏差函数 未测试慎用
{
    uint8_t i;
    uint64_t sum = 0;
    static uint32_t time;
    for(i = 0; i < (uint8_t)(Use_ROWS/3); i++)
    {
        sum += Left[i] + Right[i] - Use_Line; 
    }
    OFFSET0 = sum / (uint8_t)(Use_ROWS/3);
    sum = 0;
    
    for(i = (uint8_t)(Use_ROWS/3); i < (uint8_t)(Use_ROWS/3*2); i++)
    {
        sum += Left[i] + Right[i]  - Use_Line ; 
    }
    OFFSET1 = sum / ((uint8_t)(Use_ROWS/3*2) - (uint8_t)(Use_ROWS/3));
    sum = 0;
    
    for(i = (uint8_t)(Use_ROWS/3*2); i < Use_ROWS; i++)
    {
        sum += Left[i] + Right[i] - Use_Line; 
    }
    OFFSET2 = sum / (Use_ROWS - (uint8_t)(Use_ROWS/3*2));
    
    if(abs(OFFSET0) < 100 && abs(OFFSET1) < 100 && abs(OFFSET2) < 100 )   //直道
    {
        _status.straight_speed = 1;
    }
    else if(_status.straight_speed == 1 && abs(OFFSET0) > 100 )           //入弯
    {
        _status.brake_speed = 1;
        _status.straight_speed = 0; 
        
        
        time = _systime.get_time_ms();                                    //记住入弯的时间
    }
    else                                                                 //弯道
    {
        _status.curve_speed = 1;
    }

    
    if(_systime.get_time_ms() - time > 1000)                              //1s后取消入弯
    {
        _status.brake_speed = 0;
    }
}

void LQ_Control_Servo(void)    //PID舵机控制
{
    pid.get_pid(&Servo_pid, OFFSET2);
    LQ_SetServoDty(Servo_pid._pid_out + 3440);
}

void LQ_Control_Motor(int16_t velocity) //PID电机控制
{
    float error;
    if((fabs)(Servo_pid._pid_out) > 200 )    //大角度转弯  使用差速  
    {
    
    }
    if(_status.straight_speed)  //判断是直道
    {
        error = STRAIGHT_SPEED - velocity;    
    }
    else if(_status.brake_speed)  //判断入弯  减速
    {
        error = BRAKE_SPEED - velocity;   
    }
    else if(_status.curve_speed)  //判断是弯道
    {
        error = CURVE_SPEED - velocity;   
    } 
    pid.get_pid(&Motor_pid, error);
    if(Motor_pid._pid_out > 5000)  //限幅
    {
        Motor_pid._pid_out = 5000;
    }
    if(Motor_pid._pid_out < -2000) //限幅
    {
        Motor_pid._pid_out = -2000;
    }
    LQ_PWMA_B_SetDuty(PWM1, kPWM_Module_3, 2000 + Motor_pid._pid_out, 0);
}

/****************xia************************///
void pxp_init(void)
{
       //Configure the PXP for color space conversion.
    PXP_Init(PXP);    //开时钟
    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);
    
    
#ifdef LQMT9V034 //LQMT9V034模块
    PXP_EnableCsc1(PXP, false);              //不使能  LQMT9V034模块为单色格式

    PXP_SetProcessSurfaceScaler(PXP,IMAGEW/4, IMAGEH,  Use_Line, Use_ROWS);//缩放图像, 神眼摄像头752 * 480分辨率，数据输出格式为 八位单色彩，也就是一个字节就是一个像素点，但是单片机的csi接口配置为rgb888格式（就是摄像头本来一个字节表示一个像素点，变成了四个字节表示一个像素点）
    PXP_SetProcessSurfacePosition(PXP, 0u, 0u, Use_Line - 1U, Use_ROWS - 1U);                //设置显示位置
#else  //LQOV7725模块
    PXP_SetCsc1Mode(PXP, kPXP_Csc1YCbCr2RGB);//设置 csc1  kPXP_Csc1YCbCr转换为rgb
    PXP_EnableCsc1(PXP, false);               //使能csc1
    
    PXP_SetProcessSurfaceScaler(PXP,APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT, Use_Line, Use_ROWS);//缩放
    PXP_SetProcessSurfacePosition(PXP, 0u, 0u, Use_Line - 1U, Use_ROWS - 1U);                //设置显示位置
#endif
   
    PXP_SetProcessSurfaceBackGroundColor(PXP, 0u);  //设置背景颜色

    PXP_SetRotateConfig(PXP, kPXP_RotateOutputBuffer, kPXP_Rotate0, kPXP_FlipDisable);//图像旋转多少度，翻转与否
    
    
    /* Input config. */
#ifdef LQMT9V034 //LQMT9V034模块
        psBufferConfig.pixelFormat =  kPXP_PsPixelFormatRGB888;//像素格式
#else                  //LQOV7725模块
        psBufferConfig.pixelFormat =  kPXP_PsPixelFormatRGB565;//像素格式
#endif
        psBufferConfig.swapByte = false;                 //高8位在前还是低8位在前
        psBufferConfig.bufferAddrU = 0U;
        psBufferConfig.bufferAddrV = 0U;
        psBufferConfig.pitchBytes = APP_CAMERA_WIDTH * APP_BPP;    //一行多少个字节 
        
    /* Output config. */
        outputBufferConfig.pixelFormat = kPXP_OutputPixelFormatARGB8888;//像素格式
        outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
        outputBufferConfig.buffer1Addr = 0U;
        outputBufferConfig.pitchBytes = APP_LCD_WIDTH * 4;
        outputBufferConfig.width = APP_LCD_WIDTH;
        outputBufferConfig.height = APP_LCD_HEIGHT;
}
void sys_init(void)
{
    BOARD_ConfigMPU();                   //Config MPU.      
    BOARD_InitSDRAMPins();
    BOARD_BootClockRUN();                //Configure clock            
    LED_Init();                          //开发板上LED0会用到
    LQ_UART_Init(LPUART1, 115200);       //串口1初始化 可以使用 printf函数
    _systime.init();                     //开启systick定时器
    NVIC_SetPriorityGrouping(2);//设置中断优先级0: 0个抢占优先级16位个子优先级 1: 2个抢占优先级 8个子优先级 2: 4个抢占优先级 4个子优先级 3: 8个抢占优先级 2个子优先级 4: 16个抢占优先级 0个子优先级 


     /*初始化电机PWM接口  kPWM_PwmA  L5      kPWM_PwmB M5 */
    LQ_PWM_Init(PWM1, kPWM_Module_3, kPWM_PwmA_B, 12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz
    
    /*初始化舵机PWM接口  kPWM_PwmA  M3  */
    LQ_PWM_Init(PWM2, kPWM_Module_3, kPWM_PwmA,  200);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz
    
    LQ_SetServoDty(3440);  //中值
    
    LQ_ENC_Init(ENC1);                   //正交解码1初始化
    LQ_ENC_Init(ENC2);                   //正交解码2初始化
    LQADC_Init(ADC1);                    //ADC初始化
//    LQ_PIT_Init(kPIT_Chnl_0, 10000);     //初始化PIT chnl0中断，10 000us
    LQ_KEY_Init();                       //按键初始化
    LCD_Init();               //OLED初始化 
    UART_DMA_Init();          //串口DMA初始化，用于上报数据
    LQ_Camera_Init();
    pxp_init();
    
}
/*未验证  只有框架  感兴趣的可以自行完善*/
       
void 
TEST(void)
{
    sys_init();                     //外设初始化
    _sched.init();                  //初始化任务调度器
    
    uint32_t current_time;   
    uint64_t last_time;
 
    while(1)
    {
        current_time = _systime.get_time_us() - last_time;
        if(current_time > 5000)  // 一个周期5ms
        {         
            last_time = _systime.get_time_us();
            GPIO_PinReverse(GPIO2, 22);      //用示波器观察，看时序是否正常
            _sched.run(5000);                //任务
        }
    }
}





