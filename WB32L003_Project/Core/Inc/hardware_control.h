#ifndef __HARDWARE_CONTROL_H
#define __HARDWARE_CONTROL_H

#include "wb32l003.h"
#include <stdbool.h>

// Hardware control functions - to be implemented when GPIO assignments are known

// Power control
void HW_SetMainPower(bool enable);      // CON_POW
void HW_SetLCDPower(bool enable);       // CON_LCD

// Audio control
void HW_SetMute(bool mute);             // MUTE pin
void HW_SetAudioMode(bool mono);        // V2 pin (false=STEREO, true=MONO)

// LED control
void HW_SetGreenLED(bool on);           // Green LED
void HW_SetRedLED(bool on);             // Red LED

// Input detection
bool HW_IsCharging(void);               // CHRG pin
uint16_t HW_GetLeftAudioLevel(void);    // L_AD ADC
uint16_t HW_GetRightAudioLevel(void);   // R_AD ADC

// Initialize all hardware controls
void HW_Init(void);

// Power sequencing functions
void HW_PowerOnSequence(void);
void HW_PowerOffSequence(void);

#endif /* __HARDWARE_CONTROL_H */