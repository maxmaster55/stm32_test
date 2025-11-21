#ifndef RCC_CONFIG_H_
#define RCC_CONFIG_H_

#include "stdint.h"
#include "RCC_private.h"

typedef struct 
{
    uint8_t     SYS_CLK_SRC;
    RCC_PLLCFGR PLL_CONFIG; // union
    
}RCC_CONFIG;


#endif // RCC_CONFIG_H_