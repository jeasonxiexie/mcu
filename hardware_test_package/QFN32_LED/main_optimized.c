/* QFN32 LED Test - 优化版 */
#include "wb32l003.h"

/* Private variables */
BaseTim_InitTypeDef BaseTim_InitStructure;

/* Private function prototypes */
void Tim1_Config(void);
void GPIO_Config(void);

int main(void)
{
  SystemCoreClockUpdate();
  
  /* 系统关键：PC6输出100kHz PWM */
  Tim1_Config();
  
  /* GPIO配置：电源控制和LED */
  GPIO_Config();
  
  /* TIM10定时器配置 */
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
  BaseTim_InitStructure.BaseTim_Prescaler = BaseTim_Prescaler_DIV256;  /* 增加分频，降低闪烁频率 */
  BaseTim_InitStructure.BaseTim_BGLoad = 23437;  /* 约500ms @ 12MHz/256 */
  BaseTim_Init(TIM10, &BaseTim_InitStructure);

  BaseTim_ITConfig(TIM10, ENABLE);
  BaseTim_Cmd(TIM10, ENABLE);

  while (1)
  {
    /* 主循环 */
  }
}

/* GPIO配置 - 集中管理所有GPIO初始化 */
void GPIO_Config(void)
{
  /* 使能所有需要的GPIO时钟 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);
  
  /* PA3 - LCD电源 (Pin 7) */
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_MODE_OUT | GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOA, GPIO_Pin_3);
  
  /* PC5 - RF电源 (Pin 22) */
  GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_MODE_OUT | GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOC, GPIO_Pin_5);
  
  /* PC0 - 红色LED (Pin 17, 低电平有效) */
  GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOC, GPIO_Pin_0);
  
  /* PC1 - 绿色LED (Pin 18, 低电平有效) */
  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOC, GPIO_Pin_1);
  
  /* PD4 - TIM10外部输入 */
  GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH | GPIO_AF6);
}

/* TIM1配置 - PC6输出100kHz PWM */
void Tim1_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* PC6配置为TIM1_CH1 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH | GPIO_AF1);

  /* TIM1时钟使能 */
  RCC_APBPeriphClockCmd(RCC_APBPeriph_TIM1, ENABLE);
  
  /* TIM1配置：100kHz PWM */
  TIM_ARRPreloadConfig(TIM1, DISABLE);
  TIM_TimeBaseStructure.TIM_Period = 9;        /* ARR = 10-1 */
  TIM_TimeBaseStructure.TIM_Prescaler = 11;    /* PSC = 12-1 */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM1, ENABLE);

  /* PWM模式配置 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_Pulse = 5;           /* 50%占空比 */
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

/* TIM10中断处理 - LED闪烁 */
void TIM10_IRQHandler(void)
{
  static uint32_t toggle = 0;
  
  if(BaseTim_GetFlagStatus(TIM10) != RESET)
  {
    BaseTim_ClearFlag(TIM10);
    
    toggle = !toggle;
    if(toggle)
    {
      GPIO_ResetBits(GPIOC, GPIO_Pin_0);  /* 红LED亮 */
      GPIO_SetBits(GPIOC, GPIO_Pin_1);    /* 绿LED灭 */
    }
    else
    {
      GPIO_SetBits(GPIOC, GPIO_Pin_0);    /* 红LED灭 */
      GPIO_ResetBits(GPIOC, GPIO_Pin_1);  /* 绿LED亮 */
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