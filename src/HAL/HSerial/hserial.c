#include "HAL/HSerial/hserial.h"
#include <MCAL/RCC/rcc.h>
#include <MCAL/GPIO/gpio.h>
#include <MCAL/DMA/dma.h>

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


HSerial_error_t HSerial_init(HSerial_instance_t* h_instance)
{
    if (h_instance == NULL) return HSERIAL_NOK;
    HSerial_error_t ret = HSERIAL_NOK;


    if (h_instance->type == HSERIAL_TYPE_UART) {
        init_uart_gpio_port(h_instance->uart_cfg.uart_number);
        init_uart_rcc(h_instance->uart_cfg.uart_number);
        init_uart_pins(h_instance->uart_cfg.uart_number);

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
        ret = HSERIAL_OK;
    }




    
    return ret;
}


HSerial_error_t HSerial_deinit(HSerial_instance_t* hserial_instance)
{

}


HSerial_error_t HSerial_send_data(HSerial_instance_t* hserial_instance, const uint8_t* data, uint32_t length)
{

}


HSerial_error_t HSerial_receive_data(HSerial_instance_t* hserial_instance, uint8_t* data, uint32_t length)
{

}
