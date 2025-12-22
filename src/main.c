#include <stdio.h>
#include <stdlib.h>
#include <HAL/HSerial/hserial.h>
#include <MCAL/GPIO/gpio.h>
#include <MCAL/uart/uart.h>
#include <MCAL/RCC/rcc.h>

volatile int tx_done = 0;

void tx_do_smth(void){
    tx_done = 1;
}

HSerial_instance_t h_ser = {
    .type = HSERIAL_TYPE_UART,
    .uart_cfg.baudrate = 115200,
    .uart_cfg.uart_number = UART_NUM_1,
    .uart_cfg.word_length = UART_WORD_LENGTH_8,
    .uart_cfg.parity = UART_PARITY_NONE,
    .uart_cfg.stop_bits = UART_STOP_BITS_1,
    .uart_cfg.tx_callback = tx_do_smth,
    .uart_cfg.rx_callback = NULL
};

int main(void)
{
    HSerial_init(&h_ser);

    uint8_t tx_buffer[32];
    int counter = 0;

    while (1)
    {
        // Convert number to string
        int len = snprintf((char*)tx_buffer, sizeof(tx_buffer), "Count: %d\r\n", counter);

        tx_done = 0;
        HSerial_send_data(&h_ser, tx_buffer, len);

        // Wait for DMA to finish
        while(!tx_done) {
            for (volatile int i = 0; i < 20000; i++);
        }

        counter++;  // increment for next send

        // Delay ~2 seconds (simple busy loop)
        for (volatile int i = 0; i < 2000000; i++);
    }
}
