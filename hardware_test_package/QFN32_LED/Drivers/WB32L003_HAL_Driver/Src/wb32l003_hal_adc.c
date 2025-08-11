#include "wb32l003_hal.h"

/* Private typedef */
typedef struct
{
  __IO uint32_t ISR;
  __IO uint32_t IER;
  __IO uint32_t CR;
  __IO uint32_t CFGR1;
  __IO uint32_t CFGR2;
  __IO uint32_t SMPR;
  uint32_t RESERVED1;
  uint32_t RESERVED2;
  __IO uint32_t TR;
  uint32_t RESERVED3;
  __IO uint32_t CHSELR;
  uint32_t RESERVED4[5];
  __IO uint32_t DR;
} ADC_TypeDef;

/* ADC Registers Bits Definition */
#define ADC_ISR_ADRDY_Pos          (0U)
#define ADC_ISR_ADRDY_Msk          (0x1UL << ADC_ISR_ADRDY_Pos)
#define ADC_ISR_ADRDY              ADC_ISR_ADRDY_Msk
#define ADC_ISR_EOC_Pos            (2U)
#define ADC_ISR_EOC_Msk            (0x1UL << ADC_ISR_EOC_Pos)
#define ADC_ISR_EOC                ADC_ISR_EOC_Msk

#define ADC_CR_ADEN_Pos            (0U)
#define ADC_CR_ADEN_Msk            (0x1UL << ADC_CR_ADEN_Pos)
#define ADC_CR_ADEN                ADC_CR_ADEN_Msk
#define ADC_CR_ADSTART_Pos         (2U)
#define ADC_CR_ADSTART_Msk         (0x1UL << ADC_CR_ADSTART_Pos)
#define ADC_CR_ADSTART             ADC_CR_ADSTART_Msk
#define ADC_CR_ADSTP_Pos           (4U)
#define ADC_CR_ADSTP_Msk           (0x1UL << ADC_CR_ADSTP_Pos)
#define ADC_CR_ADSTP               ADC_CR_ADSTP_Msk
#define ADC_CR_ADCAL_Pos           (31U)
#define ADC_CR_ADCAL_Msk           (0x1UL << ADC_CR_ADCAL_Pos)
#define ADC_CR_ADCAL               ADC_CR_ADCAL_Msk

ADC_HandleTypeDef hadc;

HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc)
{
  ADC_TypeDef *ADCx;
  
  /* Check ADC handle */
  if (hadc == NULL)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  ADCx = (ADC_TypeDef *)hadc->Instance;
  
  /* Init the low level hardware */
  HAL_ADC_MspInit(hadc);
  
  /* Initialize the ADC state */
  hadc->State = HAL_ADC_STATE_BUSY_INTERNAL;
  hadc->ErrorCode = HAL_ADC_ERROR_NONE;
  
  /* Disable the ADC */
  ADCx->CR &= ~ADC_CR_ADEN;
  
  /* Configuration of ADC parameters */
  ADCx->CFGR1 = hadc->Init.Resolution | hadc->Init.DataAlign |
                hadc->Init.ScanConvMode | hadc->Init.ContinuousConvMode |
                hadc->Init.DiscontinuousConvMode | hadc->Init.ExternalTrigConvEdge |
                hadc->Init.ExternalTrigConv | hadc->Init.DMAContinuousRequests |
                hadc->Init.EOCSelection | hadc->Init.Overrun;
  
  /* Enable the ADC */
  ADCx->CR |= ADC_CR_ADEN;
  
  /* Wait for ADC ready */
  while ((ADCx->ISR & ADC_ISR_ADRDY) == 0U)
  {
  }
  
  /* Set ADC state */
  hadc->State = HAL_ADC_STATE_READY;
  
  return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc)
{
  ADC_TypeDef *ADCx = (ADC_TypeDef *)hadc->Instance;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Enable the ADC peripheral */
  if ((ADCx->CR & ADC_CR_ADEN) == 0U)
  {
    ADCx->CR |= ADC_CR_ADEN;
  }
  
  /* Start conversion */
  ADCx->CR |= ADC_CR_ADSTART;
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc)
{
  ADC_TypeDef *ADCx = (ADC_TypeDef *)hadc->Instance;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Stop conversion */
  ADCx->CR |= ADC_CR_ADSTP;
  
  /* Wait for conversion stop */
  while ((ADCx->CR & ADC_CR_ADSTP) != 0U)
  {
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout)
{
  ADC_TypeDef *ADCx = (ADC_TypeDef *)hadc->Instance;
  uint32_t tickstart = HAL_GetTick();
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Wait for conversion completion */
  while ((ADCx->ISR & ADC_ISR_EOC) == 0U)
  {
    /* Check for timeout */
    if (Timeout != HAL_MAX_DELAY)
    {
      if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))
      {
        hadc->State = HAL_ADC_STATE_TIMEOUT;
        return HAL_TIMEOUT;
      }
    }
  }
  
  /* Clear EOC flag */
  ADCx->ISR = ADC_ISR_EOC;
  
  /* Return function status */
  return HAL_OK;
}

uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc)
{
  ADC_TypeDef *ADCx = (ADC_TypeDef *)hadc->Instance;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Return the selected ADC converted value */
  return (ADCx->DR);
}

HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig)
{
  ADC_TypeDef *ADCx = (ADC_TypeDef *)hadc->Instance;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CHANNEL(sConfig->Channel));
  assert_param(IS_ADC_SAMPLE_TIME(sConfig->SamplingTime));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Clear the old channel selection */
  ADCx->CHSELR &= ~(1U << sConfig->Channel);
  
  /* Set the new channel selection */
  ADCx->CHSELR |= (1U << sConfig->Channel);
  
  /* Set the sampling time */
  ADCx->SMPR = sConfig->SamplingTime;
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return HAL_OK;
}

HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef* hadc)
{
  ADC_TypeDef *ADCx = (ADC_TypeDef *)hadc->Instance;
  uint32_t tickstart = HAL_GetTick();
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Disable the ADC */
  ADCx->CR &= ~ADC_CR_ADEN;
  
  /* Start calibration */
  ADCx->CR |= ADC_CR_ADCAL;
  
  /* Wait for calibration completion */
  while ((ADCx->CR & ADC_CR_ADCAL) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > 10U)
    {
      /* Process unlocked */
      __HAL_UNLOCK(hadc);
      return HAL_TIMEOUT;
    }
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return HAL_OK;
}

__weak void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_ADC_MspInit could be implemented in the user file
   */
}