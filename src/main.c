#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <MCAL/NVIC/nvic.h>
#include <HAL/7_Seg/seg_display.h>
#include <MCAL/systick/systick.h>

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

#define CLK 16000000UL

void do_smth(){
    volatile int x = 10;
    volatile int y = 10;
    volatile int z = x + y;

}

int main(void)
{

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);
    segment_display_init();
    NVIC_EnableIRQ(WWDG_IRQn);

    systick_init(CLK, SYSTICK_PRESCALER_NO);
    
    systick_configure_callback(do_smth);
    systick_start();    
    while (1)
    {
        for (int i = 0; i < 10; i++)
        {
            segment_write(SEGMENT_1, i);
            //delay();
            systick_wait(20);
        }
        //segment_en_dot(SEGMENT_1);
        NVIC_SetPendingIRQ(WWDG_IRQn);
    }

    return 0;
}
