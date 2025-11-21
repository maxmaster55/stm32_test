#ifndef LED_PRIVATE_H_
#define LED_PRIVATE_H_

#include "stdint.h"

#define LED_ACTIVE_LOW      0
#define LED_ACTIVE_HIGH     1

#define LED_ACTIVE_LOW_STTE     1  // 1 ^ STATE = !STATE
#define LED_ACTIVE_HIGH_STATE   0  // 0 ^ STATE = STATE

#define LED_OFF_STATE       0
#define LED_ON_STATE        1

typedef struct
{
    uint8_t LED_PORT;
    uint8_t LED_PIN;
    uint8_t LED_isPP; // PUSH/PULL OR OPEN-DRAIN
    uint8_t LED_ACTIVE_STATE; // ACTIVE LOW OR ACTIVE HIGH
}LED_cfg_t;

#endif // LED_PRIVATE_H_