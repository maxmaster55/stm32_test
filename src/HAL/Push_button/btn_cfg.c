#include "HAL/Push_button/btn_cfg.h"


const btn_config_t btn_cfg[BTN_NUM_MAX] = {
    [BTN_NAME_0] = {
        .pin = 8,
        .port = GPIOA,
        .pull = BTN_RESISTANCE_PULLUP_INTERNAL
    },
};
