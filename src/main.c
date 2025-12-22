#include <stdio.h>
#include <stdlib.h>
#include <HAL/HSerial/hserial.h>
#include <MCAL/GPIO/gpio.h>
#include <MCAL/uart/uart.h>
#include <MCAL/RCC/rcc.h>
#include <string.h>

volatile int tx_done = 1;
volatile int rx_done = 0;

uint8_t rx_byte = 0;

void tx_done_callback(void){
    tx_done = 1;
}

void rx_done_callback(void){
    rx_done = 1; // One byte received
}

HSerial_instance_t h_ser = {
    .type = HSERIAL_TYPE_UART,
    .uart_cfg.baudrate = 9600,
    .uart_cfg.uart_number = UART_NUM_1,
    .uart_cfg.word_length = UART_WORD_LENGTH_8,
    .uart_cfg.parity = UART_PARITY_NONE,
    .uart_cfg.stop_bits = UART_STOP_BITS_1,
    .uart_cfg.tx_callback = tx_done_callback,
    .uart_cfg.rx_callback = rx_done_callback
};

int main(void)
{
    HSerial_init(&h_ser);

    // Start RX DMA for 1 byte
    HSerial_receive_data(&h_ser, &rx_byte, 1);

    while (1)
    {
        // Wait until we receive a number
        if (!rx_done) continue;
        rx_done = 0;

        int count = rx_byte - '0'; // assume single digit for simplicity
        if (count < 0) count = 0;

        for (int i = 0; i < count; i++)
        {
            static char tx_buffer[16];
            int len = snprintf(tx_buffer, sizeof(tx_buffer), "hi %d\r\n", i+1);

            tx_done = 0;
            HSerial_send_data(&h_ser, (uint8_t*)tx_buffer, len);

            // Wait until TX DMA finishes
            while(!tx_done);
        }

        // Ready to receive next number
        HSerial_receive_data(&h_ser, &rx_byte, 1);
    }
}
