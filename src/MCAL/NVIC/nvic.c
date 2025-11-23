#include <MCAL/NVIC/nvic.h>


void NVIC_EnableIRQ(IRQn_Type IRQn){
    if (IRQn < 0)
        return;
    
    uint8_t reg = IRQn / 32;
    uint8_t pos = IRQn % 32;
    NVIC->ISER[reg] = 1 << pos;
}

void NVIC_DisableIRQ(IRQn_Type IRQn){
    if (IRQn < 0)
        return;
    
    uint8_t reg = IRQn / 32;
    uint8_t pos = IRQn % 32;
    NVIC->ICER[reg] = 1 << pos;
}

void NVIC_SetPendingIRQ(IRQn_Type IRQn){
    if (IRQn < 0)
        return;
    
    uint8_t reg = IRQn / 32;
    uint8_t pos = IRQn % 32;
    NVIC->ISPR[reg] = 1 << pos;
}

void NVIC_ClearPendingIRQ(IRQn_Type IRQn){
    if (IRQn < 0)
        return;
    
    uint8_t reg = IRQn / 32;
    uint8_t pos = IRQn % 32;
    NVIC->ICPR[reg] = 1 << pos;
}

uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn){
    uint8_t reg = IRQn / 32;
    uint8_t pos = IRQn % 32;

    return (NVIC->ISPR[reg] & (1U << pos)) ? 1U : 0;
}

void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority){
    if (IRQn < 0) return;

    //upper 4 bits are used in STM32F4
    NVIC->IPR[IRQn] = (priority & 0xF) << 4;
}

uint32_t NVIC_GetPriority(IRQn_Type IRQn){
    if (IRQn < 0) return 0;


    return (NVIC->IPR[IRQn] >> 4) & 0xF;
}