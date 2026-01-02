#pragma once

#include "glob.h"
#include "MCAL/GPIO/gpio.h"
#include "MCAL/i2c/i2c.h"


typedef struct{
    GPIO_RegDef_t* port; 
    uint8_t pin; 
} eeprom_a_pin_t;

typedef struct 
{
    i2c_regs_t* i2c;
    uint8_t address;
    eeprom_a_pin_t A0;
    eeprom_a_pin_t A1;
    eeprom_a_pin_t A2;
} eeprom_cfg_t;




void eeprom_init(eeprom_cfg_t* cfg);
// blocking api
void eeprom_write_blocking(eeprom_cfg_t* cfg, uint8_t mem_addr, uint8_t to_write);
uint8_t eeprom_read_blocking(eeprom_cfg_t* cfg, uint8_t mem_addr);
// async api
void eeprom_write(eeprom_cfg_t* cfg, uint8_t mem_addr, uint8_t to_write);
uint8_t eeprom_read(eeprom_cfg_t* cfg, uint8_t mem_addr);
void eeprom_write_and_verify(eeprom_cfg_t* cfg, uint8_t mem_addr, uint8_t to_write);

bool eeprom_is_done(eeprom_cfg_t* cfg);










