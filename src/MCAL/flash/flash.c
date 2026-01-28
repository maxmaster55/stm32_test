#include <MCAL/flash/flash.h>


void flash_unlock()
{
    FLASH->KEYR.KEY = 0x45670123;
    FLASH->KEYR.KEY = 0xCDEF89AB;
}


void flash_lock()
{
    while (FLASH->SR.BSY);
    FLASH->CR.LOCK = 1;
}


void flash_mass_erase()
{
    
}


void flash_sector_erase()
{
    
}


void flash_program()
{
    
}


