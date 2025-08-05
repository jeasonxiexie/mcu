#ifndef __AUDIO_INTERFACE_H
#define __AUDIO_INTERFACE_H

#include "../domain/types.h"

/**
 * @brief Audio Hardware Abstraction Interface
 * 
 * This interface abstracts audio hardware from business logic.
 */
typedef struct {
    /**
     * @brief Initialize audio system
     * @return true if successful, false otherwise
     */
    bool (*init)(void);
    
    /**
     * @brief Set audio mode (stereo/mono)
     * @param mode Audio mode
     */
    void (*set_mode)(audio_mode_t mode);
    
    /**
     * @brief Get current audio mode
     * @return Current audio mode
     */
    audio_mode_t (*get_mode)(void);
    
    /**
     * @brief Enable/disable mute
     * @param mute true to mute, false to unmute
     */
    void (*set_mute)(bool mute);
    
    /**
     * @brief Check if audio is muted
     * @return true if muted, false otherwise
     */
    bool (*is_muted)(void);
    
    /**
     * @brief Get left channel audio level
     * @return Audio level (0-31)
     */
    uint8_t (*get_left_level)(void);
    
    /**
     * @brief Get right channel audio level
     * @return Audio level (0-31)
     */
    uint8_t (*get_right_level)(void);
    
    /**
     * @brief Get audio status
     * @return Audio status structure
     */
    audio_status_t (*get_status)(void);
    
    /**
     * @brief Set volume level
     * @param level Volume level (0-100)
     */
    void (*set_volume)(uint8_t level);
    
    /**
     * @brief Get current volume level
     * @return Volume level (0-100)
     */
    uint8_t (*get_volume)(void);
} audio_interface_t;

/* Audio constants */
#define AUDIO_LEVEL_MAX         31
#define AUDIO_VOLUME_MAX        100
#define AUDIO_VOLUME_DEFAULT    75

#endif /* __AUDIO_INTERFACE_H */