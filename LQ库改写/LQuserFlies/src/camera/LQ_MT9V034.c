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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _lqmt9v034_reg
{
    uint16_t reg;
    uint8_t  size;
    uint32_t value;
} lqmt9v034_reg_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t lqmt9v034_Init(camera_device_handle_t *handle, const camera_config_t *config);
status_t lqmt9v034_Deinit(camera_device_handle_t *handle);
status_t lqmt9v034_Start(camera_device_handle_t *handle);
status_t lqmt9v034_Stop(camera_device_handle_t *handle);
status_t lqmt9v034_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg);
status_t lqmt9v034_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_device_operations_t lqmt9v034_ops = {
    .init     = lqmt9v034_Init,
    .deinit   = lqmt9v034_Deinit,
    .start    = lqmt9v034_Start,
    .stop     = lqmt9v034_Stop,
    .control  = lqmt9v034_Control,
    .init_ext = lqmt9v034_InitExt,
};

/*
[A: Default B: 80 dB HDR]
LOAD=Demo Initialization
REG= 0x08, 0x03ca 	// COARSE_SHUTTER_WIDTH_1_CONTEXTA [16]
REG= 0x09, 0x03de 	// COARSE_SHUTTER_WIDTH_2_CONTEXTA [15]
REG= 0x0A, 0x0064 	// SHUTTER_WIDTH_CONTROL_CONTEXTA [4]
REG= 0x0B, 0x03e8 	// COARSE_SHUTTER_WIDTH_TOTAL_CONTEXTA [2]
REG= 0x0F, 0x0103 	// PIXEL_OPERATION_MODE [8]
REG= 0x35, 0x8010 	// GLOBAL_GAIN_CONTEXTA_REG [1]
*/
static const lqmt9v034_reg_t ConfigFramFullScreenColor[] = {
    {0x08,1u, 0x01bb},
    {0x09,2u, 0x01dd},      
    {0x0A,1u, 0x0064},    
    {0x0B,2u, 0x03e8},    
    {0x0F,2u, 0x0103},   
    {0x35,2u, 0x8010}, 
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static status_t lqmt9v034_Read(TwoWireIf_i2c_t i2c, uint32_t reg, uint8_t size, uint32_t * value)
{
    status_t status;
    uint8_t data[6];
    uint8_t index = 0;

    data[index++] = (uint8_t)((reg >> 8U) & 0xFF);
    data[index++] = (uint8_t)(reg & 0xFF);

    while (1)
    {
        status = LPI2C_MasterStart(i2c,  MTV_DEVICE_WRITE_ADDRESS, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, data, index);

    LPI2C_MasterStart(i2c,  MTV_DEVICE_READ_ADDRESS, kLPI2C_Read);

    LPI2C_MasterReceive(i2c, data, size);

    status = LPI2C_MasterStop(i2c);

    if( size == 2)
    {
        *value = data[0];
        *value <<= 8U;
        *value += data[1];
    }
    else if( size == 4)
    {
        *value = data[0];
        *value <<= 8U;
        *value += data[1];
        *value <<= 8U;
        *value += data[2];
        *value <<= 8U;
        *value += data[3];
    }
    else if( size == 1)
    {
        *value = data[0];
    }

    return status;
}

static status_t lqmt9v034_Write(TwoWireIf_i2c_t i2c, uint16_t reg, uint8_t size, uint32_t value)
{
    status_t status;
    uint8_t data[6];
    uint8_t index = 0;

    data[index++] = (uint8_t)((reg >> 8U) & 0xFF);
    data[index++] = (uint8_t)(reg & 0xFF);

    if( size == 2)
    {
        data[index++] = (uint8_t)((value >> 8U) & 0xFF);
        data[index++] = (uint8_t)(value & 0xFF);
    }
    else if( size == 4)
    {
        data[index++] = (uint8_t)((value >> 24U) & 0xFF);
        data[index++] = (uint8_t)((value >> 26U) & 0xFF);
        data[index++] = (uint8_t)((value >> 8U) & 0xFF);
        data[index++] = (uint8_t)(value & 0xFF);
    }
    else if( size == 1)
    {
        data[index++] = value;
    }

    while (1)
    {
        status = LPI2C_MasterStart(i2c, lqmt9v034_TWO_WIRE_I2C_ADDR, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, data, index);

    return LPI2C_MasterStop(i2c);
}

static status_t lqmt9v034_MultiWrite(TwoWireIf_i2c_t i2c, const lqmt9v034_reg_t regs[], uint32_t num)
{
    status_t status = kStatus_Success;

    for (uint32_t i = 0; i < num; i++)
    {
        status = lqmt9v034_Write(i2c, regs[i].reg, regs[i].size, regs[i].value);

        if (kStatus_Success != status)
        {
            break;
        }
    }

    return status;
}

static void lqmt9v034_DelayMs(uint32_t ms)
{
    volatile uint64_t i;

    i = (uint64_t)SystemCoreClock * ms / 3000;
    while (i--)
    {
    }
}

static status_t lqmt9v034_SoftwareReset(TwoWireIf_i2c_t i2c)
{
    uint32_t value;

    lqmt9v034_Read(i2c, MTV_SOFT_RESET_REG, 2u, &value);
    lqmt9v034_Write(i2c, MTV_SOFT_RESET_REG, 2u, value | 0x03);
    lqmt9v034_DelayMs(50);
    lqmt9v034_Write(i2c, MTV_SOFT_RESET_REG, 2u, value & (~3));
    lqmt9v034_DelayMs(10);

    return kStatus_Success;
}

static status_t lqmt9v034_ChangeConfig(TwoWireIf_i2c_t i2c)
{
  /*
    uint32_t value;

    // Set the desired next state (SYS_STATE_ENTER_CONFIG_CHANGE = 0x28). 
    lqmt9v034_Write(i2c, lqmt9v034_VAR_SYSMGR_NEXT_STATE, 1u, lqmt9v034_SYS_STATE_ENTER_CONFIG_CHANGE);

    // Check that the FW is ready to accept a new command. 
    while(1)
    {
        lqmt9v034_Read(i2c, lqmt9v034_REG_COMMAND_REGISTER, 2u, &value);
        if(!(value & lqmt9v034_COMMAND_SET_STATE))
        {
            break;
        }
    }

    // Issue the Set State command. 
    lqmt9v034_Write(i2c, lqmt9v034_REG_COMMAND_REGISTER, 2u, lqmt9v034_COMMAND_SET_STATE | lqmt9v034_COMMAND_OK);

    // Wait for the FW to complete the command. 
    while(1)
    {
        lqmt9v034_DelayMs(10);
        lqmt9v034_Read(i2c, lqmt9v034_REG_COMMAND_REGISTER, 2u, &value);
        if(!(value & lqmt9v034_COMMAND_SET_STATE))
        {
            break;
        }
    }

    // Check the 'OK' bit to see if the command was successful. 
    lqmt9v034_Read(i2c, lqmt9v034_REG_COMMAND_REGISTER, 2u, &value);
    if(!(value & lqmt9v034_COMMAND_OK))
    {
        return kStatus_Fail;
    }

    // Check if the current state is streaming. 
    lqmt9v034_Read(i2c, lqmt9v034_VAR_SYSMGR_CURRENT_STATE, 1u, &value);
    if(value != lqmt9v034_SYS_STATE_STREAMING)
    {
        return kStatus_Fail;
    }

    // Check if the set-state command is successful. 
    lqmt9v034_Read(i2c, lqmt9v034_VAR_SYSMGR_CMD_STATUS, 1u, &value);
    if(value != lqmt9v034_SYS_STATE_SET_RESULT_ENOERR)
    {
        return kStatus_Fail;
    }

    LQMT9V034C_Set_Resolution(752,480);*/
    return kStatus_Success;
}

static status_t lqmt9v034_Init(camera_device_handle_t *handle, const camera_config_t *config)
{/*
    status_t status;
    uint16_t chip_id;

    lqmt9v034_resource_t *resource = (lqmt9v034_resource_t *)(handle->resource);
    TwoWireIf_i2c_t i2c = resource->twoWireIfI2C;

    if ((kCAMERA_InterfaceNonGatedClock != config->interface) &&
        (kCAMERA_InterfaceGatedClock != config->interface) &&
        (kCAMERA_InterfaceCCIR656 != config->interface))
    {
        return kStatus_InvalidArgument;
    }

    if ((FSL_VIDEO_EXTRACT_WIDTH(config->resolution) > 1280) ||
        (FSL_VIDEO_EXTRACT_HEIGHT(config->resolution) > 720))
    {
        return kStatus_InvalidArgument;
    }

    resource->pullPowerDownPin(true);       // Power down. 
    lqmt9v034_DelayMs(10);                    // Delay 10ms. 
    resource->pullPowerDownPin(false);      // Power up. 
    lqmt9v034_DelayMs(10);                    // Delay 10ms. 

    // Identify the device. 
    status = lqmt9v034_Read(i2c, lqmt9v034_REG_CHIP_ID, 2u, (uint32_t *)&chip_id);
    if (kStatus_Success != status)
    {
        return status;
    }
    if (lqmt9v034_CHIP_ID != chip_id)
    {
        return kStatus_Fail;
    }

    // SW reset. 
    lqmt9v034_SoftwareReset(i2c);

    // Configure video format. 
    status += lqmt9v034_MultiWrite(i2c, ConfigFramFullScreenColor, ARRAY_SIZE(ConfigFramFullScreenColor));

    if (status)
    {
        return kStatus_Fail;
    }

    // Execute Change-Config command. 
    lqmt9v034_ChangeConfig(i2c);

    LQMT9V034C_Set_Resolution(480,272);
*/
    return kStatus_Success;
}

static status_t lqmt9v034_Deinit(camera_device_handle_t *handle)
{
    ((lqmt9v034_resource_t *)(handle->resource))->pullPowerDownPin(true);

    return kStatus_Success;
}

static status_t lqmt9v034_Start(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

static status_t lqmt9v034_Stop(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

static status_t lqmt9v034_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg)
{
    return kStatus_Success;
}

static status_t lqmt9v034_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
    return lqmt9v034_Init(handle, config);
}
static status_t MT9V034_Read(TwoWireIf_i2c_t i2c, uint32_t reg, uint16_t value)
{
    status_t status;
    uint8_t data[6];   

    data[0] = (uint8_t)((reg >> 8U) & 0xFF);
    data[1] = (uint8_t)(reg & 0xFF);
    while (1)
    {
        status = LPI2C_MasterStart(i2c,MTV_DEVICE_WRITE_ADDRESS, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }
    LPI2C_MasterSend(i2c, data, 2);
    LPI2C_MasterStart(i2c,MTV_DEVICE_WRITE_ADDRESS, kLPI2C_Read);
    LPI2C_MasterReceive(i2c, data, 2);
    status = LPI2C_MasterStop(i2c); 
    value = data[0];
    value <<= 8U;
    value += data[1];    
    return status;
}
uint16_t MTV_ReadReg16(unsigned char address)
{  
  uint16_t Data;
  
   //MT9V034_Read(LPI2C1, address, Data);
     
  return Data;
}
static status_t MT9V034_Write(TwoWireIf_i2c_t i2c, uint16_t reg, uint16_t value)
{
    status_t status;
    uint8_t data[6];
    uint8_t index = 0;

    data[index++] = (uint8_t)((reg >> 8U) & 0xFF);
    data[index++] = (uint8_t)(reg & 0xFF);    
    data[index++] = (uint8_t)((value >> 8U) & 0xFF);
    data[index++] = (uint8_t)(value & 0xFF); 
    
    while (1)
    {
        status = LPI2C_MasterStart(i2c,MTV_DEVICE_WRITE_ADDRESS, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
            delayms(50); 
        }
        else
        {
            break;
        }
        
    }
    LPI2C_MasterSend(i2c, data, index);
    delayms(50); 
    return LPI2C_MasterStop(i2c);
}
unsigned char MTV_WriteReg16(unsigned char address, unsigned int Data)
{  
  unsigned char result=0;//MT9V034_Write(LPI2C1, address, Data);
  return result;
}

#define Context_A_Mode
//黑白摄像头，选择已有的窗口定义
//彩色摄像头 ，选择已有的窗口定义
void LQMT9V034C_Set_Resolution(unsigned int width,unsigned int height)
{
  unsigned int ret=0;
  unsigned int readmode = 0;
  
  //LQ_IIC_GPIO_Init();
  if(width> MAX_IMAGE_WIDTH)   width = MAX_IMAGE_WIDTH; //自定义窗口的宽度--列数 Frame_Width
  if(height> MAX_IMAGE_HEIGHT) height= MAX_IMAGE_HEIGHT;//自定义窗口的高度--行数 Frame_Height
  
  if ((width * 4) <= MAX_IMAGE_WIDTH && (height * 4) <= MAX_IMAGE_HEIGHT) 
  {
    width  *= 4; height *= 4;
    readmode |= MTV_READ_MODE_ROW_BIN_4;
    readmode |= MTV_READ_MODE_COL_BIN_4;
  } 
  else if ((width * 2) <= MAX_IMAGE_WIDTH && (height * 2) <= MAX_IMAGE_HEIGHT) 
  {
    width  *= 2; height *= 2;
    readmode |= MTV_READ_MODE_ROW_BIN_2;
    readmode |= MTV_READ_MODE_COL_BIN_2;
  }   
  
  readmode |= MTV_READ_MODE_ROW_FLIP;//上下反转 
  readmode |= MTV_READ_MODE_COL_FLIP;//左右镜像
  
#ifdef Context_A_Mode  
  {
  MTV_WriteReg16( MTV_READ_MODE_REG_A, readmode);         //0X0D 读取模式
 
  ret |= MTV_WriteReg16(MTV_WINDOW_WIDTH_REG_A, width);
  ret |= MTV_WriteReg16(MTV_COLUMN_START_REG_A, (MAX_IMAGE_WIDTH  - width ) / 2 + MINIMUM_COLUMN_START);
  
  ret |= MTV_WriteReg16( MTV_WINDOW_HEIGHT_REG_A, height);
  ret |= MTV_WriteReg16( MTV_ROW_START_REG_A, (MAX_IMAGE_HEIGHT - height) / 2 + MINIMUM_ROW_START);    
  
  ret |= MTV_WriteReg16(MTV_HOR_BLANKING_REG_A, MAX_IMAGE_WIDTH - width + MINIMUM_HORIZONTAL_BLANKING);//行中的空白列  0x05  消隐设置处赋值  752-64*4+1
  ret |= MTV_WriteReg16(MTV_VER_BLANKING_REG_A, 44);//列中的空白行  0x06   A: R0xAD-R0x08+7; M:R0x0B-R0x08+7;
  
  //设置曝光时间
  ret |= MTV_WriteReg16(MTV_COARSE_SW_1_REG_A, 0x01bb);   //0x08   0x01bb
  ret |= MTV_WriteReg16(MTV_COARSE_SW_2_REG_A, 0x01dd);   //0x09   0x01dd
  ret |= MTV_WriteReg16(MTV_COARSE_SW_CTRL_REG_A, 0x0164);//0x0A   0x0164
  ret |= MTV_WriteReg16(MTV_V2_CTRL_REG_A, 0x001A);       //0x32   0x001A  
  
  //0x07 Chip Control bit2-0:0逐行扫描,1无效，2场，3单场；bit5:1立体视觉使能,bit7:1并口使能；bit8:1同步模式;bit9:1正常操作；bit15:0A/1B
  ret |= MTV_WriteReg16(MTV_CHIP_CONTROL_REG, 0x0188);     //07 Context A  
  }
//-----------------------------------------------------------------------------------------------------------   

#elif defined ( Context_B_Mode )
  {
  MTV_WriteReg16( MTV_READ_MODE_REG_B, readmode);//0X0D 读取模式
  
  ret |= MTV_WriteReg16(MTV_WINDOW_WIDTH_REG_B, width);
  ret |= MTV_WriteReg16(MTV_COLUMN_START_REG_B, (MAX_IMAGE_WIDTH  - width ) / 2 + MINIMUM_COLUMN_START);
  
  ret |= MTV_WriteReg16( MTV_WINDOW_HEIGHT_REG_B, height);
  ret |= MTV_WriteReg16( MTV_ROW_START_REG_B, (MAX_IMAGE_HEIGHT - height) / 2 + MINIMUM_ROW_START);    
  
  ret |= MTV_WriteReg16(MTV_HOR_BLANKING_REG_B, MAX_IMAGE_WIDTH - width + MINIMUM_HORIZONTAL_BLANKING);//行中的空白列  0xCD  消隐设置处赋值  752-64*4+1
  ret |= MTV_WriteReg16(MTV_VER_BLANKING_REG_B, 45);//列中的空白行  0xCE   A: R0xAD-R0x08+7; M:R0xD2-R0x08+7;

  //设置曝光时间
  ret |= MTV_WriteReg16(MTV_COARSE_SW_1_REG_B, 0x01DE);   //0xCF   0x01DE-478
  ret |= MTV_WriteReg16(MTV_COARSE_SW_2_REG_B, 0x01DE);   //0xD0   0x01DE
  ret |= MTV_WriteReg16(MTV_COARSE_SW_CTRL_REG_B, 0x0164);//0xD1   0x0164
  ret |= MTV_WriteReg16(MTV_V2_CTRL_REG_B, 0x0036);       //0x3A   0x0036  
   
  ret |= MTV_WriteReg16(MTV_CHIP_CONTROL_REG, 0x8188);   //07 Context B  
  }
#endif
 
  // General Settings 
  ret |= MTV_WriteReg16(MTV_ROW_NOISE_CORR_CTRL_REG, 0);   //0x70  0x0000 
  ret |= MTV_WriteReg16(MTV_AEC_AGC_ENABLE_REG, 0x0000);   //0xAF  AEC/AGC A~bit0:1AE;bit1:1AG/B~bit2:1AE;bit3:1AG 
  //0xaf  0x0303  启动自暴光控制  启动自增益控制,灰度或者彩色,0x0000关闭AGC AEC  只有关闭这个，才能实现调整曝光时间，所谓拖影问题也是源自于此，曝光很重要，直接影响图像的效果
  //ret |= MTV_WriteReg16(MTV_HDR_ENABLE_REG, 0x0101);     // 0x0F bit8:1HDR,0linear; bit1:1彩色,0灰度;bit0:1HDR,0linear
  ret |= MTV_WriteReg16(MTV_HDR_ENABLE_REG, 0x0107);       // 0x0F bit8:1HDR,0linear; bit1:1彩色,0灰度;bit0:1HDR,0linear
  //0x0f  0x0000  
  ret |= MTV_WriteReg16(MTV_MIN_EXPOSURE_REG, 0x0001);      //0xAC  最小粗快门宽度   0x0001
  ret |= MTV_WriteReg16(MTV_MAX_EXPOSURE_REG, 0x01E0);      //0xAD  最大醋快门宽度   0x01E0-480
  ret |= MTV_WriteReg16(MTV_MAX_GAIN_REG, 50);              //0xAB  最大模拟增益     64
  readmode=width*height;
  if(readmode>44000)  readmode=0xABE0;
  //else  readmode=0x8000;
  ret |= MTV_WriteReg16(MTV_AGC_AEC_PIXEL_COUNT_REG, readmode);//0xB0  用于AEC/AGC直方图像素数目,最大44000   4096=320*240曝光值设置足够小，拍运动物体越好
  ret |= MTV_WriteReg16(MTV_AGC_AEC_DESIRED_BIN_REG, 58);   //0xA5  图像亮度  50  1-64
  ret |= MTV_WriteReg16(MTV_ADC_RES_CTRL_REG, 0x0303);      //0x1C  here is the way to regulate darkness :)    
  
  ret |= MTV_WriteReg16(MTV_SOFT_RESET_REG, 0x03);          //0x0c  复位
  
}
