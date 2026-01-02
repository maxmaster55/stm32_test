#include "glob.h"
#include <MCAL/GPIO/gpio.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/i2c/i2c.h>


volatile bool tx_done = false;
volatile bool rx_done = false;

void tx_do_smth(){
    tx_done = true;
}

void rx_do_smth(uint8_t* data, uint16_t len){
    rx_done = true;
}

uint8_t tx_buff[2] = { 'y', 'x' };
uint8_t rx_buff[1]  = { 0 };

i2c_cfg_t my_cfg = {
    .i2c = I2C1,
    .mode = I2C_MODE_MASTER,
    .freq = 100000U,
    .tx_cb = tx_do_smth,
    .rx_cb = rx_do_smth
};


int main(void)
{
    rcc_En_clk_preiph(RCC_GPIOB);
    i2c_init(&my_cfg);
    i2c_send(&my_cfg, 0x08, tx_buff, 2);

    while (i2c_is_busy(&my_cfg));
    
    i2c_receive(&my_cfg, 0x08, rx_buff, 1);
    


    while(1);
}

