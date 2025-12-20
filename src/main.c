#include <glob.h>
#include <HAL/HSerial/hserial.h>
#include <MCAL/GPIO/gpio.h>
#include <MCAL/uart/uart.h>
#include <MCAL/RCC/rcc.h>


volatile int x = 0;

uint8_t src_arr[8] = {0};

void tx_do_smth(void){
    uart_read_receive_buffer(UART_NUM_1, src_arr);
}
void rx_do_smth(void){
    uart_read_receive_buffer(UART_NUM_1, src_arr);
}

HSerial_instance_t h_ser = {
    .type = HSERIAL_TYPE_UART,
    .uart_cfg.baudrate = 115200,
    .uart_cfg.uart_number = UART_NUM_1,
    .uart_cfg.word_length = UART_WORD_LENGTH_8,
    .uart_cfg.parity = UART_PARITY_NONE,
    .uart_cfg.stop_bits = UART_STOP_BITS_1,
    .uart_cfg.tx_callback = tx_do_smth,
    .uart_cfg.rx_callback = rx_do_smth
};

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


uart_cfg_t uart1_test_cfg = {
    .baud = 9600,
    .uart_num = UART_NUM_1,
    .word_length = UART_WORD_LENGTH_8,
    .parity = UART_PARITY_NONE,
    .stop_bits = UART_STOP_BITS_1,
    .tx_callback = tx_do_smth,
    .rx_callback = rx_do_smth
} ;


int main(void)
{
    rcc_En_clk_preiph(RCC_GPIOA);
    rcc_En_clk_preiph(RCC_USART1);

    HSerial_init(&h_ser);
    gpio_init(&uart1_tx_pin_cfg);
    gpio_init(&uart1_rx_pin_cfg);
    uart_init(&uart1_test_cfg);
    uart_send_data(UART_NUM_1, "test", 5);
    uart_receive_data(UART_NUM_1, src_arr, 5);

    while (1)
    {
        // nthin
        x++;
    }
    
    return 0;
}
