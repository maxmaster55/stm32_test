#include "glob.h"
#include <MCAL/GPIO/gpio.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/i2c/i2c.h>

i2c_cfg_t my_cfg = {
    .i2c = I2C1,
    .mode = I2C_MODE_MASTER,
    .freq = 100000U,
    .my_address = 0x09
};

GPIO_PinConfig_t pin_scl = {
    .port = GPIOB,
    .pin = 6,
    .mode = GPIO_MODE_ALTFN,
    .speed = GPIO_SPEED_FAST,
    .output_type = GPIO_OUTPUT_OPENDRAIN,
    .pull = GPIO_PULL_UP,
    .alt_function = GPIO_AF4_I2C1_3
};

GPIO_PinConfig_t pin_sda= {
    .port = GPIOB,
    .pin = 7,
    .mode = GPIO_MODE_ALTFN,
    .speed = GPIO_SPEED_FAST,
    .output_type = GPIO_OUTPUT_OPENDRAIN,
    .pull = GPIO_PULL_NO,
    .alt_function = GPIO_AF4_I2C1_3
};


int main(void)
{
    rcc_En_clk_preiph(RCC_GPIOB);
    gpio_return_t ret;
    ret = gpio_init(&pin_scl);
    ret = gpio_init(&pin_sda);
    i2c_init(&my_cfg);


    while (I2C1->SR2.BUSY);

    I2C1->CR1.START = 1;
    while (!I2C1->SR1.SB);

    I2C1->DR.DR = 0x08 << 1;
    while (!I2C1->SR1.ADDR);
    (void)I2C1->SR1.reg;
    (void)I2C1->SR2.reg;

    while(1);
}