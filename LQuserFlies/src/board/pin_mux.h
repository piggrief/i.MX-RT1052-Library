
#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_
/*******************************************************************************
 * API
 ******************************************************************************/
void BOARD_InitPins(void);
void BOARD_InitSemcPins(void);
//UART调试端口管脚复用
void BOARD_InitDEBUG_UARTPins(void);

//外部SDRAM管脚复用
void BOARD_InitSDRAMPins(void);

//摄像头CSI管脚复用
void BOARD_InitCSIPins(void);

//摄像头用I2C1管脚复用
void BOARD_InitLPI2C1Pins(void);

//彩色LCD管脚复用
void BOARD_InitLCDPins(void); 

//SD卡/TF卡管脚复用
void BOARD_InitSDPins(void);

#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
