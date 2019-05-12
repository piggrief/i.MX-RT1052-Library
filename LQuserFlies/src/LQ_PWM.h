/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技LPC546XX核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2017年11月19日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#ifndef __LQPWM_H__
#define __LQPWM_H__


extern void LQ_SetMotorDty(uint8_t num, short duty);
extern void LQ_SetServoDty(uint16_t duty);

extern void Test_Servo(void);//舵机测试
extern void Test_Motor(void);//电机测试

extern void LQPWM_Init(void);
void Test_PWM_nCH(void);

extern void LQ_PWM_Init(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels, uint32_t Frequency);
extern void LQ_PWM_SetDuty(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels, uint16_t duty);
extern void LQ_PWMA_B_SetDuty(PWM_Type *base, pwm_submodule_t subModule, uint16_t duty1, uint16_t duty2);
#endif 



