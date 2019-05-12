#include "include.h"

/******************************************************************************
 * Code
 ******************************************************************************/



extern fxos_handle_t g_fxosHandle;
status_t FXOS_Init(fxos_handle_t *fxos_handle)
{
    uint8_t tmp[1] = {0};
    g_fxosHandle.xfer.slaveAddress = 0x1e;   //8700地址
    if(FXOS_ReadReg(fxos_handle, WHO_AM_I_REG, tmp, 1) != kStatus_Success) //读取WHO_AM_I 寄存器
    {
        return kStatus_Fail;
    }

    if (tmp[0] != kFXOS_WHO_AM_I_Device_ID)    //判断 WHO_AM_I 的值是否正确
    {
        return kStatus_Fail;
    }

    /* setup auto sleep with FFMT trigger */
    /* go to standby */
    if(FXOS_ReadReg(fxos_handle, CTRL_REG1, tmp, 1) != kStatus_Success)  //读取CTRL_REG1寄存器
    {
        return kStatus_Fail;
    }

    if(FXOS_WriteReg(fxos_handle, CTRL_REG1, tmp[0] & (uint8_t)~ACTIVE_MASK) != kStatus_Success)  //写CTRL_REG1寄存器，使FX8700处于待机状态 
    {
        return kStatus_Fail;
    }

    /* Read again to make sure we are in standby mode. */
    if(FXOS_ReadReg(fxos_handle, CTRL_REG1, tmp, 1) != kStatus_Success)  //再一次读，确保处于待机状态，以便下面配置其他寄存器
    {
        return kStatus_Fail;
    }
    if ((tmp[0] & ACTIVE_MASK) == ACTIVE_MASK)
    {
        return kStatus_Fail;
    }

    /* Disable the FIFO */
    if(FXOS_WriteReg(fxos_handle, F_SETUP_REG, F_MODE_DISABLED) != kStatus_Success)  //禁用FIFO
    {
        return kStatus_Fail;
    }

#ifdef LPSLEEP_HIRES
    /* enable auto-sleep, low power in sleep, high res in wake */
    if(FXOS_WriteReg(fxos_handle, CTRL_REG2, SLPE_MASK | SMOD_LOW_POWER | MOD_HIGH_RES) != kStatus_Success)  //使用低功耗，自动睡眠，高分辨
    {
        return kStatus_Fail;
    }
#else
    /* enable auto-sleep, low power in sleep, high res in wake */
    if(FXOS_WriteReg(fxos_handle, CTRL_REG2, MOD_HIGH_RES) != kStatus_Success)  //使用高分辨率模式
    {
        return kStatus_Fail;
    }

#endif

    /* set up Mag OSR and Hybrid mode using M_CTRL_REG1, use default for Acc */
    if(FXOS_WriteReg(fxos_handle, M_CTRL_REG1, (M_RST_MASK | M_OSR_MASK | M_HMS_MASK)) != kStatus_Success)//混合模式，加计和地磁计同时使用
    {
        return kStatus_Fail;
    }

    /* Enable hyrid mode auto increment using M_CTRL_REG2 */
    if(FXOS_WriteReg(fxos_handle, M_CTRL_REG2, (M_HYB_AUTOINC_MASK)) != kStatus_Success)  //当hyb_autoinc_mode = 1且快读模式被禁用(ctrl l_reg1 [f_read] = 0)时，在读完寄存器x06 (OUT_Z_LSB)后，寄存器地址将自动前进到寄存器x33 (M_OUT_X_MSB)。对于hyb_autoinc_mode = 1和启用快速读取模式(ctrl l_reg1 [f_read = 1])，在burstread模式下读取寄存器x05 (OUT_Z_MSB)后，寄存器地址将自动前进到寄存器x33 (M_OUT_X_MSB)。
    {
        return kStatus_Fail;
    }

#ifdef EN_FFMT
    /* enable FFMT for motion detect for X and Y axes, latch enable */
    if(FXOS_WriteReg(fxos_handle, FF_MT_CFG_REG, XEFE_MASK | YEFE_MASK | ELE_MASK | OAE_MASK) != kStatus_Success)  //自由落体运动/配置寄存器
    {
        return kStatus_Fail;
    }
#endif

#ifdef SET_THRESHOLD
    /* set threshold to about 0.25g */
    if(FXOS_WriteReg(fxos_handle, FT_MT_THS_REG, 0x04) != kStatus_Success)//自由落体/运动检测阈值:默认值:0b000_0000。分辨率固定在63 mg/LSB。
    {
        return kStatus_Fail;
    }
#endif

#ifdef SET_DEBOUNCE
    /* set debounce to zero */
    if(FXOS_WriteReg(fxos_handle, FF_MT_COUNT_REG, 0x00) != kStatus_Success)
    {
        return kStatus_Fail;
    }
#endif

#ifdef EN_AUTO_SLEEP                
    /* set auto-sleep wait period to 5s (=5/0.64=~8) */
    if(FXOS_WriteReg(fxos_handle, ASLP_COUNT_REG, 8) != kStatus_Success)  //自动睡眠
    {
        return kStatus_Fail;
    }
#endif
    /* default set to 4g mode */
    if(FXOS_WriteReg(fxos_handle, XYZ_DATA_CFG_REG, FULL_SCALE_4G) != kStatus_Success)  //加计 正负4g模式
    {
        return kStatus_Fail;
    }
#ifdef EN_INTERRUPTS                        //使能中断
    /* enable data-ready, auto-sleep and motion detection interrupts */
    /* FXOS1_WriteRegister(CTRL_REG4, INT_EN_DRDY_MASK | INT_EN_ASLP_MASK | INT_EN_FF_MT_MASK); */
    if(FXOS_WriteReg(fxos_handle, CTRL_REG4, 0x0) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    /* route data-ready interrupts to INT1, others INT2 (default) */
    if(FXOS_WriteReg(fxos_handle, CTRL_REG5, INT_CFG_DRDY_MASK) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    /* enable ffmt as a wake-up source */
    if(FXOS_WriteReg(fxos_handle, CTRL_REG3, WAKE_FF_MT_MASK) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    /* finally activate accel_device with ASLP ODR=0.8Hz, ODR=100Hz, FSR=2g */
    if(FXOS_WriteReg(fxos_handle, CTRL_REG1, HYB_ASLP_RATE_0_8HZ | HYB_DATA_RATE_100HZ | ACTIVE_MASK) != kStatus_Success)
    {
        return kStatus_Fail;
    }
#else
    /* Setup the ODR for 200 Hz and activate the accelerometer */
    if(FXOS_WriteReg(fxos_handle, CTRL_REG1, (HYB_DATA_RATE_200HZ | ACTIVE_MASK)) != kStatus_Success)//设置数据输出频率 200hz 并且激活FX8700
    {
        return kStatus_Fail;
    }
#endif

    /* Read Control register again to ensure we are in active mode */
    if(FXOS_ReadReg(fxos_handle, CTRL_REG1, tmp, 1) != kStatus_Success)  //确保FX8700不是在待机状态
    {
        return kStatus_Fail;
    }

    if ((tmp[0] & ACTIVE_MASK) != ACTIVE_MASK)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

status_t FXOS_ReadSensorData(fxos_handle_t *fxos_handle, fxos_data_t *sensorData)    //读FX8700原始数据函数
{
    g_fxosHandle.xfer.slaveAddress = 0x1e;   //8700地址
    status_t status = kStatus_Success;
    uint8_t tmp_buff[6] = {0};
    uint8_t i = 0;

    if (!FXOS_ReadReg(fxos_handle, OUT_X_MSB_REG, tmp_buff, 6) == kStatus_Success)  //读取加计数据
    {
        status = kStatus_Fail;
    }

    for (i = 0; i < 6; i++)
    {
        ((int8_t *)sensorData)[i] = tmp_buff[i];                                     //将数据存入缓冲区
    }

    if (!FXOS_ReadReg(fxos_handle, M_OUT_X_MSB_REG, tmp_buff, 6) == kStatus_Success) //读取地磁计数据
    {
        status = kStatus_Fail;
    }

    for (i = 0; i < 6; i++)
    {
        ((int8_t *)sensorData)[i + 6] = tmp_buff[i];                                  //将数据存入缓冲区
    }

    return status;
}

status_t FXOS2100_ReadSensorData(fxos_handle_t *fxos_handle, fxos2100_data_t *sensorData)  //读FX2100原始数据
{
    g_fxosHandle.xfer.slaveAddress = 0x20;   //2100地址
    status_t status = kStatus_Success;
    uint8_t tmp_buff[6] = {0};
    uint8_t i = 0;

    if (!FXOS_ReadReg(fxos_handle, OUT_X_MSB_REG, tmp_buff, 6) == kStatus_Success)      //读取角速度计数据
    {
        status = kStatus_Fail;
    }

    for (i = 0; i < 6; i++)
    {
        ((int8_t *)sensorData)[i] = tmp_buff[i];                                        //将数据存入缓冲区
    } 
    return status;
}
status_t Init2100(fxos_handle_t *fxos_handle)                                           //角速度计初始化
{
    g_fxosHandle.xfer.slaveAddress = 0x20;   //2100地址
    if(FXOS_WriteReg(fxos_handle, 0x0d, 0x00) != kStatus_Success)  //写CTRL_REG1寄存器 配置高低通滤波 和 量程 ±2000dps
    {
        return kStatus_Fail;
    }
    if(FXOS_WriteReg(fxos_handle, 0x13, 0x02) != kStatus_Success)  //写CTRL_REG1寄存器  启动 和 配置输出速率 800Hz
    {
        return kStatus_Fail;
    }
    return kStatus_Success;
}


status_t FXOS_ReadReg(fxos_handle_t *handle, uint8_t reg, uint8_t *val, uint8_t bytesNumber)  //读寄存器函数
{
    return IIC_ReadReg(handle, reg, val, bytesNumber);  //读寄存器函数
}

status_t FXOS_WriteReg(fxos_handle_t *handle, uint8_t reg, uint8_t val)    //写寄存器函数
{
    return IIC_WriteReg(handle, reg, val);    //写寄存器函数
}
