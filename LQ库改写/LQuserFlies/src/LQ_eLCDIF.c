/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技RT1052核心板
【编    写】Z
【备    注】
【软件版本】V1.0
【最后更新】2017年11月27日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
----------------------------------------------------------------
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

/*******************************************************************************
*  SDK提供了两种在Noncacheable区定义缓冲区和变量的方法：
*  AT_NONCACHEABLE_SECTION_ALIGN(var, alignbytes)
*  AT_NONCACHEABLE_SECTION(var)
******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t lcdFrameBuf[4][LCD_HEIGHT][LCD_WIDTH], FRAME_BUFFER_ALIGN);               //LCD数据缓存区

uint8_t counter;       //LCD有两个缓冲区，一个当前显示用，一个缓冲用
//volatile uint8_t LQ_LCD;  
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】初始化LCD时钟
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void BOARD_InitLcdifPixClock(void)
{
    /*
    * 如果期望LCD以120Hz帧率显示. 那么LCD的时钟应该配置为:
    *（LCD宽度 + APP_HSW + APP_HFP + APP_HBP）* （LCD高度 +　APP_VSW　＋　APP_VSW　＋　APP_VBP）　*　帧率　
    * 即：(480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 120 = 9.3M * 2.
    * Here set the LCDIF pixel clock to 9.3M.
    */
    
    /*
    * Initialize the Video PLL.
    * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
    */
    clock_video_pll_config_t config = {
        .loopDivider = 31, 
        .postDivider = 8, 
        .numerator   = 0, 
        .denominator = 0,
    };
    
    CLOCK_InitVideoPll(&config);
    
    /*
    * 000 derive clock from PLL2
    * 001 derive clock from PLL3 PFD3
    * 010 derive clock from PLL5
    * 011 derive clock from PLL2 PFD0
    * 100 derive clock from PLL2 PFD1
    * 101 derive clock from PLL3 PFD1
    */
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);//选择PLL5 
    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 4);//93Mhz  ５分频
    CLOCK_SetDiv(kCLOCK_LcdifDiv, 0);//再1分频

}



//把16位RGB565彩色摄像头数据转换为24位RGB彩屏显示
//static void rgb565_2_rgb888(uint8_t *rgb565, uint8_t *rgb888, int pixels)
//{
//// RGB565 in little-endian: "G2G1G0B4B3B2B1B0 R4R3R2R1R0G5G4G3 
//for(uint32_t i=0; i<pixels; i++)
//{
//rgb888[3*i] = ((rgb565[2*i] & 0x1F)<<3) | (rgb565[2*i] & 0x07);//B4B3B2B1B0B2B1B0
//rgb888[3*i+1] = ((rgb565[2*i+1] & 0x07)<<5) | ((rgb565[2*i] & 0xE0)>>3) | ((rgb565[2*i] & 0x60)>>5);//G5G4G3G2G1G0G1G0
//rgb888[3*i+2] = (rgb565[2*i+1] & 0xF8) | ((rgb565[2*i+1] & 0x38)>>3); //R4R3R2R1R0R2R1R0
//    }
//}

void FillLCDBuffer(unsigned char page,unsigned char *PicArray)
{
    uint32_t i, j,cnt=0;
    
    for (i = 0; i < LCD_HEIGHT; i++)
    {
        for (j = 0; j < LCD_WIDTH; j++)
        {    //水平扫描，24位真彩色，高位在前
            lcdFrameBuf[page][i][j] = ((uint32_t)PicArray[cnt]<<16)|((uint32_t)PicArray[cnt+1]<<8)|((uint32_t)PicArray[cnt+2]);
            cnt+=3;
        }
    }
}
void APP_LCDIF_IRQHandler(void)
{
    uint32_t intStatus;
    
    intStatus = ELCDIF_GetInterruptStatus(LCDIF);
    
    ELCDIF_ClearInterruptStatus(LCDIF, intStatus);
    
    if (intStatus & kELCDIF_CurFrameDone)
    {
//        s_frameDone = true;
    }
}
void LCDIF_IRQHandler(void)           //液晶屏中断处理函数
{
    APP_LCDIF_IRQHandler();
}

/* Enable interrupt. */
void BOARD_EnableLcdInterrupt(void)   //液晶屏使能中断
{
    EnableIRQ(LCDIF_IRQn);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】Z
【功能说明】液晶屏初始化
【软件版本】V1.0
【最后更新】2018年10月18日 
【函数名】
【返回值】无
【参数值】无
【实例】 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_TFT480_272_Init(void)
{ 
    BOARD_InitLCDPins();      //彩色LCD管脚复用 
    BOARD_InitLcdifPixClock();//LCD时钟config  
    
    uint8_t  txt[32]="LongQiu";
    //Configure the LCDIF.  
    const elcdif_rgb_mode_config_t lcdConfig = {
        .panelWidth = APP_LCD_WIDTH,      //LCD width
        .panelHeight = APP_LCD_HEIGHT,    //LCD height
        .hsw = APP_HSW,                             //表示水平同步信号的宽度，用VCLK计算
        .hfp = APP_HFP,                             //表示一行的有效数据结束到下一个水平同步信号开始之间的VCLK的个数
        .hbp = APP_HBP,                             //表示从水平同步信号开始到一行的有效数据开始之间的VCLK的个数
        .vsw = APP_VSW,                             //表示垂直同步脉冲的宽度，用行数计算
        .vfp = APP_VFP,                             //表示在一帧图像结束后，垂直同步信号以前的无效的行数
        .vbp = APP_VBP,                             //表示在一帧图像开始时，垂直同步信号以后的无效的行数
        .bufferAddr = (uint32_t)lcdFrameBuf[0],    //缓冲区地址
        .polarityFlags = APP_LCD_POL_FLAGS,         //LCD的一些配置
        .pixelFormat = kELCDIF_PixelFormatXRGB8888, //像素格式
        .dataBus = APP_LCDIF_DATA_BUS,              //LCD数据总线类型
    };
    
    ELCDIF_RgbModeInit(LCDIF, &lcdConfig);  //开时钟，config lcd
    
    ELCDIF_RgbModeStart(LCDIF);               //开始显示
    
    TFT_Draw_Part(40,44,440,250,BLACK,1);                 //填充矩形区域，行起始、终止坐标，列起始、终止坐标，颜色
    TFT_Draw_Part(60,64,420,230,GREEN,1);
    TFT_Draw_Line(40,44,440,250,RED,1);                   //画线，行起始、终止坐标，列起始、终止坐标，颜色
    TFT_Draw_Line(40,250,440,44,RED,1); 
    TFT_Draw_Rectangle(50,54,430,240,GREEN,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
    TFT_Draw_Circle(240,136,100,RED,1);                   //画圆形边框，圆心横坐标、纵坐标，半径，颜色
    TFT_P16x16Str(184,115,"北京龙邱智能科技",RED,WHITE,1);//字符串显示
    TFT_P8X16Str(208,140,(uint8_t*)txt,BLACK,WHITE,1);   //字符串显示 
    ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)lcdFrameBuf[0]); //将缓存区lcdFrameBuf 显示在TFT屏上
    
//    GT9147_Init();//初始化触摸板
    delayms(200);        //延时2000毫秒 
}





/**********************************************************
函数名称：TFT_Draw_Dot()
入口参数：起始横坐标(0-127)，纵坐标(0-159),显示颜色uint16
出口参数：无
时间：2018-01-28
功能说明：画点
其他说明：0<=x<=127
0<=y<=159
**********************************************************/
void TFT_Draw_Dot(uint16_t x,uint16_t y,uint32_t fcolor)
{  
    if(counter % 2)
    {
        lcdFrameBuf[1][y][x]=fcolor;
    }
    else
    {
        lcdFrameBuf[0][y][x]=fcolor;
    }
}

/*
【功能】 填充指定区域
【参数】 起始点x坐标，起始点y坐标，终止点x坐标，终止点有坐标，填充色
例：TFT_Show_line(0,5,WHITE);  //填充第五行
TFT_Show_line(2,5,WHITE);  //从第二个字符开始填充第五行
*/
void TFT_Fill_Area(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint32_t fcolor,uint8_t clr)
{
    unsigned int i,j;  
    
    for(i=xs;i<xe;i++)  
    {
        for(j=ys;j<ye;j++)
        {
            
                if(counter % 2)
                {
                    if(clr) lcdFrameBuf[1][j][i]=fcolor;
                    else   lcdFrameBuf[1][j][i]|=fcolor;
                }
                else
                {
                    if(clr) lcdFrameBuf[0][j][i]=fcolor;
                    else   lcdFrameBuf[0][j][i]|=fcolor;
                }
        }
    }
}

/**********************************************************
函数名称：TFT_CLS()
入口参数：color为常用的需要显示的颜色(已经提前定义)，color为
NULL时，则可将不常用颜色放入dat中加以显示
出口参数：无
时间：2018-01-28
功能说明：全屏显示单色画面
其他说明：无
**********************************************************/
void TFT_CLS(uint32_t fcolor)
{
    unsigned int i,j;
    
    for(i=0;i<APP_IMG_WIDTH;i++)      //480
    {
        for(j=0;j<APP_IMG_HEIGHT;j++)
        {
                if(counter % 2)
                {
                    lcdFrameBuf[1][j][i]=fcolor;
                }
                else
                {
                    lcdFrameBuf[0][j][i]=fcolor;
                }
        }
    }
}

/**********************************************************
函数名称：TFT_Draw_part()
入口参数：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
出口参数：无
时间：2018-01-28
功能说明：填充矩形区域
其他说明：0<=xs<xe<=127
0<=ys<ye<=159
**********************************************************/
void TFT_Draw_Part(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint32_t fcolor,uint8_t clr)
{
    uint32_t i,j;  
    
    for(j=ys;j<(ye+1);j++)
    {
        for(i=xs;i<(xe+1);i++)
        {
            if(counter % 2)
            {
                if(clr) lcdFrameBuf[1][j][i]=fcolor;
                else   lcdFrameBuf[1][j][i]|=fcolor;
            }
            else
            {
                if(clr) lcdFrameBuf[0][j][i]=fcolor;
                else   lcdFrameBuf[0][j][i]|=fcolor;
            }
            
        }
    }   
}


/**********************************************************
函数名称：TFT_Draw_line()
入口参数：起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
出口参数：无
时间：2018-01-28
功能说明：画直线
其他说明：0<=xs<xe<=127
0<=ys<ye<=159
**********************************************************/
void TFT_Draw_Line(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint32_t fcolor,uint8_t clr)
{
    uint32_t i,ds;
    int dx,dy,inc_x, inc_y;
    int xerr = 0, yerr = 0;				        //初始化变量
    
    if(xs==xe)	  						//如果是画垂直线则只需对竖直坐标计数
    {    
        for(i=ys;i<(ye+1);i++)
        {
            if(counter % 2)
            {
                if(clr) lcdFrameBuf[1][i][xs]=fcolor;
                else   lcdFrameBuf[1][i][xs]|=fcolor;
            }
            else
            {
                if(clr) lcdFrameBuf[0][i][xs]=fcolor;
                else   lcdFrameBuf[0][i][xs]|=fcolor;
            }
            
        }
    }
    else if(ys==ye)						//如果是水平线则只需要对水平坐标计数
    {   
        for(i=xs;i<(xe+1);i++)
        {
            
            if(counter % 2)
            {
                if(clr) lcdFrameBuf[1][ys][i]=fcolor;
                else   lcdFrameBuf[1][ys][i]|=fcolor;
            }
            else
            {
                if(clr) lcdFrameBuf[0][ys][i]=fcolor;
                else   lcdFrameBuf[0][ys][i]|=fcolor;
            }
        }
    } 
    else								//如果是斜线，则重新计算，使用画点函数画出直线
    {
        dx = xe - xs;						//计算坐标增量
        dy = ye - ys;
        
        if(dx > 0) inc_x = 1;					//设置单步方向
        else 
        {
            inc_x = -1; dx = -dx;
        }
        if(dy > 0) inc_y = 1;					//设置单步方向
        else
        {
            inc_y = -1; dy = -dy;
        }
        
        if(dx > dy) ds = dx;					//选取基本增量坐标轴
        else		ds = dy;
        
        for(i = 0; i <= ds+1; i++)					//画线输出
        {
            TFT_Draw_Dot(xs, ys,fcolor);	//画点
            xerr += dx;
            yerr += dy;
            if(xerr > ds)
            {
                xerr -= ds;
                xs   += inc_x;
            }
            if(yerr > ds)
            {
                yerr -= ds;
                ys   += inc_y;
            }
        }
    }  
}

/****************显示图片（从单片机的DATA区）********************************/	

void TFT_Show_Pic(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint8_t *PicArray)
{
    uint16_t i, j,cnt=0;
    
    for(i=0;i<ye-ys;i++)
    { 
        for(j=0;j<xe-xs;j++)
        {			
            lcdFrameBuf[0][j][i]=((uint32_t)PicArray[cnt]<<16)|((uint32_t)PicArray[cnt+1]<<8)|((uint32_t)PicArray[cnt+2]);		
            cnt+=3; 
        }
    }    
}



/**********************************************************
函数名称：Text_TFT_480_272()
入口参数：
出口参数：无
时间：2018-01-28
功能说明：画直线
其他说明：液晶屏测试函数
**********************************************************/
void Test_TFT_480_272(void)
{
    
    LQ_TFT480_272_Init();
    GT9147_Init();//初始化触摸板
    
    char  txt[100]="LongQiu";
    static uint8_t i, count;
    while(1)
    {
        count ++;
        if (count > 20) count = 0;
        TFT_Draw_Part(40,44,440,250,BLACK,1);                 //填充矩形区域，行起始、终止坐标，列起始、终止坐标，颜色
        TFT_Draw_Part(60,64,420,230,GREEN,1);
        TFT_Draw_Line(40,44,440,250,RED,1);                   //画线，行起始、终止坐标，列起始、终止坐标，颜色
        TFT_Draw_Line(40,250,440,44,RED,1); 
        TFT_Draw_Rectangle(50,54,430,240,GREEN,1);            //画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
        TFT_Draw_Circle(240,136,100,RED,1);                   //画圆形边框，圆心横坐标、纵坐标，半径，颜色
        TFT_P16x16Str(184,115,"北京龙邱智能科技",RED,WHITE,1);//字符串显示
        
        for(i = 0; i < count; i ++)  //给LongQiu前后加空格
        {
            sprintf(txt + i, " ");
        }
        
        sprintf(txt + count, "LongQiu");
        TFT_P8X16Str(108,140,(uint8_t*)txt,BLACK,WHITE,1);   //字符串显示 
        ELCDIF_SetNextBufferAddr(LCDIF, (uint32_t)lcdFrameBuf[0]); //将缓存区lcdFrameBuf 显示在TFT屏上
        
        //    GT9147_Init();//初始化触摸板
        delayms(1000);        //延时1000毫秒 
        while(1)
        {
            if(GT9147_Scan(0))   //触摸屏按下
            {
                uint8_t i;
                for(i=0;i<5;i++)
                {         
                    TFT_Draw_CircleCake(tp_dev.x[i],tp_dev.y[i],40,RED,1);   //触摸的地方画圆
                    TFT_Draw_CircleCake(tp_dev.x[i],tp_dev.y[i],20,BLUE,1);
                    TFT_Draw_CircleCake(tp_dev.x[i],tp_dev.y[i], 8,BLACK,1);
                }
            }      
            
        }        
    } 
}










