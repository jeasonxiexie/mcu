#ifndef __WB32L003_HAL_RCC_H
#define __WB32L003_HAL_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "wb32l003_hal_def.h"

/* RCC PLL Configuration Structure */
typedef struct
{
  uint32_t PLLState;
  uint32_t PLLSource;
  uint32_t PLLMUL;
} RCC_PLLInitTypeDef;

/* RCC Oscillator Configuration Structure */
typedef struct
{
  uint32_t OscillatorType;
  uint32_t HSEState;
  uint32_t LSEState;
  uint32_t HSIState;
  uint32_t HSICalibrationValue;
  uint32_t LSIState;
  RCC_PLLInitTypeDef PLL;
} RCC_OscInitTypeDef;

/* RCC System, AHB and APB busses clock configuration structure */
typedef struct
{
  uint32_t ClockType;
  uint32_t SYSCLKSource;
  uint32_t AHBCLKDivider;
  uint32_t APB1CLKDivider;
  uint32_t APB2CLKDivider;
} RCC_ClkInitTypeDef;

/* RCC Oscillator Type */
#define RCC_OSCILLATORTYPE_NONE        0x00000000U
#define RCC_OSCILLATORTYPE_HSE         0x00000001U
#define RCC_OSCILLATORTYPE_HSI         0x00000002U
#define RCC_OSCILLATORTYPE_LSE         0x00000004U
#define RCC_OSCILLATORTYPE_LSI         0x00000008U

/* HSE State */
#define RCC_HSE_OFF                    0x00000000U
#define RCC_HSE_ON                     0x00000001U
#define RCC_HSE_BYPASS                 0x00000002U

/* HSI State */
#define RCC_HSI_OFF                    0x00000000U
#define RCC_HSI_ON                     0x00000001U

/* HSI Calibration */
#define RCC_HSICALIBRATION_DEFAULT     0x10U

/* PLL State */
#define RCC_PLL_NONE                   0x00000000U
#define RCC_PLL_OFF                    0x00000001U
#define RCC_PLL_ON                     0x00000002U

/* PLL Source */
#define RCC_PLLSOURCE_HSI              0x00000000U
#define RCC_PLLSOURCE_HSE              0x00000001U

/* PLL Multiplication Factor */
#define RCC_PLL_MUL2                   0x00000000U
#define RCC_PLL_MUL3                   0x00000001U
#define RCC_PLL_MUL4                   0x00000002U
#define RCC_PLL_MUL5                   0x00000003U
#define RCC_PLL_MUL6                   0x00000004U
#define RCC_PLL_MUL7                   0x00000005U
#define RCC_PLL_MUL8                   0x00000006U
#define RCC_PLL_MUL9                   0x00000007U
#define RCC_PLL_MUL10                  0x00000008U
#define RCC_PLL_MUL11                  0x00000009U
#define RCC_PLL_MUL12                  0x0000000AU
#define RCC_PLL_MUL13                  0x0000000BU
#define RCC_PLL_MUL14                  0x0000000CU
#define RCC_PLL_MUL15                  0x0000000DU
#define RCC_PLL_MUL16                  0x0000000EU

/* System Clock Type */
#define RCC_CLOCKTYPE_SYSCLK           0x00000001U
#define RCC_CLOCKTYPE_HCLK             0x00000002U
#define RCC_CLOCKTYPE_PCLK1            0x00000004U

/* System Clock Source */
#define RCC_SYSCLKSOURCE_HSI           0x00000000U
#define RCC_SYSCLKSOURCE_HSE           0x00000001U
#define RCC_SYSCLKSOURCE_PLLCLK        0x00000002U

/* AHB Clock Divider */
#define RCC_SYSCLK_DIV1                0x00000000U
#define RCC_SYSCLK_DIV2                0x00000080U
#define RCC_SYSCLK_DIV4                0x00000090U
#define RCC_SYSCLK_DIV8                0x000000A0U
#define RCC_SYSCLK_DIV16               0x000000B0U
#define RCC_SYSCLK_DIV64               0x000000C0U
#define RCC_SYSCLK_DIV128              0x000000D0U
#define RCC_SYSCLK_DIV256              0x000000E0U
#define RCC_SYSCLK_DIV512              0x000000F0U

/* APB Clock Divider */
#define RCC_HCLK_DIV1                  0x00000000U
#define RCC_HCLK_DIV2                  0x00000400U
#define RCC_HCLK_DIV4                  0x00000500U
#define RCC_HCLK_DIV8                  0x00000600U
#define RCC_HCLK_DIV16                 0x00000700U

/* Flash Latency */
#define FLASH_LATENCY_0                0x00000000U
#define FLASH_LATENCY_1                0x00000001U

/* RCC Peripheral Clock Enable/Disable */
#define __HAL_RCC_GPIOA_CLK_ENABLE()   do { volatile uint32_t tmpreg; \
                                        SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN); \
                                        tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN); \
                                        UNUSED(tmpreg); } while(0U)

#define __HAL_RCC_GPIOB_CLK_ENABLE()   do { volatile uint32_t tmpreg; \
                                        SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOBEN); \
                                        tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_GPIOBEN); \
                                        UNUSED(tmpreg); } while(0U)

#define __HAL_RCC_GPIOC_CLK_ENABLE()   do { volatile uint32_t tmpreg; \
                                        SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOCEN); \
                                        tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_GPIOCEN); \
                                        UNUSED(tmpreg); } while(0U)

#define __HAL_RCC_SPI1_CLK_ENABLE()    do { volatile uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN); \
                                        UNUSED(tmpreg); } while(0U)

#define __HAL_RCC_ADC_CLK_ENABLE()     do { volatile uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADCEN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADCEN); \
                                        UNUSED(tmpreg); } while(0U)

#define __HAL_RCC_TIM2_CLK_ENABLE()    do { volatile uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN); \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN); \
                                        UNUSED(tmpreg); } while(0U)

#define __HAL_RCC_PWR_CLK_ENABLE()     do { volatile uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN); \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN); \
                                        UNUSED(tmpreg); } while(0U)

/* Exported functions */
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t FLatency);

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_RCC_H */