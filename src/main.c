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
    .uart_cfg.baudrate = 9600,
    .uart_cfg.uart_number = UART_NUM_1,
    .uart_cfg.word_length = UART_WORD_LENGTH_8,
    .uart_cfg.parity = UART_PARITY_NONE,
    .uart_cfg.stop_bits = UART_STOP_BITS_1,
    .uart_cfg.tx_callback = tx_do_smth,
    .uart_cfg.rx_callback = rx_do_smth
};


uint8_t tx_data[] = "Tello DMA UART!";
uint8_t rx_data[32] = {0};

int main(void)
{
    HSerial_init(&h_ser);



    // Start RX first
    HSerial_receive_data(&h_ser, rx_data, sizeof(rx_data));

    HSerial_send_data(&h_ser, tx_data, sizeof(tx_data)-1);

    while (1)
    {
        // Optional: blink an LED to show main is running
        // Or just loop here
        x++;
    }
}

