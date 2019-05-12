/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技RT1052核心板
【编    写】Z
【备    注】
【软件版本】V1.0
【最后更新】2018年12月17日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
----------------------------------------------------------------
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"
/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

/*
# 真正意义上的 异步IO 是说内核直接将数据拷贝至用户态的内存单元，再通知程序直接去读取数据。
# select / poll / epoll 都是同步IO的多路复用模式

# 1.同步和异步
# 同步和异步关注的是消息通信机制
# 所谓同步，就是在发出一个*调用*时，没得到结果之前，该*调用*就不返回。但是一旦调用返回就得到返回值了，*调用者*主动等待这个*调用*的结果
# 所谓异步，就是在发出一个*调用*时，这个*调用*就直接返回了，不管返回有没有结果。当一个异步过程调用发出后，*被调用者*通过状态，通知来通知*调用者*，或者通过回调函数处理这个调用

# 2.阻塞和非阻塞
# 阻塞和非阻塞关注的是程序在等待调用结果时的状态
# 阻塞调用是指调用结果返回之前，当前线程会被挂起。调用线程只有在得到结果之后才返回
# 非阻塞调用是指在不能立即得到结果之前，该调用不会阻塞当前线程

# 网络上的例子
#老张爱喝茶，废话不说，煮开水。
#出场人物：老张，水壶两把（普通水壶，简称水壶；会响的水壶，简称响水壶）。
#1 老张把水壶放到火上，立等水开。（同步阻塞）；立等就是阻塞了老张去干别的事，老张得一直主动的看着水开没，这就是同步
#2 老张把水壶放到火上，去客厅看电视，时不时去厨房看看水开没有。（同步非阻塞）；老张去看电视了，这就是非阻塞了，但是老张还是得关注着水开没，这也就是同步了
#3 老张把响水壶放到火上，立等水开。（异步阻塞）；立等就是阻塞了老张去干别的事，但是老张不用时刻关注水开没，因为水开了，响水壶会提醒他，这就是异步了
#4 老张把响水壶放到火上，去客厅看电视，水壶响之前不再去看它了，响了再去拿壶。（异步非阻塞）；老张去看电视了，这就是非阻塞了，而且，等水开了，响水壶会提醒他，这就是异步了
#所谓同步异步，只是对于水壶而言。普通水壶，同步；响水壶，异步。对应的也就是消息通信机制
#虽然都能干活，但响水壶可以在自己完工之后，提示老张水开了。这是普通水壶所不能及的。同步只能让调用者去轮询自己（情况2中），造成老张效率的低下。
#所谓阻塞非阻塞，仅仅对于老张而言。立等的老张，阻塞；对应的也就是程序等待结果时的状态
#看电视的老张，非阻塞。
#情况1和情况3中老张就是阻塞的，媳妇喊他都不知道。虽然3中响水壶是异步的，可对于立等的老张没有太大的意义。所以一般异步是配合非阻塞使用的，这样才能发挥异步的效用。
*/

//IIC1   SCLk    J11
//IIC1   SDA     K11

#ifndef  IICBlocking   //IIC非堵塞模式

lpi2c_master_handle_t g_MasterHandle;  
fxos_handle_t g_fxosHandle;    //读取地址配置



volatile static bool g_completionFlag = false;
volatile static bool g_nakFlag = false;

//IIC回调函数
void IIC_master_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData)   //I2C回调函数
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)       //信号传输成功
    {
        g_completionFlag = true;
    }
    /* Signal transfer success when received success status. */
    if (status == kStatus_LPI2C_Nak)   
    {
        g_nakFlag = true;
    }
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】IIC初始化函数
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】base：LPI2C1~ LPI2C4   baudrate:IIC波特率
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LPI2C_Init(LPI2C_Type *base, uint32_t baudrate)
{ 
    
    lpi2c_master_config_t i2cConfig;  //I2C(主机)配置结构体
    uint32_t i2cSourceClock;    //I2C   时钟
    
    BOARD_InitLPI2C1Pins();      //I2C1  引脚复用设置   使用别的IIC 需要自行修改引脚
    
    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT); //USB1 PLL (480 MHz) as master lpi2c clock source  480/8 = 60 MHz
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);// 60 / 6 = 10MHz

    i2cSourceClock = LPI2C_CLOCK_FREQUENCY;                   //  10M
    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    i2cConfig.baudRate_Hz = baudrate;
    LPI2C_MasterGetDefaultConfig(&i2cConfig);                //得到默认结构体
    
    LPI2C_MasterInit(base, &i2cConfig, i2cSourceClock);//按默认配置

    LPI2C_MasterTransferCreateHandle(base, &g_MasterHandle, IIC_master_callback, NULL);  //使用IIC非阻塞传输
    
    g_fxosHandle.base = base;
    g_fxosHandle.i2cHandle = &g_MasterHandle;
}
//iic 读寄存器
status_t IIC_ReadReg(fxos_handle_t *handle, uint8_t reg, uint8_t *val, uint8_t bytesNumber)  //读寄存器函数
{
    status_t status = kStatus_Success;

    /* Configure I2C xfer */
    handle->xfer.subaddress = (uint32_t)reg;              //寄存器地址
    handle->xfer.subaddressSize = 1U;                     //按8位读
    handle->xfer.data = val;                              //存放读出的数据
    handle->xfer.dataSize = bytesNumber;                  //要读多少个数据
    handle->xfer.direction = kLPI2C_Read;                 //I2C读模式
    handle->xfer.flags = kLPI2C_TransferDefaultFlag;

    if(LPI2C_MasterTransferNonBlocking(handle->base, handle->i2cHandle, &handle->xfer) == kStatus_Fail) 
    {
        return kStatus_Fail;
    }
    /*  wait for transfer completed. */
    while ((!g_nakFlag) && (!g_completionFlag))//等待传输完成  相当于异步堵塞   可以不等传输完成
    {
    }

    g_nakFlag = false;

    if (g_completionFlag == true)
    {
        g_completionFlag = false;
    }
    else
    {
        status = kStatus_Fail;
    }


    return status;
}
//iic 写寄存器
status_t IIC_WriteReg(fxos_handle_t *handle, uint8_t reg, uint8_t val)    //写寄存器函数
{
    status_t status = kStatus_Success;
    uint8_t buff[1];

    buff[0] = val;
    /* Set I2C xfer structure */
    handle->xfer.subaddress = (uint32_t)reg;      //寄存器地址
    handle->xfer.subaddressSize = 1U;             //按8位读
    handle->xfer.data = buff;                     //存放要写入的数据
    handle->xfer.dataSize = 1;          //要读多少个数据                                            
    handle->xfer.direction = kLPI2C_Write;        //I2C读模式
    handle->xfer.flags = kLPI2C_TransferDefaultFlag;


    if(LPI2C_MasterTransferNonBlocking(handle->base, handle->i2cHandle, &handle->xfer) == kStatus_Fail)
    {
        return kStatus_Fail;
    }

    /*  wait for transfer completed. */
    while ((!g_nakFlag) && (!g_completionFlag))   //等待传输完成
    {
    }

    g_nakFlag = false;

    if (g_completionFlag == true)
    {
        g_completionFlag = false;
    }
    else
    {
        status = kStatus_Fail;
    }
    return status;
}


#else  //iic 堵塞模式

lpi2c_master_transfer_t master_transfer; //传输结构体
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】IIC初始化函数
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】base：LPI2C1~ LPI2C4   baudrate:IIC波特率
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LPI2C_Init(LPI2C_Type *base, uint32_t baudrate)
{ 
    
    lpi2c_master_config_t i2cConfig;  //I2C1(主机)配置结构体
    uint32_t i2cSourceClock;    //I2C   时钟
    
    BOARD_InitLPI2C1Pins();      //I2C  引脚复用设置   使用别的IIC 需要自行修改引脚
    
    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT); //USB1 PLL (480 MHz) as master lpi2c clock source  480/8 = 60 MHz
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);// 60 / 6 = 10MHz

    i2cSourceClock = LPI2C_CLOCK_FREQUENCY;                   //  10M
    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    i2cConfig.baudRate_Hz = baudrate;
    LPI2C_MasterGetDefaultConfig(&i2cConfig);                //得到默认结构体
    
    LPI2C_MasterInit(base, &i2cConfig, i2cSourceClock);//按默认配置

}
//iic 读寄存器
status_t IIC_ReadReg(uint16_t addr, uint8_t reg, uint8_t *val, uint8_t bytesNumber)  //读寄存器函数
{
    status_t status = kStatus_Success;
    
    /* Configure I2C xfer */
    master_transfer.slaveAddress = addr;
    master_transfer.subaddress = (uint32_t)reg;              //寄存器地址
    master_transfer.subaddressSize = 1U;                     //按8位读
    master_transfer.data = val;                              //存放读出的数据
    master_transfer.dataSize = bytesNumber;                  //要读多少个数据
    master_transfer.direction = kLPI2C_Read;                 //I2C读模式
    master_transfer.flags = kLPI2C_TransferDefaultFlag;

    if(LPI2C_MasterTransferBlocking(LPI2C1,&master_transfer)==kStatus_Fail)
    {
        return kStatus_Fail;
    }
    return status;
}
//iic 写寄存器
status_t IIC_WriteReg(uint16_t addr, uint8_t reg, uint8_t val)    //写寄存器函数
{
    status_t status = kStatus_Success;
    uint8_t buff[1];

    buff[0] = val;
    /* Set I2C xfer structure */
    master_transfer.slaveAddress = addr;
    master_transfer.subaddress = (uint32_t)reg;              //寄存器地址
    master_transfer.subaddressSize = 1U;                     //按8位读
    master_transfer.data = buff;                              //要写入的数据
    master_transfer.direction = kLPI2C_Write;                 //I2C写模式
    master_transfer.flags = kLPI2C_TransferDefaultFlag;

    if(LPI2C_MasterTransferBlocking(LPI2C1,&master_transfer)==kStatus_Fail)
    {
        return kStatus_Fail;
    }
    return status;
}


#endif