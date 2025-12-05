#include "HAL/led_matrix/matrix.h"
#include "service/scheduler/sched.h"


static mat_cfg_t *G_matrix_cfg = NULL;

static void turn_all_on_but(mat_cfg_t *matrix_cfg, uint8_t pin);
static void turn_all_off_but(mat_cfg_t *matrix_cfg, uint8_t pin);
static uint32_t get_step(uint8_t reset);


// callback
void matrix_callback(void* args)
{
    mat_shape_t *shape = (mat_shape_t*)args;
    if (shape == NULL) return;

    static uint8_t row = 0;
    uint8_t pattern = ((uint8_t*)shape)[row];

    turn_all_off_but(G_matrix_cfg, row);

    for (uint8_t col = 0; col < 8; col++)
    {
        uint8_t bit = (pattern >> col) & 1;
        gpio_write(
            G_matrix_cfg->low_pins[col].port,
            G_matrix_cfg->low_pins[col].pin,
            bit);
    }
    
    row = (row + 1) % 8;
}


runnable_t matrix_runnable = {
    .name = "marix",
    .callback = matrix_callback,
    .every = 1, // as a default
    .first_delay = 0,
    .priority = 0,
    .args = NULL // will change later
};


void matrix_init(mat_cfg_t *matrix_cfg)
{
    if (matrix_cfg == NULL) // fk you doin??
    {
        return;
    }
    G_matrix_cfg = matrix_cfg;

    GPIO_PinConfig_t pin_cfg;

    pin_cfg.mode = GPIO_MODE_OUTPUT;
    pin_cfg.pull = GPIO_PULL_NO;
    pin_cfg.alt_function = GPIO_AF0_SYSTEM;
    pin_cfg.speed = GPIO_SPEED_FAST;
    pin_cfg.output_type = GPIO_OUTPUT_PUSHPULL;

    for (size_t i = 0; i < 8; i++)
    {
        pin_cfg.port = matrix_cfg->high_pins[i].port;
        pin_cfg.pin = matrix_cfg->high_pins[i].pin;
        gpio_init(&pin_cfg);
    }

    for (size_t i = 0; i < 8; i++)
    {
        pin_cfg.port = matrix_cfg->low_pins[i].port;
        pin_cfg.pin = matrix_cfg->low_pins[i].pin;
        gpio_init(&pin_cfg);
    }
}


void matrix_draw(mat_cfg_t *matrix_cfg, mat_shape_t *shape)
{
    matrix_runnable.args = shape;
    matrix_runnable.every = matrix_cfg->every;
    sched_register(&matrix_runnable);
}


// helpers

static uint32_t get_step(uint8_t reset)
{
    static int step_num = 0;
    step_num += 1;
    if (reset > 0)
    {
        step_num = 0;
    }
    
    return step_num;
}


static void turn_all_on_but(mat_cfg_t *matrix_cfg, uint8_t pin)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        gpio_write(
            matrix_cfg->low_pins[i].port,
            matrix_cfg->low_pins[i].pin,
            i != pin);
    }

}


static void turn_all_off_but(mat_cfg_t *matrix_cfg, uint8_t pin)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        gpio_write(
            matrix_cfg->high_pins[i].port,
            matrix_cfg->high_pins[i].pin,
            i == pin);
    }
    
}
