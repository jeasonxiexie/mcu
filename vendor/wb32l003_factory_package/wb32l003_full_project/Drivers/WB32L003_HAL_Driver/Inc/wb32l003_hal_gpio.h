#ifndef __WB32L003_HAL_GPIO_H
#define __WB32L003_HAL_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "wb32l003_hal_def.h"

/* GPIO Init structure definition */
typedef struct
{
  uint32_t Pin;
  uint32_t Mode;
  uint32_t Pull;
  uint32_t Speed;
  uint32_t Alternate;
} GPIO_InitTypeDef;

/* GPIO Bit SET/RESET enumeration */
typedef enum
{
  GPIO_BIT_RESET = 0U,
  GPIO_BIT_SET
} GPIO_PinState;

/* GPIO Port */
typedef struct
{
  __IO uint32_t MODER;
  __IO uint32_t OTYPER;
  __IO uint32_t OSPEEDR;
  __IO uint32_t PUPDR;
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t LCKR;
  __IO uint32_t AFR[2];
  __IO uint32_t BRR;
} GPIO_TypeDef;

/* GPIO mode */
#define GPIO_MODE_INPUT                 0x00000000U
#define GPIO_MODE_OUTPUT_PP             0x00000001U
#define GPIO_MODE_OUTPUT_OD             0x00000011U
#define GPIO_MODE_AF_PP                 0x00000002U
#define GPIO_MODE_AF_OD                 0x00000012U
#define GPIO_MODE_ANALOG                0x00000003U
#define GPIO_MODE_IT_RISING             0x10110000U
#define GPIO_MODE_IT_FALLING            0x10210000U
#define GPIO_MODE_IT_RISING_FALLING     0x10310000U
#define GPIO_MODE_EVT_RISING            0x10120000U
#define GPIO_MODE_EVT_FALLING           0x10220000U
#define GPIO_MODE_EVT_RISING_FALLING    0x10320000U

/* GPIO output speed */
#define GPIO_SPEED_FREQ_LOW             0x00000000U
#define GPIO_SPEED_FREQ_MEDIUM          0x00000001U
#define GPIO_SPEED_FREQ_HIGH            0x00000003U

/* GPIO pull-up/pull-down */
#define GPIO_NOPULL                     0x00000000U
#define GPIO_PULLUP                     0x00000001U
#define GPIO_PULLDOWN                   0x00000002U

/* GPIO alternate functions */
#define GPIO_AF0_EVENTOUT               0x00000000U
#define GPIO_AF0_SPI1                   0x00000000U
#define GPIO_AF0_USART1                 0x00000000U
#define GPIO_AF1_I2C1                   0x00000001U
#define GPIO_AF1_USART1                 0x00000001U
#define GPIO_AF2_TIM1                   0x00000002U
#define GPIO_AF2_TIM2                   0x00000002U
#define GPIO_AF2_TIM14                  0x00000002U
#define GPIO_AF3_EVENTOUT               0x00000003U
#define GPIO_AF3_I2C1                   0x00000003U
#define GPIO_AF4_TIM14                  0x00000004U
#define GPIO_AF4_USART2                 0x00000004U

/* GPIO instances */
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)

/* Exported macro */
#define __HAL_GPIO_EXTI_GET_FLAG(__EXTI_LINE__)       (EXTI->PR & (__EXTI_LINE__))
#define __HAL_GPIO_EXTI_CLEAR_FLAG(__EXTI_LINE__)     (EXTI->PR = (__EXTI_LINE__))
#define __HAL_GPIO_EXTI_GET_IT(__EXTI_LINE__)         (EXTI->PR & (__EXTI_LINE__))
#define __HAL_GPIO_EXTI_CLEAR_IT(__EXTI_LINE__)       (EXTI->PR = (__EXTI_LINE__))

/* Exported functions */
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);
void HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_GPIO_H */