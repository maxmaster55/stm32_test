#include "HAL/HSerial/hserial.h"
#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <MCAL/uart/uart.h>
#include <MCAL/SPI/spi.h>
#include <MCAL/NVIC/nvic.h>


static HSerial_instance_t *dma_owner[2][8] = {0};

/* IRQ handler */
static inline uint8_t dma_index(dma_regs_t *dma)
{
    return (dma == DMA1) ? 0 : 1;
}
static void register_dma_owner(HSerial_instance_t *h, dma_cfg_t *cfg)
{
    uint8_t d = dma_index(cfg->dma);
    uint8_t s = cfg->stream_number;

    dma_owner[d][s] = h;
}

static inline void dma_handle_stream_irq(HSerial_instance_t *h, dma_cfg_t *cfg)
{
    dma_regs_t *dma = cfg->dma;
    uint8_t stream = cfg->stream_number;
    uint8_t tc_flag = 0, te_flag = 0;

    if (stream <= 3) {
        dma_LISR_reg_t *isr  = &dma->LISR;
        dma_LIFCR_reg_t *ifcr = &dma->LIFCR;
        switch (stream) {
            case 0: tc_flag = isr->bits.TCIF0; te_flag = isr->bits.TEIF0; ifcr->bits.CTCIF0 = tc_flag; ifcr->bits.CTEIF0 = te_flag; break;
            case 1: tc_flag = isr->bits.TCIF1; te_flag = isr->bits.TEIF1; ifcr->bits.CTCIF1 = tc_flag; ifcr->bits.CTEIF1 = te_flag; break;
            case 2: tc_flag = isr->bits.TCIF2; te_flag = isr->bits.TEIF2; ifcr->bits.CTCIF2 = tc_flag; ifcr->bits.CTEIF2 = te_flag; break;
            case 3: tc_flag = isr->bits.TCIF3; te_flag = isr->bits.TEIF3; ifcr->bits.CTCIF3 = tc_flag; ifcr->bits.CTEIF3 = te_flag; break;
        }
    } else {
        dma_HISR_reg_t *isr  = &dma->HISR;
        dma_HIFCR_reg_t *ifcr = &dma->HIFCR;
        switch (stream) {
            case 4: tc_flag = isr->bits.TCIF4; te_flag = isr->bits.TEIF4; ifcr->bits.CTCIF4 = tc_flag; ifcr->bits.CTEIF4 = te_flag; break;
            case 5: tc_flag = isr->bits.TCIF5; te_flag = isr->bits.TEIF5; ifcr->bits.CTCIF5 = tc_flag; ifcr->bits.CTEIF5 = te_flag; break;
            case 6: tc_flag = isr->bits.TCIF6; te_flag = isr->bits.TEIF6; ifcr->bits.CTCIF6 = tc_flag; ifcr->bits.CTEIF6 = te_flag; break;
            case 7: tc_flag = isr->bits.TCIF7; te_flag = isr->bits.TEIF7; ifcr->bits.CTCIF7 = tc_flag; ifcr->bits.CTEIF7 = te_flag; break;
        }
    }

    if (tc_flag) {
        if (cfg == &h->_dma.tx_dma && h->tx_callback) h->tx_callback();
        else if (cfg == &h->_dma.rx_dma && h->rx_callback) h->rx_callback();
    }
}


void DMA_IRQ_Handler_Generic(dma_regs_t *dma, uint8_t stream)
{
    HSerial_instance_t *h = dma_owner[dma_index(dma)][stream];
    if (!h) return; // no owner registered, ignore

    dma_cfg_t *cfg = (h->_dma.tx_dma.stream_number == stream) ? &h->_dma.tx_dma : &h->_dma.rx_dma;
    dma_handle_stream_irq(h, cfg);
}
void DMA1_Stream5_IRQHandler(void) { DMA_IRQ_Handler_Generic(DMA1, 5); }
void DMA1_Stream6_IRQHandler(void) { DMA_IRQ_Handler_Generic(DMA1, 6); }
void DMA2_Stream1_IRQHandler(void) { DMA_IRQ_Handler_Generic(DMA2, 1); }
void DMA2_Stream2_IRQHandler(void) { DMA_IRQ_Handler_Generic(DMA2, 2); }
void DMA2_Stream3_IRQHandler(void) { DMA_IRQ_Handler_Generic(DMA2, 3); }
void DMA2_Stream6_IRQHandler(void) { DMA_IRQ_Handler_Generic(DMA2, 6); }
void DMA2_Stream7_IRQHandler(void) { DMA_IRQ_Handler_Generic(DMA2, 7); }

/** uart helpers
 * @note uart1 -> dma2 rx: stream(2, 5) ch4, tx: stream7 ch4
 *       uart6 -> dma2 rx: stream(1, 2) ch5, tx: stream(6, 7) ch5
 *       uart2 -> dma1 rx: stream5 ch4, tx: stream6 ch4
 *
 */
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


/** spi helpers
 * @note 
 *       
 *       
 *
 */
static void init_spi_pins(spi_num_t num)
{
    GPIO_PinConfig_t spi_pin_cfg;

    // common configuration
    spi_pin_cfg.mode = GPIO_MODE_ALTFN;
    spi_pin_cfg.pull = GPIO_PULL_NO;
    spi_pin_cfg.speed = GPIO_SPEED_HIGH;
    spi_pin_cfg.output_type = GPIO_OUTPUT_PUSHPULL;

    switch (num)
    {
        case SPI_NUM_1:
            // SCK
            spi_pin_cfg.port = GPIOA;
            spi_pin_cfg.pin = 5;
            spi_pin_cfg.alt_function = GPIO_AF5_SPI1_2;
            gpio_init(&spi_pin_cfg);

            // MISO
            spi_pin_cfg.port = GPIOA;
            spi_pin_cfg.pin = 6;
            gpio_init(&spi_pin_cfg);

            // MOSI
            spi_pin_cfg.port = GPIOA;
            spi_pin_cfg.pin = 7;
            gpio_init(&spi_pin_cfg);
            break;

        case SPI_NUM_2:
            // SCK
            spi_pin_cfg.port = GPIOB;
            spi_pin_cfg.pin = 13;
            spi_pin_cfg.alt_function = GPIO_AF5_SPI1_2;
            gpio_init(&spi_pin_cfg);

            // MISO
            spi_pin_cfg.port = GPIOB;
            spi_pin_cfg.pin = 14;
            gpio_init(&spi_pin_cfg);

            // MOSI
            spi_pin_cfg.port = GPIOB;
            spi_pin_cfg.pin = 15;
            gpio_init(&spi_pin_cfg);
            break;

        case SPI_NUM_3:
            // SCK
            spi_pin_cfg.port = GPIOB;
            spi_pin_cfg.pin = 3;
            spi_pin_cfg.alt_function = GPIO_AF6_SPI3;
            gpio_init(&spi_pin_cfg);

            // MISO
            spi_pin_cfg.port = GPIOB;
            spi_pin_cfg.pin = 4;
            gpio_init(&spi_pin_cfg);

            // MOSI
            spi_pin_cfg.port = GPIOB;
            spi_pin_cfg.pin = 5;
            gpio_init(&spi_pin_cfg);
            break;
        default:
            break;
    }
}
static void init_spi_gpio_port(spi_num_t num)
{
    switch (num)
    {
        case SPI_NUM_1:
            // SPI1 pins: PA4 (NSS), PA5 (SCK), PA6 (MISO), PA7 (MOSI)
            rcc_En_clk_preiph(RCC_GPIOA);
            break;

        case SPI_NUM_2:
            // SPI2 pins: PB12 (NSS), PB13 (SCK), PB14 (MISO), PB15 (MOSI)
            rcc_En_clk_preiph(RCC_GPIOB);
            break;

        case SPI_NUM_3:
            // SPI3 pins: PA15 (NSS), PB3 (SCK), PB4 (MISO), PB5 (MOSI)
            rcc_En_clk_preiph(RCC_GPIOA);
            rcc_En_clk_preiph(RCC_GPIOB);
            break;
        default:
            break;
    }
}
static void init_spi_rcc(spi_num_t num)
{
        switch (num)
    {
        case SPI_NUM_1:
            rcc_En_clk_preiph(RCC_SPI1);
            break;
        case SPI_NUM_4:
            rcc_En_clk_preiph(RCC_SPI1);
            break;
        case SPI_NUM_2:
            rcc_En_clk_preiph(RCC_SPI1);
            break;
        case SPI_NUM_3:
            rcc_En_clk_preiph(RCC_SPI1);
            break;
        default:
            while(1); // Invalid SPI number, trap here
    }
}
static void init_spi_dma_rcc(spi_num_t num)
{
    switch (num)
    {
        case SPI_NUM_1:
        case SPI_NUM_4:
            rcc_En_clk_preiph(RCC_DMA2); // SPI1 and SPI4 use DMA2
            break;

        case SPI_NUM_2:
        case SPI_NUM_3:
            rcc_En_clk_preiph(RCC_DMA1); // SPI2 and SPI3 use DMA1
            break;

        default:
            while(1); // Invalid SPI number, trap here
    }
}
static void spi_config_tx_dma(HSerial_instance_t *h)
{
    dma_cfg_t *cfg = &h->_dma.tx_dma;

    switch (h->spi_cfg.spi_num)
    {
        case SPI_NUM_1:
            cfg->dma = DMA2;
            cfg->stream_number = DMA_STREAM_3;
            cfg->channel = DMA_CHANNEL_3;
            break;

        case SPI_NUM_2:
            cfg->dma = DMA1;
            cfg->stream_number = DMA_STREAM_4;
            cfg->channel = DMA_CHANNEL_0;
            break;

        case SPI_NUM_3:
            cfg->dma = DMA1;
            cfg->stream_number = DMA_STREAM_5;
            cfg->channel = DMA_CHANNEL_0;
            break;

        case SPI_NUM_4:
            cfg->dma = DMA2;
            cfg->stream_number = DMA_STREAM_5;
            cfg->channel = DMA_CHANNEL_4;
            break;

        default:
            while(1); // invalid SPI
    }

    cfg->from = h->_dma.tx_buffer;           // memory
    SPI_regs_t* spi = NULL;
    switch(h->spi_cfg.spi_num) {
        case SPI_NUM_1: spi = SPI1; break;
        case SPI_NUM_2: spi = SPI2; break;
        case SPI_NUM_3: spi = SPI3; break;
        case SPI_NUM_4: spi = SPI4; break;
    }
    cfg->to = &spi->DR;
    cfg->direction = DMA_DIR_MEM_TO_PERIPH;
    cfg->from_increment = 1;
    cfg->to_increment = 0;
    cfg->element_size = ELEMENT_SIZE_8_BITS;
    cfg->data_length = h->_dma.tx_length;
}
static void spi_config_rx_dma(HSerial_instance_t *h)
{
    dma_cfg_t *cfg = &h->_dma.rx_dma;

    SPI_regs_t* spi = NULL;
    switch(h->spi_cfg.spi_num) {
        case SPI_NUM_1: spi = SPI1; break;
        case SPI_NUM_2: spi = SPI2; break;
        case SPI_NUM_3: spi = SPI3; break;
        case SPI_NUM_4: spi = SPI4; break;
    }


    switch (h->spi_cfg.spi_num)
    {
        case SPI_NUM_1:
            cfg->dma = DMA2;
            cfg->stream_number = DMA_STREAM_2;
            cfg->channel = DMA_CHANNEL_3;
            break;

        case SPI_NUM_2:
            cfg->dma = DMA1;
            cfg->stream_number = DMA_STREAM_3;
            cfg->channel = DMA_CHANNEL_0;
            break;

        case SPI_NUM_3:
            cfg->dma = DMA1;
            cfg->stream_number = DMA_STREAM_0;
            cfg->channel = DMA_CHANNEL_0;
            break;

        case SPI_NUM_4:
            cfg->dma = DMA2;
            cfg->stream_number = DMA_STREAM_4;
            cfg->channel = DMA_CHANNEL_4;
            break;

        default:
            while(1); // invalid SPI
    }

    cfg->from = &spi->DR;           // peripheral
    cfg->to   = h->_dma.rx_buffer;      // memory
    cfg->direction = DMA_DIR_PERIPH_TO_MEM;
    cfg->from_increment = 0;
    cfg->to_increment = 1;
    cfg->element_size = ELEMENT_SIZE_8_BITS;
    cfg->data_length = h->_dma.rx_length;
}
static void enable_spi_dma_nvic(spi_num_t num)
{
    switch (num)
    {
        case SPI_NUM_1:
            NVIC_EnableIRQ(DMA2_Stream2_IRQn); // RX
            NVIC_EnableIRQ(DMA2_Stream3_IRQn); // TX
            break;

        case SPI_NUM_2:
            NVIC_EnableIRQ(DMA1_Stream3_IRQn); // RX
            NVIC_EnableIRQ(DMA1_Stream4_IRQn); // TX
            break;

        case SPI_NUM_3:
            NVIC_EnableIRQ(DMA1_Stream0_IRQn); // RX
            NVIC_EnableIRQ(DMA1_Stream5_IRQn); // TX
            break;

        case SPI_NUM_4:
            NVIC_EnableIRQ(DMA2_Stream4_IRQn); // RX
            NVIC_EnableIRQ(DMA2_Stream5_IRQn); // TX
            break;

        default:
            break;
    }
}


// api
HSerial_error_t HSerial_init(HSerial_instance_t* h)
{
    if (h == NULL) return HSERIAL_NOK;
    HSerial_error_t ret = HSERIAL_NOK;

    if (h->type == HSERIAL_TYPE_UART) {
        init_uart_gpio_port(h->uart_cfg.uart_number);
        init_uart_pins(h->uart_cfg.uart_number);
        init_uart_rcc(h->uart_cfg.uart_number);
        
        uart_cfg_t uart_cfg = {
            .baud = h->uart_cfg.baudrate,
                .uart_num = h->uart_cfg.uart_number,
                .word_length = h->uart_cfg.word_length,
                .parity = h->uart_cfg.parity,
                .stop_bits = h->uart_cfg.stop_bits,
        };

        uart_init(&uart_cfg);
        init_uart_dma_rcc(h->uart_cfg.uart_number);
        enable_dma_nvic(h->uart_cfg.uart_number);
        
        uart_disable_interrupts(h->uart_cfg.uart_number);
        ret = HSERIAL_OK;
    }else if (h->type == HSERIAL_TYPE_SPI){
        init_spi_gpio_port(h->spi_cfg.spi_num);
        init_spi_pins(h->spi_cfg.spi_num);

        spi_cfg_t spi_cfg = {
            .spi_num = h->spi_cfg.spi_num,
            .mode = h->spi_cfg.mode,
            .speed = h->spi_cfg.speed,
            .data_size = SPI_DATASIZE_8BIT,
            .frame_format = SPI_FRAME_MSB_FIRST,
            .nss = SPI_NSS_SOFT,
            .protocol = SPI_PROTOCOL_MOTOROLA
        };

        init_spi_rcc(spi_cfg.spi_num);
        spi_init(&spi_cfg);
        init_spi_dma_rcc(spi_cfg.spi_num);
        spi_DMA_enable(spi_cfg.spi_num);
        spi_config_tx_dma(h);
        spi_config_rx_dma(h);
        enable_spi_dma_nvic(spi_cfg.spi_num);
        ret = HSERIAL_OK;

    }
    return ret;
}


HSerial_error_t HSerial_deinit(HSerial_instance_t* h)
{
    if (!h) return HSERIAL_NOK;

    if (h->type == HSERIAL_TYPE_UART) {
        uart_dma_disable_rx(h->uart_cfg.uart_number);
        uart_dma_disable_tx(h->uart_cfg.uart_number);
    } else if (h->type == HSERIAL_TYPE_SPI) {
        dma_stream_disable(h->_dma.rx_dma.dma, h->_dma.rx_dma.stream_number);
        dma_stream_disable(h->_dma.tx_dma.dma, h->_dma.tx_dma.stream_number);
        spi_DMA_disable(h->spi_cfg.spi_num);
    }

    return HSERIAL_OK;
}



HSerial_error_t HSerial_send_data(HSerial_instance_t* h, const uint8_t* data, uint32_t length)
{
    if (!h || !data || length == 0) return HSERIAL_NOK;

    h->_dma.tx_buffer = (uint8_t*)data;
    h->_dma.tx_length = length;

    if (h->type == HSERIAL_TYPE_UART) {
        uart_config_tx_dma(h);
        h->_dma.tx_dma.data_length = length;  
        register_dma_owner(h, &h->_dma.tx_dma);
        dma_stream_reset(h->_dma.tx_dma.dma, h->_dma.tx_dma.stream_number);
        dma_init(&h->_dma.tx_dma);
        uart_dma_enable_tx(h->uart_cfg.uart_number);
        dma_start(h->_dma.tx_dma.dma, h->_dma.tx_dma.stream_number);
    }
    else if (h->type == HSERIAL_TYPE_SPI) {
        spi_config_tx_dma(h);
        h->_dma.tx_dma.data_length = length; 
        register_dma_owner(h, &h->_dma.tx_dma);
        dma_stream_reset(h->_dma.tx_dma.dma, h->_dma.tx_dma.stream_number);
        dma_init(&h->_dma.tx_dma);
        dma_start(h->_dma.tx_dma.dma, h->_dma.tx_dma.stream_number);
    }

    return HSERIAL_OK;
}



HSerial_error_t HSerial_receive_data(HSerial_instance_t* h, uint8_t* data, uint32_t length)
{
    if (!h || !data || length == 0) return HSERIAL_NOK;

    h->_dma.rx_buffer = data;
    h->_dma.rx_length = length;

    if (h->type == HSERIAL_TYPE_UART) {
        uart_config_rx_dma(h);
        h->_dma.rx_dma.data_length = length;
        register_dma_owner(h, &h->_dma.rx_dma);
        dma_stream_reset(h->_dma.rx_dma.dma, h->_dma.rx_dma.stream_number);
        dma_init(&h->_dma.rx_dma);
        uart_dma_enable_rx(h->uart_cfg.uart_number);
        dma_start(h->_dma.rx_dma.dma, h->_dma.rx_dma.stream_number);
    }
    else if (h->type == HSERIAL_TYPE_SPI) {
        spi_config_rx_dma(h);
        h->_dma.rx_dma.data_length = length;
        register_dma_owner(h, &h->_dma.rx_dma);
        dma_stream_reset(h->_dma.rx_dma.dma, h->_dma.rx_dma.stream_number);
        dma_init(&h->_dma.rx_dma);
        dma_start(h->_dma.rx_dma.dma, h->_dma.rx_dma.stream_number);
    }

    return HSERIAL_OK;
}