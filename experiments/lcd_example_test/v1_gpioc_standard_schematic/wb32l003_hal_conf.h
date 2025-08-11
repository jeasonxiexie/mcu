#ifndef __WB32L003_HAL_CONF_H
#define __WB32L003_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Module Selection */
#define HAL_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED

/* Oscillator Values */
#define HSE_VALUE              8000000U
#define HSE_STARTUP_TIMEOUT    100U
#define HSI_VALUE              8000000U
#define HSI_STARTUP_TIMEOUT    5000U
#define LSI_VALUE              40000U
#define LSE_VALUE              32768U
#define LSE_STARTUP_TIMEOUT    5000U

/* System Configuration */
#define VDD_VALUE              3300U
#define TICK_INT_PRIORITY      0U
#define USE_RTOS               0U
#define PREFETCH_ENABLE        1U

/* Assert Selection */
#define USE_FULL_ASSERT        0U

/* Includes */
#include "hal/wb32l003_hal_rcc.h"
#include "hal/wb32l003_hal_gpio.h"
#include "hal/wb32l003_hal_dma.h"
#include "hal/wb32l003_hal_cortex.h"
#include "hal/wb32l003_hal_adc.h"
#include "hal/wb32l003_hal_flash.h"
#include "hal/wb32l003_hal_pwr.h"
#include "hal/wb32l003_hal_spi.h"
#include "hal/wb32l003_hal_tim.h"

/* Exported macro */
#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_CONF_H */