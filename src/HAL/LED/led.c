#include "HAL/LED/led.h"


extern led_config_t LED_cfg[];

led_return_t led_init(){

    for (int i = 0; i < LED_NUM_MAX; i++)
    {
        led_config_t led = LED_cfg[i];

        GPIO_PinConfig_t led_pin_cfg = {
            .port = led.port,
            .pin = led.pin,
            .mode = GPIO_MODE_OUTPUT,
            .pull = GPIO_PULL_NO,
            .alt_function = GPIO_AF0_SYSTEM,
            .speed = GPIO_SPEED_FAST,
            .output_type = led.is_PP ? GPIO_OUTPUT_PUSHPULL : GPIO_OUTPUT_OPENDRAIN
        };

        gpio_return_t ret =  gpio_init(&led_pin_cfg);
        if (ret != GPIO_RES_OK) 
            return LED_RETURN_NOK;

    }    
    

    return LED_RETURN_OK;
}


led_return_t led_Turn_on(led_name_t name){
    led_config_t led = LED_cfg[name];

    if (led.active_state == LED_ACTIVE_STATE_HIGH)
        gpio_write(led.port, led.pin, 1);
    else
        gpio_write(led.port, led.pin, 0);
}


led_return_t led_Turn_off(led_name_t name){
        led_config_t led = LED_cfg[name];

    if (led.active_state == LED_ACTIVE_STATE_HIGH)
        gpio_write(led.port, led.pin, 0);
    else
        gpio_write(led.port, led.pin, 1);
}


led_return_t led_Toggle(led_name_t name){
    led_config_t led = LED_cfg[name];

    gpio_toggle(led.port, led.pin);
}

