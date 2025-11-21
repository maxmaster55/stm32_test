#include "SWITCH_interface.h"

SW_Congig_t SW_astrConfig[SWITHC_NUM] = 
{
    [EMERGENCY_SW] = 
    {
        .SW_u8Pin = GPIO_u8PORTA,
        .SW_u8Pin = GPIO_u8PIN0,
        .SW_u8ResConfig = SW_PU_INTRNL,
    },
};