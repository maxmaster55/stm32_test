#include "led_cfg.h"

typedef enum {
    LED_RETURN_OK,
    LED_RETURN_NOK,
} led_return_t;



led_return_t led_init();
led_return_t led_Turn_on(led_name_t name);
led_return_t led_Turn_off(led_name_t name);
led_return_t led_Toggle(led_name_t name);

