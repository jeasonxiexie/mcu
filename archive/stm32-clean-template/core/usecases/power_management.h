#ifndef __POWER_MANAGEMENT_H
#define __POWER_MANAGEMENT_H

#include "../interfaces/power_interface.h"
#include "../interfaces/display_interface.h"

/**
 * @brief Power Management Use Case
 * 
 * This module implements the business logic for power management,
 * independent of specific hardware implementations.
 */

/**
 * @brief Initialize power management
 * @param power_if Power interface implementation
 * @param display_if Display interface implementation
 * @return true if successful, false otherwise
 */
bool power_mgmt_init(const power_interface_t* power_if, const display_interface_t* display_if);

/**
 * @brief Process power management (call periodically)
 */
void power_mgmt_process(void);

/**
 * @brief Get current battery status
 * @return Battery status
 */
battery_status_t power_mgmt_get_battery_status(void);

/**
 * @brief Handle low battery condition
 */
void power_mgmt_handle_low_battery(void);

/**
 * @brief Handle critical battery condition
 */
void power_mgmt_handle_critical_battery(void);

/**
 * @brief Request system shutdown
 */
void power_mgmt_request_shutdown(void);

/**
 * @brief Request sleep mode
 */
void power_mgmt_request_sleep(void);

/**
 * @brief Wake up from sleep mode
 */
void power_mgmt_wake_up(void);

/**
 * @brief Set power LED state
 * @param enable true to enable, false to disable
 */
void power_mgmt_set_led(bool enable);

/**
 * @brief Check if system should enter low power mode
 * @return true if should enter low power mode
 */
bool power_mgmt_should_sleep(void);

/**
 * @brief Update power management configuration
 * @param low_voltage Low voltage threshold
 * @param critical_voltage Critical voltage threshold
 */
void power_mgmt_set_thresholds(float low_voltage, float critical_voltage);

#endif /* __POWER_MANAGEMENT_H */