/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
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
================================================

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_GT9147TP_H_
#define _LQ_GT9147TP_H_

#include "include.h"  


#define TP_PRES_DOWN    0x80            //触屏被按下	  
#define TP_CATH_PRES    0x40            //有按键按下了 
#define CT_MAX_TOUCH    5               //电容屏支持的点数,固定为5点
//I2C读写命令	
#define GT_SCCB_ADDR	0X14            //0X14或者0X5D，
#define GT_CMD_WR	0X28     	//0X28    0XBA写命令 
#define GT_CMD_RD	0X29		//0X29    0XBB读命令

//GT9147 部分寄存器定义 
#define GT_CTRL_REG 	0X8040   	//GT9147控制寄存器
#define GT_CFGS_REG 	0X8047   	//GT9147配置起始地址寄存器
#define GT_CHECK_REG 	0X80FF   	//GT9147校验和寄存器
#define GT_PID_REG	0X8140   	//GT9147产品ID寄存器

#define GT_GSTID_REG 	0X814E   	//GT9147当前检测到的触摸情况
#define GT_TP1_REG	0X8150  	//第一个触摸点数据地址
#define GT_TP2_REG	0X8158		//第二个触摸点数据地址
#define GT_TP3_REG	0X8160		//第三个触摸点数据地址
#define GT_TP4_REG	0X8168		//第四个触摸点数据地址
#define GT_TP5_REG	0X8170		//第五个触摸点数据地址  

//触摸屏控制器
typedef struct
{  
  void   (*init)(void);		//初始化触摸屏控制器
  uint8_t (*scan)(uint8_t);		//扫描触摸屏.0,屏幕扫描;1,物理坐标;
  uint16_t x[CT_MAX_TOUCH]; 		//当前坐标
  uint16_t y[CT_MAX_TOUCH];		//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用  
  uint8_t  sta;				//笔的状态 
  //b7:按下1/松开0; 
  //b6:0,没有按键按下;1,有按键按下. 
  //b5:保留
  //b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
  /////////////////////触摸屏校准参数(电容屏不需要校准)//////////////////////								
  float xfac;					
  float yfac;
  short xoff;
  short yoff;	   
  //新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
  //b0:0,竖屏(适合左右为X坐标,上下为Y坐标的TP)
  //   1,横屏(适合左右为Y坐标,上下为X坐标的TP) 
  //b1~6:保留.
  //b7:0,电阻屏
  //   1,电容屏 
  uint8_t touchtype;
}_LQ_TP_DEV;

extern _LQ_TP_DEV tp_dev;	 	//触屏控制器在touch.c里面定义

uint8_t GT9147_Send_Cfg(uint8_t mode);
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void   GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len); 
uint8_t GT9147_Init(void);
uint8_t GT9147_Scan(uint8_t mode); 
//电阻屏/电容屏 共用函数
uint8_t TP_Scan(uint8_t tp);  //扫描						
void   TP_Init(void);//初始化
#endif
