#include "glob.h"
#include <stm32_drivers/MCAL/flash/flash.h>



void delay()
{
    for (volatile int i = 0; i < 500000; i++);
}
int main(void)
{

    flash_unlock();
    delay();
    flash_mass_erase();
    flash_lock();

    while(1);
}

