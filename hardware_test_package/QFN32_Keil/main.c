/* Includes ------------------------------------------------------------------*/
#include "wb32l003.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
BaseTim_InitTypeDef BaseTim_InitStructure;
uint32_t led_toggle_count = 0;

/* Private function prototypes -----------------------------------------------*/
void Tim1_Config(void);

/* Private functions ---------------------------------------------------------*/

void delay(uint32_t ticks)
{
  while (ticks--)
    ;
}

int main(void)
{
  SystemCoreClockUpdate();

  /* Power Control Init - QFN32 Pins */
  /* PC6 HIGH (Pin 23) - CPU Power */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_MODE_OUT | GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOC, GPIO_Pin_6);
  
  /* PA3 HIGH (Pin 7) - LCD Power */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_MODE_OUT | GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOA, GPIO_Pin_3);
  
  /* PC5 HIGH (Pin 22) - RF Power */
  GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_MODE_OUT | GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOC, GPIO_Pin_5);
  
  /* LED Init - QFN32 Pins */
  /* PC0 (Pin 17) - RED LED, Low Active */
  GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOC, GPIO_Pin_0); /* Initial OFF */
  
  /* PC1 (Pin 18) - GREEN LED, Low Active */
  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOC, GPIO_Pin_1); /* Initial OFF */

  NVIC_SetPriority(TIM10_IRQn, 1);
  NVIC_EnableIRQ(TIM10_IRQn);

  RCC_APBPeriphClockCmd(RCC_APBPeriph_BASETIM, ENABLE);
  BaseTim_DeInit(TIM10);

  BaseTim_InitStructure.BaseTim_Gate = BaseTim_Gate_Disable;
  BaseTim_InitStructure.BaseTim_GatePolarity = BaseTim_GatePolarity_High;
  BaseTim_InitStructure.BaseTim_Tog = BaseTim_Tog_Disable;
  BaseTim_InitStructure.BaseTim_Function = BaseTim_Function_Count;
  BaseTim_InitStructure.BaseTim_AutoReload = BaseTim_AutoReload_Enable;
  BaseTim_InitStructure.BaseTim_CountLevel = BaseTim_CountLevel_16BIT;
  BaseTim_InitStructure.BaseTim_CountMode = BaseTim_CountMode_Repeat;
  BaseTim_InitStructure.BaseTim_Prescaler = BaseTim_Prescaler_DIV256;
  BaseTim_InitStructure.BaseTim_BGLoad = 23437; /* ~500ms @ 12MHz/256 */
  BaseTim_Init(TIM10, &BaseTim_InitStructure);

  BaseTim_ITConfig(TIM10, ENABLE);
  BaseTim_Cmd(TIM10, ENABLE);

  /* Infinite loop */
  while (1)
  {
  }
}

/* 
 * Not needed - PC6 used for power control
 */
void Tim1_Config(void)
{
  /* Keep empty */
}

/**
  * @brief This function handles TIM10 Interrupt .
  */
void TIM10_IRQHandler(void)
{
  if(BaseTim_GetFlagStatus(TIM10) != RESET)
  {
    BaseTim_ClearFlag(TIM10);
    
    /* LED Toggle - QFN32 Pins */
    led_toggle_count++;
    if(led_toggle_count & 1)
    {
      GPIO_ResetBits(GPIOC, GPIO_Pin_0);  /* RED ON */
      GPIO_SetBits(GPIOC, GPIO_Pin_1);    /* GREEN OFF */
    }
    else
    {
      GPIO_SetBits(GPIOC, GPIO_Pin_0);    /* RED OFF */
      GPIO_ResetBits(GPIOC, GPIO_Pin_1);  /* GREEN ON */
    }
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
