#ifndef __INPUT_INTERFACE_H
#define __INPUT_INTERFACE_H

#include "../domain/types.h"

/**
 * @brief Input Hardware Abstraction Interface
 * 
 * This interface abstracts input devices (buttons, encoders, etc.) from business logic.
 */
typedef struct {
    /**
     * @brief Initialize input devices
     * @return true if successful, false otherwise
     */
    bool (*init)(void);
    
    /**
     * @brief Get current button state
     * @param button_id Button identifier
     * @return Current button state
     */
    button_state_t (*get_button_state)(uint8_t button_id);
    
    /**
     * @brief Poll for button events
     * @param button_id Button identifier
     * @return Button event (none, short press, long press)
     */
    button_event_t (*poll_button_event)(uint8_t button_id);
    
    /**
     * @brief Check if button is pressed
     * @param button_id Button identifier
     * @return true if pressed, false otherwise
     */
    bool (*is_button_pressed)(uint8_t button_id);
    
    /**
     * @brief Get button press duration
     * @param button_id Button identifier
     * @return Press duration in milliseconds
     */
    uint32_t (*get_press_duration)(uint8_t button_id);
    
    /**
     * @brief Set button debounce time
     * @param button_id Button identifier
     * @param debounce_ms Debounce time in milliseconds
     */
    void (*set_debounce_time)(uint8_t button_id, uint32_t debounce_ms);
    
    /**
     * @brief Set long press threshold
     * @param button_id Button identifier
     * @param threshold_ms Long press threshold in milliseconds
     */
    void (*set_long_press_threshold)(uint8_t button_id, uint32_t threshold_ms);
} input_interface_t;

/* Button identifiers */
#define BUTTON_POWER    0
#define BUTTON_MODE     1
#define BUTTON_UP       2
#define BUTTON_DOWN     3
#define BUTTON_SELECT   4

/* Input constants */
#define DEBOUNCE_TIME_DEFAULT_MS      50
#define LONG_PRESS_THRESHOLD_DEFAULT_MS 3000

#endif /* __INPUT_INTERFACE_H */