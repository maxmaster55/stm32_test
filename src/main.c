#include <MCAL/GPIO/gpio.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/SPI/spi.h>

// gpio conf for spi1
GPIO_PinConfig_t NSS_pin = {
    .port = GPIOA,
    .pin = 4,
    .mode = GPIO_MODE_OUTPUT,
    .pull = GPIO_PULL_NO,
    .alt_function = GPIO_AF0_SYSTEM,
    .speed = GPIO_SPEED_HIGH,
    .output_type = GPIO_OUTPUT_PUSHPULL
};

GPIO_PinConfig_t SCK_pin = {
    .port = GPIOA,
    .pin = 5,
    .mode = GPIO_MODE_ALTFN,
    .pull = GPIO_PULL_NO,
    .alt_function = GPIO_AF5_SPI1_2,
    .speed = GPIO_SPEED_HIGH,
    .output_type = GPIO_OUTPUT_PUSHPULL
};
GPIO_PinConfig_t MISO_pin = {
    .port = GPIOA,
    .pin = 6,
    .mode = GPIO_MODE_ALTFN,
    .pull = GPIO_PULL_NO,
    .alt_function = GPIO_AF5_SPI1_2,
    .speed = GPIO_SPEED_HIGH,
    .output_type = GPIO_OUTPUT_PUSHPULL
};
GPIO_PinConfig_t MOSI_pin = {
    .port = GPIOA,
    .pin = 7,
    .mode = GPIO_MODE_ALTFN,
    .pull = GPIO_PULL_NO,
    .alt_function = GPIO_AF5_SPI1_2,
    .speed = GPIO_SPEED_HIGH,
    .output_type = GPIO_OUTPUT_PUSHPULL
};



// will use spi1 to test

int main(void)
{
    rcc_En_clk_preiph(RCC_GPIOA);
    gpio_init(&NSS_pin);
    gpio_write(GPIOA, 4, 0); // always select the chip
    
    gpio_init(&SCK_pin);
    gpio_init(&MISO_pin);
    gpio_init(&MOSI_pin);
    
    rcc_En_clk_preiph(RCC_SPI1);
    SPI1->CR_1.BR = 1; // slowest speed CLK/256
    SPI1->CR_1.MSTR = 1; // i am the master now

    SPI1->CR_1.BIDIMODE = 0;
    SPI1->CR_1.RXONLY = 0;
    SPI1->CR_1.CPOL = 0;
    SPI1->CR_1.CPHA = 0;
   
    SPI1->CR_1.SSM = 1;  // software slave management
    SPI1->CR_1.SSI = 1;  // internal NSS = HIGH


    SPI1->CR_1.SPE = 1; // enable


    SPI1->DR.DR = 0xDE;

    while (1)
    {

    }
}
