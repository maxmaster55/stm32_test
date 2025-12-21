#include "HAL/HSerial/hserial.h"
#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <MCAL/uart/uart.h>
#include <MCAL/NVIC/nvic.h>

// shit you need to make the uart work

// uart_cfg_t uart1_test_cfg = {
//     .baud = 9600,
//     .uart_num = UART_NUM_1,
//     .word_length = UART_WORD_LENGTH_8,
//     .parity = UART_PARITY_NONE,
//     .stop_bits = UART_STOP_BITS_1,
//     .rx_callback = do_smth,
//     .tx_callback = do_smth
// } ;



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

    cfg->from = h->_dma.tx_buffer;              // memory
    cfg->to   = (void *)&uart_reg->DR;          // USART DR

    cfg->direction      = DMA_DIR_MEM_TO_PERIPH;
    cfg->from_increment = 1;
    cfg->to_increment   = 0;
    cfg->element_size   = ELEMENT_SIZE_8_BITS;
    cfg->data_length    = h->_dma.tx_length;
    cfg->Priority       = DMA_PRIORITY_HIGH;
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
    cfg->from = (void *)&uart_reg->DR;
    cfg->to = h->_dma.rx_buffer;

    cfg->direction = DMA_DIR_PERIPH_TO_MEM;
    cfg->from_increment = 0;
    cfg->to_increment   = 1;
    cfg->element_size   = ELEMENT_SIZE_8_BITS;
    cfg->data_length    = h->_dma.rx_length;
    cfg->Priority       = DMA_PRIORITY_HIGH;
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


HSerial_error_t HSerial_init(HSerial_instance_t* h_instance)
{
    if (h_instance == NULL) return HSERIAL_NOK;
    HSerial_error_t ret = HSERIAL_NOK;


    if (h_instance->type == HSERIAL_TYPE_UART) {
        init_uart_gpio_port(h_instance->uart_cfg.uart_number);
        init_uart_pins(h_instance->uart_cfg.uart_number);
        init_uart_rcc(h_instance->uart_cfg.uart_number);
        enable_dma_nvic(h_instance->uart_cfg.uart_number);
        
        uart_cfg_t uart_cfg = {
            .baud = h_instance->uart_cfg.baudrate,
                .uart_num = h_instance->uart_cfg.uart_number,
                .word_length = h_instance->uart_cfg.word_length,
                .parity = h_instance->uart_cfg.parity,
                .stop_bits = h_instance->uart_cfg.stop_bits,
                .rx_callback = h_instance->uart_cfg.rx_callback,
                .tx_callback = h_instance->uart_cfg.tx_callback
        };

        uart_init(&uart_cfg);
        init_uart_dma_rcc(h_instance->uart_cfg.uart_number);
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
