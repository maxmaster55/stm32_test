#pragma once
#include <glob.h>


typedef struct watch_dog
{
    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t KEY           :16;
            volatile uint32_t res0          :16;

        };
    } KR;
    

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t PR             :3;
            volatile uint32_t res0          :29;

        };
    } PR;
    

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t RL            :12;
            volatile uint32_t res0          :20;

        };
    } RLR;

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t PVU          :1;
            volatile uint32_t RVU          :1;
            volatile uint32_t res0         :30;

        };
    } SR;
    

}watch_dog_regs_t;



typedef enum{
    WD_PRESCALER_DIV4,
    WD_PRESCALER_DIV8,
    WD_PRESCALER_DIV16,
    WD_PRESCALER_DIV32,
    WD_PRESCALER_DIV64,
    WD_PRESCALER_DIV128,
    WD_PRESCALER_DIV256
} wd_prescaler_t;


void wd_init(uint16_t start_from, wd_prescaler_t prescaler);
void wd_reset();