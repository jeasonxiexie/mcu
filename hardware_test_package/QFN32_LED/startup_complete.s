/* Complete Startup Code for WB32L003 - matches original size */

    .syntax unified
    .cpu cortex-m0plus
    .fpu softvfp
    .thumb

/* Stack and Heap configuration */
.global  g_pfnVectors
.global  Default_Handler

    .word  _sidata
    .word  _sdata
    .word  _edata
    .word  _sbss
    .word  _ebss

/* Vector Table */
    .section  .isr_vector,"a",%progbits
    .type  g_pfnVectors, %object
    .size  g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word  _estack                    /* Top of Stack */
    .word  Reset_Handler              /* Reset Handler */
    .word  NMI_Handler                /* NMI Handler */
    .word  HardFault_Handler          /* Hard Fault Handler */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  SVC_Handler                /* SVCall Handler */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  PendSV_Handler             /* PendSV Handler */
    .word  SysTick_Handler            /* SysTick Handler */
    
    /* External Interrupts */
    .word  WWDG_IRQHandler            /* Window Watchdog */
    .word  0                          /* Reserved */
    .word  RTC_IRQHandler             /* RTC */
    .word  FLASH_IRQHandler           /* FLASH */
    .word  RCC_IRQHandler             /* RCC */
    .word  EXTI0_1_IRQHandler         /* EXTI Line 0 and 1 */
    .word  EXTI2_3_IRQHandler         /* EXTI Line 2 and 3 */
    .word  EXTI4_15_IRQHandler        /* EXTI Line 4 to 15 */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  ADC_IRQHandler             /* ADC */
    .word  TIM10_IRQHandler           /* TIM10 - THIS IS CRITICAL! */
    .word  TIM11_IRQHandler           /* TIM11 */
    .word  TIM1_IRQHandler            /* TIM1 */
    .word  TIM2_IRQHandler            /* TIM2 */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  0                          /* Reserved */
    .word  I2C_IRQHandler             /* I2C */
    .word  0                          /* Reserved */
    .word  SPI_IRQHandler             /* SPI */
    .word  0                          /* Reserved */
    .word  UART1_IRQHandler           /* UART1 */
    .word  UART2_IRQHandler           /* UART2 */
    .word  0                          /* Reserved */
    .word  LPUART_IRQHandler          /* LPUART */
    .word  0                          /* Reserved */

/* Reset Handler */
    .section  .text.Reset_Handler
    .weak  Reset_Handler
    .type  Reset_Handler, %function
Reset_Handler:
    /* Set stack pointer */
    ldr   r0, =_estack
    mov   sp, r0

    /* Copy data segment from flash to RAM */
    movs  r1, #0
    b     LoopCopyDataInit

CopyDataInit:
    ldr   r3, =_sidata
    ldr   r3, [r3, r1]
    str   r3, [r0, r1]
    adds  r1, r1, #4

LoopCopyDataInit:
    ldr   r0, =_sdata
    ldr   r3, =_edata
    adds  r2, r0, r1
    cmp   r2, r3
    bcc   CopyDataInit
    
    /* Zero fill the bss segment */
    ldr   r2, =_sbss
    ldr   r3, =_ebss
    movs  r0, #0
    b     LoopFillZerobss

FillZerobss:
    str   r0, [r2]
    adds  r2, r2, #4

LoopFillZerobss:
    cmp   r2, r3
    bcc   FillZerobss

    /* Call SystemInit */
    bl    SystemInit
    
    /* Call main */
    bl    main
    
    /* Should never get here */
LoopForever:
    b     LoopForever

.size  Reset_Handler, .-Reset_Handler

/* Default Handler */
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b  Infinite_Loop
    .size  Default_Handler, .-Default_Handler

/* Weak alias for all interrupts */
    .macro  def_irq_handler handler_name
    .weak   \handler_name
    .set    \handler_name, Default_Handler
    .endm

    def_irq_handler    NMI_Handler
    def_irq_handler    HardFault_Handler
    def_irq_handler    SVC_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler
    def_irq_handler    WWDG_IRQHandler
    def_irq_handler    RTC_IRQHandler
    def_irq_handler    FLASH_IRQHandler
    def_irq_handler    RCC_IRQHandler
    def_irq_handler    EXTI0_1_IRQHandler
    def_irq_handler    EXTI2_3_IRQHandler
    def_irq_handler    EXTI4_15_IRQHandler
    def_irq_handler    ADC_IRQHandler
    /* TIM10_IRQHandler is NOT weak - defined in main */
    def_irq_handler    TIM11_IRQHandler
    def_irq_handler    TIM1_IRQHandler
    def_irq_handler    TIM2_IRQHandler
    def_irq_handler    I2C_IRQHandler
    def_irq_handler    SPI_IRQHandler
    def_irq_handler    UART1_IRQHandler
    def_irq_handler    UART2_IRQHandler
    def_irq_handler    LPUART_IRQHandler