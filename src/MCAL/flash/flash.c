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
    while (FLASH->SR.BSY);
    FLASH->CR.MER = 1;
    FLASH->CR.STRT = 1;
    while (FLASH->SR.BSY);
}


void flash_sector_erase(flash_sector_t num)
{
    if (num >= FLASH_SECTOR_MAX) return; // tf you doin? 
    
    while (FLASH->SR.BSY);
    FLASH->CR.SER = 1;
    FLASH->CR.SNB = num;
    FLASH->CR.STRT = 1;
    while (FLASH->SR.BSY);
}


void flash_program()
{
    
}


