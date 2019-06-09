/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2018年2月1日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.20.1及以上版本
【Target 】 i.MX RT1052
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 528MHz
【USB PLL】 480MHz
================================================
编码器接线：
J1--A相
F14--B相
A11--I索引

D10--A相
D10--B相
E10--I索引

B14    A相
K1 B相或者方向
J4 Z相

H10   A相
B11   B相或者方向
C10   Z相
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"	

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】脉冲正交解码功能引脚初始化
【软件版本】V1.0
【最后更新】2018年11月24日 
【函数名】
【返回值】无
【参数值】无
【实例】
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_ENC_InitPin(uint8_t enc_num)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);          /* 打开io时钟 */
    
    switch(enc_num)//选择ENC模块
    {
        case 1:    //ENC1模块
        {
            
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_XBAR1_INOUT06, 1U); //GPIO_AD_B0_10 配置为 XBAR1_IN22        G13   
            //IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,1U); //GPIO_AD_B0_09 配置为 XBAR1_IN21        F14                                         
            //IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_XBAR1_INOUT14, 1U); //GPIO_AD_B0_00 配置为 XBAR1_INOUT14  M14     
            IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_XBAR1_INOUT06, 0x10B0u);
            //IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_XBAR1_IN21,0x10B0u);                               
            //IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_00_XBAR1_INOUT14, 0x10B0u);
            break;
        }
        case 2:    //ENC2模块
        {
            IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08, 1U); //GPIO_AD_B0_02 配置为 XBAR1_INOUT06   J1
            //IOMUXC_SetPinMux(
            //    IOMUXC_GPIO_B0_13_XBAR1_INOUT11,        /* GPIO_B0_13 is configured as XBAR1_INOUT11 */
            //    0U);                                    /* Software Input On Field: Input Path is determined by functionality */
            //IOMUXC_SetPinMux(
             //   IOMUXC_GPIO_B0_14_XBAR1_INOUT12,        /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
             //   0U);                                    /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_XBAR1_INOUT08, 0x10B0u);   /* GPIO_AD_B0_09 PAD functional properties : */
            //IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_13_XBAR1_INOUT11, 0x10B0u);
            //IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_14_XBAR1_INOUT12, 0x10B0u);
            break;
        }
        case 3:    //ENC3模块
        {
            /*用户可自行配置，将引脚复用为XBAR模式*/
//            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_13_XBAR1_INOUT11,1U);    //  D10用之前确保没有当作别的用途
            IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_XBAR1_IN03, 1U); //  J3  用之前确保没有当作别的用途  
            //IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04,1U); //  J4  用之前确保没有当作别的用途 
            //IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07,1U); //  K1   用之前确保没有当作别的用途 
//            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_13_XBAR1_INOUT11,0x10B0u);  
            IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_XBAR1_IN03, 0x10B0u);
            //IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_XBAR1_INOUT04,0x10B0u);
            //IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07,0x10B0u);  
            break;
        }
        case 4:    //ENC4模块
        {
            /*用户可自行配置，将引脚复用为XBAR模式*/
//            IOMUXC_SetPinMux(IOMUXC_GPIO_B0_12_XBAR1_INOUT10,1U); //        C10  用之前确保没有当作别的用途
                       IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_XBAR1_INOUT14, 1U); //        C11  用之前确保没有当作别的用途                                       
            //IOMUXC_SetPinMux(
            //    IOMUXC_GPIO_B1_14_XBAR1_IN02,           /* GPIO_B1_14 is configured as XBAR1_IN02 */
            //    0U);                                    /* Software Input On Field: Input Path is determined by functionality */
            //            IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_12_XBAR1_INOUT10,0x10B0u);  
                       IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_00_XBAR1_INOUT14, 0x10B0u);
            //IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_XBAR1_IN02, 0x10B0u);
            break;
        }
        default:
            break;    
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】脉冲正交解码功能初始化
【软件版本】V1.0
【最后更新】2018年11月24日 
【函数名】
【返回值】无
【参数值】无
【实例】 LQ_ENC_Init(ENC1)  //初始化ENC1模块
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_ENC_Init(ENC_Type *base)
{
    if(base == ENC1)   //ECR 
    {
        LQ_ENC_InitPin(1); //管脚复用选择
        XBARA_Init(XBARA1);    //初始化XBARA1模块
        
        /*将单片机引脚连接到ENC1的内部触发信号上*/
        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout06, kXBARA1_OutputEnc1PhaseAInput);
//        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc1PhaseBInput);//F14   B相或者方向
//        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc1Index);   //M14   Z相
    }
    else if(base == ENC2) //ECL
    {
        LQ_ENC_InitPin(2); //管脚复用选择
        XBARA_Init(XBARA1);    //初始化XBARA1模块

        /*将单片机引脚连接到ENC2的内部触发信号上*/
        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout08, kXBARA1_OutputEnc2PhaseAInput);
//        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc2PhaseBInput);
//        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputEnc2Index);
    }
    else if(base == ENC3)
    {
        LQ_ENC_InitPin(3); //管脚复用选择
        /*用户可自行修改*/
        XBARA_Init(XBARA1);    //初始化XBARA？模块  可选参数XBARA1
        
        /*将单片机引脚连接到ENC2的内部触发信号上*/
        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn03, kXBARA1_OutputEnc3PhaseAInput);
//        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputEnc3PhaseBInput);//K1 B相或者方向
//        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout11, kXBARA1_OutputEnc3Index);      //D10    Z相
//        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout04, kXBARA1_OutputEnc3Index);      //J4 Z相
    }
    else if(base == ENC4)
    {
        LQ_ENC_InitPin(4); //管脚复用选择
        /*用户可自行修改*/
        XBARA_Init(XBARA1);    //初始化XBARA？模块  可选参数XBARA1
        
        /*将单片机引脚连接到ENC2的内部触发信号上*/
        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout14, kXBARA1_OutputEnc4PhaseAInput); /* IOMUX_XBAR_INOUT13 output assigned to XBARA1_IN13 input is connected to XBARA1_OUT81 output assigned to ENC4_PHASE_A_INPUT */
        //        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn02, kXBARA1_OutputEnc4PhaseBInput); /* IOMUX_XBAR_IN02 output assigned to XBARA1_IN2 input is connected to XBARA1_OUT82 output assigned to ENC4_PHASE_B_INPUT */
        //        XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout10, kXBARA1_OutputEnc4Index);      //C10   Z相
    }

    enc_config_t mEncConfigStruct;  //ENC初始化结构体
    ENC_GetDefaultConfig(&mEncConfigStruct);  //得到默认参数
    /* mEncConfigStruct默认参数
    config->enableReverseDirection = false;  
    config->decoderWorkMode = kENC_DecoderWorkAsNormalMode;  //A B两相模式
    config->HOMETriggerMode = kENC_HOMETriggerDisabled;
    config->INDEXTriggerMode = kENC_INDEXTriggerDisabled;
    config->enableTRIGGERClearPositionCounter = false;
    config->enableTRIGGERClearHoldPositionCounter = false;
    config->enableWatchdog = false;
    config->watchdogTimeoutValue = 0U;
    config->filterCount = 0U;
    config->filterSamplePeriod = 0U;
    config->positionMatchMode = kENC_POSMATCHOnPositionCounterEqualToComapreValue;
    config->positionCompareValue = 0xFFFFFFFFU;
    config->revolutionCountCondition = kENC_RevolutionCountOnINDEXPulse;
    config->enableModuloCountMode = false;
    config->positionModulusValue = 0U;
    config->positionInitialValue = 0U;*/
    if(base == ENC2 || base == ENC4)  
    {
        mEncConfigStruct.enableReverseDirection = true;  //反方向
    }
    
    mEncConfigStruct.decoderWorkMode = kENC_DecoderWorkAsSignalPhaseCountMode;  //LSB+Dir模式   kENC_DecoderWorkAsNormalMode： 普通正交解码
    ENC_Init(base, &mEncConfigStruct);     //初始化正交解码模块
    ENC_DoSoftwareLoadInitialPositionValue(base); /* 位置计数器初始值更新成0. */  
    
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【功能说明】测试脉冲正交解码功能
【软件版本】V1.0
【最后更新】2018年11月24日 
【函数名】
【返回值】无
【参数值】无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_ENCoder(void)
{   
    TFTSPI_Init();               //TFT1.8初始化 
    TFTSPI_CLS(u16BLUE);        //清屏
    LQ_ENC_Init(ENC1);   //正交解码初始化
    LQ_ENC_Init(ENC2);
    LQ_ENC_Init(ENC3);
    LQ_ENC_Init(ENC4);
    short velocity1, velocity2, velocity3, velocity4;
    char txt[16];
    printf("\r\n编码器正交解码测试:\r\n");    
    while (1)
    { 
        
        delayms(100);        //延时100毫秒
        velocity1 = (int16_t)ENC_GetPositionDifferenceValue(ENC1);  //得到编码器微分值
        sprintf(txt,"ENC1:  %5d ",velocity1); 
        TFTSPI_P8X16Str(0,0,(uint8_t*)txt,u16RED,u16BLUE);
        printf("ENC1:  %5d \n",velocity1);
        velocity2 = (int16_t)ENC_GetPositionDifferenceValue(ENC2);  //得到编码器微分值
        sprintf(txt,"ENC2:  %5d ",velocity2); 
        TFTSPI_P8X16Str(0,1,(uint8_t*)txt,u16RED,u16BLUE);
        printf("ENC2:  %5d \n",velocity2);
        velocity3 = (int16_t)ENC_GetPositionDifferenceValue(ENC3);  //得到编码器微分值
        sprintf(txt,"ENC3:  %5d ",velocity3); 
        TFTSPI_P8X16Str(0,2,(uint8_t*)txt,u16RED,u16BLUE);
        printf("ENC3:  %5d \n",velocity3);
        velocity4 = (int16_t)ENC_GetPositionDifferenceValue(ENC4);  //得到编码器微分值
        sprintf(txt,"ENC4:  %5d ",velocity4); 
        TFTSPI_P8X16Str(0,3,(uint8_t*)txt,u16RED,u16BLUE);
        printf("ENC4:  %5d \n",velocity4);
        printf("\n####################################\n");
        
        LED_Ctrl(LED_R, RVS); 
        delayms(100);       //延时0.1秒
    }
}

