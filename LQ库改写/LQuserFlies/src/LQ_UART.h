/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】Z
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2018年10月18日
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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_UART_H
#define __LQ_UART_H

#include "include.h"

extern void LQ_UART_Init(LPUART_Type *base, uint32_t bound);
extern void LQ_UART_PutChar(LPUART_Type *base, uint8_t data);
extern void LQ_UART_PutBuff(LPUART_Type *base, uint8_t * buff, uint32_t length);
extern void Test_UART(void);
extern void Test_UART_DMA(void);
extern void UART_DMA_Init(void);
#endif