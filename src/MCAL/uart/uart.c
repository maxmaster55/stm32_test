#include "MCAL/uart/uart.h"
#include <MCAL/NVIC/nvic.h>
#include "MCAL/systick/systick.h"

extern uint64_t G_ticks;

static uart_ctx_t uart_ctx[3] = {0};

// helpers 
static uart_reg_t* get_uart_reg(uart_num_t uart_num){
    switch(uart_num){
        case UART_NUM_1:
            return (uart_reg_t*)UART_NUM_1;
        case UART_NUM_2:
            return (uart_reg_t*)UART_NUM_2;
        case UART_NUM_6:
            return (uart_reg_t*)UART_NUM_6;
        default:
            return NULL;
    }
}

static uint8_t uart_num_to_index(uart_num_t uart_num){
    switch(uart_num){
        case UART_NUM_1:
            return 0;
        case UART_NUM_2:
            return 1;
        case UART_NUM_6:
            return 2;
        default:
            return -1;
    }
}

static void uart_handler(uart_num_t uart_num){
    uart_reg_t* uart_reg = get_uart_reg(uart_num);
    
    uint8_t idx = uart_num_to_index(uart_num);
    uart_ctx_t* ctx = &uart_ctx[idx];

    if(uart_reg->SR.bits.RXNE){
        uint8_t data = uart_reg->DR.reg;

        
        if (ctx->rx_index < ctx->rx_length) {
            ctx->rx_buffer[ctx->rx_index++] = data;
        }
        
        if (ctx->rx_index >= ctx->rx_length)
        {
            // stop
            uart_reg->CR1.bits.RXNEIE = 0;
            // call rx callback
            ctx->rx_index = 0;
            if (ctx->rx_cb) {
                ctx->rx_cb();
            }
            return;
        }



    }
    if(uart_reg->SR.bits.TXE){

        if (ctx->tx_index >= ctx->tx_length) {
            uart_reg->CR1.bits.TXEIE = 0;
            ctx->tx_index = 0;
            ctx->tx_length = 0;
            return;
        }

    
        uart_reg->DR.reg = ctx->tx_buffer[ctx->tx_index++];
    
        // do what the user wants
        if (ctx->tx_cb)
        {
            ctx->tx_cb();
        }
        
    }

}

// interrupt handlers
void USART1_IRQHandler(void)
{
    uart_handler(UART_NUM_1);
}

void USART2_IRQHandler(void)
{
    uart_handler(UART_NUM_2);
}

void USART6_IRQHandler(void)
{
    uart_handler(UART_NUM_6);
}


// api
uart_status_t uart_init(const uart_cfg_t* cfg)
{
    uart_reg_t* uart_reg = get_uart_reg(cfg->uart_num);
    if(uart_reg == NULL){
        return UART_ERROR;
    }

    // word length
    if(cfg->word_length == UART_WORD_LENGTH_9){
        uart_reg->CR1.bits.M = 1;
    }else{
        uart_reg->CR1.bits.M = 0;
    }

    // parity
    if(cfg->parity == UART_PARITY_NONE){
        uart_reg->CR1.bits.PCE = 0;
    }else{
        uart_reg->CR1.bits.PCE = 1;
        if(cfg->parity == UART_PARITY_EVEN){
            uart_reg->CR1.bits.PS = 0;
        }else{
            uart_reg->CR1.bits.PS = 1;
        }
    }

    // stop bits
    uart_reg->CR2.bits.STOP = cfg->stop_bits;


    // baud rate

    uint32_t usartdiv16 = (CLK + cfg->baud / 2) / cfg->baud;

    uint32_t mantissa = usartdiv16 / 16;
    uint32_t fraction = usartdiv16 % 16;

    uart_reg->BRR.reg = (mantissa << 4) | (fraction & 0xF);

    // enable transmitter and receiver
    uart_reg->CR1.bits.TE = 1;
    uart_reg->CR1.bits.RE = 1;

    // enable uart
    uart_reg->CR1.bits.UE = 1;

    // set callbacks

    uint8_t uart_index = uart_num_to_index(cfg->uart_num);
    uart_ctx_t* ctx = &uart_ctx[uart_index];

    ctx->rx_cb = cfg->rx_callback;
    ctx->tx_cb = cfg->tx_callback;


    // enable interrupts if callbacks are set
    switch (cfg->uart_num) {
    case UART_NUM_1:
        NVIC_EnableIRQ(USART1_IRQn);
        break;
    case UART_NUM_2:
        NVIC_EnableIRQ(USART2_IRQn);
        break;
    case UART_NUM_6:
        NVIC_EnableIRQ(USART6_IRQn);
        break;
    }

    return UART_OK;
}

uart_status_t uart_deinit(uart_num_t uart_num)
{
    uart_reg_t* uart_reg = get_uart_reg(uart_num);
    if(uart_reg == NULL) return UART_ERROR;
    
    uint8_t uart_index = uart_num_to_index(uart_num);

    // disable uart
    uart_reg->CR1.bits.UE = 0;

    uart_ctx_t* ctx = &uart_ctx[uart_index];

    ctx->rx_cb = NULL;
    ctx->tx_cb = NULL;


    return UART_OK;
}

uart_status_t uart_send_data(uart_num_t uart_num, const uint8_t* data, uint32_t length)
{
    uart_reg_t* uart_reg = get_uart_reg(uart_num);
    if (uart_reg == NULL) return UART_ERROR;

    uint8_t uart_index = uart_num_to_index(uart_num);
    uart_ctx_t* ctx = &uart_ctx[uart_index];

    // error checking
    if (ctx->tx_cb == NULL) return UART_ERROR;

    ctx->tx_index = 0;
    ctx->tx_length = length;
    ctx->tx_buffer = data;
    uart_reg->CR1.bits.TXEIE = 1;
}

uart_status_t uart_receive_data(uart_num_t uart_num, uint8_t* data, uint32_t length)
{
    uart_reg_t* uart_reg = get_uart_reg(uart_num);
    if (uart_reg == NULL) return UART_ERROR;

    uint8_t uart_index = uart_num_to_index(uart_num);
    uart_ctx_t* ctx = &uart_ctx[uart_index];

    // error checking
    if (ctx->rx_cb == NULL) return UART_ERROR;

    ctx->rx_buffer = data;
    ctx->rx_length = length;
    ctx->rx_index  = 0;

    uart_reg->CR1.bits.RXNEIE = 1;
}


uart_status_t uart_read_receive_buffer(uart_num_t uart_num, uint8_t* data){
    uart_reg_t* uart_reg = get_uart_reg(uart_num);
    if (uart_reg == NULL) return UART_ERROR;

    uint8_t uart_index = uart_num_to_index(uart_num);
    uart_ctx_t* ctx = &uart_ctx[uart_index];

    // error checking

    for (int i = 0; i < ctx->rx_length; i++)
    {
        data[i] = ctx->rx_buffer[i];
    }
    return UART_OK;
}



uart_status_t uart_sync_send_data(uart_num_t uart_num, const uint8_t* data, uint32_t length)
{
    if (!data || length == 0) return UART_ERROR;

    uart_reg_t* uart_reg = get_uart_reg(uart_num);
    if(uart_reg == NULL){
        return UART_ERROR;
    }

    for(uint32_t i = 0; i < length; i++){
        // wait until TXE is set
        while(!uart_reg->SR.bits.TXE);
        // write data (write full register for clarity)
        uart_reg->DR.reg = data[i];
    }
    return UART_OK;
}

uart_status_t uart_sync_receive_data(uart_num_t uart_num, uint8_t* data, uint32_t length, uint32_t timeout_ms)
{
    if (!data || length == 0) return UART_ERROR;

    uart_reg_t* uart_reg = get_uart_reg(uart_num);
    if(uart_reg == NULL){
        return UART_ERROR;
    }

    for(uint32_t i = 0; i < length; i++){
        uint32_t start_time = G_ticks; // Assume this function gets the current system time in ms
        // wait until RXNE is set or timeout
        while(!uart_reg->SR.bits.RXNE){
            if(timeout_ms > 0 && (G_ticks - start_time) >= timeout_ms){
                return UART_TIMEOUT;
            }
        }
        // read data
        data[i] = (uint8_t)(uart_reg->DR.reg & 0xFF);
    }
    return UART_OK;
}

uart_status_t uart_register_rx_callback(uart_num_t uart_num, uart_callback_t callback)
{
    uint8_t uart_index = uart_num_to_index(uart_num);

    uart_ctx_t* ctx = &uart_ctx[uart_index];
    ctx->rx_cb = callback;
    return UART_OK;
}

uart_status_t uart_register_tx_callback(uart_num_t uart_num, uart_callback_t callback)
{
    uint8_t uart_index = uart_num_to_index(uart_num);

    uart_ctx_t* ctx = &uart_ctx[uart_index];
    ctx->tx_cb = callback;

    return UART_OK;
}

uint8_t uart_is_tx_busy(uart_num_t uart_num)
{
    uart_reg_t* uart_reg = get_uart_reg(uart_num);
    if(uart_reg == NULL){
        return 0;
    }
    return !uart_reg->SR.bits.TXE;
}

uint8_t uart_is_rx_busy(uart_num_t uart_num)
{
    uart_reg_t* uart_reg = get_uart_reg(uart_num);
    if(uart_reg == NULL){
        return 0;
    }
    return !uart_reg->SR.bits.RXNE;
}