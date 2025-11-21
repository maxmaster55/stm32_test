#ifndef RCC_PRIVATE_H_
#define RCC_PRIVATE_H_

#include "stdint.h"

#define RCC_CLK_RESET       0
#define RCC_CLK_SET         1
#define RCC_CLK_NOT_RDY     0
#define RCC_CLK_RDY         1

#define RCC_PERIPH_DISABLE  0
#define RCC_PERIPH_ENABLE   1

#define RCC  (*(volatile RCC_REGS *)0x40023800)

/* ---------------------- Different CLK SRC ---------------------- */

typedef enum
{
    RCC_HSI,
    RCC_HSE,
    RCC_PLL,
    RCC_PLL_I2S,
    RCC_LSI,
    RCC_LSE,
}RCC_CLK_SRC;

/* --------------------- Peripheral Definition --------------------- */
typedef enum
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOH,
    CRC,
    DMA1,
    DMA2,
    OTGFS,
    TIM2,
    TIM3,
    TIM4,
    TIM5,
    WWDG,
    SPI2,
    SPI3,
    USART2,
    I2C1,
    I2C2,
    I2C3,
    PWR,
    TIM1,
    USART1,
    USART6,
    ADC1,
    SDIO,
    SPI1,
    SPI4,
    SYSCFG,
    TIM9,
    TIM10,
    TIM11,
}PERIPH;

typedef enum
{
    AHB1,
    AHB2,
    APB1,
    APB2,
}SYSTEM_BUS;

/* ---------------------- Register Definition ---------------------- */
typedef union
{
    uint32_t WORD;
    struct 
    {
        uint32_t HSION      : 1;
        uint32_t HSIRDY     : 1;
        uint32_t RSRVD0     : 1;
        uint32_t HSITRIM    : 5;
        uint32_t HSICAL     : 8;
        uint32_t HSEON      : 1;
        uint32_t HSERDY     : 1;
        uint32_t HSEBYP     : 1;
        uint32_t CSSON      : 1;
        uint32_t RSRVD1     : 4;
        uint32_t PLLON      : 1;
        uint32_t PLLRDY     : 1;
        uint32_t PLLI2SON   : 1;
        uint32_t PLLI2SRDY  : 1;
        uint32_t RSRVD2     : 4;
    }BITS;
    
}RCC_CR;

typedef union
{
    uint32_t WORD;
    struct 
    {
        uint32_t PLLM   : 6;
        uint32_t PLLN   : 9;
        uint32_t RSRVD0 : 1;
        uint32_t PLLP   : 2;
        uint32_t RSRVD1 : 4;
        uint32_t PLLSRC : 1;
        uint32_t RSRVD2 : 1;
        uint32_t PLLQ   : 4;
        uint32_t RSRVD3 : 4;
    }BITS;
    
}RCC_PLLCFGR;

typedef union
{
    uint32_t WORD;
    struct 
    {
        uint32_t SW      : 2;
        uint32_t SWS     : 2;
        uint32_t HPRE    : 4;
        uint32_t RSRVD0  : 2;
        uint32_t PPRE1   : 3;
        uint32_t PPRE2   : 3;
        uint32_t RTCPRE  : 5;
        uint32_t MCO1    : 2;
        uint32_t I2SSRC  : 1;
        uint32_t MCO1PRE : 3;
        uint32_t MCO2PRE : 3;
        uint32_t MCO2    : 2;
    }BITS;
    
}RCC_CFGR;

typedef union
{
    uint32_t WORD;
    union 
    {
        uint32_t RSRVD0  : 6;
        uint32_t PLLI2SN : 9;
        uint32_t RSRVD1  : 13;
        uint32_t PLLI2SR : 3;
        uint32_t RSRVD2  : 1;
    }BITS;
    
}RCC_PLLI2SCFGR;

typedef union 
{
    uint32_t WORD;
    struct 
    {
        uint32_t GPIOAEN : 1;
        uint32_t GPIOBEN : 1;
        uint32_t GPIOCEN : 1;
        uint32_t GPIODEN : 1;
        uint32_t GPIOEEN : 1;
        uint32_t RSRVD0  : 2;
        uint32_t GPIOHEN : 1;
        uint32_t RSRVD1  : 4;
        uint32_t CRCEN   : 1;
        uint32_t RSRVD2  : 8;
        uint32_t DMA1EN  : 1;
        uint32_t DMA2EN  : 1;
        uint32_t RSRVD3  : 9;
    }BITS;
    
}RCC_AHB1ENR;

typedef union
{
    uint32_t WORD;
    struct 
    {
        uint32_t RSRVD0  : 7;
        uint32_t OTGFSEN : 1;
        uint32_t RSRVD1  : 24;
    }BITS;
    
}RCC_AHB2ENR;

typedef union
{
    uint32_t WORD;
    struct
    {
        uint32_t TIM2EN   : 1;
        uint32_t TIM3EN   : 1;
        uint32_t TIM4EN   : 1;
        uint32_t TIM5EN   : 1;
        uint32_t RSRVD0   : 7;
        uint32_t WWDGEN   : 1;
        uint32_t RSRVD1   : 2;
        uint32_t SPI2EN   : 1;
        uint32_t SPI3EN   : 1;
        uint32_t RSRVD2   : 1;
        uint32_t USART2EN : 1;
        uint32_t RSRVD3   : 3;
        uint32_t I2C1EN   : 1;
        uint32_t I2C2EN   : 1;
        uint32_t I2C3EN   : 1;
        uint32_t RSRVD4   : 4;
        uint32_t PWREN    : 1;
        uint32_t RSRVD5   : 3;
    }BITS;

}RCC_APB1ENR;

typedef union 
{
    uint32_t DATA;
    struct
    {
        uint32_t TIM1EN   : 1;
        uint32_t RSRVD0   : 3;
        uint32_t USART1EN : 1;
        uint32_t USART6EN : 1;
        uint32_t RSRVD1   : 2;
        uint32_t ADC1EN   : 1;
        uint32_t RSRVD2   : 2;
        uint32_t SDIOEN   : 1;
        uint32_t SPI1EN   : 1;
        uint32_t SPI4EN   : 1;
        uint32_t SYSCFGEN : 1;
        uint32_t RSRVD3   : 2;
        uint32_t TIM9EN   : 1;
        uint32_t TIM10EN  : 1;
        uint32_t TIM11EN  : 1;
        uint32_t RSRVD4   : 13;
    }BITS;
}RCC_APB2ENR;

/* ---------------------- RCC Registers ---------------------- */

typedef struct 
{
    RCC_CR          CR;
    RCC_PLLCFGR     PLLCFGR;
    RCC_CFGR        CFGR;
    uint32_t        RCC_CIR;
    uint32_t        RCC_AHB1RSTR;
    uint32_t        RCC_AHB2RSTR;

    uint32_t        RCC_RESERVED0;
    uint32_t        RCC_RESERVED1;

    uint32_t        RCC_APB1RSTR;
    uint32_t        RCC_APB2RSTR;

    uint32_t        RCC_RESERVED2;
    uint32_t        RCC_RESERVED3;

    RCC_AHB1ENR     AHB1ENR;
    RCC_AHB2ENR     AHB2ENR;

    uint32_t        RCC_RESERVED4;
    uint32_t        RCC_RESERVED5;

    RCC_APB1ENR     APB1ENR;
    RCC_APB2ENR     APB2ENR;

    uint32_t        RCC_RESERVED6;
    uint32_t        RCC_RESERVED7;

    uint32_t        RCC_AHB1LPENR;
    uint32_t        RCC_AHB2LPENR;

    uint32_t        RCC_RESERVED8;
    uint32_t        RCC_RESERVED9;

    uint32_t        RCC_APB1LPENR;
    uint32_t        RCC_APB2LPENR;

    uint32_t        RCC_RESERVED10;
    uint32_t        RCC_RESERVED11;

    uint32_t        RCC_BDCR;
    uint32_t        RCC_CSR;    

    uint32_t        RCC_RESERVED12;
    uint32_t        RCC_RESERVED13;

    uint32_t        RCC_SSCGR;
    RCC_PLLI2SCFGR  PLLI2SCFGR;

    uint32_t        RCC_RESERVED14;

    uint32_t        RCC_DCKCFGR;
}RCC_REGS;

#endif // RCC_PRIVATE_H_