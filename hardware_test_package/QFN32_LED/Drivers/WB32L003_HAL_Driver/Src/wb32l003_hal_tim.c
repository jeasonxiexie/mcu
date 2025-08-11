#include "wb32l003_hal.h"

/* Private function prototypes */
static void TIM_OC1_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config);
static void TIM_OC2_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config);
static void TIM_CCxChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelState);
static void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Structure);

/* Missing macro */
#define IS_TIM_CC1_INSTANCE(INSTANCE) ((INSTANCE) == TIM2)

/* TIM Registers Bits Definition */
#define TIM_CR1_CEN_Pos           (0U)
#define TIM_CR1_CEN_Msk           (0x1UL << TIM_CR1_CEN_Pos)
#define TIM_CR1_CEN               TIM_CR1_CEN_Msk
#define TIM_CR1_DIR_Pos           (4U)
#define TIM_CR1_DIR_Msk           (0x1UL << TIM_CR1_DIR_Pos)
#define TIM_CR1_DIR               TIM_CR1_DIR_Msk
#define TIM_CR1_CMS_Pos           (5U)
#define TIM_CR1_CMS_Msk           (0x3UL << TIM_CR1_CMS_Pos)
#define TIM_CR1_CMS               TIM_CR1_CMS_Msk
#define TIM_CR1_CMS_0             (0x1UL << TIM_CR1_CMS_Pos)
#define TIM_CR1_CMS_1             (0x2UL << TIM_CR1_CMS_Pos)
#define TIM_CR1_ARPE_Pos          (7U)
#define TIM_CR1_ARPE_Msk          (0x1UL << TIM_CR1_ARPE_Pos)
#define TIM_CR1_ARPE              TIM_CR1_ARPE_Msk
#define TIM_CR1_CKD_Pos           (8U)
#define TIM_CR1_CKD_Msk           (0x3UL << TIM_CR1_CKD_Pos)
#define TIM_CR1_CKD               TIM_CR1_CKD_Msk
#define TIM_CR1_CKD_0             (0x1UL << TIM_CR1_CKD_Pos)
#define TIM_CR1_CKD_1             (0x2UL << TIM_CR1_CKD_Pos)

#define TIM_CCMR1_OC1M_Pos        (4U)
#define TIM_CCMR1_OC1M_Msk        (0x7UL << TIM_CCMR1_OC1M_Pos)
#define TIM_CCMR1_OC1M            TIM_CCMR1_OC1M_Msk
#define TIM_CCMR1_OC1M_0          (0x1UL << TIM_CCMR1_OC1M_Pos)
#define TIM_CCMR1_OC1M_1          (0x2UL << TIM_CCMR1_OC1M_Pos)
#define TIM_CCMR1_OC1M_2          (0x4UL << TIM_CCMR1_OC1M_Pos)
#define TIM_CCMR1_OC1FE_Pos       (2U)
#define TIM_CCMR1_OC1FE_Msk       (0x1UL << TIM_CCMR1_OC1FE_Pos)
#define TIM_CCMR1_OC1FE           TIM_CCMR1_OC1FE_Msk

#define TIM_CCER_CC1E_Pos         (0U)
#define TIM_CCER_CC1E_Msk         (0x1UL << TIM_CCER_CC1E_Pos)
#define TIM_CCER_CC1E             TIM_CCER_CC1E_Msk
#define TIM_CCER_CC1P_Pos         (1U)
#define TIM_CCER_CC1P_Msk         (0x1UL << TIM_CCER_CC1P_Pos)
#define TIM_CCER_CC1P             TIM_CCER_CC1P_Msk

TIM_HandleTypeDef htim2;

HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim)
{
  /* Check the TIM handle allocation */
  if (htim == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));

  if (htim->State == HAL_TIM_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    htim->Lock = HAL_UNLOCKED;
    
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    HAL_TIM_Base_MspInit(htim);
  }

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;

  /* Set the Time Base configuration */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* Initialize the TIM state*/
  htim->State = HAL_TIM_STATE_READY;

  return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim)
{
  /* Check the TIM handle allocation */
  if (htim == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));

  if (htim->State == HAL_TIM_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    htim->Lock = HAL_UNLOCKED;

    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_PWM_MspInit(htim);
  }

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;

  /* Init the base time for the PWM */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* Initialize the TIM state*/
  htim->State = HAL_TIM_STATE_READY;

  return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel)
{
  TIM_TypeDef *TIMx = (TIM_TypeDef *)htim->Instance;
  
  /* Check the parameters */
  assert_param(IS_TIM_CHANNELS(Channel));
  assert_param(IS_TIM_PWM_MODE(sConfig->OCMode));
  assert_param(IS_TIM_OC_POLARITY(sConfig->OCPolarity));
  assert_param(IS_TIM_FAST_STATE(sConfig->OCFastMode));

  /* Process Locked */
  __HAL_LOCK(htim);

  htim->State = HAL_TIM_STATE_BUSY;

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Configure the Channel 1 in PWM mode */
      TIM_OC1_SetConfig(htim->Instance, sConfig);

      /* Set the Preload enable bit for channel1 */
      TIMx->CCMR1 |= TIM_CCMR1_OC1PE;

      /* Configure the Output Fast mode */
      TIMx->CCMR1 &= ~TIM_CCMR1_OC1FE;
      TIMx->CCMR1 |= sConfig->OCFastMode;
      break;
    }

    case TIM_CHANNEL_2:
    {
      /* Configure the Channel 2 in PWM mode */
      TIM_OC2_SetConfig(htim->Instance, sConfig);

      /* Set the Preload enable bit for channel2 */
      TIMx->CCMR1 |= TIM_CCMR1_OC2PE;

      /* Configure the Output Fast mode */
      TIMx->CCMR1 &= ~TIM_CCMR1_OC2FE;
      TIMx->CCMR1 |= sConfig->OCFastMode << 8U;
      break;
    }

    default:
      break;
  }

  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  TIM_TypeDef *TIMx = (TIM_TypeDef *)htim->Instance;
  
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* Enable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

static void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Structure)
{
  uint32_t tmpcr1;
  tmpcr1 = TIMx->CR1;

  /* Set TIM Time Base Unit parameters ---------------------------------------*/
  if (IS_TIM_COUNTER_MODE_SELECT_INSTANCE(TIMx))
  {
    /* Select the Counter Mode */
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= Structure->CounterMode;
  }

  if (IS_TIM_CLOCK_DIVISION_INSTANCE(TIMx))
  {
    /* Set the clock division */
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)Structure->ClockDivision;
  }

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);

  TIMx->CR1 = tmpcr1;

  /* Set the Autoreload value */
  TIMx->ARR = (uint32_t)Structure->Period ;

  /* Set the Prescaler value */
  TIMx->PSC = Structure->Prescaler;

  if (IS_TIM_REPETITION_COUNTER_INSTANCE(TIMx))
  {
    /* Set the Repetition Counter value */
    TIMx->RCR = Structure->RepetitionCounter;
  }

  /* Generate an update event to reload the Prescaler
     and the repetition counter (only for advanced timer) value immediately */
  TIMx->EGR = TIM_EGR_UG;
}

static void TIM_OC1_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;

  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= ~TIM_CCER_CC1E;

  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;

  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;

  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR1_OC1M;
  tmpccmrx &= ~TIM_CCMR1_CC1S;
  /* Select the Output Compare Mode */
  tmpccmrx |= OC_Config->OCMode;

  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC1P;
  /* Set the Output Compare Polarity */
  tmpccer |= OC_Config->OCPolarity;

  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_1))
  {
    /* Check parameters */
    assert_param(IS_TIM_OCN_POLARITY(OC_Config->OCNPolarity));

    /* Reset the Output N Polarity level */
    tmpccer &= ~TIM_CCER_CC1NP;
    /* Set the Output N Polarity */
    tmpccer |= OC_Config->OCNPolarity;
    /* Reset the Output N State */
    tmpccer &= ~TIM_CCER_CC1NE;
  }

  if (IS_TIM_BREAK_INSTANCE(TIMx))
  {
    /* Check parameters */
    assert_param(IS_TIM_OCNIDLE_STATE(OC_Config->OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));

    /* Reset the Output Compare and Output Compare N IDLE State */
    tmpcr2 &= ~TIM_CR2_OIS1;
    tmpcr2 &= ~TIM_CR2_OIS1N;
    /* Set the Output Idle state */
    tmpcr2 |= OC_Config->OCIdleState;
    /* Set the Output N Idle state */
    tmpcr2 |= OC_Config->OCNIdleState;
  }

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;

  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;

  /* Set the Capture Compare Register value */
  TIMx->CCR1 = OC_Config->Pulse;

  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

static void TIM_OC2_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx;
  uint32_t tmpccer;
  uint32_t tmpcr2;

  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= ~TIM_CCER_CC2E;

  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;

  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;

  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= ~TIM_CCMR1_OC2M;
  tmpccmrx &= ~TIM_CCMR1_CC2S;

  /* Select the Output Compare Mode */
  tmpccmrx |= (OC_Config->OCMode << 8U);

  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC2P;
  /* Set the Output Compare Polarity */
  tmpccer |= (OC_Config->OCPolarity << 4U);

  if (IS_TIM_CCXN_INSTANCE(TIMx, TIM_CHANNEL_2))
  {
    assert_param(IS_TIM_OCN_POLARITY(OC_Config->OCNPolarity));

    /* Reset the Output N Polarity level */
    tmpccer &= ~TIM_CCER_CC2NP;
    /* Set the Output N Polarity */
    tmpccer |= (OC_Config->OCNPolarity << 4U);
    /* Reset the Output N State */
    tmpccer &= ~TIM_CCER_CC2NE;

  }

  if (IS_TIM_BREAK_INSTANCE(TIMx))
  {
    /* Check parameters */
    assert_param(IS_TIM_OCNIDLE_STATE(OC_Config->OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(OC_Config->OCIdleState));

    /* Reset the Output Compare and Output Compare N IDLE State */
    tmpcr2 &= ~TIM_CR2_OIS2;
    tmpcr2 &= ~TIM_CR2_OIS2N;
    /* Set the Output Idle state */
    tmpcr2 |= (OC_Config->OCIdleState << 2U);
    /* Set the Output N Idle state */
    tmpcr2 |= (OC_Config->OCNIdleState << 2U);
  }

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;

  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;

  /* Set the Capture Compare Register value */
  TIMx->CCR2 = OC_Config->Pulse;

  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

static void TIM_CCxChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelState)
{
  uint32_t tmp;

  /* Check the parameters */
  assert_param(IS_TIM_CC1_INSTANCE(TIMx));
  assert_param(IS_TIM_CHANNELS(Channel));

  tmp = TIM_CCER_CC1E << (Channel & 0x1FU); /* 0x1FU = 31 bits max shift */

  /* Reset the CCxE Bit */
  TIMx->CCER &= ~tmp;

  /* Set or reset the CCxE Bit */
  TIMx->CCER |= (uint32_t)(ChannelState << (Channel & 0x1FU)); /* 0x1FU = 31 bits max shift */
}

__weak void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_Base_MspInit could be implemented in the user file
   */
}

__weak void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_MspInit could be implemented in the user file
   */
}