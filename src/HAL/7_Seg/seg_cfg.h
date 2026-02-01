#pragma once
#include <MCAL/GPIO/gpio.h>

typedef enum {
    SEGMENT_1,
    SEGMENT_MAX_NUM
} seg_name_t;

typedef enum{
    SEGMENT_COMMON_ANODE,     // turn pin off to work
    SEGMENT_COMMON_CATHODE    // turn pin on to work
} seg_type_t;


typedef struct
{
    GPIO_RegDef_t *port;
    seg_type_t type;
    uint8_t inital_val;
}seg_cfg_t;

