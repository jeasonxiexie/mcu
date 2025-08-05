/**
 * @file pin_config.c
 * @brief Pin configuration implementation
 * 
 * This file implements the pin configuration functions for the STM32 clean architecture.
 * It provides hardware abstraction for GPIO initialization.
 */

#include "pin_config.h"

/**
 * @brief Initialize all configured GPIO pins
 */
void GPIO_Init_All(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Initialize display control pins */
    
    /* CS Pin */
    GPIO_InitStruct.Pin = DISPLAY_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DISPLAY_CS_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(DISPLAY_CS_Port, DISPLAY_CS_Pin, GPIO_PIN_SET);
    
    /* DC Pin */
    GPIO_InitStruct.Pin = DISPLAY_DC_Pin;
    HAL_GPIO_Init(DISPLAY_DC_Port, &GPIO_InitStruct);
    
    /* RST Pin */
    GPIO_InitStruct.Pin = DISPLAY_RST_Pin;
    HAL_GPIO_Init(DISPLAY_RST_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(DISPLAY_RST_Port, DISPLAY_RST_Pin, GPIO_PIN_SET);
    
    /* Power management pins */
    
    /* Power LED */
    GPIO_InitStruct.Pin = POWER_LED_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(POWER_LED_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(POWER_LED_Port, POWER_LED_Pin, GPIO_PIN_SET);
    
    /* Power Control */
    GPIO_InitStruct.Pin = POWER_CTRL_Pin;
    HAL_GPIO_Init(POWER_CTRL_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(POWER_CTRL_Port, POWER_CTRL_Pin, GPIO_PIN_SET);
    
    /* Audio control pins */
    
    /* Audio Mute */
    GPIO_InitStruct.Pin = AUDIO_MUTE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(AUDIO_MUTE_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(AUDIO_MUTE_Port, AUDIO_MUTE_Pin, GPIO_PIN_RESET); /* Unmuted by default */
    
    /* Audio Mode Select */
    GPIO_InitStruct.Pin = AUDIO_MODE_Pin;
    HAL_GPIO_Init(AUDIO_MODE_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(AUDIO_MODE_Port, AUDIO_MODE_Pin, GPIO_PIN_RESET); /* Stereo by default */
    
    /* Input pins */
    
    /* Power Button */
    GPIO_InitStruct.Pin = BUTTON_POWER_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BUTTON_POWER_Port, &GPIO_InitStruct);
}

/**
 * @brief Initialize SPI GPIO pins
 */
void SPI_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Enable SPI1 clock */
    __HAL_RCC_SPI1_CLK_ENABLE();
    
    /* SPI1 GPIO Configuration:
       SCK, MISO, MOSI pins */
    
    /* SCK Pin */
    GPIO_InitStruct.Pin = SPI1_SCK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(SPI1_SCK_Port, &GPIO_InitStruct);
    
    /* MISO Pin */
    GPIO_InitStruct.Pin = SPI1_MISO_Pin;
    HAL_GPIO_Init(SPI1_MISO_Port, &GPIO_InitStruct);
    
    /* MOSI Pin */
    GPIO_InitStruct.Pin = SPI1_MOSI_Pin;
    HAL_GPIO_Init(SPI1_MOSI_Port, &GPIO_InitStruct);
}

/**
 * @brief Initialize ADC GPIO pins
 */
void ADC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Enable ADC1 clock */
    __HAL_RCC_ADC1_CLK_ENABLE();
    
    /* Battery voltage input pin - analog mode */
    GPIO_InitStruct.Pin = GPIO_PIN_0; /* PA0 for battery voltage */
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief Initialize Timer GPIO pins
 */
void Timer_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Enable TIM2 clock */
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    /* Backlight PWM pin */
    GPIO_InitStruct.Pin = DISPLAY_BL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
    HAL_GPIO_Init(DISPLAY_BL_Port, &GPIO_InitStruct);
}