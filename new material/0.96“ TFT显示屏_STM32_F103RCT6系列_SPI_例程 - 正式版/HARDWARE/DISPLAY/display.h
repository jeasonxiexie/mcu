//////////////////////////////////////////////////////////////////////////////////	 
//
//******************************************************************************/
#ifndef __DISPLAY_H
#define __DISPLAY_H			  	 
#include "sys.h"
#include "stdlib.h"	

//0:4线串行模式


#define SIZE 16
#define X_WIDTH 	80
#define Y_WIDTH 	160

#define  X_ST  0x18
#define  X_ED  (X_ST+X_WIDTH-1)

#define  Y_ST  0x00
#define  Y_ED (Y_ST+Y_WIDTH-1)

//-----------------OLED端口定义----------------  					   


#define SPI_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_11)//CLK
#define SPI_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_11)

#define SPI_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_10)//DIN
#define SPI_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define SPI_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_12)//RES
#define SPI_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define SPI_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//DC
#define SPI_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)
 		     
#define SPI_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_15)//CS
#define SPI_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define BL_PW_ON()  GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define BL_PW_OFF()  GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define CMD  0	//写命令
#define DATA 1	//写数据


//OLED控制用函数

void SPI4_WR_Byte(u8 dat,u8 cmd);	    
	   							   		    
void Disp_Device_Init(void);

void ShowChar_c(u8 x,u16 y,u8 chr,u8 colour);

void ShowString_c(u8 x,u16 y,u8 *chr,u8 colour);

void ShowHZ_c(u8 x,u16 y,u8 num,u8 colour);

void RGB_UPDATE(unsigned char R,unsigned char G,unsigned char B);

void SPI_WR_Byte(u8 dat);

void Disp_Device_IO_config(void);

void Disp_Device_RESET(void);

void picloop(void);

void set_ram_adress(void);

void set_ram(u8 XS,u8 XE,u16 YS,u16 YE);

void DELAY_US(void);

void DELAY_MS(u16 xms);
#endif  
	 



