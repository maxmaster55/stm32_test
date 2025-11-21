#include "errorState.h"

#include "../../MCAL/GPIO/GPIO_interface.h"

#include "LED_private.h"
#include "LED_configuration.h"

ErrorState_t LED_enuInit();

ErrorState_t LED_enuTurnON(uint8_t Copy_u8LedName);

ErrorState_t LED_enuTurnOFF(uint8_t Copy_u8LedName);

ErrorState_t LED_enuToggle(uint8_t Copy_u8LedName);