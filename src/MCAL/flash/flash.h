#pragma once

#include "flash_regs.h"


#define FLASH ((flash_regs_t*)0x40023C00)


typedef enum{
    FLASH_SECTOR_0,
    FLASH_SECTOR_1,
    FLASH_SECTOR_2,
    FLASH_SECTOR_3,
    FLASH_SECTOR_4,
    FLASH_SECTOR_5,
    FLASH_SECTOR_MAX,
} flash_sector_t ;




void flash_unlock();
void flash_lock();
void flash_mass_erase();
void flash_sector_erase(flash_sector_t num);
void flash_program();

