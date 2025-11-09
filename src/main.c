#include <rcc.h>



rcc_PLL_config_t pll_cfg = {
  .src   = PLL_HSI_SRC,  // HSI = 16 MHz
  .M_val = 8,
  .N_val = 193,
  .P_val = 6,
  .Q_val = 4
};


int main(void) {


    rcc_ctrlClk(RCC_CLK_HSI, RCC_CLK_ON);
    volatile rcc_return_t ret = rcc_PLL_config(pll_cfg);
    rcc_ctrlClk(RCC_CLK_PLL, RCC_CLK_ON);
    rcc_set_SysTick(RCC_CLK_PLL);

    volatile int x = 10;
    volatile int y = 20;
    volatile z = x+y;
    
    
    while (1) {
      x++;
    }
    return 0;
}
