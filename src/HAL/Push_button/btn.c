#include "HAL/Push_button/btn.h"
#include "service/scheduler/sched.h"
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

static pull_to_val(resistance_type_t pull, uint8_t value){
    switch (pull)
    {
        case BTN_RESISTANCE_PULLUP_INTERNAL:
        case BTN_RESISTANCE_PULLUP_EXTERNAL:
            return (value == 0) ? BTN_PRESSED : BTN_NOT_PRESSED;
            break;
        case BTN_RESISTANCE_PULLDOWN_INTERNAL:
        case BTN_RESISTANCE_PULLDOWN_EXTERNAL:
            return (value == 1) ? BTN_PRESSED : BTN_NOT_PRESSED;
            break;
        default:
            return value ? BTN_PRESSED : BTN_NOT_PRESSED;
            break;
    }
}

// for (int i = 0; i < BTN_NUM_MAX; i++)
// {
//     btn_config_t btn = btn_cfg[i];
//     GPIO_PinConfig_t btn_pin_cfg = {
//         .port = btn.port,
//         .pin = btn.pin,
//         .mode = GPIO_MODE_INPUT,
//         .pull = to_gpio_pull(btn.pull),
//         .alt_function = GPIO_AF0_SYSTEM,
//         .speed = GPIO_SPEED_FAST,
//         .output_type = GPIO_OUTPUT_PUSHPULL 
//     };

//     gpio_return_t ret =  gpio_init(&btn_pin_cfg);
//     if (ret != GPIO_RES_OK) 
//         return BTN_RETURN_NOK;
// }    


static uint8_t btn_curr[BTN_NUM_MAX];
static uint8_t btn_prev[BTN_NUM_MAX];
static uint8_t btn_counter[BTN_NUM_MAX];
static uint8_t btn_state[BTN_NUM_MAX];

static void btn_callback(void* args){
    for (btn_name_t btn_name = 0; btn_name < BTN_NUM_MAX; btn_name++)
    {
        btn_config_t btn = btn_cfg[btn_name];
        uint8_t value;
        gpio_return_t ret = gpio_read(btn.port, btn.pin, &value);
        if (ret != GPIO_RES_OK){
            return BTN_RETURN_NOK;
        }

        uint8_t is_pushed = pull_to_val(btn.pull, value);
        btn_curr[btn_name] = is_pushed;
        if (btn_prev[btn_name] == btn_curr[btn_name])
        {
            btn_counter[btn_name] += 1;
        }else{
            btn_counter[btn_name] = 0;
        }
        if (btn_counter >= 5)
        {
            btn_counter[btn_name] = 0;
            btn_state[btn_name] = btn_curr[btn_name];
        }
        
        btn_prev[btn_name] = btn_curr[btn_name];
    }
}


runnable_t btn_runnable = {
    .name = "btn",
    .callback = btn_callback,
    .every = 5,
    .first_delay = 1,
    .priority = 1,
    .args = NULL
};

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

    sched_register(&btn_runnable);
    return BTN_RETURN_OK;
}




btn_return_t btn_read_state(btn_name_t name, btn_state_t *state){
    btn_config_t btn = btn_cfg[name];

    *state = btn_state[name];

    return BTN_RETURN_OK;
}