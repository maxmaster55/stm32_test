#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>


#define LED_PIN 13


GPIO_PinConfig_t led_pin_cfg = {
    .port = GPIOA,
    .pin = (1U << LED_PIN),
    .mode = GPIO_MODE_OUTPUT,          // Output mode
    .pull = GPIO_PULL_NO,          // No pull-up, no pull-down
    .alt_function = GPIO_AF0_SYSTEM,  // Not used in output mode
    .speed = GPIO_SPEED_HIGH,         // Medium speed
    .output_type = GPIO_OUTPUT_PUSHPULL    // Push-pull
};

// TODO: test i2c 


void delay(void) {
    for (volatile int i = 0; i < 500000; i++);
}

int main(void) {
    

    int shift = 1;
    while (1) {
        // Turn on all pins in port A
        GPIOA->ODR <<= shift;

        delay();
    }
    return 0;
}
