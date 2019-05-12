#include "include.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
//IIC1   SCLk    J11
//IIC1   SDA     K11

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Read all data from sensor function
 *
 * @param Ax The pointer store x axis acceleration value
 * @param Ay The pointer store y axis acceleration value
 * @param Az The pointer store z axis acceleration value
 * @param Mx The pointer store x axis magnetic value
 * @param My The pointer store y axis magnetic value
 * @param Mz The pointer store z axis magnetic value
 * @note Must calculate g_dataScale before use this function.
 */
static void Sensor_ReadData(int16_t DAT[6]);

/*******************************************************************************
 * Variables
 ******************************************************************************/


void Sensor_ReadData(int16_t DAT[9])
{
    fxos_data_t fxos_data;
    if (FXOS_ReadSensorData(&g_fxosHandle, &fxos_data) != kStatus_Success)   //读取原始数据
    {
        printf("Failed to read acceleration data!\r\n");
    }
    /* Get the accel data from the sensor data structure in 14 bit left format data*/
    DAT[0] = (int16_t)(((uint16_t)fxos_data.accelXMSB << 8) | fxos_data.accelXLSB)/4U;   //加速度计14位的，除以4
    DAT[1] = (int16_t)(((uint16_t)fxos_data.accelYMSB << 8) | fxos_data.accelYLSB)/4U;
    DAT[2] = (int16_t)(((uint16_t)fxos_data.accelZMSB << 8) | fxos_data.accelZLSB)/4U;
    DAT[3] = (int16_t)(((uint16_t)fxos_data.magXMSB << 8) | fxos_data.magXLSB);          //地磁计16位的
    DAT[4] = (int16_t)(((uint16_t)fxos_data.magYMSB << 8) | fxos_data.magYLSB);
    DAT[5] = (int16_t)(((uint16_t)fxos_data.magZMSB << 8) | fxos_data.magZLSB);

    
    fxos2100_data_t  fxos2100_data;
    if (FXOS2100_ReadSensorData(&g_fxosHandle, &fxos2100_data) != kStatus_Success)//读取原始数据
    {
        printf("Failed to read acceleration data!\r\n");
    }
    DAT[6] = (int16_t)(((uint16_t)fxos2100_data.gyroXMSB << 8) | fxos2100_data.gyroXLSB);//角速度计 16位的
    DAT[7] = (int16_t)(((uint16_t)fxos2100_data.gyroYMSB << 8) | fxos2100_data.gyroYLSB);
    DAT[8] = (int16_t)(((uint16_t)fxos2100_data.gyroZMSB << 8) | fxos2100_data.gyroZLSB);
}
void LQ_init9AX(void)
{
    uint8_t regResult = 0;
    LPI2C_Init(LPI2C1, 400000);    ////支持400K I2C

    g_fxosHandle.xfer.slaveAddress = 0x1e;   //8700地址
    if (FXOS_ReadReg(&g_fxosHandle, WHO_AM_I_REG, &regResult, 1) == kStatus_Success) 
    {
        if(regResult == kFXOS_WHO_AM_I_Device_ID)   //读取WHO_AM_I 寄存器，如果结果正确，则证明I2C地址正确
        {
             printf("\r\n FX8700 is OK!");
        }
    }
    else /* Not found any sensor on board */       //没有找到FX8700
    {
            printf("\r\n FX8700 is Fail!");

    }

    /* Init sensor */
    if (FXOS_Init(&g_fxosHandle) != kStatus_Success)   //FX8700 初始化
    {
        return;
    } 
        /* Init sensor */
    if (Init2100(&g_fxosHandle) != kStatus_Success)   //FX2100 初始化
    {
        return;
    }
}

void Test_9AX(void)
{
    int16_t u16data[9];  //存放读取传感器 数据
       
    TFTSPI_Init();               //TFT1.8初始化  
    TFTSPI_CLS(u16BLUE);           //清屏
    LQ_init9AX();                //九轴初始化
    
    char txt[16];
    for (;;)
    {       
      printf("\r\n\r\n ");

      Sensor_ReadData(u16data); // Read sensor data  

      sprintf(txt,"AX:  %5d ",(int16_t)u16data[0]); 
      TFTSPI_P8X8Str(0,0,(uint8_t*)txt,u16RED,u16BLUE);
      sprintf(txt,"AY:  %5d ",(int16_t)u16data[1]); 
      TFTSPI_P8X8Str(0,1,(uint8_t*)txt,u16RED,u16BLUE); 
      sprintf(txt,"AZ:  %5d ",(int16_t)u16data[2]); 
      TFTSPI_P8X8Str(0,2,(uint8_t*)txt,u16RED,u16BLUE);
      sprintf(txt,"MX:  %5d ",(int16_t)u16data[3]);  
      TFTSPI_P8X8Str(0,3,(uint8_t*)txt,u16RED,u16BLUE);
      sprintf(txt,"MY:  %5d ",(int16_t)u16data[4]); 
      TFTSPI_P8X8Str(0,4,(uint8_t*)txt,u16RED,u16BLUE);
      sprintf(txt,"MZ:  %5d ",(int16_t)u16data[5]);
      TFTSPI_P8X8Str(0,5,(uint8_t*)txt,u16RED,u16BLUE);
      sprintf(txt,"GX:  %5d ",(int16_t)u16data[6]);  
      TFTSPI_P8X8Str(0,6,(uint8_t*)txt,u16RED,u16BLUE);
      sprintf(txt,"GY:  %5d ",(int16_t)u16data[7]);
      TFTSPI_P8X8Str(0,7,(uint8_t*)txt,u16RED,u16BLUE); 
      sprintf(txt,"GZ:  %5d ",(int16_t)u16data[8]); 
      TFTSPI_P8X8Str(0,8,(uint8_t*)txt,u16RED,u16BLUE);
      printf("\r\nAX: %d  ",(int16_t)u16data[0]); 
      printf("\r\nAY: %d  ",(int16_t)u16data[1]);
      printf("\r\nAZ: %d  ",(int16_t)u16data[2]); 
      printf("\r\nMX: %d  ",(int16_t)u16data[3]);
      printf("\r\nMY: %d  ",(int16_t)u16data[4]); 
      printf("\r\nMZ: %d  ",(int16_t)u16data[5]);
      printf("\r\nGX: %d  ",(int16_t)u16data[6]);
      printf("\r\nGY: %d  ",(int16_t)u16data[7]); 
      printf("\r\nGZ: %d  ",(int16_t)u16data[8]);
      delayms(100);//延时100毫秒   
    } /* End infinite loops */
}
