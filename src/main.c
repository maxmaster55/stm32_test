#include "glob.h"
#include <MCAL/GPIO/gpio.h>
#include <HAL/eeprom/eeprom.h>


eeprom_cfg_t eep_cfg = {
    .i2c = I2C1,
    .address = 0x50,
    .A0 = {GPIOB, 0},
    .A1 = {GPIOB, 1},
    .A2 = {GPIOB, 2}
};

volatile char val = 0;
int main(void)
{
    eeprom_init(&eep_cfg);

    eeprom_write_blocking(&eep_cfg, 0x00, 5);
    val = eeprom_read_blocking(&eep_cfg, 0x00);

    while(1);
}

