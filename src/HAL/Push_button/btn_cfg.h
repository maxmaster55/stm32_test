#pragma once

#include <MCAL/GPIO/gpio.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum {
    BTN_NAME_0,
    BTN_NUM_MAX
} btn_name_t;

typedef enum {
    BTN_RESISTANCE_PULLUP_INTERNAL,
    BTN_RESISTANCE_PULLDOWN_INTERNAL,
    BTN_RESISTANCE_PULLUP_EXTERNAL,
    BTN_RESISTANCE_PULLDOWN_EXTERNAL,
    BTN_RESISTANCE_NO
} resistance_type_t;



typedef struct
{
    uint32_t pin;
    GPIO_RegDef_t *port;
    resistance_type_t pull;
} btn_config_t ;




