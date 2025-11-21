#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

#include "stdint.h"
#include "stddef.h" // NULL def
#include "errorState.h"

#include "GPIO_config.h"
#include "GPIO_private.h"

ErrorState_t GPIO_enuInit();

ErrorState_t GPIO_enuSetPortDir(uint8_t Copyu8PortID, uint8_t Copy_u8Dir);

ErrorState_t GPIO_enuSetPortValue(uint8_t Copyu8PortID, uint8_t Copy_u8Value);

ErrorState_t GPIO_enuFlipPortValue(uint8_t Copyu8PortID);


ErrorState_t GPIO_enuSetPinDir(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8Dir);

ErrorState_t GPIO_enuSetPinValue(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8Value);

ErrorState_t GPIO_enuFlipPinValue(uint8_t Copyu8PortID, uint8_t Copy_u8Pin);

ErrorState_t GPIO_enuGetPinValue(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t *Copy_u8Value);

ErrorState_t GPIO_enuSetOutputType(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8Type);

ErrorState_t GPIO_enuSetPinSpeed(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8Speed);

ErrorState_t GPIO_enuSetPinPUPD(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8PullType);

ErrorState_t GPIO_enuCnfgAltFunc(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8AltFunc);

#endif // GPIO_INTERFACE_H_