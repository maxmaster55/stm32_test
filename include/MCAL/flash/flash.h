#pragma once

#include "flash_regs.h"

#define FLASH ((flash_regs_t*)0x40023C00)




void flash_unlock();
void flash_lock();
void flash_mass_erase();
void flash_sector_erase();
void flash_program();

