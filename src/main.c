#include <MCAL/GPIO/gpio.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/SPI/spi.h>
#include <MCAL/WDT/watch_dog.h>


GPIO_PinConfig_t pin = {
    .port = GPIOC,
    .pin = 13,
    .mode = GPIO_MODE_OUTPUT,
    .pull = GPIO_PULL_NO,
    .speed = GPIO_SPEED_HIGH,
    .output_type = GPIO_OUTPUT_PUSHPULL
};


int counter = 0;
int main(void)
{    
    rcc_En_clk_preiph(RCC_GPIOC);  
    gpio_init(&pin);


    rcc_enable_LSI();
    wd_init(10000, WD_PRESCALER_DIV32);
    
    while (1)
    {
    if (counter == 50000)
    {
        gpio_write(GPIOC,13, 1);
    }
    counter += 1;
    }
}
