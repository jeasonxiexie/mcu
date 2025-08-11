/* Minimal Startup Code for WB32L003 (Cortex-M0+) */

    .syntax unified
    .cpu cortex-m0plus
    .thumb

/* Vector Table */
    .section .isr_vector,"a",%progbits
    .type g_pfnVectors, %object
    .size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word _estack              /* Top of Stack */
    .word Reset_Handler        /* Reset Handler */
    .word NMI_Handler         /* NMI Handler */
    .word HardFault_Handler   /* Hard Fault Handler */
    .word 0                   /* Reserved */
    .word 0                   /* Reserved */
    .word 0                   /* Reserved */
    .word 0                   /* Reserved */
    .word 0                   /* Reserved */
    .word 0                   /* Reserved */
    .word 0                   /* Reserved */
    .word SVC_Handler         /* SVCall Handler */
    .word 0                   /* Reserved */
    .word 0                   /* Reserved */
    .word PendSV_Handler      /* PendSV Handler */
    .word SysTick_Handler     /* SysTick Handler */
    
    /* External Interrupts */
    .word Default_Handler     /* Window Watchdog */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* RTC */
    .word Default_Handler     /* FLASH */
    .word Default_Handler     /* RCC */
    .word Default_Handler     /* EXTI0_1 */
    .word Default_Handler     /* EXTI2_3 */
    .word Default_Handler     /* EXTI4_15 */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* ADC */
    .word Default_Handler     /* TIM1_BRK_UP_TRG_COM */
    .word Default_Handler     /* TIM1_CC */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* TIM3 */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* TIM14 */
    .word Default_Handler     /* TIM15 */
    .word Default_Handler     /* TIM16 */
    .word Default_Handler     /* TIM17 */
    .word Default_Handler     /* I2C */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* SPI */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* UART1 */
    .word Default_Handler     /* UART2 */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* Reserved */
    .word Default_Handler     /* Reserved */

/* Reset Handler */
    .section .text.Reset_Handler
    .weak Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
    /* Set stack pointer */
    ldr r0, =_estack
    mov sp, r0

    /* Copy data segment from flash to RAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    movs r3, #0
    b LoopCopyDataInit

CopyDataInit:
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4

LoopCopyDataInit:
    adds r4, r0, r3
    cmp r4, r1
    bcc CopyDataInit
    
    /* Zero fill the bss segment */
    ldr r0, =_sbss
    ldr r1, =_ebss
    movs r2, #0
    b LoopFillZerobss

FillZerobss:
    str r2, [r0]
    adds r0, r0, #4

LoopFillZerobss:
    cmp r0, r1
    bcc FillZerobss

    /* Call SystemInit */
    bl SystemInit
    
    /* Call main */
    bl main
    
    /* Infinite loop if main returns */
LoopForever:
    b LoopForever

.size Reset_Handler, .-Reset_Handler

/* Default Handler for unused interrupts */
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b Infinite_Loop
    .size Default_Handler, .-Default_Handler

/* Weak aliases for interrupt handlers */
    .weak NMI_Handler
    .thumb_set NMI_Handler,Default_Handler

    .weak HardFault_Handler
    .thumb_set HardFault_Handler,Default_Handler

    .weak SVC_Handler
    .thumb_set SVC_Handler,Default_Handler

    .weak PendSV_Handler
    .thumb_set PendSV_Handler,Default_Handler

    .weak SysTick_Handler
    .thumb_set SysTick_Handler,Default_Handler