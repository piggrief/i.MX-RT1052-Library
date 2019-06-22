# include "include.h"

Enum_UltrasoundMeassure UltrasoundMeassure;
/// <summary>
///初始化超声波模块
///</summary>
void UltrasoundInit()
{
    UART_Init(Ultrasound_Uart_Port, 9600);
    LQ_UART_PutChar(Ultrasound_Uart_Port, 0xe8);//模块地址
    LQ_UART_PutChar(Ultrasound_Uart_Port, 0x02);//寄存器2
    LQ_UART_PutChar(Ultrasound_Uart_Port, 0x75);//6级电源降噪
    
    UltrasoundMeassure = Meassured;
}
/// <summary>
///给超声波模块发送查询指令
///</summary>
void SendCMDToUltrasound()
{
    if (UltrasoundMeassure == Meassured)
    {
        UltrasoundMeassure = Meassuring;
        LQ_UART_PutChar(Ultrasound_Uart_Port, 0xe8);//模块地址
        LQ_UART_PutChar(Ultrasound_Uart_Port, 0x02);//寄存器2
        LQ_UART_PutChar(Ultrasound_Uart_Port, MaxMeassureRangeSetCMD_100cm);//写入的指令
    }
}

uint8_t DisReceiveIndex = 0;
float Distance_Meassured = 0;//单位cm
uint8_t ReceiveBuff_Dis[2];
uint16_t MeasureTime_Dis = 0;
/// <summary>
///接受距离数据反馈，要放到相应串口中断里面
///</summary>
void ReceiveDistance()
{
    uint8_t buff = 0;

    if (UltrasoundMeassure == Meassuring)
    {
        ReceiveBuff_Dis[DisReceiveIndex] = UART_Get_Char(Ultrasound_Uart_Port);
        DisReceiveIndex++;

        if (DisReceiveIndex == 2)
        {
            DisReceiveIndex = 0;
            UltrasoundMeassure = Meassured;
            MeasureTime_Dis = ReceiveBuff_Dis[0] * 256 + ReceiveBuff_Dis[0];
            Distance_Meassured = MeasureTime_Dis * 0.0173;
        }
    }
    else
    {
      buff = UART_Get_Char(Ultrasound_Uart_Port);
    }

}
