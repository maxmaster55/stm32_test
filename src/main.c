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

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);

    systick_init(CLK, SYSTICK_PRESCALER_NO);
    systick_set_val(1);
    systick_start();

    // btn_init();
    lcd_init(&lcd_cfg);
    lcd_write_string(&lcd_cfg, "YE TM");

    
    int pos = 0;
    int dir = 1;

    while (1)
    {
        lcd_clear_lcd(&lcd_cfg);

        lcd_set_cursor_pos(&lcd_cfg, 0, pos);
        lcd_write_char(&lcd_cfg, '_');

        systick_wait(150);

        pos += dir;

        if (pos >= 15) dir = -1;
        if (pos <= 0)  dir = 1;
    }

    return 0;
}
