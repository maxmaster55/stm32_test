#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <HAL/Push_button/btn.h>
#include <HAL/C_LCD/lcd.h>
#include <service/scheduler/sched.h>
#include <MCAL/systick/systick.h>

#define CLK 16000000

lcd_cfg_t lcd_cfg = {
    .port = GPIOA,
    .d_pins = {3, 4, 5, 6},
    .rs_pin = 0,
    .rw_pin = 1,
    .en_pin = 2,
};


int main(void)
{

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);

    sched_init(1);
    
    lcd_async_init(&lcd_cfg);
    char* x = "Touch some grass";
    lcd_async_write_str(&lcd_cfg, x);
    sched_start();

    while (1)
    {
        // nthin
    }
    
    return 0;
}
