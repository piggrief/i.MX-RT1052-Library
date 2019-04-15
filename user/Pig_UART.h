/*!
* @file       Pig_UART.c
* @brief      根据官方的fsl_lpuart添加一些方便的接口
* @details    关于UART的引脚、时钟需要用官方配置工具先生成
* @author     pig's grief
* @version    v1.0
* @date       2019-4-11
* @to do      增加UART的eDMA模式
*/
# ifndef _PIG_UART_H_
# define _PIG_UART_H_

#include "include.h"


/*!
* @brief UART初始化（轮询模式，无中断无DMA)
* 这里只提供修改波特率配置的选项，如果需要修改别的配置，需要自行修改相关的结构体和时钟频率
*
* @param UARTn UART通道，LPUART1、LPUART2等
* @param bound 要设定的波特率
*/
void UART_Init(LPUART_Type * UARTn, uint32_t bound);

/*!
* @brief UART接受一个字符（这里基本就是调用官方库）
*
* @param UARTn UART通道，LPUART1、LPUART2等
*/
uint8 UART_GetChar(LPUART_Type *UARTn);
/*!
* @brief UART发送一串字符串（这里基本就是调用官方库）
*
* @param UARTn UART通道，LPUART1、LPUART2等
* @param databuff 字符串数组
* @param datasize 发送数据数量
*/
void UART_Send(LPUART_Type *UARTn, uint8 * databuff, uint8 datasize);

# endif