#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <HAL/Push_button/btn.h>
#include <HAL/C_LCD/lcd.h>
#include <service/scheduler/sched.h>
#include <MCAL/systick/systick.h>

#define CLK 16000000

lcd_cfg_t lcd_cfg = {
    .port = GPIOA,
    .d_pins = {
        GPIO_PIN_3, GPIO_PIN_4, 
        GPIO_PIN_5, GPIO_PIN_6
    },
    .rs_pin = GPIO_PIN_0,
    .rw_pin = GPIO_PIN_1,
    .en_pin = GPIO_PIN_2,
};


int main(void)
{
    GPIO_PinConfig_t cfg;

    cfg.port = GPIOA;
    cfg.mode = GPIO_MODE_OUTPUT;
    cfg.pull = GPIO_PULL_NO;
    cfg.alt_function = GPIO_AF0_SYSTEM;
    cfg.speed = GPIO_SPEED_FAST;
    cfg.output_type = GPIO_MODE_OUTPUT;
    cfg.pin = GPIO_PIN_3;

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);

    gpio_init(&cfg);
    gpio_write(GPIOA, GPIO_PIN_3, 1);

    // systick_init(CLK, SYSTICK_PRESCALER_NO);
    // systick_set_val(1);
    // systick_start();

    // btn_init();
    // lcd_init(&lcd_cfg);
    // sched_init(1);
    // sched_start();
    

    while (1)
    {
        //lcd_write_char(&lcd_cfg, 'x');
        //systick_wait(1000);
    }

    return 0;
}
