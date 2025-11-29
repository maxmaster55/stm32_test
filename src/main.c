#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <service/scheduler/sched.h>


void test_f(void* arg){
    volatile int x=0;
    x += 1;

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
