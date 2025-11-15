#include "HAL/Push_button/btn.h"
#include <MCAL/GPIO/gpio.h>


extern btn_config_t btn_cfg[];

uint8_t to_gpio_pull(resistance_type_t resistance) {
    switch (resistance) {
        case BTN_RESISTANCE_PULLUP_INTERNAL:
            return GPIO_PULL_UP;
        case BTN_RESISTANCE_PULLDOWN_INTERNAL:
            return GPIO_PULL_DOWN;
        case BTN_RESISTANCE_PULLDOWN_EXTERNAL:
        case BTN_RESISTANCE_PULLUP_EXTERNAL:
        case BTN_RESISTANCE_NO:
        default:
            return GPIO_PULL_NO;
    }
}


btn_return_t btn_init(){

    for (int i = 0; i < BTN_NUM_MAX; i++)
    {
        btn_config_t btn = btn_cfg[i];

        GPIO_PinConfig_t btn_pin_cfg = {
            .port = btn.port,
            .pin = btn.pin,
            .mode = GPIO_MODE_INPUT,
            .pull = to_gpio_pull(btn.pull),
            .alt_function = GPIO_AF0_SYSTEM,
            .speed = GPIO_SPEED_FAST,
            .output_type = GPIO_OUTPUT_PUSHPULL 
        };

        gpio_return_t ret =  gpio_init(&btn_pin_cfg);
        if (ret != GPIO_RES_OK) 
            return BTN_RETURN_NOK;

    }    
    

    return BTN_RETURN_OK;
}




btn_return_t btn_read_state(btn_name_t name, btn_state *state){
    btn_config_t btn = btn_cfg[name];

    uint8_t value;
    gpio_return_t ret = gpio_read(btn.port, btn.pin, &value);
    if (ret != GPIO_RES_OK) 
        return BTN_RETURN_NOK;
    
    // check if pressed based on pull type
    switch (btn.pull)
    {
    case BTN_RESISTANCE_PULLUP_INTERNAL:
    case BTN_RESISTANCE_PULLUP_EXTERNAL:
        *state = (value == 0) ? BTN_PRESSED : BTN_NOT_PRESSED;
        break;
    case BTN_RESISTANCE_PULLDOWN_INTERNAL:
    case BTN_RESISTANCE_PULLDOWN_EXTERNAL:
        *state = (value == 1) ? BTN_PRESSED : BTN_NOT_PRESSED;
        break;
    default:
        *state = value ? BTN_PRESSED : BTN_NOT_PRESSED;
        break;
    }

    return BTN_RETURN_OK;
}