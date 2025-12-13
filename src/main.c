#include <glob.h>
#include <service/scheduler/sched.h>
#include <MCAL/systick/systick.h>
#include <MCAL/NVIC/nvic.h>
#include <HAL/C_LCD/lcd.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/uart/uart.h>
#include <MCAL/GPIO/gpio.h>


volatile int x = 0;
volatile char test_data[24] = {0};

void do_smth(void){
    uart_read_receive_buffer(UART_NUM_1, test_data);

}

uart_cfg_t uart1_test_cfg = {
    .baud = 9600,
    .uart_num = UART_NUM_1,
    .word_length = UART_WORD_LENGTH_8,
    .parity = UART_PARITY_NONE,
    .stop_bits = UART_STOP_BITS_1,
    .rx_callback = do_smth,
    .tx_callback = do_smth
} ;

// uart pins PA9 - TX, PA10 - RX
GPIO_PinConfig_t uart1_tx_pin_cfg = {
    .port = GPIOA,
    .pin = 9,
    .mode = GPIO_MODE_ALTFN,
    .pull = GPIO_PULL_NO,
    .alt_function = GPIO_AF7_USART1_2,
    .speed = GPIO_SPEED_HIGH,
    .output_type = GPIO_OUTPUT_PUSHPULL
};
GPIO_PinConfig_t uart1_rx_pin_cfg = {
    .port = GPIOA,
    .pin = 10,
    .mode = GPIO_MODE_ALTFN,
    .pull = GPIO_PULL_NO,
    .alt_function = GPIO_AF7_USART1_2,
    .speed = GPIO_SPEED_HIGH,
    .output_type = GPIO_OUTPUT_PUSHPULL
};



int main(void)
{

    // enable Clock
    rcc_En_clk_preiph(RCC_GPIOA);
    rcc_En_clk_preiph(RCC_USART1);

    // start systick
    systick_init(CLK, SYSTICK_PRESCALER_NO);
    systick_set_val(1);
    systick_start();
    // init uart1
    gpio_init(&uart1_tx_pin_cfg);
    gpio_init(&uart1_rx_pin_cfg);
    uart_init(&uart1_test_cfg);

    // test send data
    const uint8_t test_str[8] = {0};

    uart_send_data(UART_NUM_1, "5", 1);
    uart_receive_data(UART_NUM_1, test_data, 24);

    while (1)
    {
        // nthin
        x++;
    }
    
    return 0;
}
