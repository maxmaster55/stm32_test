#ifndef SWITCH_PRIVATE_H_
#define SWITCH_PRIVATE_H_

#include "stdint.h"

typedef enum
{
    SW_FLOATING = GPIO_FLOATING,
    SW_PU_INTRNL = GPIO_PULLUP,
    SW_PD_INTRNL = GPIO_PULLDOWN,
    SW_PU_EXTRNL,
    SW_PD_EXTRNL,
    SW_RESCNFG_NUM,
}ResConfig_t; // RESISTOR 

typedef struct
{
    uint8_t SW_u8Port;
    uint8_t SW_u8Pin;
    uint8_t SW_u8ResConfig;
}SW_Congig_t;


#endif // SWITCH_PRIVATE_H_