#ifndef __WB32L003_HAL_PWR_H
#define __WB32L003_HAL_PWR_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "wb32l003_hal_def.h"

/* PWR WakeUp Pins */
#define PWR_WAKEUP_PIN1                 PWR_CSR_EWUP1
#define PWR_WAKEUP_PIN2                 PWR_CSR_EWUP2

/* PWR Regulator state in Sleep/Stop mode */
#define PWR_MAINREGULATOR_ON            0x00000000U
#define PWR_LOWPOWERREGULATOR_ON        PWR_CR_LPSDSR

/* PWR SLEEP mode entry */
#define PWR_SLEEPENTRY_WFI              ((uint8_t)0x01U)
#define PWR_SLEEPENTRY_WFE              ((uint8_t)0x02U)

/* PWR STOP mode entry */
#define PWR_STOPENTRY_WFI               ((uint8_t)0x01U)
#define PWR_STOPENTRY_WFE               ((uint8_t)0x02U)

/* PWR Flag */
#define PWR_FLAG_WU                     PWR_CSR_WUF
#define PWR_FLAG_SB                     PWR_CSR_SBF
#define PWR_FLAG_PVDO                   PWR_CSR_PVDO
#define PWR_FLAG_VREFINTRDY             PWR_CSR_VREFINTRDYF

/* Exported functions */
void HAL_PWR_DeInit(void);
void HAL_PWR_EnableBkUpAccess(void);
void HAL_PWR_DisableBkUpAccess(void);

/* WakeUp pins configuration functions */
void HAL_PWR_EnableWakeUpPin(uint32_t WakeUpPinx);
void HAL_PWR_DisableWakeUpPin(uint32_t WakeUpPinx);

/* Low Power modes configuration functions */
void HAL_PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry);
void HAL_PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry);
void HAL_PWR_EnterSTANDBYMode(void);

void HAL_PWR_EnableSleepOnExit(void);
void HAL_PWR_DisableSleepOnExit(void);
void HAL_PWR_EnableSEVOnPend(void);
void HAL_PWR_DisableSEVOnPend(void);

/* Power Voltage Detector configuration functions */
void HAL_PWR_EnablePVD(void);
void HAL_PWR_DisablePVD(void);

/* WakeUp pins flags management functions */
uint32_t HAL_PWR_GetFlagStatus(uint32_t Flag);
void HAL_PWR_ClearFlag(uint32_t Flag);

/* Macros */
#define __HAL_PWR_GET_FLAG(__FLAG__)    ((PWR->CSR & (__FLAG__)) == (__FLAG__))
#define __HAL_PWR_CLEAR_FLAG(__FLAG__)  (PWR->CR |= (__FLAG__) << 2U)

#ifdef __cplusplus
}
#endif

#endif /* __WB32L003_HAL_PWR_H */