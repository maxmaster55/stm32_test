#pragma once
#include "btn_cfg.h"

typedef enum {
    BTN_RETURN_OK,
    BTN_RETURN_NOK,
} btn_return_t;

typedef enum {
    BTN_PRESSED,
    BTN_NOT_PRESSED
} btn_state;


/**
 * @brief Initialize all buttons based on btn_cfg configuration
 * 
 * @return btn_return_t 
 */
btn_return_t btn_init();

/**
 * @brief Read the state of the button
 * 
 * @param name Button name from btn_name_t enum
 * @param state Pointer to store the button state (true for pressed, false for not pressed)
 * @return btn_return_t 
 */
btn_return_t btn_read_state(btn_name_t name, btn_state *state);