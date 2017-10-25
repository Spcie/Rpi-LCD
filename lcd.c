
#include "hw_gpio.h"
#include "hw_spi.h"
#include "hw_delay.h"
#include "soft_spi.h"

#define LCD_W	320
#define LCD_H	240

#define HARDWARE_SPI 
//#undef HARDWARE_SPI 

static void LCD_SPI_Init(void);
static void LCD_SPI_unInit(void);
static void LCD_Delay_ms(unsigned int ms);
static void LCD_Delay_us(int us);
static void LCD_REST(unsigned char rest);
static void LCD_WriteModeChoose(unsigned char mode);
static void LCD_WriteBus(unsigned char byte);
static void LCD_WriteByteData(unsigned char dat);
static unsigned int LCD_Pow(unsigned char m,unsigned char n);

static volatile unsigned int* bcm_peripherals_base;
unsigned int PointColor=0x0000;		//画笔颜色
unsigned int BackColor=0xFFFF;		//背景色 

static void LCD_SPI_Init(void)
{
    bcm_peripherals_base = (volatile unsigned int *)ioremap(0x3f000000, 0x01000000);
#ifdef HARDWARE_SPI
    //delay init
    bcm_st_init(bcm_peripherals_base);

    //gpio init
    bcm_gpio_init(bcm_peripherals_base);
    bcm_gpio_fsel(RPI_GPIO_07, BCM_GPIO_FSEL_ALT0);
    bcm_gpio_fsel(RPI_GPIO_08, BCM_GPIO_FSEL_ALT0);
    bcm_gpio_fsel(RPI_GPIO_09, BCM_GPIO_FSEL_ALT0);
    bcm_gpio_fsel(RPI_GPIO_10, BCM_GPIO_FSEL_ALT0);
    bcm_gpio_fsel(RPI_GPIO_11, BCM_GPIO_FSEL_ALT0);

    //spi init
    bcm_spi_init(bcm_peripherals_base);
#else
    soft_spi_init(bcm_peripherals_base);
#endif
}
static void LCD_SPI_unInit(void)
{
#ifdef HARDWARE_SPI
    bcm_st_uninit();
    
    bcm_gpio_fsel(RPI_GPIO_07, BCM_GPIO_FSEL_INPT);
    bcm_gpio_fsel(RPI_GPIO_08, BCM_GPIO_FSEL_INPT);
    bcm_gpio_fsel(RPI_GPIO_09, BCM_GPIO_FSEL_INPT);
    bcm_gpio_fsel(RPI_GPIO_10, BCM_GPIO_FSEL_INPT);
    bcm_gpio_fsel(RPI_GPIO_11, BCM_GPIO_FSEL_INPT);

    bcm_gpio_unint();
    
 #else
    soft_spi_uninit();
 #endif
    iounmap(bcm_peripherals_base);
}

static void LCD_Delay_ms(unsigned int ms)
{
#ifdef HARDWARE_SPI     
    bcm_st_delay_ms(ms);
#else
    soft_spi_delayMs(ms);
#endif
}

static void LCD_Delay_us(int us)
{
#ifdef HARDWARE_SPI     
    bcm_st_delay_us(ms);
#else
    soft_spi_delayUs(us);
#endif
}
static void LCD_REST(unsigned char rest)
{
#ifdef HARDWARE_SPI     
    
#else
    soft_spi_rest(rest);
#endif
}
static void LCD_WriteModeChoose(unsigned char mode)
{
#ifdef HARDWARE_SPI     
    bcm_spi0_setDataMode(mode)
#else
    soft_spi_setDC(mode);
#endif
}

static void LCD_WriteBus(unsigned char byte)
{
#ifdef HARDWARE_SPI     
    bcm_spi0_write(byte, 1);
#else
    soft_spi_wirteBus(byte);
#endif
}

static void LCD_WriteByteData(unsigned char dat)
{
	LCD_WriteModeChoose(SPI_MOD_DATA);
	LCD_WriteBus(dat);
}

static void LCD_Write2ByteData(unsigned int dat)
{
	LCD_WriteModeChoose(SPI_MOD_DATA);
	LCD_WriteBus((dat>>8)&0xFF);
	LCD_WriteBus(dat&0xFF);
}

static void LCD_WriteCmd(unsigned int cmd)
{
	LCD_WriteModeChoose(SPI_MOD_COMMAND);
	LCD_WriteBus(cmd);

}

static void LCD_WriteCmdData(unsigned char cmd,unsigned char dat)
{
	LCD_WriteCmd(cmd);
	LCD_WriteByteData(dat);
}

static void LCD_SetCoordinate(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	LCD_WriteCmd(0x2a);
	LCD_WriteByteData(x1>>8);
	LCD_WriteByteData(x1&0xff);
	LCD_WriteByteData(x2>>8);
	LCD_WriteByteData(x2&0xff);
	
	LCD_WriteCmd(0x2b);
	LCD_WriteByteData(y1>>8);
	LCD_WriteByteData(y1&0xff);
	LCD_WriteByteData(y2>>8);
	LCD_WriteByteData(y2&0xff);
	
	LCD_WriteCmd(0x2c);
}

int LCD_Init(void)
{
    LCD_SPI_Init();

	LCD_REST(1);
	LCD_Delay_ms(1);
	LCD_REST(0);
	LCD_Delay_ms(10);
	LCD_REST(1);
	LCD_Delay_ms(120);
	
	LCD_WriteCmd(0x11);
	LCD_WriteCmd(0x29);
	LCD_WriteCmd(0x2c);

	LCD_WriteCmd(0xCB);
	LCD_WriteByteData(0x39);
	LCD_WriteByteData(0x2C);
	LCD_WriteByteData(0x00);
	LCD_WriteByteData(0x34);
	LCD_WriteByteData(0x02);

	LCD_WriteCmd(0xCF);
	LCD_WriteByteData(0x00);
	LCD_WriteByteData(0XC1);
	LCD_WriteByteData(0X30);
	
	LCD_WriteCmd(0xE8);
	LCD_WriteByteData(0x85);
	LCD_WriteByteData(0x00);
	LCD_WriteByteData(0x78);
	
	LCD_WriteCmd(0xEA);
	LCD_WriteByteData(0x00);
	LCD_WriteByteData(0x00);
	
	LCD_WriteCmd(0xED);
	LCD_WriteByteData(0x64);
	LCD_WriteByteData(0x03);
	LCD_WriteByteData(0X12);
	LCD_WriteByteData(0X81);
	
	LCD_WriteCmd(0xF7);
	LCD_WriteByteData(0x20);
	
	LCD_WriteCmd(0xC0);			//Power control
	LCD_WriteByteData(0x23);	//VRH[5:0] 
	
	LCD_WriteCmd(0xC1);			//Power control 
	LCD_WriteByteData(0x10);	//SAP[2:0];BT[3:0]
	
	LCD_WriteCmd(0xC5);			//VCM control 
	LCD_WriteByteData(0x3e);
	LCD_WriteByteData(0x28);
	
	LCD_WriteCmd(0xC7);			//VCM control2 
	LCD_WriteByteData(0x86);	//--
	
	LCD_WriteCmd(0x36);			// Memory Access Control 
	LCD_WriteByteData(0x48);	//C8
	
	LCD_WriteCmd(0x3A);
	LCD_WriteByteData(0x55);
	
	LCD_WriteCmd(0xB1);
	LCD_WriteByteData(0x00);
	LCD_WriteByteData(0x18);
	
	LCD_WriteCmd(0xB6);			// Display Function Control
	LCD_WriteByteData(0x08);
	LCD_WriteByteData(0x82);
	LCD_WriteByteData(0x27);
	
	LCD_WriteCmd(0xF2);			// 3Gamma Function Disable 
	LCD_WriteByteData(0x00);

	LCD_WriteCmd(0x26);			//Gamma curve selected 
	LCD_WriteByteData(0x01);
	
	LCD_WriteCmd(0xE0);			//Set Gamma
	LCD_WriteByteData(0x0F);
	LCD_WriteByteData(0x31);
	LCD_WriteByteData(0x2B);
	LCD_WriteByteData(0x0C);
	LCD_WriteByteData(0x0E);
	LCD_WriteByteData(0x08);
	LCD_WriteByteData(0x4E);
	LCD_WriteByteData(0xF1);
	LCD_WriteByteData(0x37);
	LCD_WriteByteData(0x07);
	LCD_WriteByteData(0x10);
	LCD_WriteByteData(0x03);
	LCD_WriteByteData(0x0E);
	LCD_WriteByteData(0x09);
	LCD_WriteByteData(0x00);
	
	LCD_WriteCmd(0XE1);			//Set Gamma
	LCD_WriteByteData(0x00);
	LCD_WriteByteData(0x0E);
	LCD_WriteByteData(0x14);
	LCD_WriteByteData(0x03);
	LCD_WriteByteData(0x11);
	LCD_WriteByteData(0x07);
	LCD_WriteByteData(0x31);
	LCD_WriteByteData(0xC1);
	LCD_WriteByteData(0x48);
	LCD_WriteByteData(0x08);
	LCD_WriteByteData(0x0F);
	LCD_WriteByteData(0x0C);
	LCD_WriteByteData(0x31);
	LCD_WriteByteData(0x36);
	LCD_WriteByteData(0x0F);
	
	LCD_WriteCmd(0x11);			//Exit Sleep
	LCD_WriteCmd(0x29);			//Display on
	LCD_WriteCmd(0x2c);
	return 0;
}

void LCD_unInit(void)
{
    LCD_SPI_unInit();
}

void LCD_SetPointColor(unsigned int color)
{
    PointColor = color;
}

void LCD_SetBackColor(unsigned int color)
{
    BackColor = color;
}

void LCD_Clear(unsigned int Color)
{
	unsigned char VH,VL;
	int i,j;
	VH = Color >> 8;
	VL = Color;
	LCD_SetCoordinate(0,0,239,319);
	for(i=0;i<240;i++)
	{
		for(j=0;j<320;j++)
		{
			LCD_WriteByteData(VH);
			LCD_WriteByteData(VL);
		}
	}
}

void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int Color)
{
	LCD_SetCoordinate(x,y,x,y);
	LCD_Write2ByteData(Color);
}

void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{
	unsigned int i,j;
	
	LCD_SetCoordinate(xsta,ysta,xend,yend);
	
	for(i = xsta;i <= yend;i++)
	{
		for(j = xsta;j <= xend;j++)
		{
			LCD_Write2ByteData(color);
		}
	}
}

void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color)
{
	unsigned t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	
	delta_x=x2-x1;
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1;
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x;
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )
	{
		LCD_DrawPoint(uRow,uCol,color);
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

void LCD_ClearLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color)
{
	unsigned t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	
	delta_x=x2-x1;
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1;
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x;
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )
	{
		LCD_DrawPoint(uRow,uCol,color);
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

void LCD_DrawCircle(unsigned int x0,unsigned int y0,unsigned int r,unsigned int color)
{
	int a,b;
	int di;
	a=0;b=r;
	di=3-(r<<1);
	//di=5/4-r;
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);
		LCD_DrawPoint(x0+b,y0-a,color);
		LCD_DrawPoint(x0-a,y0+b,color);
		LCD_DrawPoint(x0-b,y0-a,color);
		LCD_DrawPoint(x0-a,y0-b,color);
		LCD_DrawPoint(x0+b,y0+a,color);
		LCD_DrawPoint(x0+a,y0-b,color);
		LCD_DrawPoint(x0+a,y0+b,color);
		LCD_DrawPoint(x0-b,y0+a,color);
		a++;
		
		if(di<0)di +=4*a+6;
		else
		{
			di+=10+4*(a-b);
			b--;
		}
		LCD_DrawPoint(x0+a,y0+b,color);
	}
}

void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned char size,unsigned char mode)
{
	unsigned char temp,t1,t;
	unsigned int y0=y;
	unsigned int colortemp=PointColor;
	//设置窗口
	num=num-' ';				//得到偏移后的值
	if(!mode)				//非叠加方式
	{
		for(t=0;t<size;t++)
		{
			if(size==12)temp=asc2_1206[num][t];		//调用1206字体
			else temp=asc2_1608[num][t];		 //调用1608字体
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)PointColor=colortemp;
				else PointColor=BackColor;
				LCD_DrawPoint(x,y,PointColor);
				temp<<=1;
				y++;
				if(x>=LCD_W)
				{
					PointColor=colortemp;
					return;
				}				//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=LCD_W)
					{
						PointColor=colortemp;
						return;
					}			//超区域了
					break;
				}
			}
		}
	}else//叠加方式
	{
		for(t=0;t<size;t++)
		{
			if(size==12)temp=asc2_1206[num][t];		//调用1206字体
			else temp=asc2_1608[num][t];			//调用1608字体
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80) LCD_DrawPoint(x,y,PointColor); 
				temp<<=1;
				y++;
				if(x>=LCD_H){PointColor=colortemp;return;}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=LCD_W){PointColor=colortemp;return;}//超区域了
					break;
				}
			}
	}
	}
	PointColor=colortemp;
}

void LCD_ShowString(unsigned int x,unsigned int y,unsigned int width,unsigned int height,unsigned char size,unsigned char *p)
{
	unsigned char x0=x;
	width+=x;
	height+=y;
	while((*p<='~')&&(*p>=' '))//判断是不是非法字符
	{
		if(x>=width){x=x0;y+=size;}
		if(y>=height) break;
		LCD_ShowChar(x,y,*p,size,0);
		x+=size/2;
		p++;
	}
}

void number(unsigned int x,unsigned int y,const unsigned char number)
{
	unsigned char i,j;
	const unsigned char *temp=shuzi3216;
	LCD_SetCoordinate(x,y,x+15,y+31);
	temp+=number*64;
	for(j=0;j<64;j++)
	{
		for(i=0;i<8;i++)
		{
			if((*temp&(1<<i))!=0)
			{
				LCD_WriteByteData(PointColor);
			}
			else
			{
				LCD_WriteByteData(BackColor);
			}
		}
		temp++;
	}
}

static unsigned int LCD_Pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;
	while(n--)
	{
		result*=m;
	}
	return result;
}

void LCD_ShowNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len,unsigned char size)
{
	unsigned char t,temp;
	unsigned char enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else
			{
				enshow=1; 
			}
			
		}

		LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
}
