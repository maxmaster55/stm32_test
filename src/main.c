#include "glob.h"
#include <MCAL/GPIO/gpio.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/i2c/i2c.h>

void tx_do_smth(){
    while (true);
}

void rx_do_smth(){
    while (true);
}


i2c_cfg_t my_cfg = {
    .i2c = I2C1,
    .mode = I2C_MODE_MASTER,
    .freq = 100000U,
    .tx_callback = tx_do_smth,
    .rx_callback = rx_do_smth
};


uint8_t buff[2] = {};
int main(void)
{
    rcc_En_clk_preiph(RCC_GPIOB);
    i2c_init(&my_cfg);

    i2c_send(&my_cfg, 0x08, 'y');

    i2c_receive(&my_cfg, 0x08, buff);

    while(1);
}