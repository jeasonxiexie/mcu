/**
  * @file startup_power.s
  * @brief 包含电源锁存的启动代码
  */

  .syntax unified
  .cpu cortex-m0plus
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section. */
.word _sidata
/* start address for the .data section. */
.word _sdata
/* end address for the .data section. */
.word _edata
/* start address for the .bss section. */
.word _sbss
/* end address for the .bss section. */
.word _ebss

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0

/* 立即进行电源锁存 - PC6必须在150ms内设置为高 */
  ldr r0, =0x40021014    /* RCC_AHBENR */
  ldr r1, [r0]
  ldr r2, =0x00080000    /* GPIOCEN */
  orrs r1, r2
  str r1, [r0]
  
  ldr r0, =0x48000800    /* GPIOC_MODER */
  ldr r1, [r0]
  ldr r2, =0x00003000    /* Clear PC6 mode bits */
  bics r1, r2
  ldr r2, =0x00001000    /* PC6 = Output */
  orrs r1, r2
  str r1, [r0]
  
  ldr r0, =0x48000818    /* GPIOC_BSRR */
  ldr r1, =0x00000040    /* PC6 = 1 */
  str r1, [r0]

/* Copy the data segment initializers from flash to SRAM */
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
  
/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* Call the application's entry point.*/
  bl main

LoopForever:
  b LoopForever

.size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler

/******************************************************************************
*
* The minimal vector table for a Cortex M0.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
   .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  0
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  .word  WWDG_IRQHandler                /* Window WatchDog              */
  .word  PVD_IRQHandler                 /* PVD through EXTI Line detection */
  .word  RTC_IRQHandler                 /* RTC through the EXTI line    */
  .word  FLASH_IRQHandler               /* FLASH                        */
  .word  RCC_IRQHandler                 /* RCC                          */
  .word  EXTI0_1_IRQHandler             /* EXTI Line 0 and 1            */
  .word  EXTI2_3_IRQHandler             /* EXTI Line 2 and 3            */
  .word  EXTI4_15_IRQHandler            /* EXTI Line 4 to 15            */
  .word  0                              /* Reserved                     */
  .word  DMA1_Channel1_IRQHandler       /* DMA1 Channel 1               */
  .word  DMA1_Channel2_3_IRQHandler     /* DMA1 Channel 2 and Channel 3 */
  .word  DMA1_Channel4_5_IRQHandler     /* DMA1 Channel 4 and Channel 5 */
  .word  ADC1_IRQHandler                /* ADC1                         */
  .word  TIM1_BRK_UP_TRG_COM_IRQHandler /* TIM1 Break, Update, Trigger and Commutation */
  .word  TIM1_CC_IRQHandler             /* TIM1 Capture Compare         */
  .word  TIM2_IRQHandler                /* TIM2                         */
  .word  TIM3_IRQHandler                /* TIM3                         */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  TIM14_IRQHandler               /* TIM14                        */
  .word  0                              /* Reserved                     */
  .word  TIM16_IRQHandler               /* TIM16                        */
  .word  TIM17_IRQHandler               /* TIM17                        */
  .word  I2C1_IRQHandler                /* I2C1                         */
  .word  0                              /* Reserved                     */
  .word  SPI1_IRQHandler                /* SPI1                         */
  .word  SPI2_IRQHandler                /* SPI2                         */
  .word  UART1_IRQHandler               /* UART1                        */
  .word  UART2_IRQHandler               /* UART2                        */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  0                              /* Reserved                     */
  .word  TIM10_IRQHandler               /* TIM10                        */
  .word  TIM11_IRQHandler               /* TIM11                        */

  .weak      NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak      HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak      SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak      PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak      SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak      WWDG_IRQHandler
  .thumb_set WWDG_IRQHandler,Default_Handler

  .weak      PVD_IRQHandler
  .thumb_set PVD_IRQHandler,Default_Handler

  .weak      RTC_IRQHandler
  .thumb_set RTC_IRQHandler,Default_Handler

  .weak      FLASH_IRQHandler
  .thumb_set FLASH_IRQHandler,Default_Handler

  .weak      RCC_IRQHandler
  .thumb_set RCC_IRQHandler,Default_Handler

  .weak      EXTI0_1_IRQHandler
  .thumb_set EXTI0_1_IRQHandler,Default_Handler

  .weak      EXTI2_3_IRQHandler
  .thumb_set EXTI2_3_IRQHandler,Default_Handler

  .weak      EXTI4_15_IRQHandler
  .thumb_set EXTI4_15_IRQHandler,Default_Handler

  .weak      DMA1_Channel1_IRQHandler
  .thumb_set DMA1_Channel1_IRQHandler,Default_Handler

  .weak      DMA1_Channel2_3_IRQHandler
  .thumb_set DMA1_Channel2_3_IRQHandler,Default_Handler

  .weak      DMA1_Channel4_5_IRQHandler
  .thumb_set DMA1_Channel4_5_IRQHandler,Default_Handler

  .weak      ADC1_IRQHandler
  .thumb_set ADC1_IRQHandler,Default_Handler

  .weak      TIM1_BRK_UP_TRG_COM_IRQHandler
  .thumb_set TIM1_BRK_UP_TRG_COM_IRQHandler,Default_Handler

  .weak      TIM1_CC_IRQHandler
  .thumb_set TIM1_CC_IRQHandler,Default_Handler

  .weak      TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler,Default_Handler

  .weak      TIM3_IRQHandler
  .thumb_set TIM3_IRQHandler,Default_Handler

  .weak      TIM14_IRQHandler
  .thumb_set TIM14_IRQHandler,Default_Handler

  .weak      TIM16_IRQHandler
  .thumb_set TIM16_IRQHandler,Default_Handler

  .weak      TIM17_IRQHandler
  .thumb_set TIM17_IRQHandler,Default_Handler

  .weak      I2C1_IRQHandler
  .thumb_set I2C1_IRQHandler,Default_Handler

  .weak      SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Default_Handler

  .weak      SPI2_IRQHandler
  .thumb_set SPI2_IRQHandler,Default_Handler

  .weak      UART1_IRQHandler
  .thumb_set UART1_IRQHandler,Default_Handler

  .weak      UART2_IRQHandler
  .thumb_set UART2_IRQHandler,Default_Handler

  .weak      TIM10_IRQHandler
  .thumb_set TIM10_IRQHandler,Default_Handler

  .weak      TIM11_IRQHandler
  .thumb_set TIM11_IRQHandler,Default_Handler