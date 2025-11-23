#pragma once
#include "led_cfg.h"

typedef enum {
    LED_RETURN_OK,
    LED_RETURN_NOK,
} led_return_t;


/**
 * @brief Initialize all LEDs based on led_cfg configuration
 * 
 * @return led_return_t 
 */
led_return_t led_init();

/**
 * @brief Turn on the specified LED
 * 
 * @param name The name of the LED to turn on
 * @return led_return_t 
 */
led_return_t led_Turn_on(led_name_t name);


/**
 * @brief Turn off the specified LED
 * 
 * @param name The name of the LED to turn off
 * @return led_return_t 
 */
led_return_t led_Turn_off(led_name_t name);


/**
 * @brief Toggle the specified LED
 * 
 * @param name The name of the LED to toggle
 * @return led_return_t 
 */
led_return_t led_Toggle(led_name_t name);

