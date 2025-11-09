#include "rcc.h"

volatile rcc_RegDef_t *RCC = (rcc_RegDef_t *) RCC_BASE;



rcc_return_t rcc_set_SysTick(rcc_clktype_t rcc_type){
    switch (rcc_type) {
        case RCC_CLK_HSI:
            RCC->CFGR.bits.SW = 0;
            while (RCC->CFGR.bits.SWS != 0); // wait untill ready
            break;
        case RCC_CLK_HSE:
            RCC->CFGR.bits.SW = 1;
            while (RCC->CFGR.bits.SWS != 1); // wait untill ready
            break;
        case RCC_CLK_PLL:
        *(uint32_t*)((0x40000000UL + 0x00020000UL) + 0x3C00UL) &= ~0b111;
        *(uint32_t*)((0x40000000UL + 0x00020000UL) + 0x3C00UL) |= (2 & 0b111);
            RCC->CFGR.bits.SW = 2;
            while (RCC->CFGR.bits.SWS != 2); // wait untill ready
            break;
        default:
            break;
    }
    return RCC_RES_OK; // for now
}



rcc_return_t rcc_ctrlClk(rcc_clktype_t rcc_type, rcc_clkstate_t state){
    switch (rcc_type) {
        case RCC_CLK_HSI:
            RCC->CR.bits.HSION = state;
            while (RCC->CR.bits.HSIRDY != state); // wait untill ready
            break;
        case RCC_CLK_HSE:
            RCC->CR.bits.HSEON = state;
            while (RCC->CR.bits.HSERDY != state); // wait untill ready
            break;
        case RCC_CLK_PLL:
            RCC->CR.bits.PLLON = state;
            while (RCC->CR.bits.PLLRDY != state); // wait untill ready
            break;
        default:
            break;
    }


    return RCC_RES_OK; // for now
}


rcc_return_t rcc_PLL_config(rcc_PLL_config_t config){

    /* check values limits */
    
    // PLLM with 2 ≤ M ≤ 63
    if (config.M_val < 2 || config.M_val > 63) return RCC_RES_NOK;
    // PLLN with 192 ≤ N ≤ 432
    if (config.N_val < 192 || config.N_val > 432) return RCC_RES_NOK;
    // PLLP with P = 2, 4, 6, or 8
    if (config.P_val != 2 &&
        config.P_val != 4 &&
        config.P_val != 6 && config.P_val != 8 ) return RCC_RES_NOK;
    // PLLQ with 2 ≤ Q ≤ 15
    if (config.Q_val < 2 || config.Q_val > 15) return RCC_RES_NOK;
    
    /* do the work */

    RCC->PLLCFGR.bits.PLLM = config.M_val;
    RCC->PLLCFGR.bits.PLLN = config.N_val;

    // 2 --> 00, 4 --> 01, 6 --> 1, 8 --> 11
    RCC ->PLLCFGR.bits.PLLP = (config.P_val / 2) - 1;
    RCC->PLLCFGR.bits.PLLQ = config.Q_val;

    RCC->PLLCFGR.bits.PLLSRC = config.src; // PLL_HSI_SRC -> 0, etc
 
    return RCC_RES_OK;
}

