#include "HAL/keypad/keypad.h"
#include "service/scheduler/sched.h"
#include <MCAL/GPIO/gpio.h>

static uint8_t current_col = 0;
static char last_pressed = 0;      // only stored after RELEASE
static char current_press = 0;     // key currently being held
static uint8_t debounce_counter = 0;


static const char keypad_map[4][4] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
};


static void keypad_callback(void* args)
{
    keypad_t* cfg = (keypad_t*)args;

    char detected_key = 0;

    // ---- SCAN ALL COLUMNS ----
    for (int c = 0; c < 4 && detected_key == 0; c++)
    {
        for (int i = 0; i < 4; i++)
            gpio_write(cfg->cols[i].port, cfg->cols[i].pin, 1);

        gpio_write(cfg->cols[c].port, cfg->cols[c].pin, 0);

        for (int r = 0; r < 4; r++)
        {
            int val = 1;
            gpio_read(cfg->rows[r].port, cfg->rows[r].pin, &val);

            if (val == 0)
            {
                detected_key = keypad_map[r][c];  // save key
                break;                             // break row loop
            }
        }
    }

    // ---- DEBOUNCING ----
    const uint8_t DEBOUNCE_REQUIRED = 3;

    if (detected_key != 0)
    {
        if (current_press == 0)
        {
            current_press = detected_key;
            debounce_counter = 1;
        }
        else if (current_press == detected_key)
        {
            if (debounce_counter < DEBOUNCE_REQUIRED)
                debounce_counter++;

            if (debounce_counter >= DEBOUNCE_REQUIRED)
            {
                if (last_pressed == 0)
                    last_pressed = current_press;
            }
        }
        else
        {
            current_press = detected_key;
            debounce_counter = 1;
        }
    }
    else
    {
        current_press = 0;
        debounce_counter = 0;
    }
}



runnable_t keypad_runnable = {
    .name = "keypad",
    .callback = keypad_callback,
    .every = 5,         // scan every 5ms
    .first_delay = 1,
    .priority = 1,
    .args = NULL 
};


keypad_return_t keypad_init(keypad_t *keypad_cfg)
{
    GPIO_PinConfig_t pin_cfg;
    pin_cfg.alt_function = GPIO_AF0_SYSTEM;
    pin_cfg.speed = GPIO_SPEED_FAST;
    pin_cfg.output_type = GPIO_OUTPUT_PUSHPULL;

    // Rows as input with pull-up
    for (int i = 0; i < 4; i++)
    {
        pin_cfg.mode = GPIO_MODE_INPUT;
        pin_cfg.pull = GPIO_PULL_UP;
        pin_cfg.port = keypad_cfg->rows[i].port;
        pin_cfg.pin  = keypad_cfg->rows[i].pin;
        gpio_init(&pin_cfg);
    }

    // Columns as output (default high)
    for (int i = 0; i < 4; i++)
    {
        pin_cfg.mode = GPIO_MODE_OUTPUT;
        pin_cfg.pull = GPIO_PULL_NO;
        pin_cfg.port = keypad_cfg->cols[i].port;
        pin_cfg.pin  = keypad_cfg->cols[i].pin;
        gpio_init(&pin_cfg);

        // Set all columns high
        gpio_write(pin_cfg.port, pin_cfg.pin, 1);
    }

    keypad_runnable.args = keypad_cfg;

    sched_register(&keypad_runnable);
    return BTN_RETURN_OK;
}

//keypad_return_t keypad_read_state();
keypad_return_t keypad_is_pressed(uint8_t btn){
    return btn == last_pressed;
}

char keypad_get_last_pressed()
{
    char c = last_pressed;
    last_pressed = 0;
    return c;
}