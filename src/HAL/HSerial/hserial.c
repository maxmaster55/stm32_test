#include "HAL/HSerial/hserial.h"
#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <MCAL/uart/uart.h>
#include <MCAL/NVIC/nvic.h>


/* Generic DMA IRQ handler */
void DMA_IRQ_Handler_Generic(HSerial_instance_t *h, dma_cfg_t *cfg)
{
    dma_regs_t *dma = cfg->dma;
    uint32_t stream = cfg->stream_number;

    // Determine the correct ISR/IFCR register for this stream
    volatile uint32_t *ISR;
    volatile uint32_t *IFCR;

    if (stream <= 3) { // low streams
        ISR = &dma->LISR;
        IFCR = &dma->LIFCR;
    } else {          // high streams
        ISR = &dma->HISR;
        IFCR = &dma->HIFCR;
    }

    // FIX THIS: Correct bit calculation for stream
    uint32_t stream_offset;
    if (stream <= 3) {
        stream_offset = stream * 6;  // Each stream uses 6 bits in LISR/LIFCR
    } else {
        stream_offset = (stream - 4) * 6;  // Each stream uses 6 bits in HISR/HIFCR
    }
    
    uint32_t tc_bit = 1 << (stream_offset + 1);  // TCIFx is bit 1 for each stream
    uint32_t te_bit = 1 << (stream_offset + 3);  // TEIFx is bit 3 for each stream

    // Transfer Complete
    if (*ISR & tc_bit) {
        *IFCR = tc_bit; // clear TC flag

        // Call appropriate callback
        if (cfg == &h->_dma.tx_dma && h->uart_cfg.tx_callback)
            h->uart_cfg.tx_callback();
        else if (cfg == &h->_dma.rx_dma && h->uart_cfg.rx_callback)
            h->uart_cfg.rx_callback();
    }

    // Transfer Error
    if (*ISR & te_bit) {
        *IFCR = te_bit; // clear TE flag
        // Optional: handle error
    }
}
extern HSerial_instance_t h_ser;
// UART1 TX = DMA2 Stream7
void DMA2_Stream7_IRQHandler(void) {
    DMA_IRQ_Handler_Generic(&h_ser, &h_ser._dma.tx_dma);
}

// UART1 RX = DMA2 Stream2
void DMA2_Stream2_IRQHandler(void) {
    DMA_IRQ_Handler_Generic(&h_ser, &h_ser._dma.rx_dma);
}

// UART2 TX = DMA1 Stream6
void DMA1_Stream6_IRQHandler(void) {
    DMA_IRQ_Handler_Generic(&h_ser, &h_ser._dma.tx_dma);
}

// UART2 RX = DMA1 Stream5
void DMA1_Stream5_IRQHandler(void) {
    DMA_IRQ_Handler_Generic(&h_ser, &h_ser._dma.rx_dma);
}

// UART6 TX = DMA2 Stream6
void DMA2_Stream6_IRQHandler(void) {
    DMA_IRQ_Handler_Generic(&h_ser, &h_ser._dma.tx_dma);
}

// UART6 RX = DMA2 Stream1
void DMA2_Stream1_IRQHandler(void) {
    DMA_IRQ_Handler_Generic(&h_ser, &h_ser._dma.rx_dma);
}


static void init_uart_pins(uart_num_t num)
{
    GPIO_PinConfig_t uart_pin_cfg;

    uart_pin_cfg.mode = GPIO_MODE_ALTFN;
    uart_pin_cfg.pull = GPIO_PULL_NO;
    uart_pin_cfg.speed = GPIO_SPEED_HIGH;
    uart_pin_cfg.output_type = GPIO_OUTPUT_PUSHPULL;

    // set pin for tx
    switch (num)
    {
    case UART_NUM_1:
        uart_pin_cfg.alt_function = GPIO_AF7_USART1_2;
        uart_pin_cfg.port = GPIOA;
        uart_pin_cfg.pin = 9;
        break;
    case UART_NUM_2:
        uart_pin_cfg.alt_function = GPIO_AF7_USART1_2;
        uart_pin_cfg.port = GPIOA;
        uart_pin_cfg.pin = 2;
        break;
    case UART_NUM_6:
        uart_pin_cfg.alt_function = GPIO_AF8_USART6;
        uart_pin_cfg.port = GPIOC;
        uart_pin_cfg.pin = 6;
        break;
    default:
        break;
    }

    gpio_init(&uart_pin_cfg);

    // set pin for rx
    switch (num)
    {
    case UART_NUM_1:
        uart_pin_cfg.port = GPIOA;
        uart_pin_cfg.alt_function = GPIO_AF7_USART1_2;
        uart_pin_cfg.pin = 10;
        break;
    case UART_NUM_2:
        uart_pin_cfg.port = GPIOA;
        uart_pin_cfg.alt_function = GPIO_AF7_USART1_2;
        uart_pin_cfg.pin = 3;
        break;
    case UART_NUM_6:
        uart_pin_cfg.port = GPIOC;
        uart_pin_cfg.alt_function = GPIO_AF8_USART6;
        uart_pin_cfg.pin = 7;
        break;
    default:
        break;
    }

    gpio_init(&uart_pin_cfg);
}

static void init_uart_gpio_port(uart_num_t num)
{
    switch (num)
    {
    case UART_NUM_1:
    case UART_NUM_2:
        rcc_En_clk_preiph(RCC_GPIOA);
        break;
    case UART_NUM_6:
        rcc_En_clk_preiph(RCC_GPIOC);
        break;
    default:
        break;
    }
}

static void init_uart_rcc(uart_num_t num)
{
    switch (num)
    {
    case UART_NUM_1:
        rcc_En_clk_preiph(RCC_USART1);
        break;
    case UART_NUM_2:
        rcc_En_clk_preiph(RCC_USART2);
        break;
    case UART_NUM_6:
        rcc_En_clk_preiph(RCC_USART6);
        break;
    default:
        break;
    }
}


// uart1 -> dma2 rx: stream(2, 5) ch4, tx: stream7 ch4
// uart6 -> dma2 rx: stream(1, 2) ch5, tx: stream(6, 7) ch5
// uart2 -> dma1 rx: stream5 ch4, tx: stream6 ch4
static void init_uart_dma_rcc(uart_num_t num)
{
    switch (num)
    {
    case UART_NUM_1:
    case UART_NUM_6:
        
        rcc_En_clk_preiph(RCC_DMA2);
        break;
    case UART_NUM_2:
        rcc_En_clk_preiph(RCC_DMA1);
        break;
    default:
        while (1); // GET STUCK
        
    }
}


static void uart_config_tx_dma(HSerial_instance_t *h)
{
    dma_cfg_t *cfg = &h->_dma.tx_dma;
    uart_reg_t *uart_reg = get_uart_reg(h->uart_cfg.uart_number);

    switch (h->uart_cfg.uart_number)
    {
    case UART_NUM_1:
        cfg->dma = DMA2;
        cfg->stream_number = DMA_STREAM_7;
        cfg->channel = DMA_CHANNEL_4;
        break;

    case UART_NUM_2:
        cfg->dma = DMA1;
        cfg->stream_number = DMA_STREAM_6;
        cfg->channel = DMA_CHANNEL_4;
        break;

    case UART_NUM_6:
        cfg->dma = DMA2;
        cfg->stream_number = DMA_STREAM_6;
        cfg->channel = DMA_CHANNEL_5;
        break;

    default:
        while (1);
    }

    cfg->from = h->_dma.tx_buffer;    // memory
    cfg->to   = (void *)&uart_reg->DR; // peripheral

    cfg->direction = DMA_DIR_MEM_TO_PERIPH;
    cfg->from_increment = 1; // increment memory
    cfg->to_increment   = 0; // peripheral never increments
    cfg->element_size   = ELEMENT_SIZE_8_BITS;
    cfg->data_length    = h->_dma.tx_length;
}


static void uart_config_rx_dma(HSerial_instance_t *h)
{
    dma_cfg_t *cfg = &h->_dma.rx_dma;
    uart_reg_t *uart_reg = get_uart_reg(h->uart_cfg.uart_number);

    switch (h->uart_cfg.uart_number)
    {
    case UART_NUM_1:
        cfg->dma = DMA2;
        cfg->stream_number = DMA_STREAM_2;
        cfg->channel = DMA_CHANNEL_4;
        break;

    case UART_NUM_2:
        cfg->dma = DMA1;
        cfg->stream_number = DMA_STREAM_5;
        cfg->channel = DMA_CHANNEL_4;
        break;

    case UART_NUM_6:
        cfg->dma = DMA2;
        cfg->stream_number = DMA_STREAM_1;
        cfg->channel = DMA_CHANNEL_5;
        break;

    default:
        while (1);
    }
    cfg->from = (void *)&uart_reg->DR; // peripheral
    cfg->to   = h->_dma.rx_buffer;     // memory

    cfg->direction = DMA_DIR_PERIPH_TO_MEM;
    cfg->from_increment = 0; // peripheral never increments
    cfg->to_increment   = 1; // increment memory
    cfg->element_size   = ELEMENT_SIZE_8_BITS;
    cfg->data_length    = h->_dma.rx_length;

}


static void enable_dma_nvic(uart_num_t num)
{
    switch (num)
    {
    case UART_NUM_1:
        NVIC_EnableIRQ(DMA2_Stream2_IRQn); // RX
        NVIC_EnableIRQ(DMA2_Stream7_IRQn); // TX
        break;

    case UART_NUM_2:
        NVIC_EnableIRQ(DMA1_Stream5_IRQn);
        NVIC_EnableIRQ(DMA1_Stream6_IRQn);
        break;

    case UART_NUM_6:
        NVIC_EnableIRQ(DMA2_Stream1_IRQn);
        NVIC_EnableIRQ(DMA2_Stream6_IRQn);
        break;

    default:
        break;
    }
}


HSerial_error_t HSerial_init(HSerial_instance_t* h)
{
    if (h == NULL) return HSERIAL_NOK;
    HSerial_error_t ret = HSERIAL_NOK;

    if (h->type == HSERIAL_TYPE_UART) {
        init_uart_gpio_port(h->uart_cfg.uart_number);
        init_uart_pins(h->uart_cfg.uart_number);
        init_uart_rcc(h->uart_cfg.uart_number);
        enable_dma_nvic(h->uart_cfg.uart_number);
        
        uart_cfg_t uart_cfg = {
            .baud = h->uart_cfg.baudrate,
                .uart_num = h->uart_cfg.uart_number,
                .word_length = h->uart_cfg.word_length,
                .parity = h->uart_cfg.parity,
                .stop_bits = h->uart_cfg.stop_bits,
                .rx_callback = h->uart_cfg.rx_callback,
                .tx_callback = h->uart_cfg.tx_callback
        };

        uart_init(&uart_cfg);
        init_uart_dma_rcc(h->uart_cfg.uart_number);
        
        uart_disable_interrupts(h->uart_cfg.uart_number);
        ret = HSERIAL_OK;
    }




    
    return ret;
}


HSerial_error_t HSerial_deinit(HSerial_instance_t* h)
{
    if (!h) return HSERIAL_NOK;

    uart_dma_disable_rx(h->uart_cfg.uart_number);
    uart_dma_disable_tx(h->uart_cfg.uart_number);

    return HSERIAL_OK;
}


HSerial_error_t HSerial_send_data(
    HSerial_instance_t* h,
    const uint8_t* data,
    uint32_t length
)
{
    if (!h || !data || length == 0)
        return HSERIAL_NOK;

    /* Store TX buffer */
    h->_dma.tx_buffer = (uint8_t *)data;
    h->_dma.tx_length = length;

    /* Configure TX DMA */
    uart_config_tx_dma(h);
    dma_init(&h->_dma.tx_dma);

    /* Enable UART TX DMA */
    uart_dma_enable_tx(h->uart_cfg.uart_number);

    /* Start DMA */
    dma_start(
        h->_dma.tx_dma.dma,
        h->_dma.tx_dma.stream_number
    );

    return HSERIAL_OK;
}



HSerial_error_t HSerial_receive_data(
    HSerial_instance_t* h,
    uint8_t* data,
    uint32_t length
)
{
    if (!h || !data || length == 0)
        return HSERIAL_NOK;

    /* Store RX buffer */
    h->_dma.rx_buffer = data;
    h->_dma.rx_length = length;

    /* Configure RX DMA */
    uart_config_rx_dma(h);
    
    dma_stream_reset(h->_dma.rx_dma.dma, h->_dma.rx_dma.stream_number);

    dma_init(&h->_dma.rx_dma);

    /* Enable UART RX DMA */
    uart_dma_enable_rx(h->uart_cfg.uart_number);

    /* Start DMA */
    dma_start(
        h->_dma.rx_dma.dma,
        h->_dma.rx_dma.stream_number
    );

    return HSERIAL_OK;
}
