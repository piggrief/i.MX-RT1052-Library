#ifndef __LQ_SGP18TEST_H
#define __LQ_SGP18TEST_H

#include "include.h"

#define LCD_WIDTH   128
#define LCD_HEIGHT  160

extern uint8_t counter;
extern int OFFSET2;
extern uint16_t lcdFrameBuf[2][LCD_HEIGHT][LCD_WIDTH];
extern void Test_SGP18_OV7725(void);
extern void Test_SGP18_Camera(void);
#endif