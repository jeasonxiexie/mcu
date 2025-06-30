#ifndef __WB32L003_HAL_H
#define __WB32L003_HAL_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "wb32l003_hal_conf.h"
#include "wb32l003_hal_def.h"
#include "wb32l003_hal_def_ex.h"

/* Exported constants */
#define HAL_TICK_FREQ_10HZ         100U
#define HAL_TICK_FREQ_100HZ        10U
#define HAL_TICK_FREQ_1KHZ         1U
#define HAL_TICK_FREQ_DEFAULT      HAL_TICK_FREQ_1KHZ

/* Exported variables */
extern __IO uint32_t uwTick;
extern uint32_t uwTickPrio;
extern uint32_t uwTickFreq;

/* Exported functions */
HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_DeInit(void);
void HAL_MspInit(void);
void HAL_MspDeInit(void);
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);

void HAL_IncTick(void);
void HAL_Delay(uint32_t Delay);
uint32_t HAL_GetTick(void);
uint32_t HAL_GetTickPrio(void);
HAL_StatusTypeDef HAL_SetTickFreq(uint32_t Freq);
uint32_t HAL_GetTickFreq(void);

void HAL_SuspendTick(void);
void HAL_ResumeTick(void);
uint32_t HAL_GetHalVersion(void);
uint32_t HAL_GetREVID(void);
uint32_t HAL_GetDEVID(void);

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_H */