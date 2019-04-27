/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】Z
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2018年11月7日
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


/*************************************************************** 
* 
* 函数名称：uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
* 功能说明：求阈值大小 
* 参数说明： 
* 函数返回：阈值大小 
* 修改时间：2018年3月27日 
* 备 注： 
参考：https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html     
Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
3) i表示分类的阈值，也即一个灰度级，从0开始迭代
4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例w0，并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背景像素) 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
6) i++；转到4)，直到i为256时结束迭代
7）将最大g相应的i值作为图像的全局阈值
缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
***************************************************************/ 
uint32_t GetOSTU(void) 
{ 
  int16_t i,j; 
  uint32_t Amount = 0; 
  uint32_t PixelBack = 0; 
  uint32_t PixelIntegralBack = 0; 
  uint32_t PixelIntegral = 0; 
  int32_t PixelIntegralFore = 0; 
  int32_t PixelFore = 0; 
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差; 
  int16_t MinValue, MaxValue; 
  uint32_t Threshold = 0;
  uint8_t HistoGram[256];              //  

  for (j = 0; j < 256; j++)  HistoGram[j] = 0; //初始化灰度直方图 
  
  if(counter % 2)
  {
      for (j = 0; j < Use_ROWS; j++) 
      { 
        for (i = 0; i < Use_Line; i++) 
        { 
          HistoGram[(uint8_t)lcdFrameBuf[1][j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        } 
      }
  }
  else
  {
      for (j = 0; j < Use_ROWS; j++) 
      { 
        for (i = 0; i < Use_Line; i++) 
        { 
          HistoGram[(uint8_t)lcdFrameBuf[0][j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        } 
      } 
  }

  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值
      
  if (MaxValue == MinValue)     return MaxValue;         // 图像中只有一个颜色    
  if (MinValue + 1 == MaxValue)  return MinValue;        // 图像中只有二个颜色
    
  for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  像素总数
    
  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//灰度值总数
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];    //前景像素点数
    PixelFore = Amount - PixelBack;         //背景像素点数
    OmegaBack = (double)PixelBack / Amount;//前景像素百分比
    OmegaFore = (double)PixelFore / Amount;//背景像素百分比
    PixelIntegralBack += HistoGram[j] * j;  //前景灰度值
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
    MicroBack = (double)PixelIntegralBack / PixelBack;   //前景灰度百分比
    MicroFore = (double)PixelIntegralFore / PixelFore;   //背景灰度百分比
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
    if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //返回最佳阈值;
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
uint8_t Pixle [Use_ROWS][Use_Line];
void Camera_0_1_Handle(void)
{
    int i = 0;
    int j = 0;
//    uint32_t threshold = 0x60;  //使用固定阈值
    uint32_t threshold = GetOSTU();//使用大津法
    
    char txt[8];
    sprintf(txt,"%03d",threshold);//前者为大津法求得的阈值，后者为平均值  
    LCD_P6x8Str(85,1,(uint8_t*)txt);
    
    if(counter % 2)
    {
        for(i = 0; i < 64; i++)
        {           
            for(j = 0; j < 128; j++)
            {
                
                if((lcdFrameBuf[1][i][j] & 0x000000ff) > threshold)   //此处阈值设置，根据环境的光线来设定    
                    Pixle[Use_ROWS - 1 - i][Use_Line - 1 - j] = 1;                       //图像反的，正过来
                else                                        
                    Pixle[Use_ROWS - 1 - i][Use_Line - 1 - j] = 0;
            }    
        }   
    }
    else
    {
        for(i = 0; i < 64; i++)
        {           
            for(j = 0; j < 128; j++)
            {
                
                if((lcdFrameBuf[0][i][j] & 0x000000ff) > threshold)   //此处阈值设置，根据环境的光线来设定    
                    Pixle[Use_ROWS - 1 - i][Use_Line - 1 - j] = 1;                       //图像反的，正过来
                else                                        
                    Pixle[Use_ROWS - 1 - i][Use_Line - 1 - j] = 0;
            }    
        }   
    }
    
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】oled上显示
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Draw_Road(void)
{ 	 
  uint8_t i = 0, j = 0,temp=0;
  for(i=0;i<56;i+=8)//6*8=48行 
  {
    LCD_Set_Pos(0,i/8+1);
    for(j=0;j<128;j++) 
    { 
      temp=0;
      if(Pixle[0+i][j]) temp|=1;
      if(Pixle[1+i][j]) temp|=2;
      if(Pixle[2+i][j]) temp|=4;
      if(Pixle[3+i][j]) temp|=8;
      if(Pixle[4+i][j]) temp|=0x10;
      if(Pixle[5+i][j]) temp|=0x20;
      if(Pixle[6+i][j]) temp|=0x40;
      if(Pixle[7+i][j]) temp|=0x80;
      LCD_WrDat(temp); 	  	  	  	  
    }
  }  
}

//三面以上反数围绕清除噪点
void Pixle_Filter(void)
{  
  int nr; //行
  int nc; //列
  
  for(nr=1; nr<60-1; nr++)
  {  	    
    for(nc=1; nc<100-1; nc=nc+1)
    {
      if((Pixle[nr][nc]==0)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]>2))         
      {
        Pixle[nr][nc]=1;
      }	
      else if((Pixle[nr][nc]==1)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]<2))         
      {
        Pixle[nr][nc]=0;
      }	
    }	  
  }  
}





/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】oled + camera test
【软件版本】V1.0
【最后更新】2018年11月7日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

void Test_OLED_Camera(void)
{
    LCD_Init();               //OLED初始化 
    
    LQ_Camera_Init();         //摄像头初始化
    uint32_t fullCameraBufferAddr; //存放CSI满缓冲区地址
    

       //Configure the PXP for color space conversion.
    PXP_Init(PXP);    //开时钟
    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);
    
    
#ifdef LQMT9V034 //LQMT9V034模块
    PXP_EnableCsc1(PXP, false);              //不使能  LQMT9V034模块为单色格式

    PXP_SetProcessSurfaceScaler(PXP,IMAGEW/4, IMAGEH, Use_Line, Use_ROWS);//缩放图像, 神眼摄像头752 * 480分辨率，数据输出格式为 八位单色彩，也就是一个字节就是一个像素点，但是单片机的csi接口配置为rgb888格式（就是摄像头本来一个字节表示一个像素点，变成了四个字节表示一个像素点），帧间隔
    PXP_SetProcessSurfacePosition(PXP, 0u, 0u, Use_Line - 1U, Use_ROWS - 1U);                //设置显示位置
#else  //LQOV7725模块
    PXP_SetCsc1Mode(PXP, kPXP_Csc1YCbCr2RGB);//设置 csc1  kPXP_Csc1YCbCr转换为rgb888
    PXP_EnableCsc1(PXP, false);               //使能csc1
    
    PXP_SetProcessSurfaceScaler(PXP,APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT, Use_Line, Use_ROWS);//缩放
    PXP_SetProcessSurfacePosition(PXP, 0u, 0u, Use_Line - 1U, Use_ROWS - 1U);                //设置显示位置
#endif
   
    PXP_SetProcessSurfaceBackGroundColor(PXP, 0u);  //设置背景颜色

    PXP_SetRotateConfig(PXP, kPXP_RotateOutputBuffer, kPXP_Rotate0, kPXP_FlipDisable);//图像旋转多少度，翻转与否
    
    
    /* Input config. */
#ifdef LQMT9V034 //LQMT9V034模块
        psBufferConfig.pixelFormat =  kPXP_PsPixelFormatRGB888;//kPXP_PsPixelFormatRGB565,//kPXP_PsPixelFormatUYVY1P422,
#else                  //LQOV7725模块
        psBufferConfig.pixelFormat =  kPXP_PsPixelFormatRGB565;//kPXP_PsPixelFormatRGB565,//kPXP_PsPixelFormatUYVY1P422,
#endif
        psBufferConfig.swapByte = false;                 //高8位在前还是低8位在前
        psBufferConfig.bufferAddrU = 0U;
        psBufferConfig.bufferAddrV = 0U;
        psBufferConfig.pitchBytes = APP_CAMERA_WIDTH * APP_BPP;    //一行多少个字节 
        
    /* Output config. */  
        outputBufferConfig.pixelFormat = kPXP_OutputPixelFormatARGB8888;//kPXP_OutputPixelFormatY8,
        outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
        outputBufferConfig.buffer1Addr = 0U;
        outputBufferConfig.pitchBytes = APP_LCD_WIDTH * 4;
        outputBufferConfig.width = APP_LCD_WIDTH;
        outputBufferConfig.height = APP_LCD_HEIGHT;
        

        uint64_t now, time;
        while(1)
        {
            now = _systime.get_time_us();
            /* cpu可以做别的任务 */
            /*摄像头为60帧，  两帧间隔15ms  这里可以添加用户代码  执行完后再等待csi满*/
            while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //等待摄像头CSI缓存区满
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
            CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//将照相机缓冲区提交到缓冲队列
            time = _systime.get_time_us() - now;
            printf("采集数据时长 %lld \n", time);
            now = _systime.get_time_us();
            /* cpu可以做别的任务 */
            /*PXP图像处理   大概3ms  这里可以添加用户代码  执行完后再检查PXP转换完成*/
            while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(PXP)))  //等待pXP转换完成
            {
            }
            PXP_ClearStatusFlags(PXP, kPXP_CompleteFlag);   //清除标志位  
            
            
            time = _systime.get_time_us() - now;
            printf("PXP时长 %lld \n", time);
            now = _systime.get_time_us();
            
            Camera_0_1_Handle();   //二值化
            
            time = _systime.get_time_us() - now;
            printf("二值化时长 %lld \n", time);
            now = _systime.get_time_us();
            
            Pixle_Filter();        //滤波清噪点
            
            time = _systime.get_time_us() - now;
            printf("滤波时长 %lld \n", time);
            now = _systime.get_time_us();
            Draw_Road(); 

            time = _systime.get_time_us() - now;
            printf("显示 %lld \n", time);
            counter ++;
            
        }
       
    

}
