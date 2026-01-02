#include "glob.h"
#include <MCAL/GPIO/gpio.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/NVIC/nvic.h>
#include <MCAL/i2c/i2c.h>

#define I2C_ERR_MASK (I2C_SR1_AF | I2C_SR1_BERR | I2C_SR1_ARLO)
// inerrupt_handler
static i2c_ctx_t i2c1_ctx;
static i2c_ctx_t i2c2_ctx;
static i2c_ctx_t i2c3_ctx;

static i2c_ctx_t* i2c_get_ctx(i2c_regs_t* i2c)
{
    if (i2c == I2C1) return &i2c1_ctx;
    if (i2c == I2C2) return &i2c2_ctx;
    if (i2c == I2C3) return &i2c3_ctx;
    return NULL;
}


void i2c_inerrupt(i2c_regs_t* i2c)
{
    i2c_ctx_t *ctx = i2c_get_ctx(i2c);
    if (!ctx) return;

    uint32_t sr1 = i2c->SR1.reg;

    // ----------- ERROR HANDLING -----------
    if (sr1 & I2C_ERR_MASK)
    {
        i2c->CR1.STOP = 1; // release bus

        if (ctx->error_cb)
            ctx->error_cb(sr1);

        if (sr1 & I2C_SR1_AF) { (void)i2c->SR1.reg; i2c->CR1.STOP = 1; }
        if (sr1 & I2C_SR1_BERR) i2c->SR1.BERR = 0;
        if (sr1 & I2C_SR1_ARLO) i2c->SR1.ARLO = 0;

        ctx->state = I2C_STATE_IDLE;
        return;
    }

    // ----------- START BIT GENERATED -----------
    if (sr1 & I2C_SR1_SB)
    {
        if (ctx->state == I2C_STATE_MASTER_TX)
            i2c->DR.DR = (ctx->slave_addr << 1) | 0;
        else if (ctx->state == I2C_STATE_MASTER_RX)
            i2c->DR.DR = (ctx->slave_addr << 1) | 1;
        return;
    }

    // ----------- ADDRESS SENT -----------
    if (sr1 & I2C_SR1_ADDR)
    {
        volatile uint32_t tmp;
        tmp = i2c->SR1.reg;
        tmp = i2c->SR2.reg;
        (void)tmp;

        // RX: if 1 byte, disable ACK now, STOP generated after reading
        if (ctx->state == I2C_STATE_MASTER_RX && ctx->len == 1)
        {
            i2c->CR1.ACK = 0;
        }
        return;
    }

    // ----------- MASTER TX DATA HANDLING -----------
    if (ctx->state == I2C_STATE_MASTER_TX && (sr1 & I2C_SR1_TXE))
    {
        if (ctx->idx < ctx->len)
            i2c->DR.DR = ctx->buf[ctx->idx++];
        else
        {
            i2c->CR1.STOP = 1;
            ctx->state = I2C_STATE_IDLE;
            if (ctx->tx_done_cb) ctx->tx_done_cb();
        }
        return;
    }

    // ----------- MASTER RX DATA HANDLING -----------
    if (ctx->state == I2C_STATE_MASTER_RX && (sr1 & I2C_SR1_RXNE))
    {
        ctx->buf[ctx->idx++] = i2c->DR.DR;

        if (ctx->idx == ctx->len)
        {
            i2c->CR1.STOP = 1;   // STOP after reading last byte
            ctx->state = I2C_STATE_IDLE;
            i2c->CR1.ACK = 1;    // re-enable ACK for next transfer
            if (ctx->rx_done_cb)
                ctx->rx_done_cb(ctx->buf, ctx->len);
        }
        return;
    }
}

void I2C1_EV_IRQHandler(){ i2c_inerrupt(I2C1); }
void I2C2_EV_IRQHandler(){ i2c_inerrupt(I2C2); }
void I2C3_EV_IRQHandler(){ i2c_inerrupt(I2C3); }

// helpers
static uint32_t get_APB()
{
    uint32_t pclk1;
    uint32_t ppre1 = RCC->CFGR.bits.PPRE1; // bits 12:10 = PPRE1

    switch(ppre1)
    {
        case 0b000: pclk1 = CLK; break;          // no division
        case 0b100: pclk1 = CLK / 2; break;      
        case 0b101: pclk1 = CLK / 4; break;      
        case 0b110: pclk1 = CLK / 8; break;      
        case 0b111: pclk1 = CLK / 16; break;     
        default: pclk1 = CLK; break;             
    }

    return pclk1;
}

static void enable_i2c_rcc(i2c_regs_t* i2c)
{
    if (i2c == I2C1) rcc_En_clk_preiph(RCC_I2C1);
    else if (i2c == I2C2) rcc_En_clk_preiph(RCC_I2C2);
    else if (i2c == I2C3) rcc_En_clk_preiph(RCC_I2C3);
    else while (true);
}

static void enable_i2c_pins(i2c_regs_t* i2c)
{
    rcc_En_clk_preiph(RCC_GPIOA);
    rcc_En_clk_preiph(RCC_GPIOB);

    GPIO_PinConfig_t pin_cfg;
    pin_cfg.mode = GPIO_MODE_ALTFN;
    pin_cfg.pull = GPIO_PULL_NO;
    pin_cfg.speed = GPIO_SPEED_HIGH;
    pin_cfg.alt_function = GPIO_AF4_I2C1_3;
    pin_cfg.output_type = GPIO_OUTPUT_OPENDRAIN;

    if (i2c == I2C1)
    {
        pin_cfg.alt_function = GPIO_AF4_I2C1_3;
        pin_cfg.port = GPIOB; pin_cfg.pin = 6; gpio_init(&pin_cfg);
        pin_cfg.port = GPIOB; pin_cfg.pin = 7; gpio_init(&pin_cfg);
    }
    else if (i2c == I2C2)
    {
        pin_cfg.alt_function = GPIO_AF4_I2C1_3;
        pin_cfg.port = GPIOB; pin_cfg.pin = 10; gpio_init(&pin_cfg); // SCL

        pin_cfg.alt_function = GPIO_AF9_I2C2_3;
        pin_cfg.port = GPIOB; pin_cfg.pin = 3; gpio_init(&pin_cfg);  // SDA
    }
    else if (i2c == I2C3)
    {
        pin_cfg.alt_function = GPIO_AF4_I2C1_3;
        pin_cfg.port = GPIOA; pin_cfg.pin = 8; gpio_init(&pin_cfg);  // SCL

        pin_cfg.alt_function = GPIO_AF9_I2C2_3;
        pin_cfg.port = GPIOB; pin_cfg.pin = 4; gpio_init(&pin_cfg);  // SDA
    }

}


static void enable_i2c_nvic(i2c_regs_t* i2c)
{
    if (i2c == I2C1) NVIC_EnableIRQ(I2C1_EV_IRQn);
    else if (i2c == I2C2) NVIC_EnableIRQ(I2C2_EV_IRQn);
    else if (i2c == I2C3) NVIC_EnableIRQ(I2C3_EV_IRQn);
    else while (true);
}


// api
i2c_ret_t i2c_init(i2c_cfg_t* cfg)
{
    if (cfg == NULL) return I2C_NOK;

    enable_i2c_rcc(cfg->i2c);
    enable_i2c_pins(cfg->i2c);
    enable_i2c_nvic(cfg->i2c);
    
    i2c_ctx_t *ctx = i2c_get_ctx(cfg->i2c);
    if (!ctx)
        return I2C_NOK;
    
    ctx->tx_done_cb = cfg->tx_cb;
    ctx->rx_done_cb = cfg->rx_cb;
    ctx->error_cb   = cfg->err_cb;
    ctx->state = I2C_STATE_IDLE;
    ctx->idx   = 0;
    ctx->len   = 0;
    ctx->buf   = NULL;

    
    cfg->i2c->CR1.PE = 0;

    // enable interrupt
    cfg->i2c->CR2.ITEVTEN = 1;
    cfg->i2c->CR2.ITBUFEN = 1;
    cfg->i2c->CR2.ITERREN = 1;

    uint8_t is_fast = (cfg->freq > NORMAL_MODE_MAX) ? 1 : 0;
    uint32_t pclk = get_APB();

    cfg->i2c->CR2.FREQ = (pclk / 1000000UL) & 0x3F;

    uint32_t ccr_value;
    if (!is_fast)
    {
        ccr_value = pclk / (2 * cfg->freq);
        if(ccr_value < 4) ccr_value = 4;
        cfg->i2c->CCR.F_S = 0; // Standard mode
    }
    else
    {
        ccr_value = pclk / (3 * cfg->freq);
        if(ccr_value < 1) ccr_value = 1;
        cfg->i2c->CCR.F_S = 1;  // Fast mode
        cfg->i2c->CCR.DUTY = 0; // t_low/t_high = 2
    }
    cfg->i2c->CCR.CCR = ccr_value;

    uint32_t trise_ns = is_fast ? 300 : 1000; // max rise time
    uint32_t t_pclk_ns = 1000000000UL / pclk;
    uint32_t trise_value = (trise_ns / t_pclk_ns) + 1;
    if (trise_value > 0x3F) trise_value = 0x3F;
    
    cfg->i2c->TRISE.TRISE = trise_value;

    // Enable peripheral
    cfg->i2c->CR1.PE = 1;


    return I2C_OK;

}



i2c_ret_t i2c_send(i2c_cfg_t* cfg, uint8_t slave_addr, uint8_t *data, uint16_t len)
{
    if (!cfg || !data || len == 0) return I2C_NOK;

    i2c_ctx_t *ctx = i2c_get_ctx(cfg->i2c);
    if (!ctx || ctx->state != I2C_STATE_IDLE) return I2C_NOK;

    ctx->buf = data;
    ctx->len = len;
    ctx->idx = 0;
    ctx->slave_addr = slave_addr;
    ctx->state = I2C_STATE_MASTER_TX;

    // trigger START, ISR will handle sending the address and data
    cfg->i2c->CR1.START = 1;

    return I2C_OK;
}

i2c_ret_t i2c_receive(i2c_cfg_t* cfg, uint8_t slave_addr, uint8_t* data, uint16_t len)
{
    if (!cfg || !data || len == 0) return I2C_NOK;

    i2c_ctx_t *ctx = i2c_get_ctx(cfg->i2c);
    if (!ctx || ctx->state != I2C_STATE_IDLE) return I2C_NOK;

    ctx->buf = data;
    ctx->len = len;
    ctx->idx = 0;
    ctx->slave_addr = slave_addr;
    ctx->state = I2C_STATE_MASTER_RX;

    // trigger START, ISR will handle address and reading data
    cfg->i2c->CR1.START = 1;

    return I2C_OK;
}

bool i2c_is_error(i2c_cfg_t* cfg)
{
    if (!cfg) return false;

    i2c_ctx_t *ctx = i2c_get_ctx(cfg->i2c);
    if (!ctx) return false;
    
    return (ctx->state == I2C_STATE_IDLE && ctx->error_cb != NULL);
}

bool i2c_is_busy(i2c_cfg_t* cfg)
{
    if (!cfg) return false;

    i2c_ctx_t *ctx = i2c_get_ctx(cfg->i2c);
    if (!ctx) return false;

    return (ctx->state != I2C_STATE_IDLE);
}