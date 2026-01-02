#pragma once

#include "i2c_regs.h"


#define I2C1 ((i2c_regs_t*)0x40005400)
#define I2C2 ((i2c_regs_t*)0x40005800)
#define I2C3 ((i2c_regs_t*)0x40005C00)

// masks

#define I2C_SR1_SB       (1U << 0)   // Start bit (Master mode)
#define I2C_SR1_ADDR     (1U << 1)   // Address sent/matched
#define I2C_SR1_BTF      (1U << 2)   // Byte transfer finished
#define I2C_SR1_ADD10    (1U << 3)   // 10-bit header sent (Master)
#define I2C_SR1_STOPF    (1U << 4)   // Stop detection (Slave)
#define I2C_SR1_RXNE     (1U << 6)   // Data register not empty (receiving)
#define I2C_SR1_TXE      (1U << 7)   // Data register empty (transmitting)
#define I2C_SR1_BERR     (1U << 8)   // Bus error
#define I2C_SR1_ARLO     (1U << 9)   // Arbitration lost
#define I2C_SR1_AF       (1U << 10)  // Acknowledge failure (NACK)
#define I2C_SR1_OVR      (1U << 11)  // Overrun/Underrun
#define I2C_SR1_PECERR   (1U << 12)  // PEC error in reception
#define I2C_SR1_TIMEOUT  (1U << 14)  // Timeout (SMBus mode)
#define I2C_SR1_SMBALERT (1U << 15)  // SMBus alert


#define NORMAL_MODE_MAX (100000)
typedef enum{
    I2C_MODE_MASTER,
    I2C_MODE_SLAVE
} i2c_mode_t;


typedef enum{
    I2C_OK,
    I2C_NOK
} i2c_ret_t;


typedef enum {
    I2C_STATE_IDLE,
    I2C_STATE_MASTER_TX,
    I2C_STATE_MASTER_RX,
} i2c_state_t;


typedef void (*i2c_tx_cb_t)(void);
typedef void (*i2c_rx_cb_t)(uint8_t *data, uint16_t len);
typedef void (*i2c_err_cb_t)(uint32_t sr1);


typedef struct {
    i2c_regs_t *i2c;

    volatile i2c_state_t state;

    uint8_t  *buf;
    uint16_t  len;
    uint16_t  idx;
    uint8_t   slave_addr;
    bool no_stop;
    i2c_tx_cb_t  tx_done_cb;
    i2c_rx_cb_t  rx_done_cb;
    i2c_err_cb_t error_cb;
} i2c_ctx_t;


// inst
typedef struct
{
    i2c_regs_t* i2c;
    i2c_mode_t mode;
    uint32_t freq;
    uint8_t my_address;
    // interrupts
    i2c_tx_cb_t  tx_cb;
    i2c_rx_cb_t  rx_cb;
    i2c_err_cb_t err_cb;
} i2c_cfg_t;


// api
i2c_ret_t i2c_init(i2c_cfg_t* cfg);
i2c_ret_t i2c_send(i2c_cfg_t* cfg, uint8_t slave_addr, uint8_t *data, uint16_t len);
i2c_ret_t i2c_receive(i2c_cfg_t* cfg, uint8_t slave_addr, uint8_t* data, uint16_t len);
i2c_ret_t i2c_send_no_stop(i2c_cfg_t* cfg, uint8_t slave_addr, uint8_t *data, uint16_t len);

bool i2c_is_busy(i2c_cfg_t* cfg);
bool i2c_is_error(i2c_cfg_t* cfg);

