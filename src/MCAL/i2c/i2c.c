#include "glob.h"
#include <MCAL/GPIO/gpio.h>
#include <MCAL/RCC/rcc.h>
#include <MCAL/i2c/i2c.h>

extern rcc_RegDef_t *RCC;

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


i2c_ret_t i2c_init(i2c_cfg_t* cfg)
{
    if (cfg == NULL) return;

    enable_i2c_rcc(cfg->i2c);
    //enable_i2c_pins(cfg->i2c);

    cfg->i2c->CR1.PE = 0;

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

    // 4. TRISE calculation
    uint32_t trise_ns = is_fast ? 300 : 1000; // max rise time
    uint32_t t_pclk_ns = 1000000000UL / pclk;
    uint32_t trise_value = (trise_ns / t_pclk_ns) + 1;
    if (trise_value > 0x3F) trise_value = 0x3F;
    
    cfg->i2c->TRISE.TRISE = trise_value;

    // Enable peripheral
    cfg->i2c->CR1.PE = 1;


    return I2C_OK;

}

// Send a single byte to a slave
i2c_ret_t i2c_send(i2c_cfg_t* cfg, uint8_t slave_addr, uint8_t data)
{
    i2c_regs_t* i2c = cfg->i2c;

    while (i2c->SR2.BUSY);

    i2c->CR1.START = 1;

    while (!i2c->SR1.SB);

    (void)i2c->SR1.reg;
    i2c->DR.DR = slave_addr << 1;

    /* 4. Wait for ADDR or AF */
    while (!(i2c->SR1.ADDR || i2c->SR1.AF));

    if (i2c->SR1.AF)
    {
        i2c->SR1.AF = 0;
        i2c->CR1.STOP = 1;
        return I2C_NOK;
    }

    /* 5. Clear ADDR */
    volatile uint32_t tmp;
    tmp = i2c->SR1.reg;
    tmp = i2c->SR2.reg;
    (void)tmp;

    /* 6. Send data */
    i2c->DR.DR = data;

    /* 7. Wait for TXE */
    while (!i2c->SR1.TxE);

    /* 8. Wait for BTF */
    while (!i2c->SR1.BTF);

    /* 9. STOP */
    i2c->CR1.STOP = 1;

    return I2C_OK;
}

void i2c_receive(i2c_cfg_t* cfg, uint8_t* data)
{

}

