/*!
* @file       Pig_SPI.c
* @brief      根据官方的fsl_lpspi添加一些方便的接口
* @details    
* @author     pig's grief
* @version    v1.0
* @date       2019-4-10
* @to do
*/
# include "include.h"

//下面的是默认的SPI主机配置参数
const lpspi_master_config_t DefaultSPI_config = {
    .baudRate = 20000000,                       //波特率，按需求设定
    .bitsPerFrame = 8,                          //每帧传输的比特数，一般为8
    .cpol = kLPSPI_ClockPolarityActiveHigh,     //决定CLK是高电平有效还是低电平有效，一般为高电平有效
    .cpha = kLPSPI_ClockPhaseFirstEdge,         //决定数据线在第一个还是第二个跳变沿采样，一般为第一个跳变沿
    .direction = kLPSPI_MsbFirst,               //决定传输是MSB在前还是LSB在前，一般为LSB
    .pcsToSckDelayInNanoSec = 100,              //片选延迟（也就是片选后延时多久开始传输，由使用SPI的硬件决定）
    .lastSckToPcsDelayInNanoSec = 100,          //最后一个信号到片选的延迟，类似上面解释
    .betweenTransferDelayInNanoSec = 100,       //两次传输之间的时间，类似上面解释
    .whichPcs = kLPSPI_Pcs0,                    //要片选的端口
    .pcsActiveHighOrLow = kLPSPI_PcsActiveLow,  //片选信号低电平有效还是高电平有效，一般为低电平有效
    .pinCfg = kLPSPI_SdiInSdoOut,               //代表SDI和SDO数据输入输出的方向，一般为SPI入，SPO出
    .dataOutConfig = kLpspiDataOutRetained      //选择是否保留输出数据，一般选保留输出数据
};

//下面的是专门为TFT的SPI通讯设置的SPI主机配置参数
const lpspi_master_config_t TFTSPI_config = {
    .baudRate = 20000000,                       //波特率，按需求设定
    .bitsPerFrame = 8,                          //每帧传输的比特数，一般为8
    .cpol = kLPSPI_ClockPolarityActiveHigh,     //决定CLK是高电平有效还是低电平有效，一般为高电平有效
    .cpha = kLPSPI_ClockPhaseFirstEdge,         //决定数据线在第一个还是第二个跳变沿采样，一般为第一个跳变沿
    .direction = kLPSPI_MsbFirst,               //决定传输是MSB在前还是LSB在前，一般为LSB
    .pcsToSckDelayInNanoSec = 10,              //片选延迟（也就是片选后延时多久开始传输，由使用SPI的硬件决定）
    .lastSckToPcsDelayInNanoSec = 10,          //最后一个信号到片选的延迟，类似上面解释
    .betweenTransferDelayInNanoSec = 10,       //两次传输之间的时间，类似上面解释
    .whichPcs = kLPSPI_Pcs0,                    //要片选的端口
    .pcsActiveHighOrLow = kLPSPI_PcsActiveLow,  //片选信号低电平有效还是高电平有效，一般为低电平有效
    .pinCfg = kLPSPI_SdiInSdoOut,               //代表SDI和SDO数据输入输出的方向，一般为SPI入，SPO出
    .dataOutConfig = kLpspiDataOutRetained      //选择是否保留输出数据，一般选保留输出数据
};

//SPI1的主机配置参数
lpspi_master_config_t SPI1_config = {
    .baudRate = 20000000,                       //波特率，按需求设定
    .bitsPerFrame = 8,                          //每帧传输的比特数，一般为8
    .cpol = kLPSPI_ClockPolarityActiveHigh,     //决定CLK是高电平有效还是低电平有效，一般为高电平有效
    .cpha = kLPSPI_ClockPhaseFirstEdge,         //决定数据线在第一个还是第二个跳变沿采样，一般为第一个跳变沿
    .direction = kLPSPI_MsbFirst,               //决定传输是MSB在前还是LSB在前，一般为LSB
    .pcsToSckDelayInNanoSec = 100,              //片选延迟（也就是片选后延时多久开始传输，由使用SPI的硬件决定）
    .lastSckToPcsDelayInNanoSec = 100,          //最后一个信号到片选的延迟，类似上面解释
    .betweenTransferDelayInNanoSec = 100,       //两次传输之间的时间，类似上面解释
    .whichPcs = kLPSPI_Pcs0,                    //要片选的端口
    .pcsActiveHighOrLow = kLPSPI_PcsActiveLow,  //片选信号低电平有效还是高电平有效，一般为低电平有效
    .pinCfg = kLPSPI_SdiInSdoOut,               //代表SDI和SDO数据输入输出的方向，一般为SPI入，SPO出
    .dataOutConfig = kLpspiDataOutRetained      //选择是否保留输出数据，一般选保留输出数据
};

//SPI2的主机配置参数
lpspi_master_config_t SPI2_config = {
    .baudRate = 30000000,                       //波特率，按需求设定
    .bitsPerFrame = 8,                          //每帧传输的比特数，一般为8
    .cpol = kLPSPI_ClockPolarityActiveHigh,     //决定CLK是高电平有效还是低电平有效，一般为高电平有效
    .cpha = kLPSPI_ClockPhaseFirstEdge,         //决定数据线在第一个还是第二个跳变沿采样，一般为第一个跳变沿
    .direction = kLPSPI_MsbFirst,               //决定传输是MSB在前还是LSB在前，一般为LSB
    .pcsToSckDelayInNanoSec = 100,              //片选延迟（也就是片选后延时多久开始传输，由使用SPI的硬件决定）
    .lastSckToPcsDelayInNanoSec = 100,          //最后一个信号到片选的延迟，类似上面解释
    .betweenTransferDelayInNanoSec = 100,       //两次传输之间的时间，类似上面解释
    .whichPcs = kLPSPI_Pcs0,                    //要片选的端口
    .pcsActiveHighOrLow = kLPSPI_PcsActiveLow,  //片选信号低电平有效还是高电平有效，一般为低电平有效
    .pinCfg = kLPSPI_SdiInSdoOut,               //代表SDI和SDO数据输入输出的方向，一般为SPI入，SPO出
    .dataOutConfig = kLpspiDataOutRetained      //选择是否保留输出数据，一般选保留输出数据
};

//SPI3的主机配置参数
lpspi_master_config_t SPI3_config = {
    .baudRate = 30000000,                       //波特率，按需求设定
    .bitsPerFrame = 8,                          //每帧传输的比特数，一般为8
    .cpol = kLPSPI_ClockPolarityActiveHigh,     //决定CLK是高电平有效还是低电平有效，一般为高电平有效
    .cpha = kLPSPI_ClockPhaseFirstEdge,         //决定数据线在第一个还是第二个跳变沿采样，一般为第一个跳变沿
    .direction = kLPSPI_MsbFirst,               //决定传输是MSB在前还是LSB在前，一般为LSB
    .pcsToSckDelayInNanoSec = 100,              //片选延迟（也就是片选后延时多久开始传输，由使用SPI的硬件决定）
    .lastSckToPcsDelayInNanoSec = 100,          //最后一个信号到片选的延迟，类似上面解释
    .betweenTransferDelayInNanoSec = 100,       //两次传输之间的时间，类似上面解释
    .whichPcs = kLPSPI_Pcs0,                    //要片选的端口
    .pcsActiveHighOrLow = kLPSPI_PcsActiveLow,  //片选信号低电平有效还是高电平有效，一般为低电平有效
    .pinCfg = kLPSPI_SdiInSdoOut,               //代表SDI和SDO数据输入输出的方向，一般为SPI入，SPO出
    .dataOutConfig = kLpspiDataOutRetained      //选择是否保留输出数据，一般选保留输出数据
};

//SPI4的主机配置参数
lpspi_master_config_t SPI4_config = {
    .baudRate = 30000000,                       //波特率，按需求设定
    .bitsPerFrame = 8,                          //每帧传输的比特数，一般为8
    .cpol = kLPSPI_ClockPolarityActiveHigh,     //决定CLK是高电平有效还是低电平有效，一般为高电平有效
    .cpha = kLPSPI_ClockPhaseFirstEdge,         //决定数据线在第一个还是第二个跳变沿采样，一般为第一个跳变沿
    .direction = kLPSPI_MsbFirst,               //决定传输是MSB在前还是LSB在前，一般为LSB
    .pcsToSckDelayInNanoSec = 100,              //片选延迟（也就是片选后延时多久开始传输，由使用SPI的硬件决定）
    .lastSckToPcsDelayInNanoSec = 100,          //最后一个信号到片选的延迟，类似上面解释
    .betweenTransferDelayInNanoSec = 100,       //两次传输之间的时间，类似上面解释
    .whichPcs = kLPSPI_Pcs0,                    //要片选的端口
    .pcsActiveHighOrLow = kLPSPI_PcsActiveLow,  //片选信号低电平有效还是高电平有效，一般为低电平有效
    .pinCfg = kLPSPI_SdiInSdoOut,               //代表SDI和SDO数据输入输出的方向，一般为SPI入，SPO出
    .dataOutConfig = kLpspiDataOutRetained      //选择是否保留输出数据，一般选保留输出数据
};

void SPI_Init(LPSPI_Type * SPIn, uint32_t srcClock_Hz, int IfDefault)
{
    if (SPIn == LPSPI1)
    {
        if (IfDefault)
        {
            SPI1_config = DefaultSPI_config;
        }
        LPSPI_MasterInit(SPIn, &SPI1_config, srcClock_Hz);
    }
    else if (SPIn == LPSPI2)
    {
        if (IfDefault)
        {
            SPI2_config = DefaultSPI_config;
        }
        LPSPI_MasterInit(SPIn, &SPI2_config, srcClock_Hz);
    }
    else if (SPIn == LPSPI3)
    {
        if (IfDefault)
        {
            SPI3_config = DefaultSPI_config;
        }
        LPSPI_MasterInit(SPIn, &SPI3_config, srcClock_Hz);
    }
    else if (SPIn == LPSPI4)
    {
        if (IfDefault)
        {
            SPI4_config = DefaultSPI_config;
        }
        LPSPI_MasterInit(SPIn, &SPI4_config, srcClock_Hz);
    }
}

lpspi_transfer_t NowTrans;
void spi_mosi(LPSPI_Type * SPIn, uint8 * cmdbuff, uint8 * databuff, int dSize)
{
    NowTrans.dataSize = dSize;
    NowTrans.txData = cmdbuff;
    NowTrans.rxData = databuff;

    lpspi_which_pcs_t WhichPCS = kLPSPI_Pcs0;
    if (SPIn == LPSPI1)
        WhichPCS = SPI1_config.whichPcs;
    else if (SPIn == LPSPI2)
        WhichPCS = SPI2_config.whichPcs;
    else if (SPIn == LPSPI3)
        WhichPCS = SPI3_config.whichPcs;
    else if (SPIn == LPSPI4)
        WhichPCS = SPI4_config.whichPcs;

    if (WhichPCS == kLPSPI_Pcs0)
        NowTrans.configFlags = kLPSPI_MasterPcs0 | kLPSPI_MasterPcsContinuous;
    else if (WhichPCS == kLPSPI_Pcs1)
        NowTrans.configFlags = kLPSPI_MasterPcs1 | kLPSPI_MasterPcsContinuous;
    else if (WhichPCS == kLPSPI_Pcs2)
        NowTrans.configFlags = kLPSPI_MasterPcs2 | kLPSPI_MasterPcsContinuous;
    else if (WhichPCS == kLPSPI_Pcs3)
        NowTrans.configFlags = kLPSPI_MasterPcs3 | kLPSPI_MasterPcsContinuous;

    LPSPI_MasterTransferBlocking(SPIn, &NowTrans);
}

