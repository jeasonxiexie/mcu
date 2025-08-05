/**
 * @file resources.c
 * @brief Resource management implementation
 * 
 * This file implements the resource registry and helper functions
 * for managing generated image assets.
 */

#include "resources.h"
#include <string.h>

/* Example resource registry - update this when you add new assets */
const resource_info_t resource_registry[] = {
    /* Static images */
    {
        .name = "boot_logo",
        .width = 80,
        .height = 160, 
        .data = NULL,  /* Will be set to boot_logo_data when available */
        .is_animated = 0,
        .frame_count = 1
    },
    {
        .name = "shutdown_logo", 
        .width = 80,
        .height = 160,
        .data = NULL,  /* Will be set to shutdown_logo_data when available */
        .is_animated = 0,
        .frame_count = 1
    },
    
    /* Animated images */
    {
        .name = "meter_af",
        .width = 80,
        .height = 160,
        .data = NULL,  /* Will be set to meter_af_frame_ptrs when available */
        .is_animated = 1,
        .frame_count = 12
    }
    
    /* Add more resources here as needed */
};

const uint8_t resource_count = sizeof(resource_registry) / sizeof(resource_info_t);

/**
 * @brief Find a resource by name
 * @param name Resource name to search for
 * @return Pointer to resource info, or NULL if not found
 */
const resource_info_t* find_resource(const char* name)
{
    if (name == NULL) {
        return NULL;
    }
    
    for (uint8_t i = 0; i < resource_count; i++) {
        if (strcmp(resource_registry[i].name, name) == 0) {
            return &resource_registry[i];
        }
    }
    
    return NULL;
}

/**
 * @brief List all available resources (for debugging)
 */
void list_all_resources(void)
{
    for (uint8_t i = 0; i < resource_count; i++) {
        const resource_info_t* res = &resource_registry[i];
        
        /* Note: This would normally print to UART or debug output */
        /* For now, just a placeholder implementation */
        (void)res;  /* Suppress unused warning */
        
        /* Example debug output (implement according to your debug system):
        debug_printf("Resource %d: %s (%dx%d, %s, %d frames)\n",
                    i, res->name, res->width, res->height,
                    res->is_animated ? "animated" : "static",
                    res->frame_count);
        */
    }
}

/**
 * @brief Get resource data safely with bounds checking
 * @param resource Pointer to resource info
 * @param frame_index Frame index (0 for static images)
 * @return Pointer to image data, or NULL if invalid
 */
const void* get_resource_data(const resource_info_t* resource, uint8_t frame_index)
{
    if (resource == NULL || resource->data == NULL) {
        return NULL;
    }
    
    if (resource->is_animated) {
        if (frame_index >= resource->frame_count) {
            return NULL;
        }
        /* For animated resources, data points to array of frame pointers */
        const void** frame_ptrs = (const void**)resource->data;
        return frame_ptrs[frame_index];
    } else {
        /* For static resources, ignore frame_index and return data directly */
        return resource->data;
    }
}

/**
 * @brief Calculate total memory usage of all resources
 * @return Total memory usage in bytes
 */
uint32_t calculate_total_resource_memory(void)
{
    uint32_t total = 0;
    
    for (uint8_t i = 0; i < resource_count; i++) {
        const resource_info_t* res = &resource_registry[i];
        
        if (res->data != NULL) {
            uint32_t pixels_per_frame = res->width * res->height;
            uint32_t bytes_per_pixel = 2;  /* Assuming RGB565 format */
            uint32_t total_frames = res->is_animated ? res->frame_count : 1;
            
            total += pixels_per_frame * bytes_per_pixel * total_frames;
        }
    }
    
    return total;
}