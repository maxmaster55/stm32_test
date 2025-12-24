#pragma once

#include "glob.h"
#include <MCAL/uart/uart.h>
#include <MCAL/SPI/spi.h>
#include <MCAL/DMA/dma.h>



typedef enum{
    HSERIAL_OK,
    HSERIAL_NOK
}HSerial_error_t;


typedef enum {
    HSERIAL_TYPE_UART,
    HSERIAL_TYPE_SPI // later
}HSerial_type_t;

typedef struct {
    /* RX */
    dma_cfg_t rx_dma;
    uint8_t  *rx_buffer;
    uint32_t  rx_length;

    /* TX */
    dma_cfg_t tx_dma;
    uint8_t  *tx_buffer;
    uint32_t  tx_length;
} dma_ctx_t;


typedef void (*HSerial_callback_t)(void);

typedef struct {
    HSerial_type_t type;
    HSerial_callback_t rx_callback;
    HSerial_callback_t tx_callback;
    dma_ctx_t _dma;
    union {
        struct {
            uint32_t baudrate;
            uart_num_t uart_number;
            uart_word_length_t word_length;
            uart_parity_t parity;
            uart_stop_bits_t stop_bits;

        } uart_cfg;
        struct {
            spi_num_t spi_num;
            spi_mode_t mode;
            spi_speed_t speed;
        } spi_cfg;
    };

}HSerial_instance_t;


HSerial_error_t HSerial_init(HSerial_instance_t* h);
HSerial_error_t HSerial_deinit(HSerial_instance_t* h);

HSerial_error_t HSerial_send_data(HSerial_instance_t* h, const uint8_t* data, uint32_t length);
HSerial_error_t HSerial_receive_data(HSerial_instance_t* h, uint8_t* data, uint32_t length);
