#include <glob.h>
#include <service/scheduler/sched.h>
#include <MCAL/systick/systick.h>
#include <MCAL/NVIC/nvic.h>
#include <HAL/C_LCD/lcd.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/uart/uart.h>
#include <MCAL/GPIO/gpio.h>
#include <MCAL/DMA/dma.h>


volatile int x = 0;

uint8_t src_arr[] = "Hello from DMA2! This is a test.";
uint8_t dst_arr[33] = {0};


dma_cfg_t dma_cfg = {
    .dma = DMA2,
    .stream_number = DMA_STREAM_0,
    .channel = DMA_CHANNEL_0,
    .direction = DMA_DIR_MEM_TO_MEM,
    .from = src_arr,
    .from_increment = 1,
    .to = dst_arr,
    .to_increment = 1,
    .data_length = sizeof(src_arr),
    .element_size = ELEMENT_SIZE_8_BITS,
    .Priority = DMA_PRIORITY_HIGH,
};

int main(void)
{

    // enable Clock
    rcc_En_clk_preiph(RCC_GPIOA);
    rcc_En_clk_preiph(RCC_DMA2);

    // test dma
    dma_init(&dma_cfg);
    dma_start(DMA2, DMA_STREAM_0);

    while (1)
    {
        // nthin
        x++;
    }
    
    return 0;
}
