#ifndef __DOMAIN_TYPES_H
#define __DOMAIN_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/* ========== DISPLAY DOMAIN ========== */
typedef struct {
    uint16_t width;
    uint16_t height;
    const uint16_t* data;
} image_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} rect_t;

typedef enum {
    DISPLAY_MODE_NORMAL,
    DISPLAY_MODE_SLEEP,
    DISPLAY_MODE_OFF
} display_mode_t;

/* ========== POWER DOMAIN ========== */
typedef enum {
    POWER_MODE_NORMAL,
    POWER_MODE_SLEEP,
    POWER_MODE_STANDBY,
    POWER_MODE_OFF
} power_mode_t;

typedef enum {
    BATTERY_LEVEL_0 = 0,  // Empty
    BATTERY_LEVEL_1 = 1,  // Low
    BATTERY_LEVEL_2 = 2,  // Medium
    BATTERY_LEVEL_3 = 3,  // High
    BATTERY_LEVEL_4 = 4   // Full
} battery_level_t;

typedef struct {
    float voltage;
    battery_level_t level;
    bool is_charging;
    bool is_low_voltage;
    bool is_critical_voltage;
} battery_status_t;

/* ========== AUDIO DOMAIN ========== */
typedef enum {
    AUDIO_MODE_STEREO,
    AUDIO_MODE_MONO
} audio_mode_t;

typedef struct {
    uint8_t left_level;   // 0-31
    uint8_t right_level;  // 0-31
    audio_mode_t mode;
} audio_status_t;

/* ========== INPUT DOMAIN ========== */
typedef enum {
    BUTTON_STATE_RELEASED,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_LONG_PRESSED
} button_state_t;

typedef enum {
    BUTTON_EVENT_NONE,
    BUTTON_EVENT_SHORT_PRESS,
    BUTTON_EVENT_LONG_PRESS,
    BUTTON_EVENT_RELEASE
} button_event_t;

/* ========== UI DOMAIN ========== */
typedef enum {
    UI_STATE_BOOT,
    UI_STATE_NORMAL,
    UI_STATE_MENU,
    UI_STATE_SHUTDOWN
} ui_state_t;

typedef struct {
    ui_state_t state;
    audio_status_t audio;
    battery_status_t battery;
    uint32_t animation_frame;
    bool needs_update;
} ui_context_t;

/* ========== SYSTEM DOMAIN ========== */
typedef enum {
    SYSTEM_STATE_INIT,
    SYSTEM_STATE_RUNNING,
    SYSTEM_STATE_SLEEP,
    SYSTEM_STATE_ERROR
} system_state_t;

typedef struct {
    system_state_t state;
    uint32_t uptime_ms;
    bool factory_reset_requested;
} system_status_t;

#endif /* __DOMAIN_TYPES_H */