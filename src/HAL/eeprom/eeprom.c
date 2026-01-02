#include "HAL/eeprom/eeprom.h"
#include <stdbool.h>

static i2c_cfg_t i2c_cfg;

void eeprom_init(eeprom_cfg_t* cfg)
{
    if (cfg == NULL) return;
    
    // Calculate device address: 0b1010[A2][A1][A0]
    
    // Configure I2C
    i2c_cfg.i2c = cfg->i2c;
    i2c_cfg.freq = 100000;  // 100 kHz
    i2c_cfg.mode = I2C_MODE_MASTER;
    i2c_cfg.tx_cb = NULL;
    i2c_cfg.rx_cb = NULL;
    i2c_cfg.err_cb = NULL;
    
    i2c_init(&i2c_cfg);
    
    // Configure address pins as outputs
    GPIO_PinConfig_t pin_cfg;
    pin_cfg.mode = GPIO_MODE_OUTPUT;
    pin_cfg.pull = GPIO_PULL_NO;
    pin_cfg.speed = GPIO_SPEED_FAST;
    pin_cfg.alt_function = GPIO_AF0_SYSTEM;
    pin_cfg.output_type = GPIO_OUTPUT_PUSHPULL;
    
    // A0
    pin_cfg.port = cfg->A0.port;
    pin_cfg.pin = cfg->A0.pin;
    gpio_init(&pin_cfg);
    gpio_write(cfg->A0.port, cfg->A0.pin, 0);
    // A1
    pin_cfg.port = cfg->A1.port;
    pin_cfg.pin = cfg->A1.pin;
    gpio_init(&pin_cfg);
    gpio_write(cfg->A1.port, cfg->A1.pin, 0);
    // A2
    pin_cfg.port = cfg->A2.port;
    pin_cfg.pin = cfg->A2.pin;
    gpio_init(&pin_cfg);
    gpio_write(cfg->A2.port, cfg->A2.pin, 0);
}

// blocking api
void eeprom_write_blocking(eeprom_cfg_t* cfg, uint8_t mem_addr, uint8_t data)
{
    if (!cfg) return;

    uint8_t buf[2] = { mem_addr, data };

    // 1️⃣ send memory address + data
    if (i2c_send(&i2c_cfg, cfg->address, buf, 2) != I2C_OK) return;
    while (i2c_is_busy(&i2c_cfg));

    // 2️⃣ poll EEPROM until it ACKs (write cycle complete)
    i2c_ret_t ack;
    do {
        uint8_t dummy[1] = {0}; // any value
        ack = i2c_send(&i2c_cfg, cfg->address, dummy, 1);
        while (i2c_is_busy(&i2c_cfg));
    } while (ack != I2C_OK);
}

// blocking read
uint8_t eeprom_read_blocking(eeprom_cfg_t* cfg, uint8_t mem_addr)
{
    uint8_t val = 0xFF;

    // Send memory address (normal TX)
    if (i2c_send(&i2c_cfg, cfg->address, &mem_addr, 1) != I2C_OK)
        return 0xFF;

    while (i2c_is_busy(&i2c_cfg));

    // Now read
    if (i2c_receive(&i2c_cfg, cfg->address, &val, 1) != I2C_OK)
        return 0xFF;

    while (i2c_is_busy(&i2c_cfg));

    return val;
}
