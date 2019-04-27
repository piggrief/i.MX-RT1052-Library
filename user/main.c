/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   用V2.3.1版本库建的工程模板
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
# include "include.h"

/*******************************************************************
 * Prototypes
 *******************************************************************/


/*******************************************************************
 * Code
 *******************************************************************/
extern void delayms(uint32_t ms);
void PIT0Inter();
 long count_1s = 0;

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
 int main(void)
{
    uint32_t fullCameraBufferAddr;   
    uint16_t color = 0;

    /* 初始化内存保护单元 */
    BOARD_ConfigMPU();
    /* 初始化引脚设定 */
    BOARD_InitPins();
    /* 初始化时钟设定 */
    BOARD_BootClockRUN();
    /* 初始化所有外设 */    
    BOARD_InitBootPeripherals();
    /* 初始化调试串口 */
    BOARD_InitDebugConsole();
    /* 打印系统时钟 */
    PRINTF("\r\n");
    PRINTF("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
    PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
    PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
    PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
    PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));  
    /* 在这里添加你的代码^_^. */
  
    TFTSPI_Init();
//    UART_Init(LPUART2, 256000);
//    UART_Send(LPUART2, "123123\r\n", 8);
//    UART_Send(LPUART2, "123123\r\n", 8);
//    UART_Send(LPUART2, "123123\r\n", 8);
    //LQ_PIT_Init(kPIT_Chnl_1, 1000000);
    LQ_Camera_Init();
    TFTSPI_CLS(u16RED);
    while(1)
    {
 
            while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //摄像头CSI缓存区已满
            {
            }   
            if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {//注意，使用csiFrameBuf数组时，最好关闭D-Cache 不然上次数据可能会存放在cache里面，造成数据错乱
            SCB_DisableDCache();
            }
    
            TFTSPI_Set_Pos(0,100,(uint8_t)(APP_CAMERA_WIDTH/2-1) ,APP_CAMERA_HEIGHT);//注意 设置显示大小要与下面的实际显示大小相等，不然会显示不出来或者花屏
            for(int i = 0; i < APP_CAMERA_HEIGHT; i+=2)  //  480/4/2/2/2 = 15    //30
            {
                for(int j = 0; j < APP_CAMERA_WIDTH*2; j+=2)//隔2列取一列  752*2/4/4 = 188   //两行数据 一行47像素
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
            CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列        
            SCB_EnableDCache();

      
      //if (kStatus_Success == CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //摄像头CSI缓存区已满
      //{
        
        
        
         //CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列  
      //}  
    }     

}

 void PIT0Inter()
 {
     count_1s++;
     if (count_1s >= 999)
         count_1s = 0;
 }
/****************************END OF FILE**********************/
