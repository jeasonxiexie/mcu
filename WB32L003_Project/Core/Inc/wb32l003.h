#ifndef __WB32L003_H
#define __WB32L003_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Configuration of the Cortex-M0+ Processor and Core Peripherals */
#define __CM0PLUS_REV             0x0001U
#define __MPU_PRESENT             0U
#define __VTOR_PRESENT            1U
#define __NVIC_PRIO_BITS          2U
#define __Vendor_SysTickConfig    0U

/* WB32L003 Interrupt Number Definition */
typedef enum
{
  NonMaskableInt_IRQn         = -14,
  HardFault_IRQn              = -13,
  SVC_IRQn                    = -5,
  PendSV_IRQn                 = -2,
  SysTick_IRQn                = -1,
  
  WWDG_IRQn                   = 0,
  PVD_IRQn                    = 1,
  RTC_IRQn                    = 2,
  FLASH_IRQn                  = 3,
  RCC_IRQn                    = 4,
  EXTI0_1_IRQn                = 5,
  EXTI2_3_IRQn                = 6,
  EXTI4_15_IRQn               = 7,
  DMA1_Channel1_IRQn          = 9,
  DMA1_Channel2_3_IRQn        = 10,
  DMA1_Channel4_5_IRQn        = 11,
  ADC1_IRQn                   = 12,
  TIM1_BRK_UP_TRG_COM_IRQn    = 13,
  TIM1_CC_IRQn                = 14,
  TIM2_IRQn                   = 15,
  TIM3_IRQn                   = 16,
  TIM14_IRQn                  = 19,
  TIM16_IRQn                  = 21,
  TIM17_IRQn                  = 22,
  I2C1_IRQn                   = 23,
  SPI1_IRQn                   = 25,
  USART1_IRQn                 = 27,
  USART2_IRQn                 = 28
} IRQn_Type;

/* Includes */
#include <stdint.h>

/* CMSIS */
#include "core_cm0plus.h"

/* System Clock Frequency */
extern uint32_t SystemCoreClock;

/* Peripheral memory map */
#define FLASH_BASE            0x08000000UL
#define SRAM_BASE             0x20000000UL
#define PERIPH_BASE           0x40000000UL

#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000UL)

/* APB1 peripherals */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x00000000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x00000400UL)
#define TIM14_BASE            (APB1PERIPH_BASE + 0x00002000UL)
#define RTC_BASE              (APB1PERIPH_BASE + 0x00002800UL)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x00002C00UL)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x00003000UL)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x00005400UL)
#define PWR_BASE              (APB1PERIPH_BASE + 0x00007000UL)

/* APB2 peripherals */
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x00000000UL)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x00000400UL)
#define USART1_BASE           (APB2PERIPH_BASE + 0x00003800UL)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x00002400UL)
#define ADC_BASE              (APB2PERIPH_BASE + 0x00002400UL)
#define TIM1_BASE             (APB2PERIPH_BASE + 0x00002C00UL)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x00003000UL)
#define TIM16_BASE            (APB2PERIPH_BASE + 0x00004400UL)
#define TIM17_BASE            (APB2PERIPH_BASE + 0x00004800UL)
#define DBGMCU_BASE           (APB2PERIPH_BASE + 0x00005800UL)

/* AHB1 peripherals */
#define DMA1_BASE             (AHB1PERIPH_BASE + 0x00000000UL)
#define DMA1_Channel1_BASE    (DMA1_BASE + 0x00000008UL)
#define DMA1_Channel2_BASE    (DMA1_BASE + 0x0000001CUL)
#define DMA1_Channel3_BASE    (DMA1_BASE + 0x00000030UL)
#define DMA1_Channel4_BASE    (DMA1_BASE + 0x00000044UL)
#define DMA1_Channel5_BASE    (DMA1_BASE + 0x00000058UL)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x00001000UL)
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x00002000UL)
#define CRC_BASE              (AHB1PERIPH_BASE + 0x00003000UL)

/* AHB2 peripherals */
#define GPIOA_BASE            (AHB2PERIPH_BASE + 0x00000000UL)
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x00000400UL)
#define GPIOC_BASE            (AHB2PERIPH_BASE + 0x00000800UL)
#define GPIOD_BASE            (AHB2PERIPH_BASE + 0x00000C00UL)
#define GPIOF_BASE            (AHB2PERIPH_BASE + 0x00001400UL)

/* GPIO Definitions */
#define GPIO_PIN_0                 ((uint16_t)0x0001U)
#define GPIO_PIN_1                 ((uint16_t)0x0002U)
#define GPIO_PIN_2                 ((uint16_t)0x0004U)
#define GPIO_PIN_3                 ((uint16_t)0x0008U)
#define GPIO_PIN_4                 ((uint16_t)0x0010U)
#define GPIO_PIN_5                 ((uint16_t)0x0020U)
#define GPIO_PIN_6                 ((uint16_t)0x0040U)
#define GPIO_PIN_7                 ((uint16_t)0x0080U)
#define GPIO_PIN_8                 ((uint16_t)0x0100U)
#define GPIO_PIN_9                 ((uint16_t)0x0200U)
#define GPIO_PIN_10                ((uint16_t)0x0400U)
#define GPIO_PIN_11                ((uint16_t)0x0800U)
#define GPIO_PIN_12                ((uint16_t)0x1000U)
#define GPIO_PIN_13                ((uint16_t)0x2000U)
#define GPIO_PIN_14                ((uint16_t)0x4000U)
#define GPIO_PIN_15                ((uint16_t)0x8000U)
#define GPIO_PIN_All               ((uint16_t)0xFFFFU)

#define GPIO_PIN_RESET             0x00U
#define GPIO_PIN_SET               0x01U

/* Include HAL headers - order is important */
#include "wb32l003_hal_def.h"
#include "wb32l003_hal_def_ex.h"
#include "wb32l003_hal_conf.h"
#include "wb32l003_hal.h"

/* Exported functions */
void SystemInit(void);
void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_H */