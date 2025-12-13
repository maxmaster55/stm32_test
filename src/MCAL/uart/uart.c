#include "MCAL/uart/uart.h"
#include <MCAL/NVIC/nvic.h>
#include "MCAL/systick/systick.h"

extern uint64_t G_ticks;

static uart_callback_t uart_rx_callbacks[3] = {0};
static uart_callback_t uart_tx_callbacks[3] = {0};

volatile uint8_t* send_buffer = NULL;
volatile uint32_t send_index = 0;
volatile uint32_t send_length= 0;

volatile uint8_t* receive_buffer = NULL;
volatile uint32_t receive_index = 0;
volatile uint32_t receive_length = 0;


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


// interrupt handlers
void USART1_IRQHandler(void)
{
    uart_reg_t* uart_reg = get_uart_reg(UART_NUM_1);
    
    if(uart_reg->SR.bits.RXNE){
        uint8_t data = uart_reg->DR.reg;

        if (receive_index >= receive_length)
        {
            // stop
            uart_reg->CR1.bits.RXNEIE = 0;
            return;
        }

        receive_buffer[receive_index++] = data; 
        
        if(uart_rx_callbacks[0] != NULL){
            uart_rx_callbacks[0]();
        }
    }
    if(uart_reg->SR.bits.TXE){

        if (send_buffer[send_index] == '\0' || send_buffer == NULL)
        {
            send_index = 0;
            send_length = 0;
            return;
        }

        if (send_index >= send_length)
        {
            // stop
            uart_reg->CR1.bits.TXEIE = 0;
            return;
        }
        
    
        uart_reg->DR.reg = send_buffer[send_index++];
    
        // do what the user wants
        if (uart_tx_callbacks != NULL)
        {
            uart_tx_callbacks[0]();
        }
        
    }
    
    // not that important for now
    // // Clear any overrun errors
    // if(uart_reg->SR.bits.ORE) {
    //     volatile uint8_t dummy = uart_reg->DR.reg; // Read to clear
    // }
}

void USART2_IRQHandler(void)
{
    uart_reg_t* uart_reg = get_uart_reg(UART_NUM_2);
    if(uart_reg->SR.bits.RXNE){
        // call rx callback
        if(uart_rx_callbacks[1] != NULL){
            volatile uint8_t data = uart_reg->DR.reg; // Clears RXNE

            uart_rx_callbacks[1]();
        }
    }
    if(uart_reg->SR.bits.TXE){
        // call tx callback
        if(uart_tx_callbacks[1] != NULL){
            uart_tx_callbacks[1]();
        }
    }
}

void USART6_IRQHandler(void)
{
    uart_reg_t* uart_reg = get_uart_reg(UART_NUM_6);
    if(uart_reg->SR.bits.RXNE){
        // call rx callback
        if(uart_rx_callbacks[2] != NULL){
            volatile uint8_t data = uart_reg->DR.reg; // Clears RXNE
            uart_rx_callbacks[2]();
        }
    }
    if(uart_reg->SR.bits.TXE){
        // call tx callback
        if(uart_tx_callbacks[2] != NULL){
            uart_tx_callbacks[2]();
        }
    }
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
    switch (uart_index)
    {
    case 0:
        uart_rx_callbacks[0] = cfg->rx_callback;
        uart_tx_callbacks[0] = cfg->tx_callback;
        break;
    case 1:
        uart_rx_callbacks[1] = cfg->rx_callback;
        uart_tx_callbacks[1] = cfg->tx_callback;
        break;
    case 2:
        uart_rx_callbacks[2] = cfg->rx_callback;
        uart_tx_callbacks[2] = cfg->tx_callback;
        break;
    default:
        return UART_ERROR;
        break;
    }
    

    // enable interrupts if callbacks are set
    if(cfg->tx_callback != NULL){
        NVIC_EnableIRQ(USART1_IRQn);
        //uart_reg->CR1.bits.TXEIE = 1;
    }
    if(cfg->rx_callback != NULL){
        NVIC_EnableIRQ(USART1_IRQn);
        //uart_reg->CR1.bits.RXNEIE = 1;
    }



    return UART_OK;
}

uart_status_t uart_deinit(uart_num_t uart_num)
{
    uart_reg_t* uart_reg = get_uart_reg(uart_num);
    if(uart_reg == NULL){
        return UART_ERROR;
    }

    // disable uart
    uart_reg->CR1.bits.UE = 0;

    uint8_t uart_index = uart_num_to_index(uart_num);
    switch (uart_index)
    {
    case 0:
        uart_rx_callbacks[0] = NULL;
        uart_tx_callbacks[0] = NULL;
        break;
    case 1:
        uart_rx_callbacks[1] = NULL;
        uart_tx_callbacks[1] = NULL;
        break;
    case 2:
        uart_rx_callbacks[2] = NULL;
        uart_tx_callbacks[2] = NULL;
        break;
    default:
        return UART_ERROR;
        break;
    }

    return UART_OK;
}

uart_status_t uart_send_data(uart_num_t uart_num, const uint8_t* data, uint32_t length)
{
    uart_reg_t* uart_reg = get_uart_reg(uart_num);

    // error checking
    if (uart_reg == NULL) return UART_ERROR;
    if (uart_tx_callbacks[uart_num_to_index(uart_num)] == NULL) return UART_ERROR;

    send_index = 0;
    send_length = length;
    send_buffer = data;
    uart_reg->CR1.bits.TXEIE = 1;
}

uart_status_t uart_receive_data(uart_num_t uart_num, uint8_t* data, uint32_t length)
{
    uart_reg_t* uart_reg = get_uart_reg(uart_num);

    // error checking
    if (uart_reg == NULL) return UART_ERROR;
    if (uart_tx_callbacks[uart_num_to_index(uart_num)] == NULL) return UART_ERROR;


    // enable int
    receive_length = length;
    receive_buffer = data;
    uart_reg->CR1.bits.RXNEIE = 1;

}


uart_status_t uart_read_receive_buffer(uart_num_t uart_num, uint8_t* data){
    uart_reg_t* uart_reg = get_uart_reg(uart_num);

    // error checking
    if (uart_reg == NULL) return UART_ERROR;

    for (int i = 0; i < receive_length; i++)
    {
        data[i] = receive_buffer[i];
    }

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

    switch (uart_index)
    {
    case 0:
        uart_rx_callbacks[0] = callback;
        break;
    case 1:
        uart_rx_callbacks[1] = callback;
        break;
    case 2:
        uart_rx_callbacks[2] = callback;
        break;
    default:
        return UART_ERROR;
    }
    return UART_OK;
}

uart_status_t uart_register_tx_callback(uart_num_t uart_num, uart_callback_t callback)
{
    uint8_t uart_index = uart_num_to_index(uart_num);

    switch (uart_index)
    {
    case 0:
        uart_tx_callbacks[0] = callback;
        break;
    case 1:
        uart_tx_callbacks[1] = callback;
        break;
    case 2:
        uart_tx_callbacks[2] = callback;
        break;
    default:
        return UART_ERROR;
    }
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