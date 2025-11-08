// blink example for stm32

#include <stm32f4xx.h>

int main(void) {
    // enable clock for GPIOC
    RCC->AHB1ENR |= (1 << 2);
    // set PC13 to output mode
    GPIOC->MODER |= (1 << 26);
    while (1) {
        // toggle PC13
        GPIOC->ODR ^= (1 << 13);
        for (volatile int i = 0; i < 1000000; i++);
    }
    return 0;
}
