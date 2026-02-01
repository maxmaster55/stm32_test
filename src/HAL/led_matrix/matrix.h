#pragma once
#include "glob.h"
#include <MCAL/GPIO/gpio.h>

typedef struct
{
    GPIO_RegDef_t* port;
    uint32_t pin;
} mat_port_pin_t;


typedef struct 
{
    mat_port_pin_t high_pins[8];
    mat_port_pin_t low_pins[8];
    uint32_t every;
} mat_cfg_t;


typedef struct
{
    uint8_t line_0;
    uint8_t line_1;
    uint8_t line_2;
    uint8_t line_3;
    uint8_t line_4;
    uint8_t line_5;
    uint8_t line_6;
    uint8_t line_7;
} mat_shape_t;



void matrix_init(mat_cfg_t *matrix_cfg);
void matrix_draw(mat_shape_t *shape);
void matrix_stop();

