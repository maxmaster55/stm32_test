#include <service/scheduler/sched.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <HAL/C_LCD/lcd.h>
#include <HAL/keypad/keypad.h>
#include <MCAL/systick/systick.h>


#define CLK 16000000


volatile uint8_t x = 0;

void tester_cb(void* args){

    uint8_t pressed = keypad_get_last_pressed();
    if (pressed != 0){
        x++;
    }

}

runnable_t tester = {
    .name = "tester",
    .callback = tester_cb,
    .every = 5,
    .first_delay = 0,
    .priority = 2,
    .args = NULL
};


keypad_t keypad_cfg = {
    .cols = {{GPIOA, 0}, {GPIOA, 1}, {GPIOA, 2}, {GPIOA, 3}},
    .rows = {{GPIOA, 4}, {GPIOA, 5}, {GPIOA, 6}, {GPIOA, 7}}
};

int main(void)
{

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);
    rcc_En_clk_preiph(RCC_GPIOB);

    sched_init(1);


    // lcd_async_init(&lcd_cfg);
    keypad_init(&keypad_cfg);

    sched_register(&tester);

    sched_start();


    while (1)
    {
        // nthin
    }
    
    return 0;
}
