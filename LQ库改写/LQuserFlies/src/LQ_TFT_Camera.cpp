/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2018年6月22日
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
#include "include.h"

volatile uint8_t LQ_LCD = 0;       //直接显示在LCD上或者二值化后显示在LCD上  480*272的LCD
int OFFSET0=0;      //最远处，赛道中心值综合偏移量
int OFFSET1=0;      //第二格
int OFFSET2=0;      //最近，第三格


#define MAX_ROW   60       
#define MAX_COL   480 

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】触摸屏处理函数
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_Touch_Handle(void)
{
    /*        画按键         */
    TFT_P8X16Str(315,210,"Start",BLUE,RED,0); //字符串显示
    TFT_P8X16Str(395,210,"Stop",BLUE,RED,0); //字符串显示
    TFT_P8X16Str(395,170,"ON",BLUE,RED,0); //字符串显示
    
    TFT_P8X16Str(315,130,"LED1",BLUE,RED,0); //字符串显示
    TFT_P8X16Str(395,130,"LED2",BLUE,RED,0); //字符串显示
    TFT_Draw_Rectangle(380,200,460,240,GREEN,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    TFT_Draw_Rectangle(300,200,379,240,RED,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    TFT_Draw_Rectangle(380,160,460,199,RED,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    
    TFT_Draw_Rectangle(380,120,460,160,GREEN,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    TFT_Draw_Rectangle(300,120,379,160,RED,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    
    if(GT9147_Scan(0))   //触摸屏按下
    {
        uint8_t i; 
        for(i=0;i<5;i++)
        {
            if(tp_dev.x[i] < 460 && tp_dev.x[i] > 380 && tp_dev.y[i] < 240 && tp_dev.y[i] > 200) //stop按下
            { 
                LED_Ctrl(LED_R, RVS);
                LQ_PWMA_B_SetDuty(PWM1, kPWM_Module_3, 0, 0);  //电机pwm
            }
            if(tp_dev.x[i] < 379 && tp_dev.x[i] > 300 && tp_dev.y[i] < 240 && tp_dev.y[i] > 200) //start按下
            {
                LED_Ctrl(LED_R, RVS);
                LQ_PWMA_B_SetDuty(PWM1, kPWM_Module_3, 1000, 1000);       //电机pwm
            }
            
            if(tp_dev.x[i] < 460 && tp_dev.x[i] > 380 && tp_dev.y[i] < 199 && tp_dev.y[i] > 160) //ON按下
            {
                LED_Ctrl(LED_R, RVS);
                
                if(LQ_LCD)LQ_LCD = 0;                             
                else LQ_LCD = 1;
            }
            if(tp_dev.x[i] < 379 && tp_dev.x[i] > 300 && tp_dev.y[i] < 160 && tp_dev.y[i] > 120) //LED1按下
            {
                LED_Ctrl(LED_R, RVS);
                
                GPIO_PinReverse (GPIO2, 22);
                
            }
            if(tp_dev.x[i] < 460 && tp_dev.x[i] > 380 && tp_dev.y[i] < 160 && tp_dev.y[i] > 120) //LED2按下
            {
                LED_Ctrl(LED_R, RVS);
                GPIO_PinReverse (GPIO2, 25);
                
            }
            TFT_Draw_CircleCake(tp_dev.x[i],tp_dev.y[i],40,RED,1);   //触摸的地方画圆
            TFT_Draw_CircleCake(tp_dev.x[i],tp_dev.y[i],20,BLUE,1);
            TFT_Draw_CircleCake(tp_dev.x[i],tp_dev.y[i], 8,BLACK,1);
            tp_dev.x[i] = 0;
            tp_dev.y[i] = 0;
        }
    }      
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】摄像头+TFT480*752测试例程
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

void Test_TFT_Camera(void)
{      

    uint32_t fullCameraBufferAddr;     
    LQ_Camera_Init();
    LQ_PXP_Init();
    LQ_TFT480_272_Init();
 
    GT9147_Init();//初始化触摸板
    delayms(200);        //延时200毫秒 
//    PRINTF("Camera demo starting...\r\n");    
    while (1)
    {     
        // Wait to get the full frame buffer to show. 
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //摄像头CSI缓存区已满
        {
        }
        // Convert the camera input picture to RGB format. 
        psBufferConfig.bufferAddr = fullCameraBufferAddr;    //设置PXP转换源地址
        PXP_SetProcessSurfaceBufferConfig(PXP, &psBufferConfig); // 对PXP输入进行配置
        
        if(counter % 2)
        {
            outputBufferConfig.buffer0Addr = (uint32_t)lcdFrameBuf[1]; //设置PXP转换的目的地址
        }
        else
        {
            outputBufferConfig.buffer0Addr = (uint32_t)lcdFrameBuf[0]; //设置PXP转换的目的地址
        }
        PXP_SetOutputBufferConfig(PXP, &outputBufferConfig);   // 对PXP输出进行配置
        
        PXP_Start(PXP);    //开启PXP转换
         
        // Wait for PXP process complete.
        while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(PXP)))  //等待pXP转换完成
        {
        }
        PXP_ClearStatusFlags(PXP, kPXP_CompleteFlag);   //清除标志位
        
        ELCDIF_ClearInterruptStatus(LCDIF, kELCDIF_CurFrameDone);  //清楚LCD中断标志位
        
         // Return the camera buffer to camera queue. 
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列

        
        // Wait for the new set LCD frame buffer active.
        while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(LCDIF))) 
        {
        }    
       
        TFT_P16x16Str(184,5,"北京龙邱智能科技",BLUE,RED,0); //字符串显示

        LQ_Touch_Handle();                                  //触摸屏处理函数
        
        if(counter % 2)
        {
            ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)lcdFrameBuf[1]);  //将LCD缓存区lcdFrameBuf 显示在TFT屏上
        }
        else
        {
            ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)lcdFrameBuf[0]);  //将LCD缓存区lcdFrameBuf 显示在TFT屏上
        }
        delayms(15);        //延时15毫秒，屏消抖
       
        counter ++;
        
        LED_Ctrl(LED_R, RVS); //EVK LED闪烁  
    }
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】得到每行阈值
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
//static uint8_t counter = 0;
uint32_t LQ_Get_Threshold(void)
{
    
    uint64_t value = 0;
    int nr; //行
    int nc; //列
    if(counter % 2)
    {
        for(nr=0;nr<272;nr++)
        {  
            for(nc=0;nc<480;nc++)
            {
                value += (lcdFrameBuf[1][nr][nc] & 0x00ffffff);
                
            }
        }
    }
    else
    {
        for(nr=0;nr<272;nr++)
        {  
            for(nc=0;nc<480;nc++)
            {
                value += (lcdFrameBuf[0][nr][nc] & 0x00ffffff);
                
            } 
        }   
    }
  return  value / 480 / 272;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】二值化处理
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

//uint32_t threshold = 0x606060;   //阈值
void LQ_Camera_0_1_Handle(void)
{
    int i = 0;
    int j = 0;
    uint32_t threshold = 0x606060;
//    threshold = LQ_Get_Threshold();   //使用平均值
    for(i = 0; i < 272; i++)
    {
        if(counter % 2)
        {
            
            for(j = 0; j < 480; j++)
            {
                
                if((lcdFrameBuf[1][i][j] & 0x00ffffff) > threshold)   //此处阈值设置，根据环境的光线来设定    
                    lcdFrameBuf[2][i][j] = 0xffffff;
                else                                        
                    lcdFrameBuf[2][i][j] = 0x00;
            } 
        }
        else
        {
            
            for(j = 0; j < 480; j++)
            {
                if((lcdFrameBuf[0][i][j] & 0x00ffffff) > threshold)   //此处阈值设置，根据环境的光线来设定    
                    lcdFrameBuf[3][i][j] = 0xffffff;
                else                                        
                    lcdFrameBuf[3][i][j] = 0x00;    
            }          
        }    
    }
    
    
}

/***************************************************************************
*                                                                          *
*  函数名称：int Seek_Road(void)                                           *
*  功能说明：寻找白色区域偏差值                                            *
*  参数说明：无                                                            *
*  函数返回：值的大小                                                      *
*  修改时间：2017-07-16                                                    *
*  备    注：以中间为0，左侧减一，右侧加一，数值代表1的面积                *
*            计算区域从第一行开始到倒数第二行结束。                        *
*            如果面积为负数，数值越大说明越偏左边；                        *
*            如果面积为正数，数值越大说明越偏右边。                        *
***************************************************************************/ 
void Seek_Road(void)
{  
  int nr; //行
  int nc; //列
  int temp=0;//临时数值
  //for(nr=1; nr<MAX_ROW-1; nr++)
  temp=0;
  if(counter % 2)
  {
  for(nr=20; nr<100; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)//240to480
    {
      if(lcdFrameBuf[2][nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(lcdFrameBuf[2][nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET0=temp;
  temp=0;
  for(nr=100; nr<180; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)
    {
      if(lcdFrameBuf[2][nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(lcdFrameBuf[2][nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET1=temp;    	
  temp=0;
  for(nr=180; nr<260; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)
    {
      if(lcdFrameBuf[2][nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(lcdFrameBuf[2][nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET2=temp;   	
  
  }
  else
  {
  for(nr=20; nr<100; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)//240to480
    {
      if(lcdFrameBuf[3][nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(lcdFrameBuf[3][nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET0=temp;
  temp=0;
  for(nr=100; nr<180; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)
    {
      if(lcdFrameBuf[3][nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(lcdFrameBuf[3][nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET1=temp;    	
  temp=0;
  for(nr=180; nr<260; nr++)
  {  	    
    for(nc=MAX_COL/2;nc<MAX_COL;nc=nc+1)
    {
      if(lcdFrameBuf[3][nr][nc])
      {
        ++temp;
      }			   
    }
    for(nc=0; nc<MAX_COL/2; nc=nc+1)
    {
      if(lcdFrameBuf[3][nr][nc])
      {
        --temp;
      }			   
    }		  
  }
  OFFSET2=temp; 
  
  }
    
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】摄像头+TFT480*752测试例程
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void  LQ_Get_CameraBuff(void)
{      
    
    uint32_t fullCameraBufferAddr;  
    
    // Wait to get the full frame buffer to show. 
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //摄像头CSI缓存区已满
    {
    }
    
    
    
    // Convert the camera input picture to RGB format. 
    psBufferConfig.bufferAddr = fullCameraBufferAddr;    //设置PXP转换源地址
    PXP_SetProcessSurfaceBufferConfig(PXP, &psBufferConfig); // 对PXP输入进行配置
    if(counter % 2)
    {
        outputBufferConfig.buffer0Addr = (uint32_t)lcdFrameBuf[1]; //设置PXP转换的目的地址
    }
    else
    {
        outputBufferConfig.buffer0Addr = (uint32_t)lcdFrameBuf[0]; //设置PXP转换的目的地址
    }
    
    PXP_SetOutputBufferConfig(PXP, &outputBufferConfig);   // 对PXP输出进行配置
    
    PXP_Start(PXP);    //开启PXP转换
    
    
    /*cpu可以处理别的事情*/
    // Wait for PXP process complete.
    while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(PXP)))  //等待pXP转换完成
    {
    }
    PXP_ClearStatusFlags(PXP, kPXP_CompleteFlag);   //清除标志位
    
    ELCDIF_ClearInterruptStatus(LCDIF, kELCDIF_CurFrameDone);  //清楚LCD中断标志位
    // Set the new frame to LCD. 
    
    
    // Return the camera buffer to camera queue. 
    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列
    
    // Wait for the new set LCD frame buffer active.
    while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(LCDIF)))  //等待新设置的LCD帧缓冲区激活    
    {
    }    
    
    TFT_P16x16Str(184,5,"北京龙邱智能科技",BLUE,RED,0); //字符串显示
    TFT_P8X16Str(315,210,"Start",BLUE,RED,0); //字符串显示
    TFT_P8X16Str(395,210,"Stop",BLUE,RED,0); //字符串显示
    TFT_P8X16Str(395,170,"ON",BLUE,RED,0); //字符串显示
    
    TFT_P8X16Str(315,130,"LED1",BLUE,RED,0); //字符串显示
    TFT_P8X16Str(395,130,"LED2",BLUE,RED,0); //字符串显示
    TFT_Draw_Rectangle(380,200,460,240,GREEN,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    TFT_Draw_Rectangle(300,200,379,240,RED,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    TFT_Draw_Rectangle(380,160,460,199,RED,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    
    TFT_Draw_Rectangle(380,120,460,160,GREEN,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    TFT_Draw_Rectangle(300,120,379,160,RED,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    
    if (LQ_LCD)
    {
//        LQ_Touch_Handle();                                  //触摸屏处理函数 
        if(counter % 2)
        {
            ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)lcdFrameBuf[3]);  //将LCD缓存区lcdFrameBuf 显示在TFT屏上
        }
        else
        {
            ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)lcdFrameBuf[2]);  //将LCD缓存区lcdFrameBuf 显示在TFT屏上
        }
        
    }
    if (LQ_LCD == 0)
    {
        
        LQ_Touch_Handle();                                  //触摸屏处理函数 
        if(counter % 2)
        {
            ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)lcdFrameBuf[1]);  //将LCD缓存区lcdFrameBuf 显示在TFT屏上
        }
        else
        {
            ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)lcdFrameBuf[0]);  //将LCD缓存区lcdFrameBuf 显示在TFT屏上
        }
        
    }  
    counter ++;   
}


