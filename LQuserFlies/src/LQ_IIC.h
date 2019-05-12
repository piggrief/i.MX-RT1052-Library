#ifndef __LQ_IIC_H
#define __LQ_IIC_H

#include "fsl_common.h"
#include "fsl_lpi2c.h"

//#define IICBlocking   //IIC¶ÂÈûÄ£Ê½

typedef struct _fxos_handle
{
    LPI2C_Type *base;
    lpi2c_master_transfer_t xfer;
    lpi2c_master_handle_t *i2cHandle;
} fxos_handle_t;

#ifndef IICBlocking  //·Ç¶ÂÈûÄ£Ê½
extern lpi2c_master_handle_t g_MasterHandle;  
extern fxos_handle_t g_fxosHandle;    //¶ÁÈ¡µØÖ·ÅäÖÃ
extern void LPI2C_Init(LPI2C_Type *base, uint32_t baudrate);
status_t IIC_WriteReg(fxos_handle_t *handle, uint8_t reg, uint8_t val);    //Ğ´¼Ä´æÆ÷º¯Êı
status_t IIC_ReadReg(fxos_handle_t *handle, uint8_t reg, uint8_t *val, uint8_t bytesNumber);  //¶Á¼Ä´æÆ÷º¯Êı
#else
extern void LPI2C_Init(LPI2C_Type *base, uint32_t baudrate);
status_t IIC_WriteReg(uint16_t addr, uint8_t reg, uint8_t val);    //Ğ´¼Ä´æÆ÷º¯Êı
status_t IIC_ReadReg(uint16_t addr, uint8_t reg, uint8_t *val, uint8_t bytesNumber);  //¶Á¼Ä´æÆ÷º¯Êı
#endif
#endif