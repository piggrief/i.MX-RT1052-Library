#ifndef  __LQ_SPI_H
#define  __LQ_SPI_H

#include "fsl_lpspi.h"
#include "fsl_flexio_spi_edma.h"
#include "board.h"
#include "fsl_dmamux.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_iomuxc.h"
uint8_t LP_SPI_ReadWriteByte(LPSPI_Type * base, uint8_t txData);
void LP_SPI_Master_Init(LPSPI_Type * base, uint32_t baudrate);


#endif