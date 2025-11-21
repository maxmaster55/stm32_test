#ifndef GPIO_CONFIGURATION_H_
#define GPIO_CONFIGURATION_H_

#include "GPIO_private.h"

typedef struct
{
    uint32_t MODE;
    uint32_t OTYPE;
    uint32_t OSPEED;
    uint32_t PUPD;
    // uint16_t STATE;
    uint16_t INITIAL;
}GPIO_CONFG_t;


#endif // GPIO_CONFIGURATION_H_