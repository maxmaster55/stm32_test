#pragma once
#include "seg_cfg.h"
#include <MCAL/GPIO/gpio.h>


typedef enum{
    SEGMENT_OK,
    SEGMENT_NOK
} seg_ret_t;


seg_ret_t segment_display_init();
seg_ret_t segment_write(seg_name_t name, uint8_t val);
seg_ret_t segment_get_val(seg_name_t name, uint8_t *val);
seg_ret_t segment_en_dot(seg_name_t name);
seg_ret_t segment_disable_dot(seg_name_t name);
seg_ret_t segment_toggle_dot(seg_name_t name);