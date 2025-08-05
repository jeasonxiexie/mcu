#include "power_management.h"
#include <stddef.h>

/* Private variables */
static const power_interface_t* power_interface = NULL;
static const display_interface_t* display_interface = NULL;
static battery_status_t current_battery_status = {0};
static float low_voltage_threshold = POWER_LOW_VOLTAGE_DEFAULT;
static float critical_voltage_threshold = POWER_CRITICAL_VOLTAGE_DEFAULT;
static uint32_t last_update_time = 0;
static bool low_battery_warning_active = false;
static uint32_t shutdown_request_time = 0;

/* Private function declarations */
static void update_battery_status(void);
static void handle_battery_warnings(void);
static bool is_timeout(uint32_t start_time, uint32_t timeout_ms);

bool power_mgmt_init(const power_interface_t* power_if, const display_interface_t* display_if)
{
    if (power_if == NULL || display_if == NULL) {
        return false;
    }
    
    power_interface = power_if;
    display_interface = display_if;
    
    // Initialize power interface
    if (!power_interface->init()) {
        return false;
    }
    
    // Initial battery status update
    update_battery_status();
    
    return true;
}

void power_mgmt_process(void)
{
    if (power_interface == NULL) {
        return;
    }
    
    // Update battery status every second
    static uint32_t last_battery_update = 0;
    uint32_t current_time = HAL_GetTick(); // Assume HAL_GetTick() is available
    
    if (current_time - last_battery_update >= 1000) {
        last_battery_update = current_time;
        update_battery_status();
        handle_battery_warnings();
    }
    
    // Handle shutdown request timeout
    if (shutdown_request_time > 0 && 
        is_timeout(shutdown_request_time, 10000)) { // 10 seconds timeout
        power_interface->shutdown();
    }
}

battery_status_t power_mgmt_get_battery_status(void)
{
    return current_battery_status;
}

void power_mgmt_handle_low_battery(void)
{
    if (power_interface == NULL) {
        return;
    }
    
    // Flash power LED
    power_interface->flash_power_led(POWER_FLASH_PERIOD_MS);
    low_battery_warning_active = true;
}

void power_mgmt_handle_critical_battery(void)
{
    if (power_interface == NULL) {
        return;
    }
    
    // Initiate shutdown sequence
    shutdown_request_time = HAL_GetTick();
    
    // Set display to low power mode
    if (display_interface != NULL) {
        display_interface->set_power_mode(DISPLAY_MODE_SLEEP);
    }
}

void power_mgmt_request_shutdown(void)
{
    if (power_interface == NULL) {
        return;
    }
    
    shutdown_request_time = HAL_GetTick();
}

void power_mgmt_request_sleep(void)
{
    if (power_interface == NULL) {
        return;
    }
    
    // Set display to sleep mode
    if (display_interface != NULL) {
        display_interface->set_power_mode(DISPLAY_MODE_SLEEP);
    }
    
    // Enter low power mode
    power_interface->enter_low_power(POWER_MODE_SLEEP);
}

void power_mgmt_wake_up(void)
{
    if (display_interface == NULL) {
        return;
    }
    
    // Wake up display
    display_interface->set_power_mode(DISPLAY_MODE_NORMAL);
    low_battery_warning_active = false;
}

void power_mgmt_set_led(bool enable)
{
    if (power_interface == NULL) {
        return;
    }
    
    power_interface->set_power_led(enable);
}

bool power_mgmt_should_sleep(void)
{
    // Implement sleep logic based on system state
    // For example, no user activity for X minutes
    return false;
}

void power_mgmt_set_thresholds(float low_voltage, float critical_voltage)
{
    low_voltage_threshold = low_voltage;
    critical_voltage_threshold = critical_voltage;
}

/* Private function implementations */
static void update_battery_status(void)
{
    if (power_interface == NULL) {
        return;
    }
    
    current_battery_status = power_interface->get_battery_status();
    
    // Update voltage thresholds
    current_battery_status.is_low_voltage = 
        (current_battery_status.voltage < low_voltage_threshold);
    current_battery_status.is_critical_voltage = 
        (current_battery_status.voltage < critical_voltage_threshold);
}

static void handle_battery_warnings(void)
{
    if (current_battery_status.is_critical_voltage) {
        power_mgmt_handle_critical_battery();
    } else if (current_battery_status.is_low_voltage) {
        power_mgmt_handle_low_battery();
    } else {
        low_battery_warning_active = false;
        power_mgmt_set_led(true); // Normal power LED state
    }
}

static bool is_timeout(uint32_t start_time, uint32_t timeout_ms)
{
    uint32_t current_time = HAL_GetTick();
    return (current_time - start_time) >= timeout_ms;
}