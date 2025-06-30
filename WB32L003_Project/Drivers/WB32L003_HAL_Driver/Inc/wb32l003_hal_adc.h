#ifndef __WB32L003_HAL_ADC_H
#define __WB32L003_HAL_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "wb32l003_hal_def.h"

/* ADC handle Structure definition */
typedef struct
{
  void                  *Instance;
  struct {
    uint32_t ClockPrescaler;
    uint32_t Resolution;
    uint32_t DataAlign;
    uint32_t ScanConvMode;
    uint32_t EOCSelection;
    uint32_t LowPowerAutoWait;
    uint32_t LowPowerAutoPowerOff;
    uint32_t ContinuousConvMode;
    uint32_t DiscontinuousConvMode;
    uint32_t ExternalTrigConv;
    uint32_t ExternalTrigConvEdge;
    uint32_t DMAContinuousRequests;
    uint32_t Overrun;
  } Init;
  __IO uint32_t         State;
  __IO uint32_t         ErrorCode;
  HAL_LockTypeDef       Lock;
} ADC_HandleTypeDef;

/* ADC Configuration Structure definition */
typedef struct
{
  uint32_t Channel;
  uint32_t Rank;
  uint32_t SamplingTime;
} ADC_ChannelConfTypeDef;

/* ADC Clock Prescaler */
#define ADC_CLOCK_SYNC_PCLK_DIV1        0x00000000U
#define ADC_CLOCK_SYNC_PCLK_DIV2        0x00000001U
#define ADC_CLOCK_SYNC_PCLK_DIV4        0x00000002U

/* ADC Resolution */
#define ADC_RESOLUTION_12B              0x00000000U
#define ADC_RESOLUTION_10B              0x00000008U
#define ADC_RESOLUTION_8B               0x00000010U
#define ADC_RESOLUTION_6B               0x00000018U

/* ADC Data Alignment */
#define ADC_DATAALIGN_RIGHT             0x00000000U
#define ADC_DATAALIGN_LEFT              0x00000020U

/* ADC Scan mode */
#define ADC_SCAN_DISABLE                0x00000000U
#define ADC_SCAN_ENABLE                 0x00000100U

/* ADC End of Conversion Selection */
#define ADC_EOC_SINGLE_CONV             0x00000000U
#define ADC_EOC_SEQ_CONV                0x00000002U
#define ADC_EOC_SINGLE_SEQ_CONV         0x00000003U

/* ADC Overrun */
#define ADC_OVR_DATA_PRESERVED          0x00000000U
#define ADC_OVR_DATA_OVERWRITTEN        0x00001000U

/* ADC Channels */
#define ADC_CHANNEL_0                   0x00000000U
#define ADC_CHANNEL_1                   0x00000001U
#define ADC_CHANNEL_2                   0x00000002U
#define ADC_CHANNEL_3                   0x00000003U
#define ADC_CHANNEL_4                   0x00000004U
#define ADC_CHANNEL_5                   0x00000005U
#define ADC_CHANNEL_6                   0x00000006U
#define ADC_CHANNEL_7                   0x00000007U
#define ADC_CHANNEL_8                   0x00000008U
#define ADC_CHANNEL_9                   0x00000009U
#define ADC_CHANNEL_10                  0x0000000AU
#define ADC_CHANNEL_11                  0x0000000BU
#define ADC_CHANNEL_12                  0x0000000CU
#define ADC_CHANNEL_13                  0x0000000DU
#define ADC_CHANNEL_14                  0x0000000EU
#define ADC_CHANNEL_15                  0x0000000FU
#define ADC_CHANNEL_16                  0x00000010U
#define ADC_CHANNEL_17                  0x00000011U

/* ADC Sampling Times */
#define ADC_SAMPLETIME_1CYCLE_5         0x00000000U
#define ADC_SAMPLETIME_7CYCLES_5        0x00000001U
#define ADC_SAMPLETIME_13CYCLES_5       0x00000002U
#define ADC_SAMPLETIME_28CYCLES_5       0x00000003U
#define ADC_SAMPLETIME_41CYCLES_5       0x00000004U
#define ADC_SAMPLETIME_55CYCLES_5       0x00000005U
#define ADC_SAMPLETIME_71CYCLES_5       0x00000006U
#define ADC_SAMPLETIME_239CYCLES_5      0x00000007U

/* ADC Rank */
#define ADC_RANK_CHANNEL_NUMBER         0x00001000U

/* ADC External Trigger Edge */
#define ADC_EXTERNALTRIGCONVEDGE_NONE   0x00000000U
#define ADC_EXTERNALTRIGCONVEDGE_RISING 0x00000400U
#define ADC_EXTERNALTRIGCONVEDGE_FALLING 0x00000800U
#define ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING 0x00000C00U

/* ADC External Trigger Source */
#define ADC_SOFTWARE_START              0x00000000U

/* ADC Instances */
#define ADC1                            ((void *)ADC1_BASE)

/* Exported functions */
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);

HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);

HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Stop_IT(ADC_HandleTypeDef* hadc);

HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* hadc);

uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc);

void HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc);

HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig);
HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef* hadc);

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_ADC_H */