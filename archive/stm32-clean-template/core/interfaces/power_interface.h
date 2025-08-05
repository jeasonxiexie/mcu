#ifndef __POWER_INTERFACE_H
#define __POWER_INTERFACE_H

#include "../domain/types.h"

/**
 * @brief Power Management Hardware Abstraction Interface
 * 
 * This interface abstracts power management hardware from the business logic.
 */
typedef struct {
    /**
     * @brief Initialize power management
     * @return true if successful, false otherwise
     */
    bool (*init)(void);
    
    /**
     * @brief Get current battery voltage
     * @return Battery voltage in volts
     */
    float (*get_battery_voltage)(void);
    
    /**
     * @brief Get battery status
     * @return Battery status structure
     */
    battery_status_t (*get_battery_status)(void);
    
    /**
     * @brief Check if device is charging
     * @return true if charging, false otherwise
     */
    bool (*is_charging)(void);
    
    /**
     * @brief Set power mode
     * @param mode Target power mode
     */
    void (*set_power_mode)(power_mode_t mode);
    
    /**
     * @brief Enter low power mode
     * @param mode Sleep/standby mode
     */
    void (*enter_low_power)(power_mode_t mode);
    
    /**
     * @brief Enable/disable power LED
     * @param enable true to enable, false to disable
     */
    void (*set_power_led)(bool enable);
    
    /**
     * @brief Flash power LED (for low battery warning)
     * @param period Flash period in milliseconds
     */
    void (*flash_power_led)(uint32_t period);
    
    /**
     * @brief Shutdown system
     */
    void (*shutdown)(void);
    
    /**
     * @brief Reset system
     */
    void (*reset)(void);
} power_interface_t;

/* Power management constants */
#define POWER_LOW_VOLTAGE_DEFAULT     3.0f
#define POWER_CRITICAL_VOLTAGE_DEFAULT 2.8f
#define POWER_FULL_VOLTAGE_DEFAULT    4.2f
#define POWER_FLASH_PERIOD_MS         500

#endif /* __POWER_INTERFACE_H */