#ifndef __WB32L003_HAL_TIM_H
#define __WB32L003_HAL_TIM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "wb32l003_hal_def.h"

/* TIM State */
typedef enum
{
  HAL_TIM_STATE_RESET             = 0x00U,
  HAL_TIM_STATE_READY             = 0x01U,
  HAL_TIM_STATE_BUSY              = 0x02U,
  HAL_TIM_STATE_TIMEOUT           = 0x03U,
  HAL_TIM_STATE_ERROR             = 0x04U
} HAL_TIM_StateTypeDef;

/* TIM Active Channel */
typedef enum
{
  HAL_TIM_ACTIVE_CHANNEL_1        = 0x01U,
  HAL_TIM_ACTIVE_CHANNEL_2        = 0x02U,
  HAL_TIM_ACTIVE_CHANNEL_3        = 0x04U,
  HAL_TIM_ACTIVE_CHANNEL_4        = 0x08U,
  HAL_TIM_ACTIVE_CHANNEL_CLEARED  = 0x00U
} HAL_TIM_ActiveChannel;

/* Timer Base Configuration Structure definition */
typedef struct
{
  uint32_t Prescaler;
  uint32_t CounterMode;
  uint32_t Period;
  uint32_t ClockDivision;
  uint32_t RepetitionCounter;
  uint32_t AutoReloadPreload;
} TIM_Base_InitTypeDef;

/* Timer handle Structure definition */
typedef struct
{
  void                        *Instance;
  TIM_Base_InitTypeDef        Init;
  HAL_TIM_ActiveChannel       Channel;
  void                        *hdma[7];
  HAL_LockTypeDef             Lock;
  __IO HAL_TIM_StateTypeDef   State;
} TIM_HandleTypeDef;

/* Timer Output Compare Configuration Structure definition */
typedef struct
{
  uint32_t OCMode;
  uint32_t Pulse;
  uint32_t OCPolarity;
  uint32_t OCNPolarity;
  uint32_t OCFastMode;
  uint32_t OCIdleState;
  uint32_t OCNIdleState;
} TIM_OC_InitTypeDef;

/* Timer Input Capture Configuration Structure definition */
typedef struct
{
  uint32_t  ICPolarity;
  uint32_t  ICSelection;
  uint32_t  ICPrescaler;
  uint32_t  ICFilter;
} TIM_IC_InitTypeDef;

/* TIM Counter Mode */
#define TIM_COUNTERMODE_UP                 0x00000000U
#define TIM_COUNTERMODE_DOWN               TIM_CR1_DIR
#define TIM_COUNTERMODE_CENTERALIGNED1    TIM_CR1_CMS_0
#define TIM_COUNTERMODE_CENTERALIGNED2    TIM_CR1_CMS_1
#define TIM_COUNTERMODE_CENTERALIGNED3    TIM_CR1_CMS

/* TIM Clock Division */
#define TIM_CLOCKDIVISION_DIV1             0x00000000U
#define TIM_CLOCKDIVISION_DIV2             TIM_CR1_CKD_0
#define TIM_CLOCKDIVISION_DIV4             TIM_CR1_CKD_1

/* TIM Auto-Reload Preload */
#define TIM_AUTORELOAD_PRELOAD_DISABLE     0x00000000U
#define TIM_AUTORELOAD_PRELOAD_ENABLE      TIM_CR1_ARPE

/* TIM Output Compare Mode */
#define TIM_OCMODE_TIMING                  0x00000000U
#define TIM_OCMODE_ACTIVE                  TIM_CCMR1_OC1M_0
#define TIM_OCMODE_INACTIVE                TIM_CCMR1_OC1M_1
#define TIM_OCMODE_TOGGLE                  (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0)
#define TIM_OCMODE_PWM1                    (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1)
#define TIM_OCMODE_PWM2                    (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0)
#define TIM_OCMODE_FORCED_ACTIVE           (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0)
#define TIM_OCMODE_FORCED_INACTIVE         TIM_CCMR1_OC1M_2

/* TIM Output Compare Polarity */
#define TIM_OCPOLARITY_HIGH                0x00000000U
#define TIM_OCPOLARITY_LOW                 TIM_CCER_CC1P

/* TIM Output Compare Fast Mode */
#define TIM_OCFAST_DISABLE                 0x00000000U
#define TIM_OCFAST_ENABLE                  TIM_CCMR1_OC1FE

/* TIM Channel */
#define TIM_CHANNEL_1                      0x00000000U
#define TIM_CHANNEL_2                      0x00000004U
#define TIM_CHANNEL_3                      0x00000008U
#define TIM_CHANNEL_4                      0x0000000CU
#define TIM_CHANNEL_ALL                    0x00000018U

/* TIM Instances */
#define TIM1                               ((void *)TIM1_BASE)
#define TIM2                               ((void *)TIM2_BASE)
#define TIM3                               ((void *)TIM3_BASE)
#define TIM14                              ((void *)TIM14_BASE)
#define TIM16                              ((void *)TIM16_BASE)
#define TIM17                              ((void *)TIM17_BASE)

/* Exported functions */
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim);

HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Start_DMA(TIM_HandleTypeDef *htim, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_Base_Stop_DMA(TIM_HandleTypeDef *htim);

HAL_StatusTypeDef HAL_TIM_OC_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_OC_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef *htim);

HAL_StatusTypeDef HAL_TIM_OC_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_OC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);

HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_PWM_DeInit(TIM_HandleTypeDef *htim);
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim);

HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_TIM_PWM_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);

HAL_StatusTypeDef HAL_TIM_ConfigOCDelayedPulse(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t Pulse);
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_OC_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel);

void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim);

/* Macros */
#define __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__) \
  (*(__IO uint32_t *)(&(((TIM_TypeDef *)((__HANDLE__)->Instance))->CCR1) + ((__CHANNEL__) >> 2U)) = (__COMPARE__))

#define __HAL_TIM_GET_COMPARE(__HANDLE__, __CHANNEL__) \
  (*(__IO uint32_t *)(&(((TIM_TypeDef *)((__HANDLE__)->Instance))->CCR1) + ((__CHANNEL__) >> 2U)))

#define __HAL_TIM_SET_COUNTER(__HANDLE__, __COUNTER__) \
  (((TIM_TypeDef *)((__HANDLE__)->Instance))->CNT = (__COUNTER__))

#define __HAL_TIM_GET_COUNTER(__HANDLE__) \
  (((TIM_TypeDef *)((__HANDLE__)->Instance))->CNT)

#define __HAL_TIM_SET_AUTORELOAD(__HANDLE__, __AUTORELOAD__) \
  (((TIM_TypeDef *)((__HANDLE__)->Instance))->ARR = (__AUTORELOAD__))

#define __HAL_TIM_GET_AUTORELOAD(__HANDLE__) \
  (((TIM_TypeDef *)((__HANDLE__)->Instance))->ARR)

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_TIM_H */