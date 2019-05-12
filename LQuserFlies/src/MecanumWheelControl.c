/*!
* @file       MecanumWheelControl.c
* @brief      四轮麦轮控制部分的程序
* @details
* @author     pig's grief
* @version    v1.0
* @date       2019-2-12
* @to do      
*             解决编码器测速程序的DMA无法计数问题
*/

# include "include.h"
# include "MIMXRT1052.h"
//规定了每个轮子对应的FTM输出通道，调正反转应该这个数组
pwm_submodule_t Wheels_PWMChannel[4] = { PWMChannel_Use_W1,
                                         PWMChannel_Use_W2,
                                         PWMChannel_Use_W3,
                                         PWMChannel_Use_W4};

//PWM_Type *PWM_Type[4] = { PWMType_Use1, PWMType_Use2, PWMType_Use3, PWMType_Use4 };




///<summary>电机通道初始化</summary>
void Motor_init(void)
{
    LQ_PWM_Init(PWMType_Use1, Wheels_PWMChannel[0], kPWM_PwmA_B, 20000);
    LQ_PWM_Init(PWMType_Use2, Wheels_PWMChannel[1], kPWM_PwmA_B, 20000);
    LQ_PWM_Init(PWMType_Use3, Wheels_PWMChannel[2], kPWM_PwmA_B, 20000);
    LQ_PWM_Init(PWMType_Use4, Wheels_PWMChannel[3], kPWM_PwmA_B, 20000);
}

///<summary>对应PIDControl结构体内的f_Constructor</summary>
void PIDControl_Constructor(struct PIDControl* PID, float P_set, float D_set, float I_set, float I_Limit_Set, float MaxOutput_Set, float DeadBand_Set)
{
    PID->KP = P_set;
    PID->KD = D_set;
    PID->KI = I_set;
    PID->IntegralLimit = I_Limit_Set;
    PID->MaxOutput = MaxOutput_Set;
    PID->DeadBand = DeadBand_Set;

    PID->P_Output = 0;
    PID->D_Output = 0;
    PID->I_Output = 0;
    PID->Output = 0;
    PID->Last_Error = 0;
    PID->Last_Output = 0;
}

///<summary>PIDControl结构体内的TargetValue设定</summary>
void PID_SetTarget(struct PIDControl* PID,float TargetValue)
{
  PID->TargetValue = TargetValue;
}

///<summary>对应PIDControl结构体内的f_GetPIDControlValue</summary>
float GetPIDControlValue(struct PIDControl* PID, PIDControlModel PIDModel, float MeasuredValue)
{
    PID->Last_Error = PID->Error;
    PID->Last_Output = PID->Output;

    PID->MeasuredValue = MeasuredValue;
    PID->Error = PID->TargetValue - PID->MeasuredValue;

    PID->P_Output = PID->KP * PID->Error;

    if (PIDModel == PID_Control || PIDModel == PD_Control)
        PID->D_Output = PID->KD * (PID->Error - PID->Last_Error);
    else
        PID->D_Output = 0;
    if (PIDModel == PID_Control || PIDModel == PI_Control)
    {
        PID->I_Output += (PID->KI * PID->Error);

        if (PID->I_Output > PID->IntegralLimit)
            PID->I_Output = PID->IntegralLimit;
        if (PID->I_Output < -PID->IntegralLimit)
            PID->I_Output = -PID->IntegralLimit;
    }
    else
        PID->I_Output = 0;

    PID->Output = PID->P_Output + PID->D_Output + PID->I_Output;
    if (PID->Output > 0)
      PID->Output += PID->DeadBand;
    else
      PID->Output -= PID->DeadBand;

    if (PID->Output > PID->MaxOutput)
        PID->Output = PID->MaxOutput;
    if (PID->Output < -PID->MaxOutput)
        PID->Output = -PID->MaxOutput;

    return PID->Output;
}
///<summary>对应PIDControl结构体内的f_para_Refresh</summary>
void Para_Refresh(struct PIDControl* PID, float kp, float kd, float ki)
{
    PID->KP = kp;
    PID->KD = kd;
    PID->KI = ki;
}

/// <summary>
///使结构体PIDControl中的函数指针指向具体定义的函数
///<para>example:  PIDControl_FunctionDefine(&amp;PIDTest);</para>
///</summary>
/// <param name="PID">PIDControl结构体</param>
void PIDControl_FunctionDefine(struct PIDControl* PID)
{
    PID->f_Constructor = PIDControl_Constructor;
    PID->f_GetPIDControlValue = GetPIDControlValue;
    PID->f_para_Refresh = Para_Refresh;
}





struct PIDControl WheelControl[4];//四个轮子的PID控制数组
struct PIDControl Car_Speed_Rotate;//追灯转向闭环




///麦轮控制部分

/// <summary>
///设定整车运动速度（x、y轴平动速度和自转速度）
///<para>example:  SetTargetSpeed_Car(&amp;PIDTest, 10, 15, 20);</para>
///</summary>
/// <param name="TargetSpeed">用来存储三个速度的结构体</param>
/// <param name="Vx">x轴平动速度</param>
/// <param name="Vy">y轴平动速度</param>
/// <param name="W_yaw">自转角速度</param>
void SetTargetSpeed_Car(struct RunSpeed * TargetSpeed, float Vx, float Vy, float W_yaw)
{
    TargetSpeed->XSpeed = Vx;
    TargetSpeed->YSpeed = Vy;
    TargetSpeed->YawSpeed = W_yaw;
}
/// <summary>
///根据整车的运动状态来计算每个麦轮需要输出的速度值
///<para>example:  CalTargetSpeed_EachWheel(&amp;TargetSpeed);</para>
///</summary>
/// <param name="TargetSpeed">车整体的三个运动速度结构体</param>
void CalTargetSpeed_EachWheel(struct RunSpeed * TargetSpeed)
{
    ///O-长方形麦轮底盘的逆运动学模型
    ///用底盘运动状态解算四个轮子应有的速度
    WheelControl[0].TargetValue = TargetSpeed->YSpeed + TargetSpeed->XSpeed - TargetSpeed->YawSpeed;
    WheelControl[1].TargetValue = TargetSpeed->YSpeed - TargetSpeed->XSpeed + TargetSpeed->YawSpeed;
    WheelControl[2].TargetValue = TargetSpeed->YSpeed - TargetSpeed->XSpeed - TargetSpeed->YawSpeed;
    WheelControl[3].TargetValue = TargetSpeed->YSpeed + TargetSpeed->XSpeed + TargetSpeed->YawSpeed;
}
//PID结构体内缺乏一个范用的的设定目标的函数

///<summary>根据四个控制量ControlValue来输出四个电机的占空比</summary>
void MotorOutput(float * ControlValue)
{
    int i = 0;
    if (ControlValue[i] >= 0)
    {
        LQ_PWMA_B_SetDuty(PWMType_Use1, Wheels_PWMChannel[i], 0, (uint16_t)(ControlValue[i]));
    }
    else
    {
        LQ_PWMA_B_SetDuty(PWMType_Use1, Wheels_PWMChannel[i], (uint16_t)(-ControlValue[i]), 0);
    }
    i = 1;
    if (ControlValue[i] >= 0)
    {
        LQ_PWMA_B_SetDuty(PWMType_Use2, Wheels_PWMChannel[i], 0, (uint16_t)(ControlValue[i]));
    }
    else
    {
        LQ_PWMA_B_SetDuty(PWMType_Use2, Wheels_PWMChannel[i], (uint16_t)(-ControlValue[i]), 0);
    }
    i = 2;
    if (ControlValue[i] >= 0)
    {
        LQ_PWMA_B_SetDuty(PWMType_Use3, Wheels_PWMChannel[i], 0, (uint16_t)(ControlValue[i]));
    }
    else
    {
        LQ_PWMA_B_SetDuty(PWMType_Use3, Wheels_PWMChannel[i], (uint16_t)(-ControlValue[i]), 0);
    }
    i = 3;
    if (ControlValue[i] >= 0)
    {
        LQ_PWMA_B_SetDuty(PWMType_Use4, Wheels_PWMChannel[i], 0, (uint16_t)(ControlValue[i]));
    }
    else
    {
        LQ_PWMA_B_SetDuty(PWMType_Use4, Wheels_PWMChannel[i], (uint16_t)(-ControlValue[i]), 0);
    }
}
float ControlValue_Start[4] = { 0 };
float ControlValue_Closeloop[4];//闭环占空比输出
struct RunSpeed RS_Now;//用于解算CalTargetSpeed_EachWheel的结构体

//编码器部分
int16_t temp_Speed[4] = { 0 };
long SpeedCount[4] = { 0 };
long Speed_get[4] = { 0 };
//编码器测速用的ENC编号
ENC_Type* Encoder_ENCIndex[4] = { ENC1, ENC2, ENC3, ENC4 };
//编码器测速用的旋转方向Port总端口
GPIO_Type* EncoderDirectionPort[4] = { GPIO3, GPIO3, GPIO2, GPIO2 };
//编码器测速用的旋转方向Port端口索引
uint32_t Encoder_PORTIndex_Direction[4] = { 12, 25, 27, 26 };

///<summary>四个编码器初始化</summary>
void EncoderMeasure_Init(void)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_00_GPIO3_IO12,        /* GPIO_SD_B0_00 is configured as GPIO3_IO12 */
        0U);

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_EMC_39_GPIO3_IO25,          /* GPIO_EMC_39 is configured as GPIO3_IO25 */
        0U);

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_11_GPIO2_IO27,           /* GPIO_B1_11 is configured as GPIO2_IO27 */
        0U);

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_10_GPIO2_IO26,           /* GPIO_B1_10 is configured as GPIO2_IO26 */
        0U);

    gpio_pin_config_t GPIO_ENCDirection_Config = { kGPIO_DigitalInput,    //GPIO为输入方向
        1,                    //高电平
        kGPIO_NoIntmode,      //不触发中断
    };

    for (int i = 0; i < 4; i++)
    {
        LQ_ENC_Init(Encoder_ENCIndex[i]);   //正交解码初始化

        GPIO_PinInit(EncoderDirectionPort[i], Encoder_PORTIndex_Direction[i], &GPIO_ENCDirection_Config);

        SpeedCount[i] = 0;
        temp_Speed[i] = 0;
    }

}


//四个轮子反转时的旋转方向IO
uint8_t Flag_Reverse[4] = { 0, 0, 0, 0 };

///<summary>获得第index个轮子的转速</summary>
void GetSpeed(int index)
{
    uint8_t flag_temp = 0;

    temp_Speed[index] = ENC_GetPositionDifferenceValue(Encoder_ENCIndex[index]);
    
    flag_temp = GPIO_PinRead(EncoderDirectionPort[index], Encoder_PORTIndex_Direction[index]);
    if (flag_temp == Flag_Reverse[index])
    {
        temp_Speed[index] *= -1;
    }
    SpeedCount[index] += temp_Speed[index];
}

///<summary>速度计数清零</summary>
void SpeedClean(void)
{
    int i = 0;
    for (i = 0; i < 4; i++)
        SpeedCount[i] = 0;
}


///<summary>远程遥控程序</summary>
#ifdef Remote_UseDigitalReceive
extern RemoteCMDMode RunMode;
#else
extern ReceiveCMDData RemoteData;
#endif

#ifdef Remote_UseDigitalReceive
int Remote_Speed = 220;   
void SetSpeed_FromRemote(RemoteCMDMode mode)
{
    switch (mode)
    {
        case Left_Left:
        SetTargetSpeed_Car(&RS_Now, -Remote_Speed, 0, 0);
        break;
        case Left_Right:
        SetTargetSpeed_Car(&RS_Now, Remote_Speed, 0, 0);
        break;
        case Left_Up:
        SetTargetSpeed_Car(&RS_Now, 0, Remote_Speed, 0);
        break;
        case Left_Down:
        SetTargetSpeed_Car(&RS_Now, 0, -Remote_Speed, 0);
        break;
        case Left_Return0:
        SetTargetSpeed_Car(&RS_Now, 0, 0, 0);
        break;
        case Right_Left:
        SetTargetSpeed_Car(&RS_Now, 0, 0, Remote_Speed);
        break;
        case Right_Right:
        SetTargetSpeed_Car(&RS_Now, 0, 0, -Remote_Speed);
        break;
        case Right_Return0:
        SetTargetSpeed_Car(&RS_Now, 0, 0, 0);	
        case Start:
        SetTargetSpeed_Car(&RS_Now, 0, 0, 0);   
        default:
        break;
    }
    CalTargetSpeed_EachWheel(&RS_Now);
    for (int i = 0; i < 4; i++)
    {
        ControlValue_Closeloop[i] = GetPIDControlValue(&WheelControl[i], PID_Control, Speed_get[i]); 
    }
    if (mode ==Start)
      MotorOutput(ControlValue_Start);
    else
      MotorOutput(ControlValue_Closeloop);
}

#else 
void SetSpeed_FromRemote_Analog(void)
{
    SetTargetSpeed_Car(&RS_Now,2*(RemoteData.Left_X-127), 2.8*(RemoteData.Left_Y-127), 0.4*(127-RemoteData.Right_X));
    CalTargetSpeed_EachWheel(&RS_Now);
    for (int i = 0; i < 4; i++)
    {
        ControlValue_Closeloop[i] = GetPIDControlValue(&WheelControl[i], PID_Control, Speed_get[i]); 
    }
    MotorOutput(ControlValue_Closeloop);
}

#endif
//串级控制部分
float Rotate_sp = 0;
float Series_Speed = 300;
///<summary>速度环初始化函数</summary>
void PID_Speedloop_init( float *P_set, float *D_set, float *I_set, float I_Limit_Set, float MaxOutput_Set, float *DeadBand_Set)
{
  int i = 0;
  for(i = 0;i < 4;i++)
  {
    PIDControl_FunctionDefine(&WheelControl[i]);
    PIDControl_Constructor(&WheelControl[i], P_set[i], D_set[i], I_set[i], I_Limit_Set, MaxOutput_Set, DeadBand_Set[i]);
  }
  
}
///<summary>位置环初始化</summary>
void PID_locationloop_init( float P_set, float D_set, float I_set, float I_Limit_Set, float MaxOutput_Set, float DeadBand_Set)
{
    PIDControl_FunctionDefine(&Car_Speed_Rotate);
    PIDControl_Constructor(&Car_Speed_Rotate, P_set, D_set, I_set, I_Limit_Set, MaxOutput_Set, DeadBand_Set);
    Car_Speed_Rotate.TargetValue = 94;//设定位置环目标值94
}
extern struct GYROData GYRO_OriginData;
///<summary>串级控制 输入量为位置环偏差 中值94 Y轴</summary>
void Series_Control(float deviation)
{
    //Get_Gyro(&GYRO_OriginData);//z轴为地磁轴
    //int a = (int)(GYRO_OriginData.Z*0.001);
  if(RunMode == Start)
    SetTargetSpeed_Car(&RS_Now, 0, 0, 0);
  else
  {
    if(deviation == 0)
    {
      SetTargetSpeed_Car(&RS_Now, 0, 0, 80);
      FIFO_five_depth_Clean();
    }
    else
    {
      Rotate_sp = GetPIDControlValue(&Car_Speed_Rotate, PD_Control, deviation);
      SetTargetSpeed_Car(&RS_Now, 0, Series_Speed, Rotate_sp);
    }
  }
    CalTargetSpeed_EachWheel(&RS_Now);
    for (int i = 0; i < 4; i++)
    {
        ControlValue_Closeloop[i] = GetPIDControlValue(&WheelControl[i], PID_Control, Speed_get[i]); 
    }
      MotorOutput(ControlValue_Closeloop);
}