/* QFN32 LED Test - 基于验证工作的TIM10_Count最小修改版 */
#include "wb32l003.h"

/* Private variables */
BaseTim_InitTypeDef BaseTim_InitStructure;

/* TIM1配置 - PC6输出100kHz PWM（保持不变，系统关键信号！） */
void Tim1_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_PUPD_UP| GPIO_OTYPE_PP| GPIO_SPEED_HIGH| GPIO_AF1);

  RCC_APBPeriphClockCmd(RCC_APBPeriph_TIM1, ENABLE);
  
  TIM_ARRPreloadConfig(TIM1, DISABLE);
  /* TIM1 100K PWM */
  TIM_TimeBaseStructure.TIM_Period = (10 - 1);
  TIM_TimeBaseStructure.TIM_Prescaler = (12 - 1);
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM1, ENABLE);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_Pulse = 5;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

  TIM_CCPreloadControl(TIM1, ENABLE);
  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

int main(void)
{
  SystemCoreClockUpdate();

  /* 保持PC6的100kHz PWM - 这是系统工作的关键！ */
  Tim1_Config();

  /* QFN32电源控制 */
  /* PA3 HIGH - LCD Power */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_MODE_OUT | GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOA, GPIO_Pin_3);
  
  /* PC5 HIGH - RF Power */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_MODE_OUT | GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOC, GPIO_Pin_5);
  
  /* QFN32 LED控制 */
  /* PC0 - RED LED */
  GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOC, GPIO_Pin_0);  /* OFF */
  
  /* PC1 - GREEN LED */
  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOC, GPIO_Pin_1);  /* OFF */
  
  /* PD4保持原样 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH | GPIO_AF6);

  /* TIM10配置 */
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
  BaseTim_InitStructure.BaseTim_Prescaler = BaseTim_Prescaler_DIV1;
  BaseTim_InitStructure.BaseTim_BGLoad = 65435;
  BaseTim_Init(TIM10, &BaseTim_InitStructure);

  BaseTim_ITConfig(TIM10, ENABLE);
  BaseTim_Cmd(TIM10, ENABLE);

  while (1)
  {
  }
}

/* TIM10中断处理 - QFN32 LED闪烁 */
void TIM10_IRQHandler(void)
{
  static uint32_t toggle = 0;
  
  if(BaseTim_GetFlagStatus(TIM10) != RESET)
  {
    BaseTim_ClearFlag(TIM10);
    
    /* 红绿LED交替闪烁 */
    toggle = !toggle;
    if(toggle)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  while (1)
  {
  }
}
#endif