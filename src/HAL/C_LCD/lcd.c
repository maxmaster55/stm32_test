#include <HAL/C_LCD/lcd.h>
#include  <MCAL/systick/systick.h>



static void lcd_pulse_en(lcd_cfg_t* lcd) {
    gpio_write(lcd->port, lcd->en_pin, 1);
    systick_wait(1);    // EN high
    gpio_write(lcd->port, lcd->en_pin, 0);
    systick_wait(1);   // EN Low
}
static void lcd_send_nibble(lcd_cfg_t* lcd, uint8_t nibble) {
    for(uint8_t i = 0; i < 4; i++) {
        gpio_write(lcd->port, lcd->d_pins[i], (nibble >> i) & 1);
    }
    lcd_pulse_en(lcd);
}

static void lcd_send_cmd_4bit(lcd_cfg_t* lcd, uint8_t cmd) {
    gpio_write(lcd->port, lcd->rs_pin, 0);   // command
    gpio_write(lcd->port, lcd->rw_pin, 0);   // write

    lcd_send_nibble(lcd, cmd >> 4);  // high nibble
    lcd_send_nibble(lcd, cmd & 0x0F); // low nibble
}


static void lcd_send_init_commands(lcd_cfg_t* lcd_cfg){
    // for 8 bits
#if LCD_MODE == LCD_MODE_8

    lcd_send_cmd(lcd_cfg, LCD_CMD_TWO_LINES | LCD_CMD_LINE_FONT_7);
    systick_wait(1);
    lcd_send_cmd(lcd_cfg, LCD_CMD_DISPLAY_ON | LCD_CMD_CURSOR_ON | LCD_CMD_CURSOR_BLINK_ON);
    systick_wait(1);
    lcd_clear_lcd(lcd_cfg);
    systick_wait(1);
    lcd_send_cmd(lcd_cfg, LCD_CMD_INCREMENT_ON);
    systick_wait(1);
#else
    systick_wait(15);
    lcd_send_cmd_4bit(lcd_cfg, 0x30);
    systick_wait(5);
    lcd_send_cmd_4bit(lcd_cfg, 0x30);
    systick_wait(1);
    lcd_send_cmd_4bit(lcd_cfg, 0x30);
    systick_wait(1);
    lcd_send_cmd_4bit(lcd_cfg, 0x20);
    systick_wait(1);
#endif
}



lcd_ret_t lcd_init(lcd_cfg_t* lcd_cfg){
    GPIO_PinConfig_t rs_cfg = {
            .port = lcd_cfg->port,
            .pin = lcd_cfg->rs_pin,
            .mode = GPIO_MODE_OUTPUT,
            .pull = GPIO_PULL_NO,
            .alt_function = GPIO_AF0_SYSTEM,
            .speed = GPIO_SPEED_FAST,
            .output_type = GPIO_MODE_OUTPUT
    };
        
    GPIO_PinConfig_t en_cfg = {
        .port = lcd_cfg->port,
        .pin = lcd_cfg->en_pin,
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NO,
        .alt_function = GPIO_AF0_SYSTEM,
        .speed = GPIO_SPEED_FAST,
        .output_type = GPIO_MODE_OUTPUT
    };
    
    GPIO_PinConfig_t rw_cfg = {
        .port = lcd_cfg->port,
        .pin = lcd_cfg->rw_pin,
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NO,
        .alt_function = GPIO_AF0_SYSTEM,
        .speed = GPIO_SPEED_FAST,
        .output_type = GPIO_MODE_OUTPUT
    };

    gpio_init(&rs_cfg);
    gpio_init(&en_cfg);
    gpio_init(&rw_cfg);
    
#if LCD_MODE == LCD_MODE_8
        
    for (uint8_t i = 0; i < LCD_MODE_8; i++)
    {
        GPIO_PinConfig_t pin_cfg = {
            .port = lcd_cfg->port,
            .pin = lcd_cfg->d_pins[i],
            .mode = GPIO_MODE_OUTPUT,
            .pull = GPIO_PULL_NO,
            .alt_function = GPIO_AF0_SYSTEM,
            .speed = GPIO_SPEED_FAST,
            .output_type = GPIO_MODE_OUTPUT
        };
        
        gpio_init(&pin_cfg);
    }

#else
    for (uint8_t i = 0; i < LCD_MODE_4; i++)
    {
        GPIO_PinConfig_t pin_cfg = {
            .port = lcd_cfg->port,
            .pin = lcd_cfg->d_pins[i],
            .mode = GPIO_MODE_OUTPUT,
            .pull = GPIO_PULL_NO,
            .alt_function = GPIO_AF0_SYSTEM,
            .speed = GPIO_SPEED_FAST,
            .output_type = GPIO_MODE_OUTPUT
        };
        
        gpio_init(&pin_cfg);
    }
#endif

    lcd_send_init_commands(lcd_cfg);
    return LCD_RET_OK;
}




lcd_ret_t lcd_write_char(lcd_cfg_t* lcd_cfg, char c){
    gpio_write(lcd_cfg->port, lcd_cfg->rs_pin, LCD_RS_DATA);
    gpio_write(lcd_cfg->port, lcd_cfg->rw_pin, LCD_RW_WRITE);


#if LCD_MODE == LCD_MODE_8
    for (uint8_t i = 0; i < LCD_MODE_8; i++)
    {
        uint8_t d = (c >> i) & 1;
        gpio_write(lcd_cfg->port, lcd_cfg->d_pins[i], d);
    }
    
#else
    lcd_send_nibble(lcd_cfg, c >> 4);  // high nibble
    lcd_send_nibble(lcd_cfg, c & 0x0F); // low nibble
#endif
}



lcd_ret_t lcd_write_string(lcd_cfg_t* lcd_cfg, char* str){

    while(*str) {
        lcd_write_char(lcd_cfg, *str++);
    }

    return LCD_RET_OK;
}


lcd_ret_t lcd_clear_lcd(lcd_cfg_t* lcd_cfg){
#if LCD_MODE == LCD_MODE_8
    lcd_send_cmd(lcd_cfg, 0x01);
    systick_wait(1);
#else
// later
#endif
    return LCD_RET_OK;
}



lcd_ret_t lcd_set_cursor_pos(lcd_cfg_t* lcd_cfg, uint8_t row, uint8_t col){
#if LCD_MODE == LCD_MODE_8


#else
// later
#endif
}


lcd_ret_t lcd_save_custom_char(lcd_cfg_t* lcd_cfg, uint8_t* custom, uint8_t index){
#if LCD_MODE == LCD_MODE_8


#else
// later
#endif
}



