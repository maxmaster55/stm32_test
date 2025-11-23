#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <HAL/7_Seg/seg_display.h>

void delay(void)
{
    for (volatile int i = 0; i < 500000; i++);
}

int main(void)
{

    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOA);
    segment_display_init();

    while (1)
    {
        for (int i = 0; i < 10; i++)
        {
            segment_write(SEGMENT_1, i);
            delay();
        }
        segment_en_dot(SEGMENT_1);
    }
    
    return 0;
}
