#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <MCAL/NVIC/nvic.h>
#include <HAL/7_Seg/seg_display.h>
#include <MCAL/systick/systick.h>
#include <HAL/C_LCD/lcd.h>

#define CLK 16000000UL

void delay(void)
{
    for (volatile int i = 0; i < 500000; i++);
}


void WWDG_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(WWDG_IRQn); 

    segment_write(SEGMENT_1, 10);
    delay();
    delay();
    delay();

}



lcd_cfg_t lcd_cfg = {
    .port = GPIOA,
    .d_pins = {GPIO_PIN_0, GPIO_PIN_1,
                GPIO_PIN_2, GPIO_PIN_3},
    .rs_pin = GPIO_PIN_4,
    .rw_pin = GPIO_PIN_5,
    .en_pin = GPIO_PIN_6
};

void do_smth(){
volatile static int x = 10;
x += 1;
}

int main(void)
{

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);
    

    // lcd stuff
    //lcd_init(&lcd_cfg);
    

    while (1)
    {
        //lcd_write_char(&lcd_cfg, 'x');
        systick_wait(1000);
    }

    return 0;
}
