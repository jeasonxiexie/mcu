#ifndef __WB32L003_HAL_SPI_H
#define __WB32L003_HAL_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "wb32l003_hal_def.h"

/* SPI handle Structure definition */
typedef struct __SPI_HandleTypeDef
{
  void                  *Instance;
  struct {
    uint32_t Mode;
    uint32_t Direction;
    uint32_t DataSize;
    uint32_t CLKPolarity;
    uint32_t CLKPhase;
    uint32_t NSS;
    uint32_t BaudRatePrescaler;
    uint32_t FirstBit;
    uint32_t TIMode;
    uint32_t CRCCalculation;
    uint32_t CRCPolynomial;
  } Init;
  uint8_t               *pTxBuffPtr;
  uint16_t              TxXferSize;
  __IO uint16_t         TxXferCount;
  uint8_t               *pRxBuffPtr;
  uint16_t              RxXferSize;
  __IO uint16_t         RxXferCount;
  void                  (*RxISR)(struct __SPI_HandleTypeDef *hspi);
  void                  (*TxISR)(struct __SPI_HandleTypeDef *hspi);
  __IO uint32_t         State;
  __IO uint32_t         ErrorCode;
  HAL_LockTypeDef       Lock;
} SPI_HandleTypeDef;

/* SPI mode */
#define SPI_MODE_SLAVE                  0x00000000U
#define SPI_MODE_MASTER                 0x00000004U

/* SPI Direction */
#define SPI_DIRECTION_2LINES            0x00000000U
#define SPI_DIRECTION_2LINES_RXONLY     0x00000400U
#define SPI_DIRECTION_1LINE             0x00008000U

/* SPI Data Size */
#define SPI_DATASIZE_8BIT               0x00000000U
#define SPI_DATASIZE_16BIT              0x00000800U

/* SPI Clock Polarity */
#define SPI_POLARITY_LOW                0x00000000U
#define SPI_POLARITY_HIGH               0x00000002U

/* SPI Clock Phase */
#define SPI_PHASE_1EDGE                 0x00000000U
#define SPI_PHASE_2EDGE                 0x00000001U

/* SPI Slave Select management */
#define SPI_NSS_SOFT                    0x00000200U
#define SPI_NSS_HARD_INPUT              0x00000000U
#define SPI_NSS_HARD_OUTPUT             0x00040000U

/* SPI BaudRate Prescaler */
#define SPI_BAUDRATEPRESCALER_2         0x00000000U
#define SPI_BAUDRATEPRESCALER_4         0x00000008U
#define SPI_BAUDRATEPRESCALER_8         0x00000010U
#define SPI_BAUDRATEPRESCALER_16        0x00000018U
#define SPI_BAUDRATEPRESCALER_32        0x00000020U
#define SPI_BAUDRATEPRESCALER_64        0x00000028U
#define SPI_BAUDRATEPRESCALER_128       0x00000030U
#define SPI_BAUDRATEPRESCALER_256       0x00000038U

/* SPI MSB/LSB First */
#define SPI_FIRSTBIT_MSB                0x00000000U
#define SPI_FIRSTBIT_LSB                0x00000080U

/* SPI TI Mode */
#define SPI_TIMODE_DISABLE              0x00000000U
#define SPI_TIMODE_ENABLE               0x00000010U

/* SPI CRC Calculation */
#define SPI_CRCCALCULATION_DISABLE      0x00000000U
#define SPI_CRCCALCULATION_ENABLE       0x00002000U

/* SPI Instances */
#define SPI1                            ((void *)SPI1_BASE)

/* Exported functions */
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);

HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);

HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);

void HAL_SPI_IRQHandler(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_SPI_H */