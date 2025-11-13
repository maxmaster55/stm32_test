#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>


#define LED_PIN 13


void delay(void) {
    for (volatile int i = 0; i < 50000; i++);
}


GPIO_PinConfig_t led_pin_cfg = {
    .port = GPIOC,
    .pin = (1 << LED_PIN),
    .mode = GPIO_MODE_OUTPUT,
    .pull = GPIO_PULL_NO,
    .alt_function = 0x00,
    .speed = 0x02,
    .output_type = 0x00
};

int main(void) {


    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOC);

    // Set PC13 as output (01)
    gpio_init(&led_pin_cfg);

    while (1) { 
        // toggle LED
        gpio_toggle(GPIOC, GPIO_PIN_13);
        delay();
    }
    return 0;
}
