#include "glob.h"
#include <stm32_drivers/MCAL/flash/flash.h>
#include <stm32_drivers/HAL/HSerial/hserial.h>

// __attribute__((section(".magic")))
// uint8_t x[3] = {0xA5, 0xA5, 0xA5};

volatile int rx = 0;
volatile int tx = 0;

void rx_cb(){
    rx++;
}

void tx_cb(){
    tx++;
}


HSerial_instance_t h =  {
    .type = HSERIAL_TYPE_UART,
    .rx_callback = rx_cb,
    .tx_callback = tx_cb,
    .uart_cfg.baudrate = 9600,
    .uart_cfg.uart_number = UART_NUM_1,
    .uart_cfg.word_length = UART_WORD_LENGTH_8,
    .uart_cfg.parity = UART_PARITY_NONE,
    .uart_cfg.stop_bits = UART_STOP_BITS_1,
};

void delay(){
    for (volatile int i = 0; i < 400000; i++)
    {
        
    }
    
}

char buff[5];

int main(void)
{
    flash_unlock();
    delay();
    // flash_erase_sector(5);
    flash_erase_sector(FLASH_SECTOR_5);
    flash_write(0x08020000, 0x77777777, FLASH_WRITE_BYTE);
    while(1);
}

