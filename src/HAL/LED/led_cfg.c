#include "HAL/LED/led_cfg.h"


const led_config_t LED_cfg[LED_NUM_MAX] = {
    [LED_NAME_0] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = 0,
        .port = GPIOA,
        .is_PP = true
    },    
    [LED_NAME_1] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = 1,
        .port = GPIOA,
        .is_PP = true
    },
    [LED_NAME_2] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = 2,
        .port = GPIOA,
        .is_PP = true
    },    
    [LED_NAME_3] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = 3,
        .port = GPIOA,
        .is_PP = true
    },
    [LED_NAME_4] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = 4,
        .port = GPIOA,
        .is_PP = true
    },    
    [LED_NAME_5] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = 5,
        .port = GPIOA,
        .is_PP = true
    },
    [LED_NAME_6] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = 6,
        .port = GPIOA,
        .is_PP = true
    },
    [LED_NAME_7] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = 7,
        .port = GPIOA,
        .is_PP = true
    }
};
