#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>

GPIO_PinConfig_t pin = {
    .port = GPIOC,
    .pin = 13,
    .mode = GPIO_MODE_OUTPUT,
    .pull = GPIO_PULL_NO,
    .speed = GPIO_SPEED_HIGH,
    .output_type = GPIO_OUTPUT_PUSHPULL
};

int main(void)
{
    rcc_En_clk_preiph(RCC_GPIOC);  
    gpio_init(&pin);

    while (1)
    {
        gpio_toggle(GPIOC, 13);
        for (volatile int i = 0; i < 500000; i++);
    }
}
