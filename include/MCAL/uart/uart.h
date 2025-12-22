#pragma once

#include "glob.h"


typedef struct
{

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t PE       :1;
            volatile uint32_t FE       :1;
            volatile uint32_t NE       :1;
            volatile uint32_t ORE      :1;
            volatile uint32_t IDLE     :1;
            volatile uint32_t RXNE     :1;
            volatile uint32_t TC       :1;
            volatile uint32_t TXE      :1;
            volatile uint32_t LBD      :1;
            volatile uint32_t CTS      :1;
            volatile uint32_t res0     :22;
        }bits;
    }SR;
    
    
    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t DR       :9;
            volatile uint32_t res0     :23;
        }bits;
    }DR;

    
    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t DIV_Fraction  :4;
            volatile uint32_t DIV_Mantissa  :12;
            volatile uint32_t res0          :16;
        }bits;
    }BRR;

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t SBK       :1;
            volatile uint32_t RWU       :1;
            volatile uint32_t RE        :1;
            volatile uint32_t TE        :1;
            volatile uint32_t IDLEIE    :1;
            volatile uint32_t RXNEIE    :1;
            volatile uint32_t TCIE      :1;
            volatile uint32_t TXEIE     :1;
            volatile uint32_t PEIE      :1;
            volatile uint32_t PS        :1;
            volatile uint32_t PCE       :1;
            volatile uint32_t WAKE      :1;
            volatile uint32_t M         :1;
            volatile uint32_t UE        :1;
            volatile uint32_t res0      :1;
            volatile uint32_t OVER8     :1;
            volatile uint32_t res1      :16;
        }bits;
    }CR1;

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t ADD       :4;
            volatile uint32_t res0      :1;
            volatile uint32_t LBDL      :1;
            volatile uint32_t LBDIE     :1;
            volatile uint32_t res1      :1;
            volatile uint32_t LBCL      :1;
            volatile uint32_t CPHA      :1;
            volatile uint32_t CPOL      :1;
            volatile uint32_t CLKEN     :1;
            volatile uint32_t STOP      :2;
            volatile uint32_t LINEN     :1;
            volatile uint32_t res2      :17;
        }bits;
    }CR2;

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t EIE       :1;
            volatile uint32_t IREN      :1;
            volatile uint32_t IRLP      :1;
            volatile uint32_t HDSEL     :1;
            volatile uint32_t NACK      :1;
            volatile uint32_t SCEN      :1;
            volatile uint32_t DMAR      :1;
            volatile uint32_t DMAT      :1;
            volatile uint32_t RTSE      :1;
            volatile uint32_t CTSE      :1;
            volatile uint32_t CTSIE     :1;
            volatile uint32_t ONEBIT    :1;
            volatile uint32_t res0      :20;
        }bits;
    }CR3;

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t PSC       :8;
            volatile uint32_t GT        :8;
            volatile uint32_t res0      :16;
        }bits;
    }GTPR;

} uart_reg_t;


typedef enum{
    UART_NUM_1 = 0x40011000,
    UART_NUM_2 = 0x40004400,
    UART_NUM_6 = 0x40011400
} uart_num_t;


typedef enum{
    UART_OK = 0,
    UART_ERROR = -1,
    UART_BUSY = -2,
    UART_TIMEOUT = -3
} uart_status_t;

typedef enum{
    UART_WORD_LENGTH_8 = 0,
    UART_WORD_LENGTH_9 = 1
} uart_word_length_t;


typedef enum{
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN = 1,
    UART_PARITY_ODD = 2
} uart_parity_t;

typedef enum{
    UART_STOP_BITS_1 = 0,
    UART_STOP_BITS_0_5 = 1,
    UART_STOP_BITS_2 = 2,
    UART_STOP_BITS_1_5 = 3
} uart_stop_bits_t;

typedef enum{
    UART_iNT_EN_DISABLE = 0,
    UART_iNT_EN_ENABLE = 1
} uart_int_en_t;

typedef void (*uart_callback_t)(void);


typedef struct {
    volatile uint8_t* rx_buffer;
    volatile uint32_t rx_index;
    volatile uint32_t rx_length;

    volatile uint8_t* tx_buffer;
    volatile uint32_t tx_index;
    volatile uint32_t tx_length;

    uart_callback_t rx_cb;
    uart_callback_t tx_cb;
} uart_ctx_t;


typedef struct
{
    uint32_t baud;
    uart_num_t uart_num;
    uart_word_length_t word_length;
    uart_parity_t parity;
    uart_stop_bits_t stop_bits;
    uart_callback_t rx_callback;
    uart_callback_t tx_callback;
} uart_cfg_t;



uart_reg_t* get_uart_reg(uart_num_t uart_num);
uart_status_t uart_init(const uart_cfg_t* cfg);
uart_status_t uart_deinit(uart_num_t uart_num);

// async
uart_status_t uart_send_data(uart_num_t uart_num, const uint8_t* data, uint32_t length);
uart_status_t uart_receive_data(uart_num_t uart_num, uint8_t* data, uint32_t length);
uart_status_t uart_read_receive_buffer(uart_num_t uart_num, uint8_t* data);

// sync
uart_status_t uart_sync_send_data(uart_num_t uart_num, const uint8_t* data, uint32_t length);
uart_status_t uart_sync_receive_data(uart_num_t uart_num, uint8_t* data, uint32_t length, uint32_t timeout_ms);

uart_status_t uart_register_rx_callback(uart_num_t uart_num, uart_callback_t callback);
uart_status_t uart_register_tx_callback(uart_num_t uart_num, uart_callback_t callback);
uint8_t uart_is_tx_busy(uart_num_t uart_num);
uint8_t uart_is_rx_busy(uart_num_t uart_num);

// dma stuff

void uart_dma_enable_tx(uart_num_t uart_num);
void uart_dma_enable_rx(uart_num_t uart_num);
void uart_dma_disable_tx(uart_num_t uart_num);
void uart_dma_disable_rx(uart_num_t uart_num);

void uart_enable_interrupts(uart_num_t num);
void uart_disable_interrupts(uart_num_t num);