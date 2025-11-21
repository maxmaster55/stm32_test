#ifndef SW_INTERFACE_H_
#define SW_INTERFACE_H_

#include "stdint.h"
#include "errorState.h"

#include "../../MCAL/GPIO/GPIO_interface.h"

#include "SWITCH_private.h"
#include "SWITCH_configuration.h"

ErrorState_t SW_enuInit();

ErrorState_t SW_enuReadState(uint8_t Copy_u8SwitchName, uint8_t *Copy_u8Value);

#endif // SW_INTERFACE_H_