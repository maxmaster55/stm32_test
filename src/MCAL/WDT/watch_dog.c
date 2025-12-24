#include "MCAL/WDT/watch_dog.h"



#define WD ((watch_dog_regs_t*)0x40003000)

void wd_init(uint16_t start_from, wd_prescaler_t prescaler)
{

    WD->KR.KEY = 0x5555; // enable edit
    while (WD->SR.PVU);   // wait until writeable
    WD->PR.PR = prescaler;
    while (WD->SR.RVU);   // wait until writeable
    WD->RLR.RL = start_from;

    WD->KR.KEY = 0xAAAA;
    WD->KR.KEY = 0xCCCC; // start
}

void wd_reset()
{
    WD->KR.KEY = 0xAAAA;
}

