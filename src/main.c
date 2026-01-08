#include "glob.h"
#include <service/scheduler/sched.h>
#include <MCAL/GPIO/gpio.h>
#include <HAL/eeprom/eeprom.h>


eeprom_cfg_t eep_cfg = {
    .i2c = I2C1,
    .address = 0x50,
    .A0 = {GPIOB, 0},
    .A1 = {GPIOB, 1},
    .A2 = {GPIOB, 2}
};


int val = 0;

void edit_val(uint8_t data){
    val = data;
}

void tester_cb(void* args){

    if (!eeprom_is_done()) return;
    eeprom_read(&eep_cfg, 0x00, edit_val);
}

runnable_t tester = {
    .name = "tester",
    .callback = tester_cb,
    .every = 6,
    .first_delay = 0,
    .priority = 2,
    .args = NULL
};


int main(void)
{
    sched_init(1);
    eeprom_init(&eep_cfg);

    eeprom_write(&eep_cfg, 0x00, 67, NULL);
    sched_register(&tester);
    sched_start();


    while(1);
}

