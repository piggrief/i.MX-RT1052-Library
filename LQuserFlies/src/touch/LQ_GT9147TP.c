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
Board settings
================================================

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

_LQ_TP_DEV tp_dev=
{
  TP_Init,
  GT9147_Scan,	
  0,
  0, 
  0,
  0,
  0,
  0,	  	 		
  0,
  0,	  	 		
};

const uint8_t GT9147_CFG_TBL[]=
{ 
  0x41,0xE0,0x01,0x10,0x01,0x05,0x0F,0x00,0x01,0x08,
  0x28,0x05,0x50,0x32,0x03,0x05,0x00,0x00,0xFF,0xFF,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x89,0x28,0x0A,
  0x17,0x15,0x31,0x0D,0x00,0x00,0x02,0x9B,0x03,0x25,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x32,0x00,0x00,
  0x00,0x0F,0x94,0x94,0xC5,0x02,0x07,0x00,0x00,0x04,
  0x8D,0x13,0x00,0x5C,0x1E,0x00,0x3C,0x30,0x00,0x29,
  0x4C,0x00,0x1E,0x78,0x00,0x1E,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,
  0x18,0x1A,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0x00,0x02,0x04,0x05,0x06,0x08,0x0A,0x0C,
  0x0E,0x1D,0x1E,0x1F,0x20,0x22,0x24,0x28,0x29,0xFF,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0x30,0x01,
};  
/*
0x62,0xe0,0x01,0x20,0x03,0x05,0x34,0xc0,0x01,0x08, 
0x28,0x0f,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00, 
0x00,0x00,0x06,0x16,0x16,0x1f,0x14,0x89,0x28,0x0a, 
0x17,0x15,0x31,0x0d,0x00,0x00,0x08,0x22,0x04,0x11, 
0x00,0x00,0x00,0x00,0x00,0x03,0x82,0x08,0x08,0x00, 
0x00,0x0f,0x2c,0x94,0xc5,0x02,0x07,0x00,0x00,0x04, 
0x9d,0x10,0x00,0x84,0x14,0x00,0x70,0x19,0x00,0x5f, 
0x20,0x00,0x55,0x27,0x00,0x54,0x00,0x00,0x00,0x00, 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01, 
0x00,0x00,0x1a,0x18,0x16,0x14,0x12,0x10,0x0e,0x0c, 
0x0a,0x08,0x00,0x00,0x00,0x00,0x1f,0xff,0xff,0xff, 
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, 
0xff,0x00,0x00,0x02,0x04,0x05,0x06,0x08,0x0a,0x0c, 
0x0e,0x1d,0x1e,0x1f,0x20,0x22,0x24,0x28,0x29,0xff, 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff, 
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf6, 
0xff,0xff,0xff,0xff,
*/


//发送GT9147配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
uint8_t GT9147_Send_Cfg(uint8_t mode)
{
  uint8_t buf[2];
  uint8_t i=0;
  buf[0]=0;
  //buf[1]=mode;	//是否写入到GT9147 FLASH?  即是否掉电保存
  for(i=0;i<(sizeof(GT9147_CFG_TBL)-2);i++)  buf[0]+=GT9147_CFG_TBL[i];//计算校验和
  buf[0]=(~buf[0])+1;
  printf("checksum:0x%x\r\n",buf[0]);
  GT9147_WR_Reg(GT_CFGS_REG,(uint8_t*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//发送寄存器配置
  //GT9147_WR_Reg(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
  return 0;
} 
#define GT_WriteReg(i2c, reg, val) SCCB_WriteReg(i2c, GT_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (val))

#define GT_ReadReg(i2c, reg, val) SCCB_ReadReg(i2c, GT_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (val))

//向GT9147写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{    
  return  SCCB_WriteMultiRegs(LPI2C1,GT_SCCB_ADDR,kSCCB_RegAddr16Bit,reg,buf,len);
}

//从GT9147读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{  
  SCCB_ReadMultiRegs(LPI2C1,GT_SCCB_ADDR,kSCCB_RegAddr16Bit,reg,buf,len);  
} 

//初始化GT9147触摸屏
//返回值:0,初始化成功;1,初始化失败 
uint8_t GT9147_Init(void)
{
    
    LPI2C1_Init(400000);
  uint8_t temp[5];  
  /*
  PUS 数据位15-14 
  PUS(Pull Up / Down Config. Field)：上拉/下拉配置？？？主要作用是提高输出信号的驱动能力、确定输入信号的电平（防止干扰） 
  可选的值： 
  00 PUS_0_100K_Ohm_Pull_Down — 100K Ohm Pull Down ：100K欧姆的拉低 
  01 PUS_1_47K_Ohm_Pull_Up — 47K Ohm Pull Up ：47K欧姆的拉高 
  10 PUS_2_100K_Ohm_Pull_Up — 100K Ohm Pull Up ：100K欧姆的拉高 
  11 PUS_3_22K_Ohm_Pull_Up — 22K Ohm Pull Up ：22K欧姆的拉高
  */
  //复位管脚初始化
  CLOCK_EnableClock(kCLOCK_Iomuxc);                 // IO口时钟使能
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_02_GPIO1_IO02, 0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_02_GPIO1_IO02, 0x10B0u|IOMUXC_SW_PAD_CTL_PAD_PUS_MASK);  
  GPIO_PinInit(GPIO1, 2, &GPIO_Output_Config);     //M11 RST 
  
  //中断管脚初始化
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_11_GPIO1_IO11,0U);//管脚M4  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_11_GPIO1_IO11, IOMUXC_SW_PAD_CTL_PAD_PUS_MASK);  //开启上拉
  GPIO_PinInit(GPIO1, 11, &GPIO_Input_Config);         //G10 INT    GPIO输入口，非中断
  
  //LQ_IIC_GPIO_Init(); 
  
  GPIO_PinWrite(GPIO1,2, 0U);//复位
  delayms(50);  
  GPIO_PinWrite(GPIO1,2, 1U);//正常
  delayms(10); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_11_GPIO1_IO11,0U);//管脚M4  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_11_GPIO1_IO11,0);  //取消上拉
  GPIO_PinInit(GPIO1, 11, &GPIO_Input_Config);         //G10 INT    GPIO输入口，非中断    
  
  delayms(50);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_11_GPIO1_IO11, 0);
  GT9147_RD_Reg(GT_PID_REG,temp,4);//读取产品ID
  temp[4]=0;
  PRINTF("\r\nCTP ID:0x%02X%02X%02X%02X\r\n",temp[0],temp[1],temp[2],temp[3]);	//打印ID
  //printf("Default Ver:0x%x\r\n",temp[0]);
  //GT9147_Send_Cfg(1);
  
  if(strcmp((char*)temp,"9147")==0)//ID==9147
  {
    temp[0]=0X02;			
    GT9147_WR_Reg(GT_CTRL_REG,temp,1);//软复位GT9147
    GT9147_RD_Reg(GT_CFGS_REG,temp,1);//读取GT_CFGS_REG寄存器
    //if(temp[0]<0X60)//默认版本比较低,需要更新flash配置
    //{
    PRINTF("Default Ver:0x%x\r\n",temp[0]);
    //if(lcddev.id==0X5510)GT9147_Send_Cfg(1);//更新并保存配置
    //GT9147_Send_Cfg(1);
    //}
    delayms(10);
    temp[0]=0X00;	 
    GT9147_WR_Reg(GT_CTRL_REG,temp,1);//结束复位   
    return 0;
  } 
  return 1;
}

const uint16_t GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
uint8_t GT9147_Scan(uint8_t mode)
{
  uint8_t buf[4];
  uint8_t i=0;
  uint8_t res=0;
  uint8_t temp;
  uint8_t tempsta;
  static uint8_t t=0;//控制查询间隔,从而降低CPU占用率   
  t++;
  if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
  {
    GT9147_RD_Reg(GT_GSTID_REG,&mode,1);	//读取触摸点的状态  
    if(mode&0X80&&((mode&0XF)<6))
    {
      temp=0;
      GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//清标志 		
    }		
    if((mode&0XF)&&((mode&0XF)<6))
    {
      res=1;
      temp=0XFF<<(mode&0XF);		//将点的个数转换为1的位数,匹配tp_dev.sta定义 
      tempsta=tp_dev.sta;		//保存当前的tp_dev.sta值
      tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
      tp_dev.x[4]=tp_dev.x[0];	//保存触点0的数据
      tp_dev.y[4]=tp_dev.y[0];
      for(i=0;i<5;i++)
      {
        if(tp_dev.sta&(1<<i))	//触摸有效?
        {
          GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值
          if(tp_dev.touchtype&0X01)//横屏
          {
            tp_dev.x[i]=(((uint16_t)buf[1]<<8)+buf[0]);
            tp_dev.y[i]=(((uint16_t)buf[3]<<8)+buf[2]);
            
          }
          else
          {
            //tp_dev.y[i]=((uint16_t)buf[1]<<8)+buf[0];
            //tp_dev.x[i]=272-(((uint16_t)buf[3]<<8)+buf[2]);
            tp_dev.x[i]=(((uint16_t)buf[1]<<8)+buf[0]);
            tp_dev.y[i]=(((uint16_t)buf[3]<<8)+buf[2]);
          }

//          PRINTF("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);          
        }			
      }    
//      delayms(50);
      if(tp_dev.x[0]>APP_IMG_WIDTH||tp_dev.y[0]>APP_IMG_HEIGHT)//非法数据(坐标超出了)
      { 
        if((mode&0XF)>1)		//有其他点有数据,则复第二个触点的数据到第一个触点.
        {
          tp_dev.x[0]=tp_dev.x[1];
          tp_dev.y[0]=tp_dev.y[1];
          t=0;				//触发一次,则会最少连续监测10次,从而提高命中率
        }else					//非法数据,则忽略此次数据(还原原来的)  
        {
          tp_dev.x[0]=tp_dev.x[4];
          tp_dev.y[0]=tp_dev.y[4];
          mode=0X80;		
          tp_dev.sta=tempsta;	//恢复tp_dev.sta
        }
      }else t=0;			//触发一次,则会最少连续监测10次,从而提高命中率
    }
  }
  if((mode&0X8F)==0X80)//无触摸点按下
  { 
    if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
    {
      tp_dev.sta&=~(1<<7);	//标记按键松开
    }else						//之前就没有被按下
    { 
      tp_dev.x[0]=0xffff;
      tp_dev.y[0]=0xffff;
      tp_dev.sta&=0XE0;	//清除点有效标记	
    }	 
  } 	
  if(t>240)t=10;//重新从10开始计数
  return res;
}

void TP_Init(void)
{	  
  //扫描函数指向触摸屏扫描
  GT9147_Init();	
  tp_dev.scan=GT9147_Scan;  
}




