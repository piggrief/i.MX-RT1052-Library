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
    UART_Init(LPUART2, 256000);
    UART_Send(LPUART2, "123123\r\n", 8);
    UART_Send(LPUART2, "123123\r\n", 8);
    UART_Send(LPUART2, "123123\r\n", 8);
    //LQ_PIT_Init(kPIT_Chnl_1, 1000000);
    //MT9V032Init();
    TFTSPI_CLS(u16RED);
    while(1)
    {
 
      
      
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
