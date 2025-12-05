#include <service/scheduler/sched.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <HAL/C_LCD/lcd.h>
#include <HAL/led_matrix/matrix.h>
#include <MCAL/systick/systick.h>


#define CLK 16000000


lcd_cfg_t lcd_cfg = {
    .port = GPIOA,
    .d_pins = {
        3, 4, 
        5, 6
    },
    .rs_pin = 0,
    .rw_pin = 1,
    .en_pin = 2,
};


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


uint8_t random_shit[8] = {
  0x0E,
  0x0E,
  0x1F,
  0x15,
  0x1F,
  0x15,
  0x1F,
  0x0E
};


int main(void)
{

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);
    rcc_En_clk_preiph(RCC_GPIOB);

    sched_init(1);


    // lcd_async_init(&lcd_cfg);
    lcd_async_init(&lcd_cfg);

    lcd_async_write_str(&lcd_cfg, "test");
    lcd_async_write_str(&lcd_cfg, "help");
    lcd_async_write_str(&lcd_cfg, "test1");
    lcd_async_write_str(&lcd_cfg, "test2");

    sched_start();


    while (1)
    {
        // nthin
    }
    
    return 0;
}
