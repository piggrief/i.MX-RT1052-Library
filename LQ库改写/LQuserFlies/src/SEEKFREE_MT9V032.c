/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_MT9V032.c
 * @brief      		总钻风(灰度摄像头)函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.2 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note	
					MT9V032接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SDA(51的RX)         C17
						SCL(51的TX)         C16
						场中断              C6
						行中断				未使用，因此不接
						像素中断            C18           
						数据口              C8-C15 
					------------------------------------ 
	
					默认分辨率是            188*120
					默认FPS                 50帧
 ********************************************************************************************************************/



# include "include.h"
# include "SEEKFREE_MT9V032.h"

# define UART_Put_Buff LQ_UART_PutBuff
# define LPTMR_delay_ms(x) _systime.delay_ms(x)

#define uint8 unsigned char
#define uint16 unsigned int
#define int16 int
uint8   image_1[ROW][COL];      //图像数组
uint8   image_2[ROW][COL];      //图像数组

uint8   receive_1[3];
uint8   receive_2[3];
uint8   receive_num_1 = 0;
uint8   receive_num_2 = 0;
uint8   uart_receive_flag_1 = 1;
uint8   uart_receive_flag_2 = 1;

ImageDealState Image_1_DealState_Now = Image_Collecting;
ImageDealState Image_2_DealState_Now = Image_Collecting;

//需要配置到摄像头的数据
int16 MT9V032_CFG_1[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
                              //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
    {EXP_TIME,          300},//3118, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
    {FPS,               50},  //50图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
    {SET_COL,           COL}, //图像列数量        范围1-752     K60采集不允许超过188
    {SET_ROW,           ROW}, //图像行数量        范围1-480
    {LR_OFFSET,         0},   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {UD_OFFSET,         0},   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {GAIN,              32},  //32 图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度

    
    {INIT,              0}    //摄像头开始初始化
};
//需要配置到摄像头的数据
int16 MT9V032_CFG_2[CONFIG_FINISH][2] =
{
    { AUTO_EXP, 0 },   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
    //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
    { EXP_TIME, 300 },//3118, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
    { FPS, 50 },  //50图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
    { SET_COL, COL }, //图像列数量        范围1-752     K60采集不允许超过188
    { SET_ROW, ROW }, //图像行数量        范围1-480
    { LR_OFFSET, 0 },   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    { UD_OFFSET, 0 },   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    { GAIN, 32 },  //32 图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度


    { INIT, 0 }    //摄像头开始初始化
};
//从摄像头内部获取到的配置数据
int16 GET_CFG_1[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},   //自动曝光设置      
    {EXP_TIME,          0},   //曝光时间          
    {FPS,               0},   //图像帧率          
    {SET_COL,           0},   //图像列数量        
    {SET_ROW,           0},   //图像行数量        
    {LR_OFFSET,         0},   //图像左右偏移量    
    {UD_OFFSET,         0},   //图像上下偏移量    
    {GAIN,              0},   //图像增益          
};
//从摄像头内部获取到的配置数据
int16 GET_CFG_2[CONFIG_FINISH - 1][2] =
{
    { AUTO_EXP, 0 },   //自动曝光设置      
    { EXP_TIME, 0 },   //曝光时间          
    { FPS, 0 },   //图像帧率          
    { SET_COL, 0 },   //图像列数量        
    { SET_ROW, 0 },   //图像行数量        
    { LR_OFFSET, 0 },   //图像左右偏移量    
    { UD_OFFSET, 0 },   //图像上下偏移量    
    { GAIN, 0 },   //图像增益          
};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头串口中断函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       该函数在ISR文件 串口3中断程序被调用
//-------------------------------------------------------------------------------------------------------------------
//void mt9v032_cof_uart_interrupt(void)
//{
//    uart_getchar(MT9V032_COF_UART,&receive[receive_num]);;
//    receive_num++;
//    
//    if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
//    if(3 == receive_num)
//    {
//        receive_num = 0;
//        uart_receive_flag = 1;
//    }
//        
//}

//void UART3_IRQHandler(void)
//{
//    receive_1[receive_num_1] = UART_Get_Char(MT9V032_COF_UART_1);
//    receive_num_1++;
//
//    if (1 == receive_num_1 && 0XA5 != receive_1[0])  receive_num_1 = 0;
//    if (3 == receive_num_1)
//    {
//        receive_num_1 = 0;
//        uart_receive_flag_1 = 1;
//    }
//}
//void UART0_IRQHandler(void)
//{
//    receive_2[receive_num_2] = UART_Get_Char(MT9V032_COF_UART_2);
//    receive_num_2++;
//
//    if (1 == receive_num_2 && 0XA5 != receive_2[0])  receive_num_2 = 0;
//    if (3 == receive_num_2)
//    {
//        receive_num_2 = 0;
//        uart_receive_flag_2 = 1;
//    }
//}
void LPUART4_IRQHandler(void)
{
    static uint8_t res = 0;

    if ((LPUART4->STAT)&kLPUART_RxDataRegFullFlag) //接收中断
    {
        receive_1[receive_num_1] = LPUART_ReadByte(MT9V032_COF_UART_1);				//读取数据
        receive_num_1++;

        if (1 == receive_num_1 && 0XA5 != receive_1[0])  receive_num_1 = 0;
        if (3 == receive_num_1)
        {
            receive_num_1 = 0;
            uart_receive_flag_1 = 1;
        }
    }
    __DSB();				//数据同步屏蔽指令
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头场中断
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
//void VSYNC_1(void)
//{
//    //DMA_PORTx2BUFF_Init(DMA_CH4, (void *)&MT9V032_DATAPORT, (void *)image, MT9V032_PCLK, DMA_BYTE1, COL*ROW, DMA_rising_down_keepon);
//    DMA_DIS(DMA_CH4);
//    DMA_IRQ_CLEAN(DMA_CH4);
//    DMA0->TCD[DMA_CH4].BITER_ELINKNO |= DMA_BITER_ELINKNO_BITER(COL * ROW);
//    DMA0->TCD[DMA_CH4].CITER_ELINKNO |= DMA_CITER_ELINKNO_CITER(COL * ROW);
//    DMA0->TCD[DMA_CH4].DADDR = (uint32)image_1;
//
//    if (Image_1_DealState_Now == Image_Collecting)
//    {
//        Image_1_DealState_Now = Image_CollectFinish;
//    }
//
//    if (Image_1_DealState_Now == Image_DealingFinsh)
//    {
//        Image_1_DealState_Now = Image_Collecting;
//        DMA_IRQ_EN(DMA_CH4);
//        DMA_EN(DMA_CH4);
//    }
//
//}
//
//void VSYNC_2(void)
//{
//    //DMA_PORTx2BUFF_Init(DMA_CH4, (void *)&MT9V032_DATAPORT, (void *)image, MT9V032_PCLK, DMA_BYTE1, COL*ROW, DMA_rising_down_keepon);
//    DMA_DIS(DMA_CH5);
//    DMA_IRQ_CLEAN(DMA_CH5);
//    DMA0->TCD[DMA_CH5].BITER_ELINKNO |= DMA_BITER_ELINKNO_BITER(COL * ROW);
//    DMA0->TCD[DMA_CH5].CITER_ELINKNO |= DMA_CITER_ELINKNO_CITER(COL * ROW);
//    DMA0->TCD[DMA_CH5].DADDR = (uint32)image_2;
//
//    if (Image_2_DealState_Now == Image_Collecting)
//    {
//        Image_2_DealState_Now = Image_CollectFinish;
//    }
//
//    if (Image_2_DealState_Now == Image_DealingFinsh)
//    {
//        Image_2_DealState_Now = Image_Collecting;
//        DMA_IRQ_EN(DMA_CH5);
//        DMA_EN(DMA_CH5);
//    }
//
//}


uint8   mt9v032_finish_flag = 0;      //一场图像采集完成标志位
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头DMA完成中断
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
//void row_finished(void)
//{
//
//    //disable_irq(PORTC_IRQn);//关闭场中断
//
//    //一副图像从采集开始到采集结束耗时3.33MS左右(50FPS、188*120分辨率)
//}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风摄像头图像发送至上位机查看图像
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart2
//-------------------------------------------------------------------------------------------------------------------
extern uint32_t fullCameraBufferAddr;
void seekfree_sendimg_032(void)
{
    LQ_UART_PutChar(LPUART1, 0x00);
    LQ_UART_PutChar(LPUART1, 0xff);
    LQ_UART_PutChar(LPUART1, 0x01);
    LQ_UART_PutChar(LPUART1, 0x01);
    LQ_UART_PutBuff(LPUART1, (uint8_t *)fullCameraBufferAddr, ROW*COL);

}

//void sendimg(void)
//{
//  
//  int i = 0, j = 0; 
//  UART_Put_Char(UART_3,0x01); //发送帧头标志 WindowsFormsApplication1.exe
//  UART_Put_Char(UART_3,0xfe); //发送帧头标志 WindowsFormsApplication1.exe
//  for(i=0;i < ROW; i++) 
//  { 
//    for(j=0;j <COL;j++) 
//    { 
//      if(image_2[i][j]==0xfe) 
//      {
//	UART_Put_Char(UART_3,0xff); 
//      }
//      else
//          UART_Put_Char(UART_3, image_2[i][j]);
//    } 
//  }
//  UART_Put_Char(UART_3,0xfe); //发送帧尾标志 
//  UART_Put_Char(UART_3,0x01); //发送帧尾标志
//
//
//}

/// <summary>
///计算图像求和面积表(其中SumList_Height，SumList_Width要额外被定义)
///<para>example:  CalSumList(image,sum_list);</para>
///</summary>
/// <param name="image">原图像数组，不会被篡改</param>
/// <param name="sum_list">求和表数组</param>
void CalSumList(uint8 image[SumList_Height][SumList_Width], long sum_list[SumList_Height][SumList_Width])
{
    long sum = 0;
    for (int i = 0; i < SumList_Height; i++)
    {
        for (int j = 0; j < SumList_Width; j++)
        {
            sum_list[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < SumList_Height; i++)
    {
        sum = 0;
        for (int j = 0; j < SumList_Width; j++)
        {
            if (j > 0)
                sum_list[i][j] += sum;
            if (i < SumList_Height - 1)
                sum_list[i + 1][j] += sum_list[i][j];
            sum += image[i][j];
        }
    }
}
/// <summary>
///DBGB的改进型Wallner二值化算法，参数为S和T
///<para>example:  ImageBinary_DBGB(image, BinaryImage, SumList_Width / 8, 15);</para>
///</summary>
/// <param name="image">原图像数组，不会被篡改</param>
/// <param name="BinaryImage">二值化图像数组</param>
/// <param name="S">参数S，为二值化窗的宽度，该窗为正方形</param>
/// <param name="T">参数T，类似为像素二值化的阈值</param>
void ImageBinary_DBGB(uint8 image[SumList_Height][SumList_Width], uint8 BinaryImage[SumList_Height][SumList_Width], int S, int T)
{
    long sum_list[SumList_Height][SumList_Width];
    int x1, y1, x2, y2;
    int s2 = S / 2;
    int count = 0;
    long sum = 0;
int a = 0;
    CalSumList(image, sum_list);
    for (int i = 0; i<SumList_Height; i++)
    {
        for (int j = 0; j<SumList_Width; j++)
        {
	  if(j == 108)
	    a = 0;
            // set the SxS region  
            x1 = i - s2; x2 = i + s2;
            y1 = j - s2; y2 = j + s2;
            // check the border  
            if (x1 < 0) x1 = 0;
            if (x2 >= SumList_Height) x2 = SumList_Height - 1;
            if (y1 < 0) y1 = 0;
            if (y2 >= SumList_Width) y2 = SumList_Width - 1;
            count = (x2 - x1)*(y2 - y1);
            // I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)  
            sum = sum_list[x2][y2] -
                sum_list[x2][y1] -
                sum_list[x1][y2] +
                sum_list[x1][y1];
            if ((long)(image[i][j] * count) < (long)(sum*(100 - T) / 100))
                BinaryImage[i][j] = 0;
            else
                BinaryImage[i][j] = 255;
        }
    }
}
void ImageBinary_Normal(uint8 image[SumList_Height][SumList_Width], uint8 BinaryImage[SumList_Height][SumList_Width], int GateValue)
{
    for (int i = 0; i < SumList_Height; i++)
    {
        for (int j = 0; j < SumList_Width; j++)
        {
            if (image[i][j] < GateValue)
                BinaryImage[i][j] = 255;
            else
                BinaryImage[i][j] = 0;
        }
    }
}