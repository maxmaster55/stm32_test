#include <MCAL/systick/systick.h>
#include <HAL/C_LCD/lcd.h>

static void lcd_pulse_en(lcd_cfg_t* lcd_cfg)
{
    gpio_write(lcd_cfg->port, lcd_cfg->en_pin, 1);
    systick_wait(1);
    gpio_write(lcd_cfg->port, lcd_cfg->en_pin, 0);
    systick_wait(1);
}

static void lcd_write_nibble(lcd_cfg_t* lcd_cfg, uint8_t nibble)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t bit = (nibble >> i) & 0x01;
        gpio_write(lcd_cfg->port, lcd_cfg->d_pins[i], bit);
    }
    lcd_pulse_en(lcd_cfg);
}

static void lcd_write_byte(lcd_cfg_t* lcd_cfg, uint8_t data)
{
#if LCD_MODE == LCD_MODE_4
    // Send high nibble
    lcd_write_nibble(lcd_cfg, (data >> 4) & 0x0F);
    // Send low nibble
    lcd_write_nibble(lcd_cfg, data & 0x0F);

#elif LCD_MODE == LCD_MODE_8
    // Direct write to all 8 pins
    for (uint8_t i = 0; i < 8; i++)
    {
        uint8_t bit = (data >> i) & 0x01;
        gpio_write_pin(lcd_cfg->port, lcd_cfg->d_pins[i], bit);
    }
    lcd_pulse_en(lcd_cfg);

#else
#error "Invalid LCD_MODE! Must be LCD_MODE_4 or LCD_MODE_8"
#endif
}

void lcd_send_cmd_bit(lcd_cfg_t* lcd_cfg, uint8_t cmd)
{
    // RS = 0 → Command
    gpio_write(lcd_cfg->port, lcd_cfg->rs_pin, 0);
    // RW = 0 → Write
    gpio_write(lcd_cfg->port, lcd_cfg->rw_pin, 0);

    lcd_write_byte(lcd_cfg, cmd);
}

void lcd_send_data_bit(lcd_cfg_t* lcd_cfg, uint8_t data)
{
    // RS = 1 → Data
    gpio_write(lcd_cfg->port, lcd_cfg->rs_pin, 1);
    // RW = 0 → Write
    gpio_write(lcd_cfg->port, lcd_cfg->rw_pin, 0);

    lcd_write_byte(lcd_cfg, data);
}


lcd_ret_t lcd_init(lcd_cfg_t* lcd_cfg)
{
    GPIO_PinConfig_t cfg;

    cfg.port = lcd_cfg->port;
    cfg.mode = GPIO_MODE_OUTPUT;
    cfg.pull = GPIO_PULL_NO;
    cfg.alt_function = GPIO_AF0_SYSTEM;
    cfg.speed = GPIO_SPEED_FAST;
    cfg.output_type = GPIO_OUTPUT_PUSHPULL;
    
    cfg.pin = lcd_cfg->rs_pin; gpio_init(&cfg);
    cfg.pin = lcd_cfg->rw_pin; gpio_init(&cfg);
    cfg.pin = lcd_cfg->en_pin; gpio_init(&cfg);

#if LCD_MODE == LCD_MODE_4
    for (uint8_t i = 0; i < 4; i++)
    {
        cfg.pin = lcd_cfg->d_pins[i];
        gpio_init(&cfg);
    }

#elif LCD_MODE == LCD_MODE_8
    // -------- Init D0..D7 --------
    for (uint8_t i = 0; i < 8; i++)
    {
        cfg.pin = lcd_cfg->d_pins[i];
        gpio_init(&cfg);
    }
#endif

    systick_wait(40);

#if LCD_MODE == LCD_MODE_4
    gpio_write(lcd_cfg->port, lcd_cfg->rs_pin, 0);
    gpio_write(lcd_cfg->port, lcd_cfg->rw_pin, 0);

    // Function set (8-bit mode, three times)
    lcd_write_nibble(lcd_cfg, 0x03);
    systick_wait(5);

    lcd_write_nibble(lcd_cfg, 0x03);
    systick_wait(1);

    lcd_write_nibble(lcd_cfg, 0x03);
    systick_wait(1);

    // Switch to 4-bit mode
    lcd_write_nibble(lcd_cfg, 0x02);
    systick_wait(1);

#endif

#if LCD_MODE == LCD_MODE_4
    lcd_send_cmd_bit(lcd_cfg, 0x28);   // 4-bit, 2-line, 5x8 font
#elif LCD_MODE == LCD_MODE_8
    lcd_send_cmd_bit(lcd_cfg, 0x38);   // 8-bit, 2-line, 5x8 font
#endif
    systick_wait(1);

    lcd_send_cmd_bit(lcd_cfg, 0x0C);   // Display ON, cursor OFF
    systick_wait(1);

    lcd_send_cmd_bit(lcd_cfg, 0x01);   // Clear display
    systick_wait(2);

    lcd_send_cmd_bit(lcd_cfg, 0x06);   // Entry mode (increment)
    systick_wait(1);

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
    lcd_write_nibble(lcd_cfg, c >> 4);  // high nibble
    lcd_write_nibble(lcd_cfg, c & 0x0F); // low nibble
#endif
}



lcd_ret_t lcd_write_string(lcd_cfg_t* lcd, char* str) {

    while(*str) {
        lcd_write_char(lcd, *str++);
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


