#ifndef __POWER_H
#define __POWER_H

#include "wb32l003.h"

#define BATTERY_FULL_VOLTAGE    4.2f
#define BATTERY_LOW_VOLTAGE     3.0f
#define BATTERY_CRITICAL_VOLTAGE 2.8f

#define ADC_VREF 3.3f
#define ADC_RESOLUTION 4096.0f

typedef enum {
    POWER_MODE_NORMAL = 0,
    POWER_MODE_STANDBY
} power_mode_t;

typedef enum {
    BATTERY_LEVEL_0 = 0,
    BATTERY_LEVEL_1,
    BATTERY_LEVEL_2,
    BATTERY_LEVEL_3,
    BATTERY_LEVEL_4
} battery_level_t;

void Power_Init(void);
void Power_Process(void);
float Power_GetBatteryVoltage(void);
battery_level_t Power_GetBatteryLevel(void);
power_mode_t Power_GetMode(void);
void Power_SetMode(power_mode_t mode);
void Power_Off(void);
void Power_EnterStandby(void);
void Power_ExitStandby(void);

#endif