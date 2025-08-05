/**
 * @file absolute_minimal.c
 * @brief Absolute minimal - just busy loop
 */

/* Main function - just busy loop */
int main(void)
{
    volatile unsigned int i = 0;
    
    /* Busy loop to generate heat */
    while(1)
    {
        i++;
        if(i > 1000000) i = 0;
    }
}

/* Empty stubs for startup */
void SystemInit(void) {}
void Early_PowerLatch(void) {}
void __libc_init_array(void) {}