#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <HAL/Push_button/btn.h>
#include <service/scheduler/sched.h>


int x = 0;

void test_f(void* arg){
    btn_state_t state = BTN_NOT_PRESSED;
    btn_read_state(BTN_NAME_0, &state);
    if (state == BTN_PRESSED)
    {
        x += 1;
    }
    
}

runnable_t t_runnable = {
    .name = "test",
    .callback = test_f,
    .every = 3,
    .first_delay = 0,
    .priority = 0,
    .args = 1
};

int main(void)
{

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);

    btn_init();
    sched_init(1);
    sched_register(&t_runnable);
    sched_start();
    

    while (1)
    {
        //lcd_write_char(&lcd_cfg, 'x');
        //systick_wait(1000);
    }

    return 0;
}
