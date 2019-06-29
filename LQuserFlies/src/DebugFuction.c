/*!
* @file       DebugFuction.c
* @brief      用于调试用的函数库
* @details
* @author     pig's grief
* @version    v1.0
* @date       2019-2-26
* @to do
*             
*/
# include "include.h"
DebugSeriesDMAStatus DMASendStatus;
extern lpuart_transfer_t sendXfer;
extern lpuart_edma_handle_t g_lpuartEdmaHandle;
void UART_Put_Buff_DMA(uint8_t *dataToSend, uint8_t length)
{
    /* If TX is idle and g_txBuffer is full, start to send data. */
    /*使用DMA + 串口，无需占用CPU时间 */
    sendXfer.data = dataToSend;
    sendXfer.dataSize = length;
    if (DMASendStatus == SendFinish)
    {
        DMASendStatus = Sending;
        LPUART_SendEDMA(LPUART1, &g_lpuartEdmaHandle, &sendXfer);
    }

}
/*************************************************************/
/*****************虚拟示波器部分************/
/*************************************************************/
float OutData[4] = { 0 };//程序会将数组中的四个数发送给虚拟示波器
//float temp = 0.0;
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

void OutPut_Data(void)
{
      int temp[4] = {0};
      unsigned int temp1[4] = {0};
      unsigned char databuf[10] = {0};
      unsigned char i;
      unsigned short CRC16 = 0;
      for(i=0;i<4;i++)
      {
        temp[i]  = (int)OutData[i];
        temp1[i] = (unsigned int)temp[i];
    
    }

    for(i=0;i<4;i++) 
    {
        databuf[i*2]   = (unsigned char)(temp1[i]%256);
        databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }

    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;

    UART_Put_Buff_DMA(databuf, 10);
}
/// <summary>
///给虚拟示波器发送a,b,c,d取整之后的值
///<para>example:  SEND(a,b,c,d);</para>
///</summary>
/// <param name="a">待发送的变量1</param>
/// <param name="b">待发送的变量2</param>
/// <param name="c">待发送的变量3</param>
/// <param name="d">待发送的变量4</param>
void SEND(float a,float b,float c,float d)
{       
 OutData[0] = a;                   
 OutData[1] = b;                    
 OutData[2] = c;                
 OutData[3] = d;
 OutPut_Data();
}
/*************************************************************/
/*****************数据采集部分************/
/*************************************************************/
/*===================================================================
功能：串口发送数据用于Matlab数据处理
===================================================================*/
void DATA_SEND(long num)
{
    int weishu = 0;
    long num_buff = num;
    long buff = 1;
    int index = 0;

    if (num < 0)
    {
        UART_Put_Buff(CRC_Uart_Port, "-", 1);
        num = -num;
    }

    if (num_buff == 0)
    {
        weishu = 1;
        buff = 10;
    }

    while (num_buff != 0)
    {
        num_buff /= 10;
        weishu++;
        buff *= 10;
    }
    buff /= 10;

    for (index = 0; index < weishu; index++)
    {
        //UART_Put_Buff(CRC_Uart_Port, ((num / buff) % 10) + '0', 1);

        buff /= 10;
    }
    UART_Put_Buff(CRC_Uart_Port, " ", 1);
}

/*************************************************************/
/*****************遥控部分************/
/*************************************************************/
/// <summary>
///初始化遥控器的串口
///</summary>
void RemoteInit()
{
    UART_Init(Remote_Uart_Port, 9600);
}
/// <summary>
///初始化模拟遥控器的接收数据
///</summary>
ReceiveCMDData RemoteData;
void RemoteData_init(void)
{
    RemoteData.Left_Y = 126;
    RemoteData.Left_X = 126;
    RemoteData.Right_Y = 126;
    RemoteData.Right_X = 126;
  
}
int ReceiveIndex = 0;
char ReceiveBuff[3] = {0};
unsigned char StartReceive = 0;
#ifdef Remote_UseDigitalReceive
RemoteCMDMode RunMode;//遥控模式
/// <summary>
///接受遥控指令程序，应放入对应的串口中断内
///</summary>
void ReceiveCMD_Remote(void)
{
    char buff = 0;
    buff = UART_Get_Char(Remote_Uart_Port);
    if (buff == 0xFF && StartReceive == 0)
    {
        StartReceive = 1;
        return;
    }
    else
    {
      if(StartReceive == 0)
       ReceiveIndex = 0;
    }
    if (StartReceive == 1)
    {
        if (ReceiveIndex < 3)
        {
            ReceiveBuff[ReceiveIndex] = buff;
            ReceiveIndex++;
        }
        else
        {
            if (buff == 0xFF)
            {
                StartReceive = 0;
                ReceiveIndex = 0;
                    if (ReceiveBuff[0] == 0x00)//左摇杆
                    {
                        switch (ReceiveBuff[1])
                        {
                            case 0x00:
                            RunMode = Left_Return0;
                            break;
                            case 0x01:
                            RunMode = Left_Up;
                            break;
                            case 0x02:
                            RunMode = Left_Down;
                            break;
                            case 0x03:
                            RunMode = Left_Left;
                            break;
                            case 0x04:
                            RunMode = Left_Right;
                            break;
                            default:
                            break;
                        }
                    }
                    else if (ReceiveBuff[0] == 0x35)//左摇杆
                    {
                        switch (ReceiveBuff[1])
                        {
                            case 0x00:
                            RunMode = Right_Return0;
                            break;
                            case 0x01:
                            RunMode = Right_Up;
                            break;
                            case 0x02:
                            RunMode = Right_Down;
                            break;
                            case 0x03:
                            RunMode = Right_Left;
                            break;
                            case 0x04:
                            RunMode = Right_Right;
                            break;
                            default:
                            break;
                        }
                    }
                    else if (ReceiveBuff[0] == 0xAA)
                    {
                        RunMode = Start;
                    }
                }
            }
        }

}
#else
/// <summary>
///请求遥控指令程序，模拟用
///</summary>
Remote_State Remote_CMD_ReceiveStatus = Sleep;
long count_error_left = 0;
long count_error_right = 0;

void SendRemoteCMDData(void)
{
    if (Remote_CMD_ReceiveStatus == Sleep)
    {
        Remote_CMD_ReceiveStatus = SendLeftCMD;
        Remote_CMD_ReceiveStatus = ReceivingLeftCMD;
        UART_Put_Char(Remote_Uart_Port, 0xBB);
    }
    if(Remote_CMD_ReceiveStatus == ReceivingLeftCMD)
    {
      count_error_left++;
      count_error_right = 0;
    }
    else if(Remote_CMD_ReceiveStatus == ReceivingRightCMD)
    {
      count_error_right++; 
      count_error_left = 0;
    }
    else
    {
      count_error_left = 0;
      count_error_right = 0;
    }
    
    if(count_error_left >= 500 || count_error_right >= 500)
    {
      RemoteData.Left_Y = 127;
      RemoteData.Left_X = 127;
      RemoteData.Right_Y = 127;
      RemoteData.Right_X = 127;
      Remote_CMD_ReceiveStatus = Sleep;
      count_error_left = 0;
      count_error_right = 0;
    }
}
/// <summary>
///接受遥控指令程序，应放入对应的串口中断内
///</summary>
void ReceiveCMD_Remote()
{
    char buff = 0;
    buff = UART_Get_Char(Remote_Uart_Port);
    if (buff == 0xFF && StartReceive == 0)
    {
        StartReceive = 1;
        return;
    }
    else
    {
        if (StartReceive == 0)
            ReceiveIndex = 0;
    }
    if (StartReceive == 1)
    {
        if (ReceiveIndex < 3)
        {
            ReceiveBuff[ReceiveIndex] = buff;
            ReceiveIndex++;
        }
        else
        {
            if (buff == 0xFF)
            {
                StartReceive = 0;
                ReceiveIndex = 0;
                if (ReceiveBuff[0] == 0xBB)//左摇杆
                {
                    if (Remote_CMD_ReceiveStatus == ReceivingLeftCMD)
                    {
                        RemoteData.Left_Y = ReceiveBuff[1];
                        RemoteData.Left_X = ReceiveBuff[2];
                        Remote_CMD_ReceiveStatus = ReceivedLeftCMD;
                        Remote_CMD_ReceiveStatus = ReceivingRightCMD;
                        UART_Put_Char(Remote_Uart_Port, 0xCC);          
                    }
                    
                }
                else if (ReceiveBuff[0] == 0xCC)//右摇杆
                {
                    if (Remote_CMD_ReceiveStatus == ReceivingRightCMD)
                    {
                        RemoteData.Right_Y = ReceiveBuff[1];
                        RemoteData.Right_X = ReceiveBuff[2];
                        Remote_CMD_ReceiveStatus = ReceivedRightCMD;
                        Remote_CMD_ReceiveStatus = Sleep;
                    }
                }
                else
                {
                        RemoteData.Left_Y = 127;
                        RemoteData.Left_X = 127;
                        RemoteData.Right_Y = 127;
                        RemoteData.Right_X = 127;
                  Remote_CMD_ReceiveStatus = Sleep;
                }
            }
        }
    }

}
#endif

///<summary>按键初始化</summary>
void ButtonInit()
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

    /* GPIO configuration of k3 on GPIO_AD_B1_00 (pin J11) */
    gpio_pin_config_t k3_config = {
        .direction = kGPIO_DigitalInput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_IntRisingOrFallingEdge
    };
    /* Initialize GPIO functionality on GPIO_AD_B1_00 (pin J11) */
    GPIO_PinInit(GPIO1, 16U, &k3_config);
    /* Enable GPIO pin interrupt on GPIO_AD_B1_00 (pin J11) */
    GPIO_PortEnableInterrupts(GPIO1, 1U << 16U);

    /* GPIO configuration of k1 on GPIO_B1_08 (pin A12) */
    gpio_pin_config_t k1_config = {
        .direction = kGPIO_DigitalInput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_IntRisingOrFallingEdge
    };
    /* Initialize GPIO functionality on GPIO_B1_08 (pin A12) */
    GPIO_PinInit(GPIO2, 24U, &k1_config);
    /* Enable GPIO pin interrupt on GPIO_B1_08 (pin A12) */
    GPIO_PortEnableInterrupts(GPIO2, 1U << 24U);

    /* GPIO configuration of k2 on GPIO_B1_09 (pin A13) */
    gpio_pin_config_t k2_config = {
        .direction = kGPIO_DigitalInput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_IntRisingOrFallingEdge
    };
    /* Initialize GPIO functionality on GPIO_B1_09 (pin A13) */
    GPIO_PinInit(GPIO2, 25U, &k2_config);
    /* Enable GPIO pin interrupt on GPIO_B1_09 (pin A13) */
    GPIO_PortEnableInterrupts(GPIO2, 1U << 25U);

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_00_GPIO1_IO16,        /* GPIO_AD_B1_00 is configured as GPIO1_IO16 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_08_GPIO2_IO24,           /* GPIO_B1_08 is configured as GPIO2_IO24 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_09_GPIO2_IO25,           /* GPIO_B1_09 is configured as GPIO2_IO25 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B1_00_GPIO1_IO16,        /* GPIO_AD_B1_00 PAD functional properties : */
        0x0110B0U);                             /* Slew Rate Field: Slow Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: medium(100MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_08_GPIO2_IO24,           /* GPIO_B1_08 PAD functional properties : */
        0x0110B0U);                             /* Slew Rate Field: Slow Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: medium(100MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_09_GPIO2_IO25,           /* GPIO_B1_09 PAD functional properties : */
        0x0110B0U);                             /* Slew Rate Field: Slow Slew Rate
                                                Drive Strength Field: R0/6*/

    EnableIRQ(GPIO2_Combined_16_31_IRQn);

    EnableIRQ(GPIO1_Combined_16_31_IRQn);

    TFT_showstr(0, 0, "Please Press!", BLACK, WHITE);
}
ButtonStatus Button[3] = { NotPress, NotPress, NotPress };//PTE12,PTE11,PTE10


///<summary>板间通信</summary>
uint8 SeriesReceive = 0;
uint8 SeriesIndex = 0;
uint8 Series_ReceiveBuff[4] = { 0 };
uint8 Front_Distance_ReceiveBuff[3] = { 0 };
uint8 Back_Distance_ReceiveBuff[3] = { 0 };
void Series_RX(void)
{
    uint8 buff = 0;
    buff = UART_Get_Char(Series_Uart_Port);
    if (buff == 0xFF && SeriesReceive == 0)
    {
        SeriesReceive = 1;
        return;
    }
    else
    {
      if(SeriesReceive == 0)
       SeriesIndex = 0;
    }
    if (SeriesReceive == 1)
    {
        if (SeriesIndex < 4)
        {
          Series_ReceiveBuff[SeriesIndex] = buff;
          SeriesIndex++;
        }
        else
        {
            if (buff == 0xFF)
            {
              FIFO(Front_Distance_ReceiveBuff, 3, Series_ReceiveBuff[1]);
              FIFO(Back_Distance_ReceiveBuff, 3, Series_ReceiveBuff[3]);
              SeriesReceive = 0;
              SeriesIndex = 0;
            }
        }
    }
}
///<summary>串级通信初始化</summary>
void Series_Receive_init(void)
{
    UART_Init(Series_Uart_Port, 115200);
}


///<summary>按键扫描中断</summary>
void GPIO2_Combined_16_31_IRQHandler(void)
{
    unsigned char keybuff = 0;
    unsigned char n = 24;
    /* clear the interrupt status */
    if (GPIO_GetPinsInterruptFlags(GPIO2)&(1 << n))    //判断是否为GPIO5_0中断
    {
            keybuff = GPIO_PinRead(GPIO2, n);
            if (keybuff == 0)
            {
                Button[0] = Press;
            }
            else
            {
                Button[0] = NotPress;
            }
    }
    GPIO_PortClearInterruptFlags(GPIO2, 1U << n);   //清除标志位

    n = 25;
    /* clear the interrupt status */
    if (GPIO_GetPinsInterruptFlags(GPIO2)&(1 << n))    //判断是否为GPIO5_0中断
    {
        keybuff = GPIO_PinRead(GPIO2, n);
        if (keybuff == 0)
        {
            Button[1] = Press;
        }
        else
        {
            Button[1] = NotPress;
        }
    }
    GPIO_PortClearInterruptFlags(GPIO2, 1U << n);   //清除标志位

    /* Change state of switch. */
    __DSB();				//数据同步屏蔽指令
}

void GPIO1_Combined_16_31_IRQHandler(void)
{
    unsigned char keybuff = 0;
    unsigned char n = 16;
    /* clear the interrupt status */
    if (GPIO_GetPinsInterruptFlags(GPIO1)&(1 << n))    //判断是否为GPIO5_0中断
    {
        keybuff = GPIO_PinRead(GPIO1, n);
        if (keybuff == 0)
        {
            Button[2] = Press;
        }
        else
        {
            Button[2] = NotPress;
        }
    }
    GPIO_PortClearInterruptFlags(GPIO1, 1U << n);   //清除标志位

    /* Change state of switch. */
    __DSB();				//数据同步屏蔽指令
}

int ButtonOnceBuffFlag[3] = { 0 };//按键按下一次缓存标志
int ButtonOnceFlag[3] = { 0 };//按键按下一次的标志
int QuitSetFlag = 0;
extern float Battery_V;
///// <summary>
/////按键菜单程序，用于参数设定等功能，放于主函数的主要功能前
/////<para>注：一定要放在TFT初始化后，另外其他有中断的模块初始化都必须放在这个函数后面</para>
/////</summary>
void ButtonMenu()
{
    BatteryVoltageCollect_Init(1);
    ButtonInit();
    
    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (Button[i] == Press)
            {
                ButtonOnceBuffFlag[i] = 1;
            }
            if ((Button[i] == NotPress) && (ButtonOnceBuffFlag[i] == 1))
            {
                ButtonOnceFlag[i] = 1;
                ButtonOnceBuffFlag[i] = 0;
            }
        }

        if (ButtonOnceFlag[0] == 1)
        {
            ButtonOnceFlag[0] = 0;
            /* 在此编写按下按键1的处理程序 */
            TFT_showstr(0, 0, "Button1 Pressed", RED, WHITE);
        }
        if (ButtonOnceFlag[1] == 1)
        {
            ButtonOnceFlag[1] = 0;
            /* 在此编写按下按键2的处理程序 */
            TFT_showstr(0, 0, "Button2 Pressed", RED, WHITE);

        }
        if (ButtonOnceFlag[2] == 1)
        {
            ButtonOnceFlag[2] = 0;
            /* 在此编写按下按键3的处理程序 */
            TFT_showstr(0, 0, "Button3 Pressed", RED, WHITE);
            QuitSetFlag = 1;
        }
        if (QuitSetFlag == 1)
        {
            dsp_single_colour(WHITE);
            TFT_showstr(0, 0, "QuitButtonSet", RED, WHITE);
            Flag_TFTShow = !Flag_TFTShow;
            break;
        }
        Battery_V = GetBatteryVoltage(7.5);
        LCD_ShowBatteryVoltage(0,1,Battery_V);
    }
}
int UseBeepFlag = 0;
///<summary>电量采集</summary>
void BatteryVoltageCollect_Init(int IfUseBeep)
{
    LQADC_Init(BatteryCollectADC);
    UseBeepFlag = IfUseBeep;
    if(IfUseBeep)
    {
      IOMUXC_SetPinMux(
        IOMUXC_GPIO_EMC_41_GPIO3_IO27,          /* GPIO_EMC_41 is configured as GPIO3_IO27 */
        0U); 
      /* GPIO configuration of DC on GPIO_SD_B1_03 (pin M4) */
      gpio_pin_config_t Out_config = {
          .direction = kGPIO_DigitalOutput,
          .outputLogic = 0U,
          .interruptMode = kGPIO_NoIntmode
      };
      /* Initialize GPIO functionality on GPIO_SD_B1_03 (pin M4) */
      GPIO_PinInit(GPIO3, 27, &Out_config);
    }
}
uint16_t adc_value = 0;
float GetBatteryVoltage(float HintVoltage)
{
    adc_value = ReadADC(BatteryCollectADC, BatteryCollectADCChn);

    float result = adc_value*0.01;
    
    if(UseBeepFlag)
    {
      if(result < HintVoltage && result > 2)
      {
        GPIO_WritePinOutput(GPIO3, 27, 0);
      }     
      else
      {
        GPIO_WritePinOutput(GPIO3, 27, 1);        
      }
    }
    return result;
}

void LCD_ShowBatteryVoltage(unsigned char x, unsigned char y, float num)
{
    TFT_showUfloat(x, y, num, 1, 2, RED, WHITE);
    TFT_showchar(x+32,y+15,'V',RED,WHITE);
}

///<summary>FIFO</summary>
void FIFO(uint8 *head, uint8 depth, uint8 num)
{
  int k;
  for( k = 0;k < depth-1;k++)
  {
    *(head+k) = *(head+k+1);
  }
  *(head+k) = num;
  return;
}

void FIFO_Clean(uint8 *head, uint8 depth)
{
    int i = 0;
    for (i = 0; i < depth; i++)
        *(head+i) = 0;
    return;
}

///<summary>单层最值检索</summary>
uint8 One_loop_bubblesort(uint8 *lis, uint8 depth)
{
//  uint8 biggest = 0;
  uint8 smallest = 0;
  uint8 *const arry = (uint8 *)malloc(sizeof(uint8)*depth);//向系统申请内存，长度为传递进来的列表深度
  uint8 temp_exchange = 0;
	for (int i = 0; i <= depth-1; i++)
	{
		arry[i] = lis[i];
	}
        ///<summary>单层最大值检索</summary>
//	for (int i = 0; i < depth-1; i++)
//	{
//                if (arry[i] > arry[i + 1])
//                {
//                        temp_exchange = arry[i + 1];
//                        arry[i + 1] = arry[i];
//                        arry[i] = temp_exchange;
//                }
//	}
//        biggest = arry[depth-1];
//        return biggest;
        ///<summary>单层最小值检索</summary>
        for (int i = 0; i < depth-1; i++)
	{
                if (arry[i] < arry[i + 1])
                {
                        temp_exchange = arry[i + 1];
                        arry[i + 1] = arry[i];
                        arry[i] = temp_exchange;
                }
	}
        smallest = arry[depth-1];
        return smallest;
}
