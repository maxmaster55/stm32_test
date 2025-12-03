#include <service/scheduler/sched.h>
#include <MCAL/systick/systick.h>
#include <HAL/C_LCD/lcd.h>

// helpers 

static uint32_t get_step(uint8_t reset);
// with no wait
static void lcd_en_high(lcd_cfg_t* lcd_cfg);
// with no wait
static void lcd_en_low(lcd_cfg_t* lcd_cfg);
// with no wait
static void lcd_async_write_nibble(lcd_cfg_t* lcd_cfg, uint8_t nibble);
// with no wait
static void lcd_async_send_cmd(lcd_cfg_t* lcd_cfg, uint8_t cmd, uint8_t part);
// with no wait
static void lcd_async_send_data(lcd_cfg_t* lcd_cfg, uint8_t data, uint8_t part);

static void lcd_pulse_en(lcd_cfg_t* lcd_cfg);
static void lcd_write_nibble(lcd_cfg_t* lcd_cfg, uint8_t nibble);
static void lcd_write_byte(lcd_cfg_t* lcd_cfg, uint8_t data);
static void lcd_send_cmd_bit(lcd_cfg_t* lcd_cfg, uint8_t cmd);
static void lcd_send_data_bit(lcd_cfg_t* lcd_cfg, uint8_t data);



// scheduler stuff
static lcd_state_t lcd_state = LCD_STATE_INIT;
static lcd_init_state_t init_state = LCD_INIT_1_LINE_FONT;
static char* pending_write = NULL;

void lcd_callback(void* args){
    lcd_cfg_t* lcd_cfg = (lcd_cfg_t*) args;


    switch (lcd_state)
    {
    case LCD_STATE_INIT:
        switch (init_state)
        {
        case LCD_INIT_1_LINE_FONT:
            // 4-bit, 2-line, 5x8 font
            switch (get_step(0))
            {
            case 1:lcd_async_send_cmd(lcd_cfg, 0x28, 1);return;
            case 2:lcd_en_high(lcd_cfg);return;
            case 3:lcd_en_low(lcd_cfg);return;
            case 4:lcd_async_send_cmd(lcd_cfg, 0x28, 2);return;
            case 5:lcd_en_high(lcd_cfg);return;
            case 6:lcd_en_low(lcd_cfg);return;
            }
            get_step(1);
            init_state = LCD_INIT_2_DISPLAY_ON;
            break;
        case LCD_INIT_2_DISPLAY_ON:
            // Display ON, cursor OFF
            switch (get_step(0))
            {
            case 1:lcd_async_send_cmd(lcd_cfg, 0x0C, 1);return;
            case 2:lcd_en_high(lcd_cfg);return;
            case 3:lcd_en_low(lcd_cfg);return;
            case 4:lcd_async_send_cmd(lcd_cfg, 0x0C, 2);return;
            case 5:lcd_en_high(lcd_cfg);return;
            case 6:lcd_en_low(lcd_cfg);return;
            }
            get_step(1);
            init_state = LCD_INIT_3_CLEAR;
            break;
        case LCD_INIT_3_CLEAR:
            // Clear display
            switch (get_step(0))
            {
            case 1:lcd_async_send_cmd(lcd_cfg, 0x01, 1);return;
            case 2:lcd_en_high(lcd_cfg);return;
            case 3:lcd_en_low(lcd_cfg);return;
            case 4:lcd_async_send_cmd(lcd_cfg, 0x01, 2);return;
            case 5:lcd_en_high(lcd_cfg);return;
            case 6:lcd_en_low(lcd_cfg);return;
            }
            get_step(1);
            init_state = LCD_INIT_4_ENTRY_MODE;
            break;
        case LCD_INIT_4_ENTRY_MODE:
            switch (get_step(0))
            {
            case 1:lcd_async_send_cmd(lcd_cfg, 0x06, 1);return;
            case 2:lcd_en_high(lcd_cfg);return;
            case 3:lcd_en_low(lcd_cfg);return;
            case 4:lcd_async_send_cmd(lcd_cfg, 0x06, 2);return;
            case 5:lcd_en_high(lcd_cfg);return;
            case 6:lcd_en_low(lcd_cfg);return;
            }
            get_step(1);
            init_state = LCD_INIT_5_DONE;
            break;
        case LCD_INIT_5_DONE:
            lcd_state = LCD_STATE_IDLE;
            get_step(1);
            break;
        }
        break;
    case LCD_STATE_WRITING:
        if (*pending_write == '\0')
        {
            pending_write = NULL;
            lcd_state = LCD_STATE_IDLE;return;
        }
        switch (get_step(0))
        {
        case 1:lcd_async_send_data(lcd_cfg, *pending_write, 1);return;
        case 2:lcd_en_high(lcd_cfg);return;
        case 3:lcd_en_low(lcd_cfg);return;
        case 4:lcd_async_send_data(lcd_cfg, *pending_write, 2);return;
        case 5:lcd_en_high(lcd_cfg);return;
        case 6:lcd_en_low(lcd_cfg);return;
        }
        get_step(1);
        pending_write += 1;
        break;
    case LCD_STATE_IDLE:
        if (pending_write != NULL)
        {
            lcd_state = LCD_STATE_WRITING;return;
        }
        break;
    }
}


runnable_t lcd_runnable = {
    .name = "lcd",
    .callback = lcd_callback,
    .every = 2,
    .first_delay = 40,
    .priority = 2,
    .args = NULL // will change in the init function
};


lcd_ret_t lcd_async_init(lcd_cfg_t* lcd_cfg)
{
    if (lcd_cfg == NULL)
    {
        return  LCD_RET_NOK;
    }

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
    gpio_write(lcd_cfg->port, lcd_cfg->rs_pin, 0);
    gpio_write(lcd_cfg->port, lcd_cfg->rw_pin, 0);

    lcd_runnable.args = lcd_cfg;

    sched_register(&lcd_runnable);
    return LCD_RET_OK;
}


lcd_ret_t lcd_async_write_str(lcd_cfg_t* lcd_cfg, const char *str)
{
    pending_write = str;
    return LCD_RET_OK;
}

// api

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

    gpio_write(lcd_cfg->port, lcd_cfg->rs_pin, 0);
    gpio_write(lcd_cfg->port, lcd_cfg->rw_pin, 0);


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


lcd_ret_t lcd_write_char(lcd_cfg_t* lcd_cfg, char c)
{
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


lcd_ret_t lcd_write_string(lcd_cfg_t* lcd, char* str)
{

    while(*str) {
        lcd_write_char(lcd, *str++);
    }

    return LCD_RET_OK;
}


lcd_ret_t lcd_clear_lcd(lcd_cfg_t* lcd_cfg)
{

    lcd_send_cmd_bit(lcd_cfg, 0x01);

    return LCD_RET_OK;
}



lcd_ret_t lcd_set_cursor_pos(lcd_cfg_t* lcd_cfg, uint8_t row, uint8_t col)
{
    uint8_t address = 0;

    // clamp col to 0–15 for safety
    if (col > 15) col = 15;

    switch (row)
    {
        case 0:
            address = 0x00 + col;
            break;

        case 1:
            address = 0x40 + col;
            break;

        default:
            return LCD_RET_NOK; // invalid row
    }

    lcd_send_cmd_bit(lcd_cfg, 0x80 | address);
    systick_wait(1);

    return LCD_RET_OK;
}


lcd_ret_t lcd_save_custom_char(lcd_cfg_t* lcd_cfg, uint8_t* custom, uint8_t index)
{
#if LCD_MODE == LCD_MODE_8


#else
// later
#endif
}


// helpers implementation

static uint32_t get_step(uint8_t reset)
{
    static step_num = 0;
    step_num += 1;
    if (reset > 0)
    {
        step_num = 0;
    }
    
    return step_num;
}

// with no wait
static void lcd_en_high(lcd_cfg_t* lcd_cfg)
{
    gpio_write(lcd_cfg->port, lcd_cfg->en_pin, 1);
}

// with no wait
static void lcd_en_low(lcd_cfg_t* lcd_cfg)
{
    gpio_write(lcd_cfg->port, lcd_cfg->en_pin, 0);
}

// with no wait
static void lcd_async_write_nibble(lcd_cfg_t* lcd_cfg, uint8_t nibble)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t bit = (nibble >> i) & 0x01;
        gpio_write(lcd_cfg->port, lcd_cfg->d_pins[i], bit);
    }
}

// with no wait
static void lcd_async_send_cmd(lcd_cfg_t* lcd_cfg, uint8_t cmd, uint8_t part)
{
    // RS = 0 → Command
    gpio_write(lcd_cfg->port, lcd_cfg->rs_pin, 0);
    // RW = 0 → Write
    gpio_write(lcd_cfg->port, lcd_cfg->rw_pin, 0);


    if (part == 1)
    {
        // Send high nibble
        lcd_async_write_nibble(lcd_cfg, (cmd >> 4) & 0x0F);
    }else{
        // Send low nibble
        lcd_async_write_nibble(lcd_cfg, cmd & 0x0F);
    }
}

// with no wait
static void lcd_async_send_data(lcd_cfg_t* lcd_cfg, uint8_t data, uint8_t part)
{
    // RS = 1 → Data
    gpio_write(lcd_cfg->port, lcd_cfg->rs_pin, 1);
    // RW = 0 → Write
    gpio_write(lcd_cfg->port, lcd_cfg->rw_pin, 0);


    if (part == 1)
    {
        // Send high nibble
        lcd_async_write_nibble(lcd_cfg, (data >> 4) & 0x0F);
    }else{
        // Send low nibble
        lcd_async_write_nibble(lcd_cfg, data & 0x0F);
    }
}

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

static void lcd_send_cmd_bit(lcd_cfg_t* lcd_cfg, uint8_t cmd)
{
    // RS = 0 → Command
    gpio_write(lcd_cfg->port, lcd_cfg->rs_pin, 0);
    // RW = 0 → Write
    gpio_write(lcd_cfg->port, lcd_cfg->rw_pin, 0);

    lcd_write_byte(lcd_cfg, cmd);
}

static void lcd_send_data_bit(lcd_cfg_t* lcd_cfg, uint8_t data)
{
    // RS = 1 → Data
    gpio_write(lcd_cfg->port, lcd_cfg->rs_pin, 1);
    // RW = 0 → Write
    gpio_write(lcd_cfg->port, lcd_cfg->rw_pin, 0);

    lcd_write_byte(lcd_cfg, data);
}

