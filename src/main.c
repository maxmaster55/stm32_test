#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <HAL/LED/led.h>



void delay(void) {
    for (volatile int i = 0; i < 500000; i++);
}


GPIO_PinConfig_t led_pin_cfg = {
    .port = GPIOC,
    .pin = GPIO_PIN_13,
    .mode = GPIO_MODE_OUTPUT,
    .pull = GPIO_PULL_NO,
    .alt_function = 0x00,
    .speed = 0x02,
    .output_type = 0x00
};

int main(void) {


    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);
    led_init();



    while (1) { 
        uint32_t led_mask = 0;
        for (led_name_t led = LED_NAME_0; led < LED_NUM_MAX; led++) {
            led_mask |= (1 << led);
            led_Turn_on(led);
            delay();
            led_Turn_off(led);
            delay();
        }
    }

    return 0;
}
