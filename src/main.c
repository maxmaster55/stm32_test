#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <HAL/LED/led.h>
#include <HAL/Push_button/btn.h>

void delay(void)
{
    for (volatile int i = 0; i < 50000; i++);
}

int main(void)
{

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);
    rcc_En_clk_preiph(RCC_GPIOB);
    led_init();
    btn_init();

    while (1)
    {
        btn_state btn_state;
        btn_read_state(BTN_NAME_0, &btn_state);
        uint32_t led_mask = 0;
        if (btn_state == BTN_PRESSED)
        {
            for (led_name_t led = LED_NAME_0; led < LED_NUM_MAX; led++)
            {
                led_mask |= (1 << led);
                led_Turn_on(led);
                delay();
                led_Turn_off(led);
                delay();
            }
        }
    }

    return 0;
}
