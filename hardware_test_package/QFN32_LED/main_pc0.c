volatile unsigned int* GPIOC_BSRR = (volatile unsigned int*)0x48000818;

void TIM10_IRQHandler(void)
{
    // Clear interrupt flag (simplified)
    volatile unsigned int* TIM10_SR = (volatile unsigned int*)0x40014410;
    *TIM10_SR = 0;
    
    // Toggle PC0 instead of PC4
    static int toggle = 0;
    if(toggle) {
        *GPIOC_BSRR = (1 << 0);  // PC0 LOW
    } else {
        *GPIOC_BSRR = (1 << 16); // PC0 HIGH
    }
    toggle = !toggle;
}
