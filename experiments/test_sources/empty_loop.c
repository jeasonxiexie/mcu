/**
 * @file empty_loop.c
 * @brief Empty loop - do nothing, just keep MCU running
 */

int main(void)
{
    /* Infinite loop - do nothing */
    while(1)
    {
        /* Empty - just keep running */
    }
}

/* Empty Early_PowerLatch - do nothing */
void Early_PowerLatch(void)
{
    /* Do nothing - let's see what happens */
}