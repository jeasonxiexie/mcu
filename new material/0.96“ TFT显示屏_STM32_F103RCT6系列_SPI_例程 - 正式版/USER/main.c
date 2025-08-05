//////////////////////////////////////////////////////////////////////////////////	 
//
//******************************************************************************/

#include "display.h"

int main(void)
{	
		Disp_Device_Init();
		while(1)
		{	
			RGB_UPDATE(0x1f,0x00,0x00);// È«ºì
			DELAY_MS(1000);
			RGB_UPDATE(0x00,0x3f,0x00);// È«ÂÌ
			DELAY_MS(1000);
			RGB_UPDATE(0x00,0x00,0x1f);// È«À¶
			DELAY_MS(1000);
			picloop();
			DELAY_MS(1000);
		}
}		

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
