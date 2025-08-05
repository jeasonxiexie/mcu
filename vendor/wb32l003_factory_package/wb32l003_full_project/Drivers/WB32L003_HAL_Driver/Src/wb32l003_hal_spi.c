#include "wb32l003_hal.h"

/* Private typedef */
typedef struct
{
  __IO uint32_t CR1;
  __IO uint32_t CR2;
  __IO uint32_t SR;
  __IO uint32_t DR;
  __IO uint32_t CRCPR;
  __IO uint32_t RXCRCR;
  __IO uint32_t TXCRCR;
  __IO uint32_t I2SCFGR;
  __IO uint32_t I2SPR;
} SPI_TypeDef;

/* SPI Registers Bits Definition */
#define SPI_CR1_CPHA_Pos         (0U)
#define SPI_CR1_CPHA_Msk         (0x1UL << SPI_CR1_CPHA_Pos)
#define SPI_CR1_CPHA             SPI_CR1_CPHA_Msk
#define SPI_CR1_CPOL_Pos         (1U)
#define SPI_CR1_CPOL_Msk         (0x1UL << SPI_CR1_CPOL_Pos)
#define SPI_CR1_CPOL             SPI_CR1_CPOL_Msk
#define SPI_CR1_MSTR_Pos         (2U)
#define SPI_CR1_MSTR_Msk         (0x1UL << SPI_CR1_MSTR_Pos)
#define SPI_CR1_MSTR             SPI_CR1_MSTR_Msk
#define SPI_CR1_BR_Pos           (3U)
#define SPI_CR1_BR_Msk           (0x7UL << SPI_CR1_BR_Pos)
#define SPI_CR1_BR               SPI_CR1_BR_Msk
#define SPI_CR1_SPE_Pos          (6U)
#define SPI_CR1_SPE_Msk          (0x1UL << SPI_CR1_SPE_Pos)
#define SPI_CR1_SPE              SPI_CR1_SPE_Msk
#define SPI_CR1_SSM_Pos          (9U)
#define SPI_CR1_SSM_Msk          (0x1UL << SPI_CR1_SSM_Pos)
#define SPI_CR1_SSM              SPI_CR1_SSM_Msk
#define SPI_CR1_SSI_Pos          (8U)
#define SPI_CR1_SSI_Msk          (0x1UL << SPI_CR1_SSI_Pos)
#define SPI_CR1_SSI              SPI_CR1_SSI_Msk

#define SPI_SR_RXNE_Pos          (0U)
#define SPI_SR_RXNE_Msk          (0x1UL << SPI_SR_RXNE_Pos)
#define SPI_SR_RXNE              SPI_SR_RXNE_Msk
#define SPI_SR_TXE_Pos           (1U)
#define SPI_SR_TXE_Msk           (0x1UL << SPI_SR_TXE_Pos)
#define SPI_SR_TXE               SPI_SR_TXE_Msk
#define SPI_SR_BSY_Pos           (7U)
#define SPI_SR_BSY_Msk           (0x1UL << SPI_SR_BSY_Pos)
#define SPI_SR_BSY               SPI_SR_BSY_Msk

SPI_HandleTypeDef hspi1;

HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
{
  SPI_TypeDef *SPIx = (SPI_TypeDef *)hspi->Instance;
  
  /* Check the SPI handle allocation */
  if (hspi == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));
  assert_param(IS_SPI_MODE(hspi->Init.Mode));
  assert_param(IS_SPI_DIRECTION(hspi->Init.Direction));
  assert_param(IS_SPI_DATASIZE(hspi->Init.DataSize));
  assert_param(IS_SPI_CPOL(hspi->Init.CLKPolarity));
  assert_param(IS_SPI_CPHA(hspi->Init.CLKPhase));
  assert_param(IS_SPI_NSS(hspi->Init.NSS));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRatePrescaler));
  assert_param(IS_SPI_FIRST_BIT(hspi->Init.FirstBit));

  /* Init the low level hardware : GPIO, CLOCK, NVIC... */
  HAL_SPI_MspInit(hspi);

  /* Disable the SPI peripheral */
  SPIx->CR1 &= ~SPI_CR1_SPE;

  /* Configure SPI */
  SPIx->CR1 = hspi->Init.Mode | hspi->Init.Direction | hspi->Init.DataSize |
              hspi->Init.CLKPolarity | hspi->Init.CLKPhase |
              hspi->Init.NSS | hspi->Init.BaudRatePrescaler |
              hspi->Init.FirstBit | hspi->Init.CRCCalculation;

  /* Configure NSS management */
  if (hspi->Init.NSS == SPI_NSS_SOFT)
  {
    SPIx->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  }

  /* Enable the SPI peripheral */
  SPIx->CR1 |= SPI_CR1_SPE;

  /* Initialize the SPI state */
  hspi->State = HAL_SPI_STATE_READY;
  hspi->ErrorCode = HAL_SPI_ERROR_NONE;

  return HAL_OK;
}

HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  SPI_TypeDef *SPIx = (SPI_TypeDef *)hspi->Instance;
  uint32_t tickstart = HAL_GetTick();
  HAL_StatusTypeDef errorcode = HAL_OK;

  /* Check parameters */
  if ((pData == NULL) || (Size == 0U))
  {
    return HAL_ERROR;
  }

  /* Lock the process */
  __HAL_LOCK(hspi);

  /* Set the transaction information */
  hspi->State = HAL_SPI_STATE_BUSY_TX;
  hspi->ErrorCode = HAL_SPI_ERROR_NONE;
  hspi->pTxBuffPtr = pData;
  hspi->TxXferSize = Size;
  hspi->TxXferCount = Size;

  /* Transmit data */
  while (hspi->TxXferCount > 0U)
  {
    /* Wait until TXE flag is set to send data */
    if ((SPIx->SR & SPI_SR_TXE) == SPI_SR_TXE)
    {
      *(__IO uint8_t *)&SPIx->DR = (*hspi->pTxBuffPtr++);
      hspi->TxXferCount--;
    }
    else
    {
      /* Timeout management */
      if ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick() - tickstart) >= Timeout))
      {
        errorcode = HAL_TIMEOUT;
        goto error;
      }
    }
  }

  /* Wait until BSY flag is reset */
  while ((SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY)
  {
    if ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick() - tickstart) >= Timeout))
    {
      errorcode = HAL_TIMEOUT;
      goto error;
    }
  }

  /* Clear overrun flag */
  __HAL_SPI_CLEAR_OVRFLAG(hspi);

error:
  hspi->State = HAL_SPI_STATE_READY;
  __HAL_UNLOCK(hspi);
  return errorcode;
}

__weak void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_MspInit should be implemented in the user file
   */
}