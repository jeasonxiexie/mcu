#ifndef __WB32L003_HAL_CORTEX_H
#define __WB32L003_HAL_CORTEX_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "wb32l003_hal_def.h"

/* Exported types */

/* Exported constants */
#define NVIC_PRIORITYGROUP_0         0x00000007U /*!< 0 bit  for pre-emption priority,
                                                      4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         0x00000006U /*!< 1 bit  for pre-emption priority,
                                                      3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         0x00000005U /*!< 2 bits for pre-emption priority,
                                                      2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         0x00000004U /*!< 3 bits for pre-emption priority,
                                                      1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         0x00000003U /*!< 4 bits for pre-emption priority,
                                                      0 bit  for subpriority */

/* SysTick clock source */
#define SYSTICK_CLKSOURCE_HCLK_DIV8    0x00000000U
#define SYSTICK_CLKSOURCE_HCLK         0x00000004U

/* Exported Macros */

/* Exported functions */
/* Initialization and de-initialization functions */
void HAL_NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
void HAL_NVIC_DisableIRQ(IRQn_Type IRQn);
void HAL_NVIC_SystemReset(void);
uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb);

/* Peripheral Control functions */
uint32_t HAL_NVIC_GetPriorityGrouping(void);
void HAL_NVIC_GetPriority(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t* pPreemptPriority, uint32_t* pSubPriority);
uint32_t HAL_NVIC_GetPendingIRQ(IRQn_Type IRQn);
void HAL_NVIC_SetPendingIRQ(IRQn_Type IRQn);
void HAL_NVIC_ClearPendingIRQ(IRQn_Type IRQn);
uint32_t HAL_NVIC_GetActive(IRQn_Type IRQn);
void HAL_SYSTICK_CLKSourceConfig(uint32_t CLKSource);
void HAL_SYSTICK_IRQHandler(void);
void HAL_SYSTICK_Callback(void);

/* Inline functions */
__STATIC_INLINE void __disable_irq(void)
{
  __ASM volatile ("cpsid i");
}

__STATIC_INLINE void __enable_irq(void)
{
  __ASM volatile ("cpsie i");
}

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_CORTEX_H */