#include "GPIO_config.h"

// make it constant to take space in flash(.rodata section)
// save space on RAM
const GPIO_CONFG_t GPIO_astrConfig[GPIO_u8PORT_MAX] = 
{
    {
        .MODE = GPIO_OUTPUT,
        .OTYPE = GPIO_PUSHPULL,
        .OSPEED = GPIO_LOWSPEED,
        .PUPD = GPIO_FLOATING,
        .INITIAL = 0xffffffff, // HIGH
    },
    
    {
        .MODE = GPIO_OUTPUT,
        .OTYPE = GPIO_PUSHPULL,
        .OSPEED = GPIO_LOWSPEED,
        .PUPD = GPIO_FLOATING,
        .INITIAL = 0xff00ff00,
    },

    {
        .MODE = GPIO_INPUT,
        .OTYPE = GPIO_PUSHPULL,
        .OSPEED = GPIO_LOWSPEED,
        .PUPD = GPIO_PULLUP,
        .INITIAL = 0x00000000,
    },
    
    {
        .MODE = GPIO_INPUT,
        .OTYPE = GPIO_PUSHPULL,
        .OSPEED = GPIO_LOWSPEED,
        .PUPD = GPIO_PULLDOWN,
        .INITIAL = 0x00000000,
    },

    {
        .MODE = GPIO_ALTFNC,
        .OTYPE = GPIO_PUSHPULL,
        .OSPEED = GPIO_HIGHSPEED,
        .PUPD = GPIO_PULLUP,
        .INITIAL = 0xffffffff,
    },

    {
        .MODE = GPIO_ANALOG,
        .OTYPE = GPIO_PUSHPULL,
        .OSPEED = GPIO_LOWSPEED,
        .PUPD = GPIO_PULLUP,
        .INITIAL = 0x00000000,
    }
};