#include <MCAL/GPIO/gpio.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/SPI/spi.h>
#include <MCAL/WDT/watch_dog.h>




int main(void)
{
    rcc_enable_LSI();
    wd_init(10000, WD_PRESCALER_DIV32);
    
    while (1)
    {
        
    }
}
