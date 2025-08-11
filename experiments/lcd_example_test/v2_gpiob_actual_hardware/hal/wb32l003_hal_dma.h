#ifndef __WB32L003_HAL_DMA_H
#define __WB32L003_HAL_DMA_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "wb32l003_hal_def.h"

/* DMA State Structure definition */
typedef enum
{
  HAL_DMA_STATE_RESET             = 0x00U,
  HAL_DMA_STATE_READY             = 0x01U,
  HAL_DMA_STATE_BUSY              = 0x02U,
  HAL_DMA_STATE_TIMEOUT           = 0x03U,
  HAL_DMA_STATE_ERROR             = 0x04U,
  HAL_DMA_STATE_ABORT             = 0x05U
} HAL_DMA_StateTypeDef;

/* DMA handle Structure definition */
typedef struct __DMA_HandleTypeDef
{
  void                  *Instance;
  struct {
    uint32_t Direction;
    uint32_t PeriphInc;
    uint32_t MemInc;
    uint32_t PeriphDataAlignment;
    uint32_t MemDataAlignment;
    uint32_t Mode;
    uint32_t Priority;
  } Init;
  HAL_LockTypeDef       Lock;
  __IO HAL_DMA_StateTypeDef State;
  void                  *Parent;
  void                  (* XferCpltCallback)(struct __DMA_HandleTypeDef * hdma);
  void                  (* XferHalfCpltCallback)(struct __DMA_HandleTypeDef * hdma);
  void                  (* XferErrorCallback)(struct __DMA_HandleTypeDef * hdma);
  void                  (* XferAbortCallback)(struct __DMA_HandleTypeDef * hdma);
  __IO uint32_t         ErrorCode;
} DMA_HandleTypeDef;

/* DMA transfer direction */
#define DMA_PERIPH_TO_MEMORY            0x00000000U
#define DMA_MEMORY_TO_PERIPH            DMA_CCR_DIR
#define DMA_MEMORY_TO_MEMORY            DMA_CCR_MEM2MEM

/* DMA Peripheral increment mode */
#define DMA_PINC_ENABLE                 DMA_CCR_PINC
#define DMA_PINC_DISABLE                0x00000000U

/* DMA Memory increment mode */
#define DMA_MINC_ENABLE                 DMA_CCR_MINC
#define DMA_MINC_DISABLE                0x00000000U

/* DMA Peripheral data alignment */
#define DMA_PDATAALIGN_BYTE             0x00000000U
#define DMA_PDATAALIGN_HALFWORD         DMA_CCR_PSIZE_0
#define DMA_PDATAALIGN_WORD             DMA_CCR_PSIZE_1

/* DMA Memory data alignment */
#define DMA_MDATAALIGN_BYTE             0x00000000U
#define DMA_MDATAALIGN_HALFWORD         DMA_CCR_MSIZE_0
#define DMA_MDATAALIGN_WORD             DMA_CCR_MSIZE_1

/* DMA mode */
#define DMA_NORMAL                      0x00000000U
#define DMA_CIRCULAR                    DMA_CCR_CIRC

/* DMA Priority level */
#define DMA_PRIORITY_LOW                0x00000000U
#define DMA_PRIORITY_MEDIUM             DMA_CCR_PL_0
#define DMA_PRIORITY_HIGH               DMA_CCR_PL_1
#define DMA_PRIORITY_VERY_HIGH          DMA_CCR_PL

/* Exported functions */
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma);

HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, uint32_t CompleteLevel, uint32_t Timeout);
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma);

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_DMA_H */