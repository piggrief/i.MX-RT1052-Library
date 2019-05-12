/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技LPC546XX核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2017年11月27日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
----------------------------------------------------------------
使用PWM需要完成以下几个步骤：
第一步(共五步)，选择所用通道及管脚，开启通道选择宏定义    
第二步(共五步)，修改管脚宏定义，配置所用管脚               
第三步(共五步)，调用配置管脚函数，启用管脚对应功能         
第四步(共五步)，调用PWM初始化函数，设置PWM频率和初始占空比 
第五步(共五步)，调用PWM刷新函数，实现电机和舵机控制       
测试函数：Test_Servo();      测试PWM功能      数字舵机驱动控制               
测试函数：Test_Motor();      测试PWM功能      电机驱动控制                  
----------------------------------------------------------------
PWM1, kPWM_Module_0, kPWM_PwmA   J4
PWM1, kPWM_Module_0, kPWM_PwmB   J3
PWM1, kPWM_Module_1, kPWM_PwmA   J1
PWM1, kPWM_Module_1, kPWM_PwmB   K1
PWM1, kPWM_Module_2, kPWM_PwmA   H2
PWM1, kPWM_Module_2, kPWM_PwmB   J2
PWM1, kPWM_Module_3, kPWM_PwmA   L5     电机1接口
PWM1, kPWM_Module_3, kPWM_PwmB   M5     电机1接口

PWM2, kPWM_Module_0, kPWM_PwmA   A8     电机2接口
PWM2, kPWM_Module_0, kPWM_PwmB   A9     电机2接口
PWM2, kPWM_Module_1, kPWM_PwmA   B9     电机3接口
PWM2, kPWM_Module_1, kPWM_PwmB   C9     电机3接口
PWM2, kPWM_Module_2, kPWM_PwmA   D9
PWM2, kPWM_Module_2, kPWM_PwmB   A10    电机4接口
PWM2, kPWM_Module_3, kPWM_PwmA   M3      舵机接口（用电机4的时候不能用舵机，同一个PWM子模块A B相 频率一致）
PWM2, kPWM_Module_3, kPWM_PwmB   H10    电机4接口
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h" 

#define DUTY_MAX  10000     //duty最大值
void PWM_UpdateDuty(PWM_Type *base,
                            pwm_submodule_t subModule,
                            pwm_channels_t pwmSignal,
                            pwm_mode_t currPwmMode,
                            uint16_t duty)
{    
    assert(pwmSignal < 2);
    uint16_t pulseCnt = 0, pwmHighPulse = 0;
    int16_t modulo = 0;

    switch (currPwmMode)
    {
        case kPWM_SignedCenterAligned:
            modulo = base->SM[subModule].VAL1;
            pulseCnt = modulo * 2;
            /* Calculate pulse width */
            pwmHighPulse = (pulseCnt * duty) / DUTY_MAX;   //占空比 =  duty / DUTY_MAX 可修改

            /* Setup the PWM dutycycle */
            if (pwmSignal == kPWM_PwmA)
            {
                base->SM[subModule].VAL2 = (-(pwmHighPulse / 2));
                base->SM[subModule].VAL3 = (pwmHighPulse / 2);
            }
            else
            {
                base->SM[subModule].VAL4 = (-(pwmHighPulse / 2));
                base->SM[subModule].VAL5 = (pwmHighPulse / 2);
            }
            break;
        case kPWM_CenterAligned:
            pulseCnt = base->SM[subModule].VAL1;
            /* Calculate pulse width */
            pwmHighPulse = (pulseCnt * duty) / DUTY_MAX;   //占空比 =  duty / DUTY_MAX 可修改

            /* Setup the PWM dutycycle */
            if (pwmSignal == kPWM_PwmA)
            {
                base->SM[subModule].VAL2 = ((pulseCnt - pwmHighPulse) / 2);
                base->SM[subModule].VAL3 = ((pulseCnt + pwmHighPulse) / 2);
            }
            else
            {
                base->SM[subModule].VAL4 = ((pulseCnt - pwmHighPulse) / 2);
                base->SM[subModule].VAL5 = ((pulseCnt + pwmHighPulse) / 2);
            }
            break;
        case kPWM_SignedEdgeAligned:
            modulo = base->SM[subModule].VAL1;
            pulseCnt = modulo * 2;
            /* Calculate pulse width */
            pwmHighPulse =  (pulseCnt * duty) / DUTY_MAX;   //占空比 =  duty / DUTY_MAX 可修改

            /* Setup the PWM dutycycle */
            if (pwmSignal == kPWM_PwmA)
            {
                base->SM[subModule].VAL2 = (-modulo);
                base->SM[subModule].VAL3 = (-modulo + pwmHighPulse);
            }
            else
            {
                base->SM[subModule].VAL4 = (-modulo);
                base->SM[subModule].VAL5 = (-modulo + pwmHighPulse);
            }
            break;
        case kPWM_EdgeAligned:
            pulseCnt = base->SM[subModule].VAL1;
            /* Calculate pulse width */
            pwmHighPulse =  (pulseCnt * duty) / DUTY_MAX;   //占空比 =  duty / DUTY_MAX 可修改

            /* Setup the PWM dutycycle */
            if (pwmSignal == kPWM_PwmA)
            {
                base->SM[subModule].VAL2 = 0;
                base->SM[subModule].VAL3 = pwmHighPulse;
            }
            else
            {
                base->SM[subModule].VAL4 = 0;
                base->SM[subModule].VAL5 = pwmHighPulse;
            }
            break;
        default:
            break;
    }
}
//测试PWM占空比变化
void Test_PWM_nCH(void)
{    
    uint32_t pwmVal = 4;  
    
     /*初始化电机PWM接口  */
    LQ_PWM_Init(PWM2, kPWM_Module_0, kPWM_PwmA_B, 100);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz      A8 A9
    LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmA_B, 1000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     B9 C9
    LQ_PWM_Init(PWM2, kPWM_Module_2, kPWM_PwmB, 10000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz      A10
    LQ_PWM_Init(PWM1, kPWM_Module_3, kPWM_PwmA_B, 20000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz    L5 M5
    
    LQ_PWM_Init(PWM2, kPWM_Module_3, kPWM_PwmA_B,  200);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     M3 M4

    while (1U)
    {
        delayms(100);        //延时100毫秒
        pwmVal = pwmVal + 4;
        
        /* Reset the duty cycle percentage */
        if (pwmVal > 100)
        {
            pwmVal =2;
        }
        LQ_PWMA_B_SetDuty(PWM1, kPWM_Module_3, pwmVal*100, pwmVal*50); 
        LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_0, pwmVal*100, pwmVal*50);
        LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_1, pwmVal*100, pwmVal*50);
        LQ_PWM_SetDuty(PWM2,    kPWM_Module_2, kPWM_PwmB , pwmVal*100);
        LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_3, pwmVal*100, pwmVal*50);  
    }
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【参数值】uint8_t pwm_num = 1 表示 PWM1 模块，pwm_num = 2 表示 PWM2 模块，
          pwm_num = 3 表示 PWM3 模块，pwm_num = 4 表示 PWM4 模块
          pwm_submodule_t subModule  pwm子模块  
【软件版本】V1.0
【最后更新】2018年10月17日 
【函数名】 LQ_PWM_InitPins
【返回值】无
【备注】  函数只初始化PWM模块的PWMA引脚，使用PWMB引脚的请自行初始化
【实例】LQ_PWM_InitPins(1, kPWM_Module_0)  //初始化PWM1的kPWM_Module_0子模块的 PWMA引脚
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
/*   
 
  PWM模块    |      subModule0                          |     subModule1                               |     subModule2                          |      subModule3
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  PWM1       |IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00      |  IOMUXC_GPIO_SD_B0_02_FLEXPWM1_PWMA01        |IOMUXC_GPIO_SD_B0_04_FLEXPWM1_PWMA02     |IOMUXC_GPIO_AD_B0_10_FLEXPWM1_PWMA03
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  PWM2       |IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00         |   IOMUXC_GPIO_B0_08_FLEXPWM2_PWMA01          |IOMUXC_GPIO_B0_10_FLEXPWM2_PWMA02        |IOMUXC_GPIO_SD_B1_02_FLEXPWM2_PWMA03
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  PWM3       |IOMUXC_GPIO_EMC_29_FLEXPWM3_PWMA00        |  IOMUXC_GPIO_EMC_31_FLEXPWM3_PWMA01          |IOMUXC_GPIO_EMC_33_FLEXPWM3_PWMA02       |IOMUXC_GPIO_EMC_21_FLEXPWM3_PWMA03
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  PWM4       |IOMUXC_GPIO_AD_B1_08_FLEXPWM4_PWMA00      | IOMUXC_GPIO_AD_B1_09_FLEXPWM4_PWMA01         |IOMUXC_GPIO_B1_14_FLEXPWM4_PWMA02        |IOMUXC_GPIO_B1_15_FLEXPWM4_PWMA03
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void LQ_PWM_InitPins(uint8_t pwm_num, pwm_submodule_t subModule, pwm_channels_t pwm_channels)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);          /* 打开io时钟 */
    
    switch(pwm_num)//选择PWM模块
    {
        case 1:    //PWM1模块
        {
            switch(subModule)  //选择PWM子模块
            {
                case 0:        //选择PWM1模块的subModule0子模块
                {
                    if(pwm_channels == kPWM_PwmA || pwm_channels == kPWM_PwmA_B)  //使用PWMA要初始化的引脚
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00, 0U);   /* GPIO_SD_B0_00 is configured as FLEXPWM1_PWMA00 */
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00, 0x10B0u);   
                    }
                    if(pwm_channels == kPWM_PwmB || pwm_channels == kPWM_PwmA_B)  //使用PWMB要初始化的引脚
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00, 0U);   /* GPIO_SD_B0_00 is configured as FLEXPWM1_PWMA00 */
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00, 0x10B0u);  
                    }
                    break;
                }
                case 1:        //选择PWM1模块的subModule1子模块
                {
                    if(pwm_channels == kPWM_PwmA || pwm_channels == kPWM_PwmA_B)
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_FLEXPWM1_PWMA01, 0U);   //J1
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_FLEXPWM1_PWMA01, 0x10B0u);
                    }
                    if(pwm_channels == kPWM_PwmB || pwm_channels == kPWM_PwmA_B)
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_FLEXPWM1_PWMB01, 0U);   //K1
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_FLEXPWM1_PWMB01, 0x10B0u);  
                    }                                                         
                    break;
                }
                case 2:        //选择PWM1模块的subModule2子模块
                {
                    if(pwm_channels == kPWM_PwmA || pwm_channels == kPWM_PwmA_B)
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_04_FLEXPWM1_PWMA02, 0U);  //H2
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_FLEXPWM1_PWMA02, 0x10B0u);  
                    }
                    if(pwm_channels == kPWM_PwmB || pwm_channels == kPWM_PwmA_B)
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_FLEXPWM1_PWMB02, 0U);   //J2
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_FLEXPWM1_PWMB02, 0x10B0u);
                    }
                    break;
                }
                case 3:        //选择PWM1模块的subModule3子模块
                {
                    if(pwm_channels == kPWM_PwmA || pwm_channels == kPWM_PwmA_B)
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_FLEXPWM1_PWMA03, 0U);   //L5
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_00_FLEXPWM1_PWMA03, 0x10B0u);    
                    }
                    if(pwm_channels == kPWM_PwmB || pwm_channels == kPWM_PwmA_B)
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_FLEXPWM1_PWMB03, 0U);   //M5
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_01_FLEXPWM1_PWMB03, 0x10B0u);                    
                    }
                                                                          
                    break;
                }
                default:
                    break;
            }
            
            break;
        }
        case 2:    //PWM2模块
        {
            switch(subModule)  //选择PWM子模块
            {
                case 0:        //选择PWM2模块的subModule0子模块
                {
                    if(pwm_channels == kPWM_PwmA || pwm_channels == kPWM_PwmA_B)
                    {
                      IOMUXC_SetPinMux(IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00, 0U);    //A8
                      IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00, 0x10B0u);  
                    }
                    if(pwm_channels == kPWM_PwmB || pwm_channels == kPWM_PwmA_B)
                    {
                      IOMUXC_SetPinMux(IOMUXC_GPIO_B0_07_FLEXPWM2_PWMB00, 0U);    //A9
                      IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_07_FLEXPWM2_PWMB00, 0x10B0u);
                    }
                    break;
                }
                case 1:        //选择PWM2模块的subModule1子模块
                {
                    if(pwm_channels == kPWM_PwmA || pwm_channels == kPWM_PwmA_B)
                    {
                      IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_FLEXPWM2_PWMA01, 0U);    //B9
                      IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_08_FLEXPWM2_PWMA01, 0x10B0u); 
                    }
                    if(pwm_channels == kPWM_PwmB || pwm_channels == kPWM_PwmA_B)
                    {
                      IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_FLEXPWM2_PWMB01, 0U);     //C9
                      IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_09_FLEXPWM2_PWMB01, 0x10B0u);
                    }
                    break;
                }
                case 2:        //选择PWM2模块的subModule2子模块
                {
                    if(pwm_channels == kPWM_PwmA || pwm_channels == kPWM_PwmA_B)
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B0_10_FLEXPWM2_PWMA02, 0U);   //D9
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_10_FLEXPWM2_PWMA02, 0x10B0u);    
                    }
                    if(pwm_channels == kPWM_PwmB || pwm_channels == kPWM_PwmA_B)
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_FLEXPWM2_PWMB02, 0U);     //A10
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_11_FLEXPWM2_PWMB02, 0x10B0u);
                    }
                    break;
                }
                case 3:        //选择PWM2模块的subModule3子模块
                {
                    if(pwm_channels == kPWM_PwmA || pwm_channels == kPWM_PwmA_B)
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_FLEXPWM2_PWMA03, 0U);      //M3
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_02_FLEXPWM2_PWMA03, 0x10B0u);    
                    }
                    if(pwm_channels == kPWM_PwmB || pwm_channels == kPWM_PwmA_B)
                    {
                        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_FLEXPWM2_PWMB03, 0U);      //M4
                        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_03_FLEXPWM2_PWMB03, 0x10B0u); 
//                        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_01_FLEXPWM2_PWMB03, 0U);        //H10                    
//                        IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_01_FLEXPWM2_PWMB03, 0x10B0u); 
                    }
                                                                             
                    break;
                }
                default:
                    break;
            }
            
            break;
        }
        case 3:    //PWM3模块
        {
            switch(subModule)  //选择PWM子模块
            {
                case 0:        //选择PWM1模块的subModule0子模块
                {
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_29_FLEXPWM3_PWMA00, 0U);   /* GPIO_SD_B0_00 is configured as FLEXPWM1_PWMA00 */
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_29_FLEXPWM3_PWMA00, 0x10B0u);                                                         
                    break;
                }
                case 1:        //选择PWM1模块的subModule1子模块
                {
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_31_FLEXPWM3_PWMA01, 0U);   /* GPIO_SD_B0_00 is configured as FLEXPWM1_PWMA00 */
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_31_FLEXPWM3_PWMA01, 0x10B0u);                                                         
                    break;
                }
                case 2:        //选择PWM1模块的subModule2子模块
                {
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_33_FLEXPWM3_PWMA02, 0U);   /* GPIO_SD_B0_00 is configured as FLEXPWM1_PWMA00 */
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_33_FLEXPWM3_PWMA02, 0x10B0u);                                                         
                    break;
                }
                case 3:        //选择PWM1模块的subModule3子模块
                {
                    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_21_FLEXPWM3_PWMA03, 0U);   /* GPIO_SD_B0_00 is configured as FLEXPWM1_PWMA00 */
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_21_FLEXPWM3_PWMA03, 0x10B0u);                                                         
                    break;
                }
                default:
                    break;
            }
            
            break;
        }
        case 4:    //PWM4模块
        {
            switch(subModule)  //选择PWM子模块
            {
                case 0:        //选择PWM1模块的subModule0子模块
                {
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_FLEXPWM4_PWMA00, 0U);   /* GPIO_SD_B0_00 is configured as FLEXPWM1_PWMA00 */
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_08_FLEXPWM4_PWMA00, 0x10B0u);                                                         
                    break;
                }
                case 1:        //选择PWM1模块的subModule1子模块
                {
                    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_FLEXPWM4_PWMA01, 0U);   /* GPIO_SD_B0_00 is configured as FLEXPWM1_PWMA00 */
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_09_FLEXPWM4_PWMA01, 0x10B0u);                                                         
                    break;
                }
                case 2:        //选择PWM1模块的subModule2子模块
                {
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_FLEXPWM4_PWMA02, 0U);   /* GPIO_SD_B0_00 is configured as FLEXPWM1_PWMA00 */
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_FLEXPWM4_PWMA02, 0x10B0u);                                                         
                    break;
                }
                case 3:        //选择PWM1模块的subModule3子模块
                {
                    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_FLEXPWM4_PWMA03, 0U);   /* GPIO_SD_B0_00 is configured as FLEXPWM1_PWMA00 */
                    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_FLEXPWM4_PWMA03, 0x10B0u);                                                         
                    break;
                }
                default:
                    break;
            }
            
            break;
        }
        default:
            break;    
    }
    
    
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【参数值】
【软件版本】V1.0
【最后更新】2018年10月16日 
【函数名】 LQ_PWM4_Init
【返回值】无
【备注】  PWM时钟为200MHz的32分频 = 6250 000  PWM的VAL1寄存器为16位 最大值65535 PWM的最低频率 = 6250 000 / VAL1  = 96Hz  
           如果要使用96hz以下的PWM信号，可以降低PWM的时钟频率（增加pwmConfig.prescale的分频系数或者降低PWM时钟源kCLOCK_IpgClk频率）
【实例】  LQ_PWM_Init(PWM2, kPWM_Module_0, kPWM_PwmA, 1000); //PWM2的kPWM_Module_0子模块 初始化为频率1000hz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_PWM_Init(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels, uint32_t Frequency)
{
    if(base == PWM1)
    {
        LQ_PWM_InitPins(1, subModule, pwm_channels); //管脚复用选择
    }
    else if(base == PWM2)
    {
        LQ_PWM_InitPins(2, subModule, pwm_channels); //管脚复用选择
    }
    else if(base == PWM3)
    {
        LQ_PWM_InitPins(3, subModule, pwm_channels); //管脚复用选择
    }
    else if(base == PWM4)
    {
        LQ_PWM_InitPins(4, subModule, pwm_channels); //管脚复用选择
    }
    else printf("Please input PWM1 ~ PWM4 \n"); 
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0); /* Set AHB PODF to 0, divide by 1 */ //600MHz 
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x2); /* Set IPG PODF to 2, divede by 3 */ //600MHz 3分频 200MHz
    
    uint32_t pwmSourceClockInHz;   //PWM时钟源
    pwmSourceClockInHz = CLOCK_GetFreq(kCLOCK_IpgClk);//时钟源  200MHz
    printf("pwmSourceClockInHz  %d\n", pwmSourceClockInHz); 
    
    
    pwm_config_t pwmConfig;// PWM初始化结构体
    /*pwmConfig默认参数
    * pwmConfig.enableDebugMode = false;
    * pwmConfig.enableWait = false;
    * pwmConfig.reloadSelect = kPWM_LocalReload;
    * pwmConfig.faultFilterCount = 0;
    * pwmConfig.faultFilterPeriod = 0;
    * pwmConfig.clockSource = kPWM_BusClock;
    * pwmConfig.prescale = kPWM_Prescale_Divide_1;
    * pwmConfig.initializationControl = kPWM_Initialize_LocalSync;
    * pwmConfig.forceTrigger = kPWM_Force_Local;
    * pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity;
    * pwmConfig.reloadLogic = kPWM_ReloadImmediate;
    * pwmConfig.pairOperation = kPWM_Independent;   //PWMA和PWMB独立输出
    */
    PWM_GetDefaultConfig(&pwmConfig);  //得到默认的PWM初始化结构体
    
    /* Use full cycle reload */
    pwmConfig.reloadLogic   = kPWM_ReloadPwmFullCycle;   //循环输出
    
    pwmConfig.enableDebugMode = true;    
    pwmConfig.prescale = kPWM_Prescale_Divide_32;        //PWM时钟为 pwmSourceClockInHz 32分频 
//    pwmConfig.pairOperation = kPWM_ComplementaryPwmA;   //PWMA和PWMB互补输出
    /* Initialize submodule 0 */
    if (PWM_Init(base, subModule, &pwmConfig) == kStatus_Fail)
    {
        printf("PWM subModule %d initialization failed\n",subModule);
        return ;
    }  
    base->SM[subModule].DISMAP[0]=0;      //屏蔽故障检测功能
    
    pwm_signal_param_t pwmSignal[2];     //PWM子模块初始化结构体
    uint16_t deadTimeVal = 0;
    /* Set deadtime count, we set this to about 650ns */
    //  deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;//死区设定 多用于无刷电机PWMA和PWMB互补输出时使用 
    
    pwmSignal[0].pwmChannel = kPWM_PwmA; //默认使用通道A
    if(pwm_channels == kPWM_PwmB)   //如果使用通道B
    {
        pwmSignal[0].pwmChannel = kPWM_PwmB;  // 使用PWMB
    }
    pwmSignal[0].level = kPWM_HighTrue;   //输出电平为高电平
    pwmSignal[0].dutyCyclePercent = 0;    //初始占空比 0%
    pwmSignal[0].deadtimeValue = deadTimeVal;//死区时间
    
    /*当AB两通道同时使用 才有作用*/
    pwmSignal[1].pwmChannel = kPWM_PwmB;  // 使用PWMB
    pwmSignal[1].level = kPWM_HighTrue;   //输出电平为高电平
    pwmSignal[1].dutyCyclePercent = 0;    //初始占空比 0%
    pwmSignal[1].deadtimeValue = deadTimeVal;//死区时间
    
    if(pwm_channels == kPWM_PwmA_B)  //A B两通道同时使用
    {
           /*设置pwm的时钟 = pwmSourceClockInHz，频率 = Frequency 对齐方式 = kPWM_SignedCenterAligned，*/
        PWM_SetupPwm(base, subModule, pwmSignal, 2, kPWM_SignedCenterAligned, Frequency,pwmSourceClockInHz); 
    }
    else  //仅使用A B中的一个时
    {
         /*设置pwm的时钟 = pwmSourceClockInHz，频率 = Frequency 对齐方式 = kPWM_SignedCenterAligned，*/
        PWM_SetupPwm(base, subModule, pwmSignal, 1, kPWM_SignedCenterAligned, Frequency,pwmSourceClockInHz); 
    }   
    
    /* Set the load okay bit for all submodules to load registers from their buffer */
    PWM_SetPwmLdok(base, 1u<<subModule, true);//设置pwm的 load ok位
    
    /* Start the PWM generation from Submodules */
    PWM_StartTimer(base, 1u<<subModule); //开启定时器
    
    
  
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【参数值】  duty最大值默认为10000，当duty = 10000时，占空比为100%  可修改宏定义DUTY_MAX修改duty最大值
【软件版本】V1.0
【最后更新】2018年10月16日 
【函数名】 LQ_PWM_SetDuty
【返回值】无  如果同时使用subModule 的A B ，可以先设置A的Duty 再设置B的Duty 最后设置pwm的 load ok位  不然pwm容易卡死或者丢失
【备注】默认使用kPWM_PwmA，对齐方式kPWM_SignedCenterAligned， 可以自行修改  
【实例】LQ_PWM_SetDuty(PWM1, kPWM_Module_0, kPWM_PwmA, 5000)   //设置PWM1的kPWM_Module_0子模块的kPWM_PwmA 占空比为50%
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

inline void LQ_PWM_SetDuty(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels, uint16_t duty)
{
    assert(duty <= DUTY_MAX);
    PWM_UpdateDuty(base, subModule, pwm_channels, kPWM_SignedCenterAligned, duty); //设置DUTY  对齐方式kPWM_SignedCenterAligned， 可以自行修改
//    PWM_UpdateDuty(base, subModule, pwm_channels, kPWM_SignedCenterAligned, duty); //设置DUTY   对齐方式kPWM_SignedCenterAligned， 可以自行修改
    PWM_SetPwmLdok(base, 1u<<subModule, true);//设置pwm的 load ok位   如果同时使用subModule 的A B ，可以先设置A的Duty 再设置B的Duty 最后设置pwm的 load ok位  不然pwm容易卡死或者丢失
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【参数值】  duty最大值默认为10000，当duty = 10000时，占空比为100%  可修改宏定义DUTY_MAX修改duty最大值
【软件版本】V1.0
【最后更新】2018年10月16日 
【函数名】 LQ_PWM_SetDuty
【返回值】无  如果同时使用subModule 的A B ，可以先设置A的Duty 再设置B的Duty 最后设置pwm的 load ok位  不然pwm容易卡死或者丢失
【备注】默认使用kPWM_PwmA，对齐方式kPWM_SignedCenterAligned， 可以自行修改  
【实例】LQ_PWMA_B_SetDuty(PWM1, kPWM_Module_0, 5000, 1000)   //设置PWM1的kPWM_Module_0子模块的kPWM_PwmA 占空比为50%,PWMB占空比10%
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
inline void LQ_PWMA_B_SetDuty(PWM_Type *base, pwm_submodule_t subModule, uint16_t dutyA, uint16_t dutyB)
{
    assert( dutyA <= DUTY_MAX);
    assert( dutyB <= DUTY_MAX);
    PWM_UpdateDuty(base, subModule, kPWM_PwmA, kPWM_SignedCenterAligned, dutyA); //设置DUTY  对齐方式kPWM_SignedCenterAligned， 可以自行修改
    PWM_UpdateDuty(base, subModule, kPWM_PwmB, kPWM_SignedCenterAligned, dutyB); //设置DUTY   对齐方式kPWM_SignedCenterAligned， 可以自行修改
    PWM_SetPwmLdok(base, 1u<<subModule, true);//设置pwm的 load ok位   如果同时使用subModule 的A B ，可以先设置A的Duty 再设置B的Duty 最后设置pwm的 load ok位  不然pwm容易卡死或者丢失
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【功能说明】舵机占空比控制
【软件版本】V1.0
【最后更新】2017年11月24日 
【函数名】
【返回值】无
【注意】  初次使用此函数时，舵机不要装舵盘，防止舵机卡死，设置中值后再装车，可以自行设置中值
【参数值】舵机高电平时间在 500--2500us内  可以根据设置的PWM频率自己计算duty 这里舵机中值为3440,PWM频率200Hz 中值高电平时间为 3440/10000 * 5ms = 1.72ms
LQ_SetServoDty(duty);
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
inline void LQ_SetServoDty(uint16_t duty)  //设置舵机      舵机中值 1.72ms高电平的PWM波    1.5~2.0ms之间     
{
    if(duty > 3740) duty = 3740;
    if(duty < 3140) duty = 3140;
    LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_3, duty, duty); //默认是M3 H10 如果要使用M3 M4，请修改PWM初始化中的引脚初始化将H10改为M4的
//    LQ_PWM_SetDuty(PWM2, kPWM_Module_3, kPWM_PwmA, duty); //M3   
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】舵机占空比控制
【软件版本】V1.0
【最后更新】2017年11月24日 
【函数名】  注意： 电机1： 使用 L5  M5   电机2：使用A8  A9  电机3：使用 B9 C9  电机4：A10 H10
【返回值】电机正反转可以调节电机接线调整，也可以控制PWM调整  如果电机正反转和程序中定义的不一样，可以自行修改
【参数值】num: 电机编号 1~4   duty： -DUTY_MAX ~ DUTY_MAX   可以正反转
LQ_SetMotorDty(1, -2000)  //电机1  反转  占空比  2000 / 10000 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
inline void LQ_SetMotorDty(uint8_t num, short duty)  //设置电机占空比     
{
    assert(num < 5);
    
    switch(num)
    {
        case 1:
            if(duty >= 0)  //  正转  电机正反转可以调节电机接线调整，也可以控制PWM调整  如果电机正反转和程序中定义的不一样，可以自行修改
            {
                LQ_PWMA_B_SetDuty(PWM1, kPWM_Module_3, 0, duty);
            }
            else
            {
                LQ_PWMA_B_SetDuty(PWM1, kPWM_Module_3, -duty, 0);
            }
            break;
            
        case 2:
            if(duty >= 0)  //  正转  电机正反转可以调节电机接线调整，也可以控制PWM调整  如果电机正反转和程序中定义的不一样，可以自行修改
            {
                LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_0, 0, duty);
            }
            else
            {
                LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_0, -duty, 0);
            }
            break;
            
        case 3:
            if(duty >= 0)  //  正转  电机正反转可以调节电机接线调整，也可以控制PWM调整  如果电机正反转和程序中定义的不一样，可以自行修改
            {
                LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_1, 0, duty);
            }
            else
            {
                LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_1, -duty, 0);
            }
            break;
            
        case 4:
            if(duty >= 0)  //  正转  电机正反转可以调节电机接线调整，也可以控制PWM调整  如果电机正反转和程序中定义的不一样，可以自行修改
            {
                LQ_PWM_SetDuty(PWM2,    kPWM_Module_2, kPWM_PwmB , 0);
                LQ_PWM_SetDuty(PWM1,    kPWM_Module_1, kPWM_PwmA , duty);
            }
            else
            {
                LQ_PWM_SetDuty(PWM2,    kPWM_Module_2, kPWM_PwmB , -duty);
                LQ_PWM_SetDuty(PWM1,    kPWM_Module_1, kPWM_PwmA , 0);
            }
            break;
    }
  
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【参数值】
【软件版本】V1.0
【最后更新】2017年10月27日 
【函数名】
【返回值】无
【参数值】
//通过母板按键和舵机接口，测试舵机转向及舵机标定，显示于OLED
【实例】初次使用此函数时，舵机不要装舵盘，防止舵机卡死，设置中值后再装车，可以自行设置中值
【参数值】舵机高电平时间在 500--2500us内  可以根据设置的PWM频率自己计算duty 这里舵机中值为3440,PWM频率200Hz 中值高电平时间为 3440/10000 * 5ms = 1.72ms
LQ_SetServoDty(duty);
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_Servo(void)
{
    char txt[16];
    int servopwm = 0; 
    
    TFTSPI_Init();               //TFT1.8初始化  
    TFTSPI_CLS(u16BLUE);           //清屏
    LQ_KEY_Init();          //按键及输入口初始化   
    
    /*初始化舵机PWM接口  kPWM_PwmA  M3  M4*/
//    LQ_PWM_Init(PWM2, kPWM_Module_3, kPWM_PwmA,  200);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz  M3
    LQ_PWM_Init(PWM2, kPWM_Module_3, kPWM_PwmA_B,  200);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz //默认是M3 H10 如果要使用M3 M4，请修改PWM初始化中的引脚初始化将H10改为M4的
    
    LQ_SetServoDty(3440);  //中值
    
    TFTSPI_P8X16Str(3,0,(uint8_t*)"LQ Servo PWM",u16RED,u16BLUE);
    
    while (1)
    {    
        
        switch(LQ_KEY_Read(0))  //检测按键
        {
            case 0:
            break;
            case 1:
                servopwm -= 10;
                LQ_SetServoDty(3440 + servopwm);//刷新servopwm频率
                break;           
            case 2:           
                servopwm = 0;
                LQ_SetServoDty(3440 + servopwm);//刷新servopwm频率
                break;
            case 3:           
                servopwm += 10;
                LQ_SetServoDty(3440 + servopwm);//刷新servopwm频率
                break;
        }
        sprintf(txt,"PWM: %4.2f %",(float)servopwm/100.0f);
        TFTSPI_P8X16Str(3,2,(uint8_t*)txt,u16RED,u16BLUE);
        printf(txt); //P0_30口输出 
        
        //LED闪烁
        LED_Color(red);     //红灯   
        delayms(50);
        LED_Color(blue);     //蓝灯   
        delayms(50);
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】CHIUSIR
【参数值】
【软件版本】V1.0
【最后更新】2017年10月27日 
【函数名】
【返回值】无
【参数值】
//电机1： 使用 L5  M5   电机2：使用A8  A9  电机3：使用 B9 C9  电机4：A10 J1
【实例】
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_Motor(void)
{    
    char txt[16];
    short motorpwm=0;   
    TFTSPI_Init();               //TFT1.8初始化  
    TFTSPI_CLS(u16BLUE);           //清屏
    LQ_KEY_Init();          //按键及输入口初始化   
    /*初始化电机PWM接口   */
    LQ_PWM_Init(PWM2, kPWM_Module_0, kPWM_PwmA_B, 12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     A8 A9
    LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmA_B, 12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     B9 C9
    LQ_PWM_Init(PWM2, kPWM_Module_2, kPWM_PwmB,   12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     A10
    LQ_PWM_Init(PWM1, kPWM_Module_1, kPWM_PwmA,   12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     J1
    LQ_PWM_Init(PWM1, kPWM_Module_3, kPWM_PwmA_B, 12000);//PWM的最低频率 = 6250 000 / VAL1  = 96Hz     L5 M5
    TFTSPI_P8X16Str(3,0,(uint8_t*)"LQ Motor PWM",u16RED,u16BLUE);
    
    while (1)
    {        
        switch(LQ_KEY_Read(0))  //检测按键
        {
            case 0:
                break;
            case 1:
                motorpwm -= 100;
                if(motorpwm < -10000) motorpwm = -10000;
                LQ_SetMotorDty(1, motorpwm);
                LQ_SetMotorDty(2, motorpwm);
                LQ_SetMotorDty(3, motorpwm);
                LQ_SetMotorDty(4, motorpwm);
                break;           
            case 3:           
                motorpwm += 100;
                if(motorpwm > 10000) motorpwm = 10000;
                LQ_SetMotorDty(1, motorpwm);
                LQ_SetMotorDty(2, motorpwm);
                LQ_SetMotorDty(3, motorpwm);
                LQ_SetMotorDty(4, motorpwm);
                break;
            case 2:
                motorpwm = 0;
                LQ_SetMotorDty(1, motorpwm);
                LQ_SetMotorDty(2, motorpwm);
                LQ_SetMotorDty(3, motorpwm);
                LQ_SetMotorDty(4, motorpwm);
            break;
        }
        
        sprintf(txt,"PWM: %4.2f %",motorpwm/100.0f);
        TFTSPI_P8X16Str(3,3,(uint8_t*)txt,u16RED,u16BLUE);
        PRINTF(txt); //P0_30口输出 
        
        //LED闪烁
        LED_Color(red);     //红灯   
        delayms(50);
        LED_Color(blue);     //红灯   
        delayms(50);
    }
}


//