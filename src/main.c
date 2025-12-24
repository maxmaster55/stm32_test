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


spi_cfg_t spi1_cfg = {
    .spi = SPI1,
    .mode = SPI_MODE_MASTER,
    .speed = SPI_SPEED_DIV256,
    .polarity = SPI_POLARITY_LOW,
    .phase = SPI_PHASE_1EDGE,
    .data_size = SPI_DATASIZE_8BIT,
    .frame_format = SPI_FRAME_MSB_FIRST,
    .nss = SPI_NSS_SOFT,
    .protocol = SPI_PROTOCOL_MOTOROLA
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
    spi_init(&spi1_cfg);


    while (1)
    {
        for (int i = 0; i < 50000; i++)
        {
            SPI1->DR.DR = i;   
        }
        
    }
}
