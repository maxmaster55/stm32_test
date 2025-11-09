#include <rcc.h>



rcc_PLL_config_t pll_cfg = {
  .src   = PLL_HSI_SRC,  // HSI = 16 MHz
  .M_val = 16,
  .N_val = 336,
  .P_val = 2,           
  .Q_val = 7
};
int main(void) {


    rcc_ctrlClk(RCC_CLK_HSI, RCC_CLK_ON);
    volatile rcc_return_t ret = rcc_PLL_config(pll_cfg);
    rcc_ctrlClk(RCC_CLK_PLL, RCC_CLK_ON);
    rcc_set_SysTick(RCC_CLK_PLL);

    //rcc_ctrlClk(RCC_CLK_HSI, RCC_CLK_OFF);
    volatile int x = 10;
    volatile int y = 20;
    volatile z = x+y;
    x++;

    
    // set PC13 to output mode
    while (1) {
        // toggle PC13
        for (volatile int i = 0; i < 1000000; i++);
    }
    return 0;
}
