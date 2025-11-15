#include "HAL/LED/led_cfg.h"


const led_config_t LED_cfg[LED_NUM_MAX] = {
    [LED_NAME_0] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = GPIO_PIN_0,
        .port = GPIOA,
        .is_PP = true
    },    
    [LED_NAME_1] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = GPIO_PIN_1,
        .port = GPIOA,
        .is_PP = true
    },
    [LED_NAME_2] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = GPIO_PIN_2,
        .port = GPIOA,
        .is_PP = true
    },    
    [LED_NAME_3] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = GPIO_PIN_3,
        .port = GPIOA,
        .is_PP = true
    },
    [LED_NAME_4] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = GPIO_PIN_4,
        .port = GPIOA,
        .is_PP = true
    },    
    [LED_NAME_5] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = GPIO_PIN_5,
        .port = GPIOA,
        .is_PP = true
    },
    [LED_NAME_6] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = GPIO_PIN_6,
        .port = GPIOA,
        .is_PP = true
    },
    [LED_NAME_7] = {
        .active_state = LED_ACTIVE_STATE_HIGH,
        .pin = GPIO_PIN_7,
        .port = GPIOA,
        .is_PP = true
    }
};
