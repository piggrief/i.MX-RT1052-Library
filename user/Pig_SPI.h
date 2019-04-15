/*!
* @file       Pig_SPI.h
* @brief      根据官方的fsl_lpspi添加一些方便的接口
* @details    关于SPI的引脚、时钟需要用官方配置工具先生成
* @author     pig's grief
* @version    v1.0
* @date       2019-4-10
* @to do      
*/
# ifndef _PIG_SPI_H_
# define _PIG_SPI_H_

# include "include.h"

extern const lpspi_master_config_t DefaultSPI_config;
extern const lpspi_master_config_t TFTSPI_config;
extern lpspi_master_config_t SPI1_config;
extern lpspi_master_config_t SPI2_config;
extern lpspi_master_config_t SPI3_config;
extern lpspi_master_config_t SPI4_config;


/*!
* @brief SPI初始化
*
* @param SPIn SPI通道，LPSPI1、LPSPI2等
* @param srcClock_Hz SPI时钟频率，一般在peripheral.h中定义
* @param IfDefault 选择是否使用默认SPI配置，为0则会直接使用相关配置，不为0则会先将相关配置变成默认配置
*/
void SPI_Init(LPSPI_Type * SPIn, uint32_t srcClock_Hz, int IfDefault);

/*!
* @brief SPI发送数据
*
* @param SPIn SPI通道，LPSPI1、LPSPI2等
* @param *cmdbuff 命令缓存区（写入缓存区）
* @param *databuff 数据缓存区（读取缓存区）
* @param dSize 传输数据个数（1个，2个等）
*/
void spi_mosi(LPSPI_Type * SPIn, uint8 * cmdbuff, uint8 * databuff, int dSize);


# endif