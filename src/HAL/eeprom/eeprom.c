#include "HAL/eeprom/eeprom.h"
#include "service/scheduler/sched.h"
#include <stdbool.h>


static i2c_cfg_t i2c_cfg;

static uint8_t mem_addr;
static uint8_t write_data;
static uint8_t read_data;

static write_callback_t write_cb = NULL;
static read_callback_t read_cb  = NULL;

static eeprom_async_state_t eeprom_state = EEPROM_STATE_IDLE;


void eeprom_callback(void* args)
{
    eeprom_cfg_t* cfg = args;
    uint8_t slave_addr = cfg->address;

    uint8_t dummy = 0;

    switch (eeprom_state)
    {
        case EEPROM_STATE_IDLE:
            /* nothing to do */
            break;

        case EEPROM_STATE_WRITING:
        {
            uint8_t buf[2] = { mem_addr, write_data };

            if (i2c_send(&i2c_cfg, slave_addr, buf, 2) == I2C_OK)
            {
                eeprom_state = EEPROM_STATE_CHECKING;
            }
            break;
        }

        case EEPROM_STATE_READING:
        {
            /* send memory address */
            if (i2c_send(&i2c_cfg, slave_addr, &mem_addr, 1) == I2C_OK)
            {
                eeprom_state = EEPROM_STATE_READING_DATA;
            }
            break;
        }

        case EEPROM_STATE_READING_DATA:
        {
            if (i2c_receive(&i2c_cfg, slave_addr, &read_data, 1) == I2C_OK)
            {
                eeprom_state = EEPROM_STATE_CHECKING;
            }
            break;
        }

        case EEPROM_STATE_CHECKING:
        {
            /* ACK polling */
            if (i2c_send(&i2c_cfg, slave_addr, &dummy, 1) == I2C_OK)
            {
                if (read_cb)
                    read_cb(read_data);

                if (write_cb)
                    write_cb();
                read_cb  = NULL;
                write_cb = NULL;

                eeprom_state = EEPROM_STATE_IDLE;
            }
            break;
        }

        default:
            eeprom_state = EEPROM_STATE_IDLE;
            break;
    }
}


runnable_t eeprom_runnable = {
    .name = "eeprom",
    .callback = eeprom_callback,
    .every = 2,
    .first_delay = 2,
    .priority = 5,
    .args = NULL
};


void eeprom_init(eeprom_cfg_t* cfg)
{
    if (!cfg) return;

    i2c_cfg.i2c = cfg->i2c;
    i2c_cfg.freq = 100000;
    i2c_cfg.mode = I2C_MODE_MASTER;
    i2c_cfg.tx_cb = NULL;
    i2c_cfg.rx_cb = NULL;
    i2c_cfg.err_cb = NULL;

    i2c_init(&i2c_cfg);

    GPIO_PinConfig_t pin_cfg = {
        .mode = GPIO_MODE_OUTPUT,
        .pull = GPIO_PULL_NO,
        .speed = GPIO_SPEED_FAST,
        .alt_function = GPIO_AF0_SYSTEM,
        .output_type = GPIO_OUTPUT_PUSHPULL,
    };

    /* A0 */
    pin_cfg.port = cfg->A0.port;
    pin_cfg.pin  = cfg->A0.pin;
    gpio_init(&pin_cfg);
    gpio_write(cfg->A0.port, cfg->A0.pin, 0);

    /* A1 */
    pin_cfg.port = cfg->A1.port;
    pin_cfg.pin  = cfg->A1.pin;
    gpio_init(&pin_cfg);
    gpio_write(cfg->A1.port, cfg->A1.pin, 0);

    /* A2 */
    pin_cfg.port = cfg->A2.port;
    pin_cfg.pin  = cfg->A2.pin;
    gpio_init(&pin_cfg);
    gpio_write(cfg->A2.port, cfg->A2.pin, 0);

    eeprom_runnable.args = cfg;
    sched_register(&eeprom_runnable);
}

// blocking api
void eeprom_write_blocking(eeprom_cfg_t* cfg, uint8_t addr, uint8_t data)
{
    if (!cfg) return;

    uint8_t buf[2] = { addr, data };

    if (i2c_send(&i2c_cfg, cfg->address, buf, 2) != I2C_OK)
        return;

    while (i2c_is_busy(&i2c_cfg));

    uint8_t dummy = 0;
    while (i2c_send(&i2c_cfg, cfg->address, &dummy, 1) != I2C_OK)
    {
        while (i2c_is_busy(&i2c_cfg));
    }
}

uint8_t eeprom_read_blocking(eeprom_cfg_t* cfg, uint8_t addr)
{
    uint8_t val = 0xFF;

    if (i2c_send(&i2c_cfg, cfg->address, &addr, 1) != I2C_OK)
        return val;

    while (i2c_is_busy(&i2c_cfg));

    if (i2c_receive(&i2c_cfg, cfg->address, &val, 1) != I2C_OK)
        return val;

    while (i2c_is_busy(&i2c_cfg));

    return val;
}

// async api
void eeprom_write(eeprom_cfg_t* cfg, uint8_t addr, uint8_t data, write_callback_t on_done)
{
    if (!cfg || eeprom_state != EEPROM_STATE_IDLE) return;

    mem_addr = addr;
    write_data = data;
    write_cb = on_done;
    eeprom_state = EEPROM_STATE_WRITING;
}

void eeprom_read(eeprom_cfg_t* cfg, uint8_t _mem_addr, read_callback_t on_done)
{
    if (!cfg || eeprom_state != EEPROM_STATE_IDLE) return;

    mem_addr = _mem_addr;
    read_cb = on_done;

    eeprom_state = EEPROM_STATE_READING;
}

bool eeprom_is_done()
{
    return (eeprom_state == EEPROM_STATE_IDLE);
}
