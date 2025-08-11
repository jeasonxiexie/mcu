/* Minimal modification of TIM10_Count - Only change PC4 to PC0 */
#include "wb32l003.h"

/* Private variables */
BaseTim_InitTypeDef BaseTim_InitStructure;

/* Private function prototypes */
void Tim1_Config(void);

void delay(uint32_t ticks)
{
    while (ticks--);
}

int main(void)
{
    SystemCoreClockUpdate();

    Tim1_Config();  /* Keep PC6 100kHz PWM exactly as original */

    /* Change PC4 to PC0 for QFN32 - RED LED */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_MODE_OUT | GPIO_PUPD_NOPULL | GPIO_OTYPE_PP | GPIO_SPEED_HIGH);
    
    /* Keep PD4 as original */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_PUPD_UP | GPIO_OTYPE_PP | GPIO_SPEED_HIGH | GPIO_AF6);

    NVIC_SetPriority(TIM10_IRQn, 1);
    NVIC_EnableIRQ(TIM10_IRQn);

    RCC_APBPeriphClockCmd(RCC_APBPeriph_BASETIM, ENABLE);
    BaseTim_DeInit(TIM10);

    /* Keep all TIM10 settings exactly as original */
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

/* Keep Tim1_Config exactly as original */
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

/* TIM10 interrupt - toggle PC0 instead of PC4 */
void TIM10_IRQHandler(void)
{
    if(BaseTim_GetFlagStatus(TIM10) != RESET)
    {
        BaseTim_ClearFlag(TIM10);
        GPIO_ToggleBits(GPIOC, GPIO_Pin_0);  /* Toggle PC0 (RED LED) */
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