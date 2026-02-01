#pragma once

#include <MCAL/GPIO/gpio.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum {
    LED_NAME_0,
    LED_NAME_1,
    LED_NAME_2,
    LED_NAME_3,
    LED_NAME_4,
    LED_NAME_5,
    LED_NAME_6,
    LED_NAME_7,
    LED_NUM_MAX
} led_name_t;

typedef enum {
    LED_ACTIVE_STATE_HIGH,
    LED_ACTIVE_STATE_LOW
} led_active_state_t;



typedef struct
{
    uint32_t pin;
    GPIO_RegDef_t *port;
    led_active_state_t active_state;
    bool is_PP;
} led_config_t ;




