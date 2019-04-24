/*!
* @file       Pig_UART.h
* @brief      根据官方的fsl_pit添加一些方便的接口
* @details    关于PIT中断优先级，如果有需求的话需要额外配置，一定要先用官方配置工具生成一个中断名定义
* @author     pig's grief
* @version    v1.0
* @date       2019-4-12
* @to do      
*/
# ifndef _PIG_PIT_H_
# define _PIG_PIT_H_
# include "fsl_pit.h"
/*!
* @brief PIT定时中断初始化
* 定时时长不要超过50秒，即TimeInterval <= 50 000 000
*
* @param PITn PIT通道，kPIT_Chnl_0、kPIT_Chnl_1等
* @param TimeInterval 要设定的定时间隔，单位微秒
* @param enableRunInDebug 是否让该PIT运行在DEBUG模式
* @example PIT_Init(kPIT_Chnl_0, 1000, false);//代表初始化1ms中断
*/
void LQ_PIT_Init(pit_chnl_t channel, uint32_t count);

# endif