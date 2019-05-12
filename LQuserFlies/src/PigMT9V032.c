//# include "include.h"
//# include "PigMT9V032.h"
//
//# define UART_Put_Buff LQ_UART_PutBuff
//# define LPTMR_delay_ms(x) _systime.delay_ms(x)
//#define uint8 unsigned char
//#define uint16 unsigned int
//#define int16 int
//
//uint8   image_1[ROW][COL];      //图像数组
//uint8   image_2[ROW][COL];      //图像数组
//
//uint8   receive_1[3];
//uint8   receive_2[3];
//uint8   receive_num_1 = 0;
//uint8   receive_num_2 = 0;
//uint8   uart_receive_flag_1 = 1;
//uint8   uart_receive_flag_2 = 1;
//
//ImageDealState Image_1_DealState_Now = Image_Collecting;
//ImageDealState Image_2_DealState_Now = Image_Collecting;
//
////需要配置到摄像头的数据
//int16 MT9V032_CFG_1[CONFIG_FINISH][2] =
//{
//    { AUTO_EXP, 0 },   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
//    //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
//    { EXP_TIME, 300 },//3118, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
//    { FPS, 50 },  //50图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
//    { SET_COL, COL }, //图像列数量        范围1-752     K60采集不允许超过188
//    { SET_ROW, ROW }, //图像行数量        范围1-480
//    { LR_OFFSET, 0 },   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
//    { UD_OFFSET, 0 },   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
//    { GAIN, 32 },  //32 图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度
//
//
//    { INIT, 0 }    //摄像头开始初始化
//};
////需要配置到摄像头的数据
//int16 MT9V032_CFG_2[CONFIG_FINISH][2] =
//{
//    { AUTO_EXP, 0 },   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
//    //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
//    { EXP_TIME, 300 },//3118, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
//    { FPS, 50 },  //50图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
//    { SET_COL, COL }, //图像列数量        范围1-752     K60采集不允许超过188
//    { SET_ROW, ROW }, //图像行数量        范围1-480
//    { LR_OFFSET, 0 },   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
//    { UD_OFFSET, 0 },   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
//    { GAIN, 32 },  //32 图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度
//
//
//    { INIT, 0 }    //摄像头开始初始化
//};
////从摄像头内部获取到的配置数据
//int16 GET_CFG_1[CONFIG_FINISH - 1][2] =
//{
//    { AUTO_EXP, 0 },   //自动曝光设置      
//    { EXP_TIME, 0 },   //曝光时间          
//    { FPS, 0 },   //图像帧率          
//    { SET_COL, 0 },   //图像列数量        
//    { SET_ROW, 0 },   //图像行数量        
//    { LR_OFFSET, 0 },   //图像左右偏移量    
//    { UD_OFFSET, 0 },   //图像上下偏移量    
//    { GAIN, 0 },   //图像增益          
//};
////从摄像头内部获取到的配置数据
//int16 GET_CFG_2[CONFIG_FINISH - 1][2] =
//{
//    { AUTO_EXP, 0 },   //自动曝光设置      
//    { EXP_TIME, 0 },   //曝光时间          
//    { FPS, 0 },   //图像帧率          
//    { SET_COL, 0 },   //图像列数量        
//    { SET_ROW, 0 },   //图像行数量        
//    { LR_OFFSET, 0 },   //图像左右偏移量    
//    { UD_OFFSET, 0 },   //图像上下偏移量    
//    { GAIN, 0 },   //图像增益          
//};
//
//void LPUART4_IRQHandler(void)
//{
//    static uint8_t res = 0;
//
//    if ((LPUART4->STAT)&kLPUART_RxDataRegFullFlag) //接收中断
//    {
//        receive_1[receive_num_1] = LPUART_ReadByte(MT9V032_COF_UART_1);				//读取数据
//        receive_num_1++;
//
//        if (1 == receive_num_1 && 0XA5 != receive_1[0])  receive_num_1 = 0;
//        if (3 == receive_num_1)
//        {
//            receive_num_1 = 0;
//            uart_receive_flag_1 = 1;
//        }
//    }
//    __DSB();				//数据同步屏蔽指令
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      获取摄像头内部配置信息
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:				调用该函数前请先初始化uart3
////-------------------------------------------------------------------------------------------------------------------
//void get_config_1(void)
//{
//    uint16 temp, i;
//    uint8  send_buffer[4];
//
//    for (i = 0; i<CONFIG_FINISH - 1; i++)
//    {
//        send_buffer[0] = 0xA5;
//        send_buffer[1] = GET_STATUS;
//        temp = GET_CFG_1[i][0];
//        send_buffer[2] = temp >> 8;
//        send_buffer[3] = (uint8)temp;
//
//        LQ_UART_PutBuff(MT9V032_COF_UART_1, send_buffer, 4);
//
//        //等待接受回传数据
//        while (!uart_receive_flag_1);
//        uart_receive_flag_1 = 0;
//
//        GET_CFG_1[i][1] = receive_1[1] << 8 | receive_1[2];
//
//    }
//}
//void get_config_2(void)
//{
//    uint16 temp, i;
//    uint8  send_buffer[4];
//
//    for (i = 0; i<CONFIG_FINISH - 1; i++)
//    {
//        send_buffer[0] = 0xA5;
//        send_buffer[1] = GET_STATUS;
//        temp = GET_CFG_2[i][0];
//        send_buffer[2] = temp >> 8;
//        send_buffer[3] = (uint8)temp;
//
//        LQ_UART_PutBuff(MT9V032_COF_UART_2, send_buffer, 4);
//
//        //等待接受回传数据
//        while (!uart_receive_flag_2);
//        uart_receive_flag_2 = 0;
//
//        GET_CFG_2[i][1] = receive_2[1] << 8 | receive_2[2];
//
//    }
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      获取摄像头固件版本
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:				调用该函数前请先初始化uart3
////-------------------------------------------------------------------------------------------------------------------
//uint16 get_version(void)
//{
//    uint16 temp;
//    uint8  send_buffer[4];
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = GET_STATUS;
//    temp = GET_VERSION;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    LQ_UART_PutBuff(MT9V032_COF_UART_1, send_buffer, 4);
//
//    //等待接受回传数据
//    while (!uart_receive_flag_1);
//    uart_receive_flag_1 = 0;
//
//    return ((uint16)(receive_1[1] << 8) | receive_1[2]);
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      单独设置摄像头曝光时间
////  @param      light   设置曝光时间越大图像越亮，摄像头收到后会根据分辨率及FPS计算最大曝光时间如果设置的数据过大，那么摄像头将会设置这个最大值
////  @return     uint16  当前曝光值，用于确认是否正确写入
////  @since      v1.0
////  Sample usage:				
////-------------------------------------------------------------------------------------------------------------------
//uint16 set_exposure_time(uint16 light)
//{
//    uint16 temp;
//    uint8  send_buffer[4];
//
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = SET_EXP_TIME;
//    temp = light;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    LQ_UART_PutBuff(MT9V032_COF_UART_1, send_buffer, 4);
//
//    //等待接受回传数据
//    while (!uart_receive_flag_1);
//    uart_receive_flag_1 = 0;
//
//    temp = receive_1[1] << 8 | receive_1[2];
//    return temp;
//
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      对摄像头内部寄存器进行写操作
////  @param      addr    摄像头内部寄存器地址
////  @param      data    需要写入的数据
////  @return     uint16  寄存器当前数据，用于确认是否写入成功
////  @since      v1.0
////  Sample usage:				
////-------------------------------------------------------------------------------------------------------------------
//uint16 set_mt9v032_reg_1(uint8 addr, uint16 data)
//{
//    uint16 temp;
//    uint8  send_buffer[4];
//
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = SET_ADDR;
//    temp = addr;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    LQ_UART_PutBuff(MT9V032_COF_UART_1, send_buffer, 4);
//
//    _systime.delay_ms(10);
//
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = SET_DATA;
//    temp = data;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    LQ_UART_PutBuff(MT9V032_COF_UART_1, send_buffer, 4);
//
//    UART_Put_Buff(MT9V032_COF_UART_1, send_buffer, 4);
//
//    //等待接受回传数据
//    while (!uart_receive_flag_1);
//    uart_receive_flag_1 = 0;
//
//    temp = receive_1[1] << 8 | receive_1[2];
//    return temp;
//
//}
//uint16 set_mt9v032_reg_2(uint8 addr, uint16 data)
//{
//    uint16 temp;
//    uint8  send_buffer[4];
//
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = SET_ADDR;
//    temp = addr;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    UART_Put_Buff(MT9V032_COF_UART_2, send_buffer, 4);
//    LPTMR_delay_ms(10);
//
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = SET_DATA;
//    temp = data;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    UART_Put_Buff(MT9V032_COF_UART_2, send_buffer, 4);
//
//    //等待接受回传数据
//    while (!uart_receive_flag_2);
//    uart_receive_flag_2 = 0;
//
//    temp = receive_2[1] << 8 | receive_2[2];
//    return temp;
//
//}
//volatile bool g_Transfer_Done = false;
//    edma_handle_t g_EDMA_Handle;
//    edma_handle_t g_EDMA_Handle_5;
//    edma_config_t userConfig;
//
//    edma_transfer_config_t transferConfig;
//
///* User callback function for EDMA transfer. */
//void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
//{
//    if (transferDone)
//    {
//        g_Transfer_Done = true;
//    }
//}
//
//uint16_t buff_tmr = 0;
//uint8_t buff_num = 0;
//uint32_t TestLiveWatch = 0;
////-------------------------------------------------------------------------------------------------------------------
////  @brief      MT9V032摄像头初始化
////  @param      NULL
////  @return     void					
////  @since      v1.0
////  Sample usage:		
////-------------------------------------------------------------------------------------------------------------------
//void camera_init_1(void)
//{
//    //设置参数    具体请参看使用手册
//    uint16 temp, i;
//    uint8  send_buffer[4];
//
//    LQ_UART_Init(MT9V032_COF_UART_1, 9600);	//初始换串口 配置摄像头    
//
//    //等待摄像头上电初始化成功
//    LPTMR_delay_ms(1000);
//    uart_receive_flag_1 = 0;
//
//    //开始配置摄像头并重新初始化
//    for (i = 0; i<CONFIG_FINISH; i++)
//    {
//        send_buffer[0] = 0xA5;
//        send_buffer[1] = MT9V032_CFG_1[i][0];
//        temp = MT9V032_CFG_1[i][1];
//        send_buffer[2] = temp >> 8;
//        send_buffer[3] = (uint8)temp;
//        UART_Put_Buff(MT9V032_COF_UART_1, send_buffer, 4);
//        LPTMR_delay_ms(2);
//    }
//    ////等待摄像头初始化成功
//    while (!uart_receive_flag_1);
//    uart_receive_flag_1 = 0;
//    while ((0xff != receive_1[1]) || (0xff != receive_1[2]));
//    //以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
//    //利用set_exposure_time函数单独配置的曝光数据不存储在eeprom中
//
//    //set_mt9v032_reg_1(0x05, 113);  //FPS微调
//
//    //获取配置便于查看配置是否正确
//    get_config_1();
//
//    //摄像头采集初始化
//    /*管脚初始化*/
//    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */
//    CLOCK_EnableClock(kCLOCK_Xbar1);            /* xbar1 clock (xbar1_clk_enable): 0x03U */
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,        /* GPIO_AD_B0_09 is configured as GPIO1_IO09 */
//        0U);//VST
//    //PCLK
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_14_XBAR1_INOUT12,        /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
//        (~(IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
//        | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL(0x01U) /* QTIMER2 TMR0 input select: input from XBAR */
//        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U) /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
//        );
//    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputQtimer2Tmr0Input); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT90 output assigned to QTIMER2_TMR0_INPUT */
//
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_00_GPIO2_IO00,           /* GPIO_B0_00 is configured as GPIO2_IO00 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_01_GPIO2_IO01,           /* GPIO_B0_01 is configured as GPIO2_IO01 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_02_GPIO2_IO02,           /* GPIO_B0_02 is configured as GPIO2_IO02 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_03_GPIO2_IO03,           /* GPIO_B0_03 is configured as GPIO2_IO03 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_04_GPIO2_IO04,           /* GPIO_B0_04 is configured as GPIO2_IO04 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_05_GPIO2_IO05,           /* GPIO_B0_05 is configured as GPIO2_IO05 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_06_GPIO2_IO06,           /* GPIO_B0_06 is configured as GPIO2_IO06 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_07_GPIO2_IO07,           /* GPIO_B0_07 is configured as GPIO2_IO07 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    //一般输入口管脚定义
//    gpio_pin_config_t GPIO_Input_Config = { kGPIO_DigitalInput,    //GPIO为输入方向
//        1,                    //高电平
//        kGPIO_NoIntmode,      //不触发中断
//    };
//    GPIO_PinInit(GPIO2, 0, &GPIO_Input_Config);        // GPIO输入口，非中断
//    GPIO_PinInit(GPIO2, 1, &GPIO_Input_Config);        // GPIO输入口，非中断
//    GPIO_PinInit(GPIO2, 2, &GPIO_Input_Config);        // GPIO输入口，非中断
//    GPIO_PinInit(GPIO2, 3, &GPIO_Input_Config);        // GPIO输入口，非中断
//    GPIO_PinInit(GPIO2, 4, &GPIO_Input_Config);        // GPIO输入口，非中断
//    GPIO_PinInit(GPIO2, 5, &GPIO_Input_Config);        // GPIO输入口，非中断
//    GPIO_PinInit(GPIO2, 6, &GPIO_Input_Config);        // GPIO输入口，非中断
//    GPIO_PinInit(GPIO2, 7, &GPIO_Input_Config);        // GPIO输入口，非中断
//
//
//
//    /*场中断初始化*/
////    gpio_pin_config_t GPIO_Input_Config = { kGPIO_DigitalInput,    //GPIO为输入方向
////        1,                    //高电平
////        kGPIO_NoIntmode,      //不触发中断
////    };
//    GPIO_PinInit(GPIO1, 9, &GPIO_Input_Config);
//    GPIO_PinSetInterruptConfig(GPIO1, 9, kGPIO_IntFallingEdge);
//    EnableIRQ(GPIO1_Combined_0_15_IRQn);
//
//    /*dMA*/
//    const qtmr_config_t QuadTimer_1_Channel_0_config = {
//    .primarySource = kQTMR_ClockDivide_2,
//    .secondarySource = kQTMR_Counter0InputPin,
//    .enableMasterMode = false,
//    .enableExternalForce = false,
//    .faultFilterCount = 0,
//    .faultFilterPeriod = 0,
//    .debugMode = kQTMR_RunNormalInDebug
//    };
//    /*QTMR输入捕捉*/
//    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */
//    CLOCK_EnableClock(kCLOCK_Xbar1);            /* xbar1 clock (xbar1_clk_enable): 0x03U */
//    
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_14_XBAR1_INOUT12,        /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
//        (~(IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
//        | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL(0x01U) /* QTIMER2 TMR0 input select: input from XBAR */
//        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U) /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
//        );
//    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputQtimer2Tmr0Input); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT90 output assigned to QTIMER2_TMR0_INPUT */
//    
//    //QTMR_GetDefaultConfig(&qtmrcpatureconfig);
//    QTMR_Init(TMR2, kQTMR_Channel_0, &QuadTimer_1_Channel_0_config);
//    QTMR_SetupInputCapture(TMR2, kQTMR_Channel_0, kQTMR_Counter0InputPin, false, false, kQTMR_FallingEdge);
//    QTMR_EnableDma(TMR2, kQTMR_Channel_0, kQTMR_InputEdgeFlagDmaEnable);
//    //PWM_StartTimer(PWM1, 1u << kPWM_Module_0); //开启定时器
//    /*eDMA初始化*/
//    DMAMUX_Init(DMAMUX);
//    //DMAMUX_EnableAlwaysOn(DMAMUX, 0, true);
//    DMAMUX_SetSource(DMAMUX, 5, kDmaRequestMuxQTIMER2CaptTimer0);
//    DMAMUX_EnableChannel(DMAMUX, 5);
//    DMAMUX_SetSource(DMAMUX, 6, 10);
//    DMAMUX_EnableChannel(DMAMUX, 6);    
//        
//    
//    EDMA_GetDefaultConfig(&userConfig);
//    //userConfig.enableContinuousLinkMode = false;
//    EDMA_Init(DMA0, &userConfig);
//    EDMA_CreateHandle(&g_EDMA_Handle, DMA0, 5);
//    EDMA_CreateHandle(&g_EDMA_Handle_5, DMA0, 6);
//    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
//    //EDMA_SetCallback(&g_EDMA_Handle_5, EDMA_Callback_5, NULL);
//    TestLiveWatch = ROW*COL;        
//    /*eDMA传输*/
//    EDMA_PrepareTransfer(&transferConfig, (uint8_t *)(&PT2_BYTE0), 1, image_1, 1
//        , 1, TestLiveWatch, kEDMA_PeripheralToMemory);
//    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
//    //EDMA_SetChannelPreemptionConfig(DMA0, 6, &eDMA_1_DMA5_preemption);
//    EDMA_SetChannelLink(DMA0, 5, kEDMA_MinorLink, 6);
//        
//    EDMA_PrepareTransfer(&transferConfig, (uint32_t *)(0x401E0004), 2, &buff_tmr, 2
//                        , 2, 2, 20);
//    EDMA_SubmitTransfer(&g_EDMA_Handle_5, &transferConfig);
//        
//    DMA0->CR &= ~DMA_CR_EMLM(1);
//    DMA0->TCD[6].CSR &= ~DMA_CSR_INTMAJOR(1);
//    DMA0->TCD[6].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(100);//100
//    DMA0->TCD[6].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(100);//100
//    DMA0->TCD[6].SOFF = DMA_SOFF_SOFF(0);
//    DMA0->TCD[6].DOFF = DMA_DOFF_DOFF(0);
//    
//    EDMA_StartTransfer(&g_EDMA_Handle_5);
//    EDMA_StartTransfer(&g_EDMA_Handle);   
//    QTMR_StartTimer(TMR2, kQTMR_Channel_0, kQTMR_PriSrcRiseEdge);
//    /* Wait for EDMA transfer finish */
//    while (g_Transfer_Done != true)
//    {
//      buff_num = *(uint32_t *)(PT2_BYTE0);
//    }
//
//    while (1)
//    {
//      g_Transfer_Done = false;
//        /*eDMA传输*/
//      EDMA_PrepareTransfer(&transferConfig, (uint8_t *)(&PT2_BYTE0), 1, image_1, 1
//          , 1, ROW*COL, kEDMA_PeripheralToMemory);
//      EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
//      //EDMA_SetChannelPreemptionConfig(DMA0, 6, &eDMA_1_DMA5_preemption);
//      EDMA_SetChannelLink(DMA0, 5, kEDMA_MinorLink, 6);
//          
//      EDMA_PrepareTransfer(&transferConfig, (uint32_t *)(0x401E0004), 2, &buff_tmr, 2
//                          , 2, 2, 20);
//      EDMA_SubmitTransfer(&g_EDMA_Handle_5, &transferConfig);
//          
//      DMA0->CR &= ~DMA_CR_EMLM(1);
//      DMA0->TCD[6].CSR &= ~DMA_CSR_INTMAJOR(1);
//      DMA0->TCD[6].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(100);
//      DMA0->TCD[6].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(100);
//      DMA0->TCD[6].SOFF = DMA_SOFF_SOFF(0);
//      DMA0->TCD[6].DOFF = DMA_DOFF_DOFF(0);
//      
//      DMA0->TCD[6].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(ROW*COL);
//      DMA0->TCD[6].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(ROW*COL);
//
//      EDMA_StartTransfer(&g_EDMA_Handle_5);
//      EDMA_StartTransfer(&g_EDMA_Handle); 
//        /* Wait for EDMA transfer finish */
//        while (g_Transfer_Done != true)
//        {
//            buff_num = *(uint32_t *)(PT2_BYTE0);
//        }
//    }
//}
////-------------------------------------------------------------------------------------------------------------------
////  @brief      第二个MT9V032摄像头初始化
////  @param      NULL
////  @return     void					
////  @since      v1.0
////  Sample usage:		
////-------------------------------------------------------------------------------------------------------------------
//void camera_init_2(void)
//{
//    //设置参数    具体请参看使用手册
//    uint16 temp, i;
//    uint8  send_buffer[4];
//
//
//    LQ_UART_Init(MT9V032_COF_UART_2, 9600);	//初始换串口 配置摄像头    
//    //UART_Irq_En(MT9V032_COF_UART_2);
//
//
//    //等待摄像头上电初始化成功
//    LPTMR_delay_ms(1000);
//    uart_receive_flag_2 = 0;
//
//    //开始配置摄像头并重新初始化
//    for (i = 0; i<CONFIG_FINISH; i++)
//    {
//        send_buffer[0] = 0xA5;
//        send_buffer[1] = MT9V032_CFG_2[i][0];
//        temp = MT9V032_CFG_2[i][1];
//        send_buffer[2] = temp >> 8;
//        send_buffer[3] = (uint8)temp;
//        UART_Put_Buff(MT9V032_COF_UART_2, send_buffer, 4);
//        LPTMR_delay_ms(2);
//    }
//    //等待摄像头初始化成功
//    while (!uart_receive_flag_2);
//    uart_receive_flag_2 = 0;
//    while ((0xff != receive_2[1]) || (0xff != receive_2[2]));
//    //以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
//    //利用set_exposure_time函数单独配置的曝光数据不存储在eeprom中
//
//    //set_mt9v032_reg_2(0x05, 113);  //FPS微调
//
//    //获取配置便于查看配置是否正确
//    get_config_2();
//
//    //摄像头采集初始化
//
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      MT9V032摄像头场中断
////  @param      NULL
////  @return     void			
////  @since      v1.0
////  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
////-------------------------------------------------------------------------------------------------------------------
////void VSYNC_1(void)
////{
////    //DMA_PORTx2BUFF_Init(DMA_CH4, (void *)&MT9V032_DATAPORT, (void *)image, MT9V032_PCLK, DMA_BYTE1, COL*ROW, DMA_rising_down_keepon);
////    DMA_DIS(DMA_CH4);
////    DMA_IRQ_CLEAN(DMA_CH4);
////    DMA0->TCD[DMA_CH4].BITER_ELINKNO |= DMA_BITER_ELINKNO_BITER(COL * ROW);
////    DMA0->TCD[DMA_CH4].CITER_ELINKNO |= DMA_CITER_ELINKNO_CITER(COL * ROW);
////    DMA0->TCD[DMA_CH4].DADDR = (uint32)image_1;
////
////    if (Image_1_DealState_Now == Image_Collecting)
////    {
////        Image_1_DealState_Now = Image_CollectFinish;
////    }
////
////    if (Image_1_DealState_Now == Image_DealingFinsh)
////    {
////        Image_1_DealState_Now = Image_Collecting;
////        DMA_IRQ_EN(DMA_CH4);
////        DMA_EN(DMA_CH4);
////    }
////
////}
////
////void VSYNC_2(void)
////{
////    //DMA_PORTx2BUFF_Init(DMA_CH4, (void *)&MT9V032_DATAPORT, (void *)image, MT9V032_PCLK, DMA_BYTE1, COL*ROW, DMA_rising_down_keepon);
////    DMA_DIS(DMA_CH5);
////    DMA_IRQ_CLEAN(DMA_CH5);
////    DMA0->TCD[DMA_CH5].BITER_ELINKNO |= DMA_BITER_ELINKNO_BITER(COL * ROW);
////    DMA0->TCD[DMA_CH5].CITER_ELINKNO |= DMA_CITER_ELINKNO_CITER(COL * ROW);
////    DMA0->TCD[DMA_CH5].DADDR = (uint32)image_2;
////
////    if (Image_2_DealState_Now == Image_Collecting)
////    {
////        Image_2_DealState_Now = Image_CollectFinish;
////    }
////
////    if (Image_2_DealState_Now == Image_DealingFinsh)
////    {
////        Image_2_DealState_Now = Image_Collecting;
////        DMA_IRQ_EN(DMA_CH5);
////        DMA_EN(DMA_CH5);
////    }
////
////}
////-------------------------------------------------------------------------------------------------------------------
////  @brief      总钻风摄像头图像发送至上位机查看图像
////  @param      NULL
////  @return     void			
////  @since      v1.0
////  Sample usage:				调用该函数前请先初始化uart2
////-------------------------------------------------------------------------------------------------------------------
//extern uint32_t fullCameraBufferAddr;
//void seekfree_sendimg_032(void)
//{
//    LQ_UART_PutChar(LPUART1, 0x00);
//    LQ_UART_PutChar(LPUART1, 0xff);
//    LQ_UART_PutChar(LPUART1, 0x01);
//    LQ_UART_PutChar(LPUART1, 0x01);
//    LQ_UART_PutBuff(LPUART1, (uint8_t *)fullCameraBufferAddr, ROW*COL);
//
//}
