#include <service/scheduler/sched.h>
#include <MCAL/RCC/rcc.h>
#include <HAL/C_LCD/lcd.h>
#include <MCAL/systick/systick.h>

#define CLK 16000000

uint8_t heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
};

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
    rcc_En_clk_preiph(RCC_GPIOB);

    sched_init(1);
    
    lcd_async_init(&lcd_cfg);
    char* x = "Touch some grass";
    systick_wait(500);
    lcd_async_save_custom_char(&lcd_cfg, &heart, 0);
    lcd_async_write_custom_char(&lcd_cfg, 0);
    sched_start();

    // lcd_init(&lcd_cfg);
    // lcd_write_char(&lcd_cfg, 'a');


    while (1)
    {
        // nthin
    }
    
    return 0;
}
