#include "MCAL/RCC/RCC_interface.h"

int main()
{
	RCC_enuEnableClk(RCC_HSI);
	// RCC_enuEnableClk(RCC_HSE);
	// RCC_enuEnableClk(RCC_PLL);
	// RCC_enuEnableClk(RCC_PLL_I2S);

	RCC_enuDisableClk(RCC_HSI);
	// RCC_enuDisableClk(RCC_HSE);
	// RCC_enuDisableClk(RCC_PLL);
	// RCC_enuDisableClk(RCC_PLL_I2S);

	// RCC_enuEnableClk(RCC_HSI);
	// RCC_enuSetSysClk(RCC_HSI);
	// RCC_enuEnablePeripheralClk(AHB1, GPIOA);
	
  return 0;
}