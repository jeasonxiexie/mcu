#ifndef __WB32L003_HAL_FLASH_H
#define __WB32L003_HAL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "wb32l003_hal_def.h"

/* FLASH Error Codes */
#define HAL_FLASH_ERROR_NONE      0x00000000U
#define HAL_FLASH_ERROR_PROG      0x00000001U
#define HAL_FLASH_ERROR_WRP       0x00000002U
#define HAL_FLASH_ERROR_OPTV      0x00000004U
#define HAL_FLASH_ERROR_SIZE      0x00000008U
#define HAL_FLASH_ERROR_RD        0x00000010U
#define HAL_FLASH_ERROR_OPERATION 0x00000020U

/* FLASH Type Erase */
#define FLASH_TYPEERASE_PAGES     0x00000000U
#define FLASH_TYPEERASE_MASS      0x00000001U

/* FLASH Type Program */
#define FLASH_TYPEPROGRAM_HALFWORD    0x00000001U
#define FLASH_TYPEPROGRAM_WORD        0x00000002U
#define FLASH_TYPEPROGRAM_DOUBLEWORD  0x00000003U

/* FLASH Latency */
#define FLASH_LATENCY_0           0x00000000U
#define FLASH_LATENCY_1           0x00000001U

/* FLASH Keys */
#define FLASH_KEY1                0x45670123U
#define FLASH_KEY2                0xCDEF89ABU
#define FLASH_OPTKEY1             0x08192A3BU
#define FLASH_OPTKEY2             0x4C5D6E7FU

/* FLASH Page Size */
#define FLASH_PAGE_SIZE           0x400U

/* FLASH Timeout values */
#define FLASH_TIMEOUT_VALUE       50000U

/* Exported types */
typedef struct
{
  uint32_t TypeErase;
  uint32_t PageAddress;
  uint32_t NbPages;
} FLASH_EraseInitTypeDef;

typedef struct
{
  uint32_t OptionType;
  uint32_t WRPState;
  uint32_t WRPPage;
  uint8_t RDPLevel;
  uint8_t USERConfig;
  uint32_t DATAAddress;
  uint8_t DATAData;
} FLASH_OBProgramInitTypeDef;

typedef struct
{
  HAL_LockTypeDef Lock;
  __IO uint32_t ErrorCode;
  __IO uint32_t ProcedureOnGoing;
  __IO uint32_t Address;
  __IO uint32_t DataRemaining;
  __IO uint32_t NbPagesRemaining;
  __IO uint32_t Page;
} FLASH_ProcessTypeDef;

/* Exported functions */
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
HAL_StatusTypeDef HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t Address, uint64_t Data);

void HAL_FLASH_IRQHandler(void);
void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);
void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue);

HAL_StatusTypeDef HAL_FLASH_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_Lock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Lock(void);
void HAL_FLASH_OB_Launch(void);

uint32_t HAL_FLASH_GetError(void);

HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError);
HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit);

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_FLASH_H */