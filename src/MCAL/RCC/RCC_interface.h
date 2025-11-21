#ifndef RCC_INTERFACE_H_
#define RCC_INTERFACE_H_

#include "stdint.h"
#include "errorState.h"

#include "RCC_private.h"

ErrorState_t RCC_enuEnableClk(uint8_t Copy_u8Clk);

ErrorState_t RCC_enuDisableClk(uint8_t Copy_u8Clk);

ErrorState_t RCC_enuConfigurePLL(RCC_PLLCFGR *Copy_uniPLLConfig);

ErrorState_t RCC_enuSetSysClk(uint8_t Copy_u8Clk);

ErrorState_t RCC_enuSetAHB_Clk(uint8_t Copy_u8Presc);

ErrorState_t RCC_enuSetAPB_Clk(uint8_t Copy_u8Presc);

ErrorState_t RCC_enuEnablePeripheralClk(uint8_t Copy_u8Bus, uint8_t Copy_u8Peripheral);

ErrorState_t RCC_enuDisablePeripheralClk(uint8_t Copy_u8Bus, uint8_t Copy_u8Peripheral);

#endif // RCC_INTERFACE_H_