// Minimal test for PC0 - compile with Keil
#include "wb32l003.h"

int main(void)
{
    SystemCoreClockUpdate();
    
    // Enable GPIOC clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    
    // PC0 as output
    GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
    
    // Simple blink
    while(1)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_0);  // LED ON
        for(volatile int i = 0; i < 100000; i++);
        GPIO_SetBits(GPIOC, GPIO_Pin_0);    // LED OFF
        for(volatile int i = 0; i < 100000; i++);
    }
}
