#include "rcc.h"


void rcc_set_SysTick(rcc_clktype_t rcc){
    switch (rcc) {
        case RCC_CLK_HSI:
            RCC->CR.bits.HSION = 1;
            while (!RCC->CR.bits.HSIRDY); // wait untill ready
            RCC->CFGR.bits.SW = 1;
            while (RCC->CFGR.bits.SWS != 0x00); // wait untill ready

            break;
        case RCC_CLK_HSE:
            RCC->CR.bits.HSEON = 1;
            while (!RCC->CR.bits.HSERDY); // wait untill ready
            RCC->CFGR.bits.SW = 1;
            while (!RCC->CFGR.bits.SWS == 0x01); // wait untill ready
            break;
        case RCC_CLK_PLL:
        
            break;
    }
}