/**
 * @file no_power_control.c
 * @brief Test without ANY power control - just busy loop
 */

/* Main function - busy loop only */
int main(void)
{
    volatile unsigned int i = 0;
    
    /* No GPIO setup at all - just busy loop */
    while(1)
    {
        i++;
        if(i > 1000000) i = 0;
    }
}

/* Empty power latch - do nothing */
void Early_PowerLatch(void) {}

/* Empty stubs */
void SystemInit(void) {}
void __libc_init_array(void) {}