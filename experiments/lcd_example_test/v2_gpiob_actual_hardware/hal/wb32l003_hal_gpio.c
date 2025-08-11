#include "wb32l003_hal.h"

void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
  uint32_t position = 0x00U;
  uint32_t iocurrent = 0x00U;
  uint32_t temp = 0x00U;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
  assert_param(IS_GPIO_MODE(GPIO_Init->Mode));
  assert_param(IS_GPIO_PULL(GPIO_Init->Pull));

  /* Configure the port pins */
  while (((GPIO_Init->Pin) >> position) != 0x00U)
  {
    /* Get current io position */
    iocurrent = (GPIO_Init->Pin) & (0x00000001U << position);

    if (iocurrent != 0x00U)
    {
      /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
      temp = GPIOx->MODER;
      temp &= ~(GPIO_MODER_MODER0 << (position * 2U));
      temp |= ((GPIO_Init->Mode & 0x00000003U) << (position * 2U));
      GPIOx->MODER = temp;

      /* Configure the Output type */
      if ((GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP) || (GPIO_Init->Mode == GPIO_MODE_AF_PP))
      {
        temp = GPIOx->OTYPER;
        temp &= ~(GPIO_OTYPER_OT_0 << position);
        GPIOx->OTYPER = temp;
      }
      else if ((GPIO_Init->Mode == GPIO_MODE_OUTPUT_OD) || (GPIO_Init->Mode == GPIO_MODE_AF_OD))
      {
        temp = GPIOx->OTYPER;
        temp |= (GPIO_OTYPER_OT_0 << position);
        GPIOx->OTYPER = temp;
      }

      /* Configure the IO Speed */
      temp = GPIOx->OSPEEDR;
      temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2U));
      temp |= (GPIO_Init->Speed << (position * 2U));
      GPIOx->OSPEEDR = temp;

      /* Configure the IO Pull-up/Pull-down resistor */
      temp = GPIOx->PUPDR;
      temp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2U));
      temp |= ((GPIO_Init->Pull) << (position * 2U));
      GPIOx->PUPDR = temp;

      /* Configure Alternate function */
      if ((GPIO_Init->Mode == GPIO_MODE_AF_PP) || (GPIO_Init->Mode == GPIO_MODE_AF_OD))
      {
        temp = GPIOx->AFR[position >> 3U];
        temp &= ~(0xFU << ((position & 0x07U) * 4U));
        temp |= ((GPIO_Init->Alternate) << ((position & 0x07U) * 4U));
        GPIOx->AFR[position >> 3U] = temp;
      }
    }
    position++;
  }
}

void HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  uint32_t position = 0x00U;
  uint32_t iocurrent = 0x00U;
  uint32_t tmp = 0x00U;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  /* Configure the port pins */
  while ((GPIO_Pin >> position) != 0x00U)
  {
    /* Get current io position */
    iocurrent = GPIO_Pin & (0x00000001U << position);

    if (iocurrent != 0x00U)
    {
      /* Configure the port mode register */
      tmp = GPIOx->MODER;
      tmp &= ~(GPIO_MODER_MODER0 << (position * 2U));
      GPIOx->MODER = tmp;

      /* Configure the port output type register */
      tmp = GPIOx->OTYPER;
      tmp &= ~(GPIO_OTYPER_OT_0 << position);
      GPIOx->OTYPER = tmp;

      /* Configure the port output speed register */
      tmp = GPIOx->OSPEEDR;
      tmp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2U));
      GPIOx->OSPEEDR = tmp;

      /* Configure the port pull-up/pull-down register */
      tmp = GPIOx->PUPDR;
      tmp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2U));
      GPIOx->PUPDR = tmp;

      /* Configure the port alternate function register */
      tmp = GPIOx->AFR[position >> 3U];
      tmp &= ~(0xFU << ((position & 0x07U) * 4U));
      GPIOx->AFR[position >> 3U] = tmp;
    }
    position++;
  }
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  GPIO_PinState bitstatus;

  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  if ((GPIOx->IDR & GPIO_Pin) != 0x00U)
  {
    bitstatus = GPIO_PIN_SET;
  }
  else
  {
    bitstatus = GPIO_PIN_RESET;
  }
  return bitstatus;
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_PIN_ACTION(PinState));

  if (PinState != GPIO_PIN_RESET)
  {
    GPIOx->BSRR = (uint32_t)GPIO_Pin;
  }
  else
  {
    GPIOx->BRR = (uint32_t)GPIO_Pin;
  }
}

void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  GPIOx->ODR ^= GPIO_Pin;
}