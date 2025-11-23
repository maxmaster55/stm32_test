#include <HAL/7_Seg/seg_display.h>


extern seg_cfg_t SEG_cfg[];

uint8_t seg_lut[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // B (uppercase)
    0x39, // C
    0x5E, // D
    0x79, // E
    0x71, // F
    0x40, // - (only segment G)
    0x00  // blank
};

seg_ret_t segment_display_init(){
    for (int i = 0; i < SEGMENT_MAX_NUM; i++){
        seg_cfg_t cfg = SEG_cfg[i];
        for (uint16_t i = 0; i < 8; i++)
        {     
            GPIO_PinConfig_t pin_cfg = {
                .port = cfg.port,
                .pin = (1 << i), // pin mask
                .mode = GPIO_MODE_OUTPUT,
                .pull = GPIO_PULL_NO,
                .alt_function = GPIO_AF0_SYSTEM,
                .speed = GPIO_SPEED_FAST,
                .output_type = GPIO_OUTPUT_PUSHPULL
            };
            gpio_init(&pin_cfg);
        }
    }
}
seg_ret_t segment_write(seg_name_t name, uint8_t val){
    seg_cfg_t cfg = SEG_cfg[name];
    if (cfg.type == SEGMENT_COMMON_ANODE)
    {
        gpio_set_port_value(cfg.port, ~seg_lut[val]);
    }else{
        gpio_set_port_value(cfg.port, seg_lut[val]);
    }
    return SEGMENT_OK;
}

seg_ret_t segment_get_val(seg_name_t name, uint8_t *val){
    seg_cfg_t cfg = SEG_cfg[name];
    gpio_get_port_value(cfg.port, val);
    return SEGMENT_OK;
}
seg_ret_t segment_en_dot(seg_name_t name) {
    seg_cfg_t cfg = SEG_cfg[name];
    uint8_t curr = 0;
    volatile uint8_t test = 9;
    segment_write(name, test);
    segment_get_val(name, &curr);

    if (cfg.type == SEGMENT_COMMON_ANODE) {
        curr &= ~GPIO_PIN_7;     // dot ON (active low)
    } else { // COMMON CATHODE
        curr |= GPIO_PIN_7;      // dot ON (active high)
    }
    
    gpio_set_port_value(cfg.port, curr);
    return SEGMENT_OK;
}

seg_ret_t segment_disable_dot(seg_name_t name);
seg_ret_t segment_toggle_dot(seg_name_t name);