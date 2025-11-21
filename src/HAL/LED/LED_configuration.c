#include "../../MCAL/RCC/RCC_interface.h"


#include "LED_interface.h"

// must have configuration example
// must document each configuration different possible values

const LED_cfg_t LED_astrcfg[LED_NUM] = 
{
    [LED_WARNING] = {
        .LED_ACTIVE_STATE = LED_ACTIVE_LOW,
        .LED_PORT = GPIO_u8PORTA,
        .LED_PIN = 0,
        .LED_isPP = 1
    },

    [LED_DOOROPEN] = {
        .LED_ACTIVE_STATE = LED_ACTIVE_HIGH,
        .LED_PIN = 2,
        .LED_PORT = GPIO_u8PORTC,
        .LED_isPP = 1
    },

    [LED_ERROR] = {
        .LED_PORT = GPIO_u8PORTC,
        .LED_PIN = 0,
        .LED_isPP = 1,
        .LED_ACTIVE_STATE = LED_ACTIVE_HIGH
    }
};