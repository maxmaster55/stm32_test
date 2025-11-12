#include <MCAL/RCC/rcc.h>

#define RCC_BASE        0x40023800UL
#define GPIOC_BASE      0x40020800UL

#define GPIOC_MODER     (*(volatile unsigned long *)(GPIOC_BASE + 0x00))
#define GPIOC_ODR       (*(volatile unsigned long *)(GPIOC_BASE + 0x14))
#define LED_PIN 13


void delay(void) {
    for (volatile int i = 0; i < 500000; i++);
}


rcc_PLL_config_t pll_cfg = {
  .src   = PLL_HSI_SRC,  // HSI = 16 MHz
  .M_val = 8,
  .N_val = 193,
  .P_val = 6,
  .Q_val = 4
};


int main(void) {


    // enable GPIO Clock
    rcc_En_clk_preiph(RCC_GPIOC);

    // Set PC13 as output (01)
    GPIOC_MODER &= ~(3 << (LED_PIN * 2));  // Clear mode bits
    GPIOC_MODER |=  (1 << (LED_PIN * 2));  // Set as output

    while (1) { 
        // toggle LED
        GPIOC_ODR ^= (1 << LED_PIN);
        delay();
    }
    return 0;
}
