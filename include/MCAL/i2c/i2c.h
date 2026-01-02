#pragma once

#include "i2c_regs.h"


#define I2C1 ((i2c_regs_t*)0x40005400)
#define I2C2 ((i2c_regs_t*)0x40005800)
#define I2C3 ((i2c_regs_t*)0x40005C00)

#define NORMAL_MODE_MAX (100000)
typedef enum{
    I2C_MODE_MASTER,
    I2C_MODE_SLAVE
} i2c_mode_t;


typedef (*i2c_callback) (void);

typedef enum{
    I2C_OK,
    I2C_NOK
} i2c_ret_t;

// inst
typedef struct
{
    i2c_regs_t* i2c;
    i2c_mode_t mode;
    uint32_t freq;
    uint8_t my_address;
    // clock stretching
    // interrupts
    i2c_callback rx_callback;
    i2c_callback tx_callback;

} i2c_cfg_t;



// api
void i2c_init(i2c_cfg_t* cfg);
void i2c_send(i2c_cfg_t* cfg, uint8_t address, uint8_t data);
void i2c_receive(i2c_cfg_t* cfg, uint8_t* data);
