#include <service/scheduler/sched.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <HAL/led_matrix/matrix.h>


#define CLK 16000000

mat_cfg_t mat_cfg = {
    .every = 1,
    .high_pins = {{GPIOA, 0},{GPIOA, 1},
                  {GPIOA, 2},{GPIOA, 3},
                  {GPIOA, 4},{GPIOA, 5},
                  {GPIOA, 6},{GPIOA, 7}},
    .low_pins = {{GPIOB, 0},{GPIOB, 1},
                 {GPIOB, 2},{GPIOB, 3},
                 {GPIOB, 4},{GPIOB, 5},
                 {GPIOB, 6},{GPIOB, 7}},
};

mat_shape_t heart = {
    0b00000000,
    0b01100110,
    0b11111111,
    0b11111111,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000
};


int main(void)
{

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);
    rcc_En_clk_preiph(RCC_GPIOB);

    sched_init(1);
    
    matrix_init(&mat_cfg);
    matrix_draw(&heart);

    sched_start();

    while (1)
    {
        // nthin
    }
    
    return 0;
}
