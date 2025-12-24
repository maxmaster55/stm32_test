#pragma once
#include "spi_regs.h"

#define SPI4 ((SPI_regs_t *)0x40013400)
#define SPI1 ((SPI_regs_t *)0x40013000)
#define SPI3 ((SPI_regs_t *)0x40003C00)
#define SPI2 ((SPI_regs_t *)0x40003800)

// typedef struct
// {
//     uint32_t baud;
//     uart_num_t uart_num;
//     uart_word_length_t word_length;
//     uart_parity_t parity;
//     uart_stop_bits_t stop_bits;
//     uart_callback_t rx_callback;
//     uart_callback_t tx_callback;
// } uart_cfg_t;

typedef enum {
    SPI_NUM_1,
    SPI_NUM_2,
    SPI_NUM_3,
    SPI_NUM_4,
} spi_num_t;


typedef enum {
    SPI_MODE_SLAVE = 0,
    SPI_MODE_MASTER = 1
} spi_mode_t;

typedef enum {
    SPI_SPEED_DIV2   = 0,
    SPI_SPEED_DIV4   = 1,
    SPI_SPEED_DIV8   = 2,
    SPI_SPEED_DIV16  = 3,
    SPI_SPEED_DIV32  = 4,
    SPI_SPEED_DIV64  = 5,
    SPI_SPEED_DIV128 = 6,
    SPI_SPEED_DIV256 = 7
} spi_speed_t;

typedef enum {
    SPI_POLARITY_LOW  = 0, // CPOL = 0
    SPI_POLARITY_HIGH = 1  // CPOL = 1
} spi_clock_polarity_t;

typedef enum {
    SPI_PHASE_1EDGE = 0,   // CPHA = 0
    SPI_PHASE_2EDGE = 1    // CPHA = 1
} spi_clock_phase_t;

typedef enum {
    SPI_FRAME_MSB_FIRST = 0,
    SPI_FRAME_LSB_FIRST = 1
} spi_frame_format_t;

typedef enum {
    SPI_DATASIZE_8BIT  = 0,
    SPI_DATASIZE_16BIT = 1
} spi_data_size_t;

typedef enum {
    SPI_NSS_HARD_INPUT   = 0,
    SPI_NSS_HARD_OUTPUT  = 1,
    SPI_NSS_SOFT         = 2
} spi_nss_mode_t;

typedef enum {
    SPI_PROTOCOL_MOTOROLA = 0,
    SPI_PROTOCOL_TI       = 1
} spi_protocol_t;


typedef struct
{
    spi_num_t spi_num;              // SPI numbe

    spi_mode_t mode;                // Master or Slave
    spi_speed_t speed;              // BR[2:0]
    spi_clock_polarity_t polarity;  // CPOL
    spi_clock_phase_t phase;        // CPHA

    spi_data_size_t data_size;     // DFF: 8 or 16 bits
    spi_frame_format_t frame_format; // LSBFIRST

    spi_nss_mode_t nss;            // NSS hardware input/output or software
    spi_protocol_t protocol;       // FRF: Motorola or TI

} spi_cfg_t;



void spi_init(const spi_cfg_t* cfg);
void spi_write(const spi_cfg_t* cfg, uint8_t data);
void spi_read(const spi_cfg_t* cfg, uint8_t* data);

void spi_DMA_enable(spi_num_t spi_num);
void spi_DMA_disable(spi_num_t spi_num);

