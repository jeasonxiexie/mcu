//////////////////////////////////////////////////////////////////////////////////	 
//
//******************************************************************************/

#include "display.h"
#include "stdlib.h"
#include "font.h"  	 
#include "bmp.h"


//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;

void SPI4_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  SPI_DC_Set();
	else 
	  SPI_DC_Clr();		  
	SPI_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		SPI_SCLK_Clr();
		if(dat&0x80)
		   SPI_SDIN_Set();
		else 
		   SPI_SDIN_Clr();
		SPI_SCLK_Set();
		dat<<=1;   
	}				 		  
	SPI_CS_Set();
	SPI_DC_Set();   	  
} 
void SPI_WR_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{			  
		SPI_SCLK_Clr();
		if(dat&0x80)
		   SPI_SDIN_Set();
		else 
		   SPI_SDIN_Clr();
		SPI_SCLK_Set();
		dat<<=1;   
	}		
}



//在指定位置显示一个字符,包括部分字符

void ShowChar_c(u8 x,u16 y,u8 chr,u8 colour)
{      	
	unsigned char c=0,i=0,k,t,s,temp[2];
		c=chr-' ';//???????			
		if(x>79){x=0;y=y+2;}
		if(SIZE ==16)
			{
				set_ram(X_ST+x,X_ST+x+7,Y_ST+y,Y_ST+y+15);
				for(i=0;i<16;i++)
					{
						t=F8X16[c*16+i];
						for(k=0;k<8;k++)
						{
							s=t&0x80;
								switch(colour)
								{
									case 0://white
										if(s==0x80)
										{
											temp[0]=0xff;
											temp[1]=0xff;
										}
										else
										{
											temp[0]=0x00;
											temp[1]=0x00;
										}
		
										SPI4_WR_Byte(temp[0],DATA);
										SPI4_WR_Byte(temp[1],DATA);
									break;
									
										case 1://red
										if(s==0x80)
										{
											temp[0]=0xf1;
											temp[1]=0x00;
										}
										else
										{
											temp[0]=0x00;
											temp[1]=0x00;
										}
		
										SPI4_WR_Byte(temp[0],DATA);
										SPI4_WR_Byte(temp[1],DATA);
									break;
									
										case 2://gleen
										if(s==0x80)
										{
											temp[0]=0x07;
											temp[1]=0xe0;
										}
										else
										{
											temp[0]=0x00;
											temp[1]=0x00;
										}
		
										SPI4_WR_Byte(temp[0],DATA);
										SPI4_WR_Byte(temp[1],DATA);
									break;
									
										case 3://blue
										if(s==0x80)
										{
											temp[0]=0x00;
											temp[1]=0x1f;
										}
										else
										{
											temp[0]=0x00;
											temp[1]=0x00;
										}
		
										SPI4_WR_Byte(temp[0],DATA);
										SPI4_WR_Byte(temp[1],DATA);
									break;
								
								default:
									break;
									
								}
							t=t<<1;
						}

					}
			}
}

void ShowString_c(u8 x,u16 y,u8 *chr,u8 colour)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		ShowChar_c(x,y,chr[j],colour);
			x+=8;
		if(x>79){x=0;y+=16;}
			j++;
	}
}
//在指定位置显示一个汉字

void ShowHZ_c(u8 x,u16 y,u8 num,u8 colour)
{      	
	unsigned char c=0,i=0,k,t,s,temp[2];
		c=num;//???????			
		if(x>79){x=0;y=y+2;}

				set_ram(X_ST+x,X_ST+x+15,Y_ST+y,Y_ST+y+15);
				for(i=0;i<32;i++)
					{
						t=Hzk[c*32+i];
						for(k=0;k<8;k++)
						{
							s=t&0x80;
								switch(colour)
								{
									case 0://white
										if(s==0x80)
										{
											temp[0]=0xff;
											temp[1]=0xff;
										}
										else
										{
											temp[0]=0x00;
											temp[1]=0x00;
										}
		
										SPI4_WR_Byte(temp[0],DATA);
										SPI4_WR_Byte(temp[1],DATA);
									break;
									
										case 1://red
										if(s==0x80)
										{
											temp[0]=0xf1;
											temp[1]=0x00;
										}
										else
										{
											temp[0]=0x00;
											temp[1]=0x00;
										}
		
										SPI4_WR_Byte(temp[0],DATA);
										SPI4_WR_Byte(temp[1],DATA);
									break;
									
										case 2://gleen
										if(s==0x80)
										{
											temp[0]=0x07;
											temp[1]=0xe0;
										}
										else
										{
											temp[0]=0x00;
											temp[1]=0x00;
										}
		
										SPI4_WR_Byte(temp[0],DATA);
										SPI4_WR_Byte(temp[1],DATA);
									break;
									
										case 3://blue
										if(s==0x80)
										{
											temp[0]=0x00;
											temp[1]=0x1f;
										}
										else
										{
											temp[0]=0x00;
											temp[1]=0x00;
										}
		
										SPI4_WR_Byte(temp[0],DATA);
										SPI4_WR_Byte(temp[1],DATA);
									break;
								
								default:
									break;
									
								}
							t=t<<1;
						}

					}
}

//============================================================
void Disp_Device_IO_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能B端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//OD开漏输出	 //PP推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //
 	GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15);	

}

void Disp_Device_RESET(void)
{
	SPI_RST_Set();
	DELAY_MS(1);
	SPI_RST_Clr();
	DELAY_MS(2);
	SPI_RST_Set(); 
	DELAY_MS(2);
}
//*********************************************************************************
//初始化7735S				    
void Disp_Device_Init(void)
{ 	
 	Disp_Device_IO_config();
 
	BL_PW_OFF();//BL OFF
	
	Disp_Device_RESET();
	
	SPI4_WR_Byte(0x11,CMD); 
	
	DELAY_MS(120);		
	  
	SPI4_WR_Byte( 0xB1,CMD);     
	SPI4_WR_Byte( 0x05,DATA);   
	SPI4_WR_Byte( 0x3C,DATA);   
	SPI4_WR_Byte( 0x3C,DATA);   
	
	SPI4_WR_Byte( 0xB2,CMD);     
	SPI4_WR_Byte( 0x05,DATA);   
	SPI4_WR_Byte( 0x3C,DATA);   
	SPI4_WR_Byte( 0x3C,DATA);   
	
	SPI4_WR_Byte( 0xB3,CMD);     
	SPI4_WR_Byte( 0x05,DATA);   
	SPI4_WR_Byte( 0x3C,DATA);   
	SPI4_WR_Byte( 0x3C,DATA);   
	SPI4_WR_Byte( 0x05,DATA);   
	SPI4_WR_Byte( 0x3C,DATA);   
	SPI4_WR_Byte( 0x3C,DATA);   
	
	SPI4_WR_Byte( 0xB4,CMD);     //Dot inversion
	SPI4_WR_Byte( 0x03,DATA);   
	
	SPI4_WR_Byte( 0xC0,CMD);     
	SPI4_WR_Byte( 0x0e,DATA);   
	SPI4_WR_Byte( 0x0e,DATA);   
	SPI4_WR_Byte( 0x04,DATA);   
	
	SPI4_WR_Byte( 0xC1,CMD);     
	SPI4_WR_Byte( 0xC0,DATA);   
	
	SPI4_WR_Byte( 0xC2,CMD);     
	SPI4_WR_Byte( 0x0D,DATA);   
	SPI4_WR_Byte( 0x00,DATA);   
	
	SPI4_WR_Byte( 0xC3,CMD);     
	SPI4_WR_Byte( 0x8D,DATA);   
	SPI4_WR_Byte( 0x2A,DATA);   
	
	SPI4_WR_Byte( 0xC4,CMD);     
	SPI4_WR_Byte( 0x8D,DATA);   
	SPI4_WR_Byte( 0xEE,DATA);   
	
	SPI4_WR_Byte( 0xC5,CMD);     //VCOM
	SPI4_WR_Byte( 0x04,DATA);   
	
	SPI4_WR_Byte(0xE0,CMD);
	SPI4_WR_Byte( 0x05,DATA);
	SPI4_WR_Byte( 0x1A,DATA);
	SPI4_WR_Byte( 0x0B,DATA);
	SPI4_WR_Byte( 0x15,DATA);
	SPI4_WR_Byte( 0x3D,DATA);
	SPI4_WR_Byte( 0x38,DATA);
	SPI4_WR_Byte( 0x2E,DATA);
	SPI4_WR_Byte( 0x30,DATA);
	SPI4_WR_Byte( 0x2D,DATA);
	SPI4_WR_Byte( 0x28,DATA);
	SPI4_WR_Byte( 0x30,DATA);
	SPI4_WR_Byte( 0x3B,DATA);
	SPI4_WR_Byte( 0x00,DATA);
	SPI4_WR_Byte( 0x01,DATA);
	SPI4_WR_Byte( 0x02,DATA);
	SPI4_WR_Byte( 0x10,DATA);
	
	
	SPI4_WR_Byte(0xE1,CMD);
	SPI4_WR_Byte( 0x05,DATA);
	SPI4_WR_Byte( 0x1A,DATA);
	SPI4_WR_Byte( 0x0B,DATA);
	SPI4_WR_Byte( 0x15,DATA);
	SPI4_WR_Byte( 0x36,DATA);
	SPI4_WR_Byte( 0x2E,DATA);
	SPI4_WR_Byte( 0x28,DATA);
	SPI4_WR_Byte( 0x2B,DATA);
	SPI4_WR_Byte( 0x2B,DATA);
	SPI4_WR_Byte( 0x28,DATA);
	SPI4_WR_Byte( 0x30,DATA);
	SPI4_WR_Byte( 0x3B,DATA);
	SPI4_WR_Byte( 0x00,DATA);
	SPI4_WR_Byte( 0x01,DATA);
	SPI4_WR_Byte( 0x02,DATA);
	SPI4_WR_Byte( 0x10,DATA);
	
	SPI4_WR_Byte( 0x3A,CMD);   
	SPI4_WR_Byte( 0x05,DATA);   
	
	SPI4_WR_Byte( 0x36,CMD);     
	SPI4_WR_Byte( 0xC8,DATA);   

	RGB_UPDATE(0,0,0);//全黑，清显示
	
	SPI4_WR_Byte(0x29,CMD);     
	
	BL_PW_ON();//BL ON    
} 

void set_ram_adress(void)
{

	SPI4_WR_Byte(0x2A,CMD);     //Column Address Set
	SPI4_WR_Byte((X_ST&0x0f00)>>8,DATA);   
	SPI4_WR_Byte(X_ST&0xff,DATA);   //0
	SPI4_WR_Byte((X_ED&0x0f00)>>8,DATA);   
	SPI4_WR_Byte(X_ED&0xff,DATA);   //
	
	SPI4_WR_Byte(0x2B,CMD);     //Row Address Set
	SPI4_WR_Byte((Y_ST&0x0f00)>>8,DATA);   
	SPI4_WR_Byte(Y_ST&0xff,DATA);   //0
	SPI4_WR_Byte((Y_ED&0xf00)>>8,DATA);   
	SPI4_WR_Byte(Y_ED&0xff,DATA);   //
	
	SPI4_WR_Byte(0x2C,CMD);
} 

void set_ram(u8 XS,u8 XE,u16 YS,u16 YE)
{

	SPI4_WR_Byte(0x2A,CMD);     //Column Address Set
	SPI4_WR_Byte((XS&0x0f00)>>8,DATA);   
	SPI4_WR_Byte(XS&0xff,DATA);   //0
	SPI4_WR_Byte((XE&0x0f00)>>8,DATA);   
	SPI4_WR_Byte(XE&0xff,DATA);   //
	
	SPI4_WR_Byte(0x2B,CMD);     //Row Address Set
	SPI4_WR_Byte((YS&0x0f00)>>8,DATA);   
	SPI4_WR_Byte(YS&0xff,DATA);   //0
	SPI4_WR_Byte((YE&0xf00)>>8,DATA);   
	SPI4_WR_Byte(YE&0xff,DATA);   //
	
	SPI4_WR_Byte(0x2C,CMD);
}

void RGB_UPDATE(unsigned char R,unsigned char G,unsigned char B)
{    
  u16 i,j,temp;	
  
	set_ram_adress();
	
  for(j=0;j<80;j++)
  {
	  for(i=0;i<160;i++)
		  {
		  	temp = R<<5;
			  temp |= G&0X07;
		  	SPI4_WR_Byte(temp,DATA);
		  	temp = G<<5;
			  temp |= B&0X1F;
			  SPI4_WR_Byte(temp,DATA);
			}
	}
}

void picloop(void)
{
 u16 i,j;	
 u8 *pic; 
 
	set_ram_adress();
	
	pic = (u8*)(gImage_222);
		
  for(j=0;j<160;j++)
  {
	  for(i=0;i<80;i++)
		  {
				SPI4_WR_Byte(*pic,DATA); 
		  	pic++;
				SPI4_WR_Byte(*pic,DATA); 
		  	pic++;
			}
	}
}

void DELAY_US(void)
{
	u16 i;
	for(i=0;i<8000;i++)
		__NOP();
}

void DELAY_MS(u16 xms)
{
	u16 i;
	for(i=0;i<xms;i++)
	DELAY_US();
}
