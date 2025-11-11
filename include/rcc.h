#pragma once

#include "rcc_cfg.h"

#include <stdint.h>


typedef union
{
    uint32_t RC;
    struct
    {
        uint32_t HSION      :1;
        uint32_t HSIRDY     :1;
        uint32_t res0       :1;
        uint32_t HSITRIM    :5;
        uint32_t HSICAL     :8;
        uint32_t HSEON      :1;
        uint32_t HSERDY     :1;
        uint32_t HSEBYP     :1;
        uint32_t CSSON      :1;
        uint32_t res1       :4;
        uint32_t PLLON      :1;
        uint32_t PLLRDY     :1;
        uint32_t PLLI2SON   :1;
        uint32_t PLLI2SRDY  :1;
        uint32_t res2       :4;
    } bits;

} rcc_RC_t;


typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t PLLM   : 6;
        uint32_t PLLN   : 9;
        uint32_t res0   : 1;
        uint32_t PLLP   : 2;
        uint32_t res1   : 4;
        uint32_t PLLSRC : 1;
        uint32_t res2   : 1;
        uint32_t PLLQ   : 4;
        uint32_t res3   : 4;
    } bits;
} rcc_PLLCFGR_t;

typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t SW       : 2; 
        uint32_t SWS      : 2;
        uint32_t HPRE     : 4;
        uint32_t res0     : 2;
        uint32_t PPRE1    : 3;
        uint32_t PPRE2    : 3;
        uint32_t RTCPRE   : 5;
        uint32_t MCO1     : 2;
        uint32_t I2SSRC   : 1;
        uint32_t MCO1PRE  : 3;
        uint32_t MCO2PRE  : 3;
        uint32_t MCO2     : 2;
    } bits;
} rcc_CFGR_t;


typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t LSIRDYF     :1;
        uint32_t LSERDYF     :1;
        uint32_t HSIRDYF     :1;
        uint32_t HSERDYF     :1;
        uint32_t PLLRDYF     :1;
        uint32_t PLLI2SRDYF  :1;
        uint32_t res0        :1;
        uint32_t CSSF        :1;
        uint32_t LSIRDYIE    :1;
        uint32_t LSERDYIE    :1;
        uint32_t HSIRDYIE    :1;
        uint32_t HSERDYIE    :1;
        uint32_t PLLRDYIE    :1;
        uint32_t PLLI2SRDYIE :1;
        uint32_t res1        :2;
        uint32_t LSIRDYC     :1;
        uint32_t LSERDYC     :1;
        uint32_t HSIRDYC     :1;
        uint32_t HSERDYC     :1;
        uint32_t PLLRDYC     :1;
        uint32_t PLLI2SRDYC  :1;
        uint32_t res2        :1;
        uint32_t CSSC        :1;
        uint32_t res3        :8;
    } bits;
} rcc_CIR_t;



typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t GPIOARST   :1;
        uint32_t GPIOBRST   :1;
        uint32_t GPIOCRST   :1;
        uint32_t GPIODRST   :1;
        uint32_t GPIOERST   :1;
        uint32_t res0       :2;
        uint32_t GPIOHRST   :1;
        uint32_t res1       :4;
        uint32_t CRCRST     :1;
        uint32_t res2       :8;
        uint32_t DMA1RST    :1;
        uint32_t DMA2RST    :1;
        uint32_t res3       :9;
    } bits;
} rcc_AHB1RSTR_t;


typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t res0       :7;
        uint32_t OTGFSRST   :1;
        uint32_t res1       :24;
    } bits;
    
} rcc_AHB2RSTR_t;



typedef union rcc
{
    uint32_t reg;
    struct
    {
        uint32_t TIM2RST    :1;
        uint32_t TIM3RST    :1;
        uint32_t TIM4RST    :1;
        uint32_t TIM5RST    :1;
        uint32_t res0       :7;
        uint32_t WWDGRST    :1;
        uint32_t res1       :2;
        uint32_t SPI2RST    :1;
        uint32_t SPI3RST    :1;
        uint32_t res2       :1;
        uint32_t USART2RST  :1;
        uint32_t res3       :3;
        uint32_t I2C1RST    :1;
        uint32_t I2C2RST    :1;
        uint32_t I2C3RST    :1;
        uint32_t res4       :4;
        uint32_t PWRRST     :1;
        uint32_t res5       :3;
    } bits;
    
} rcc_APB1RSTR_t;


typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t TIM1RST    :1;
        uint32_t res0       :3;
        uint32_t USART1RST  :1;
        uint32_t USART6RST  :1;
        uint32_t res1       :2;
        uint32_t ADC1RST    :1;
        uint32_t res2       :2;
        uint32_t SDIORST    :1;
        uint32_t SPI1RST    :1;
        uint32_t SPI4RST    :1;
        uint32_t SYSCFGRST  :1;
        uint32_t res3       :1;
        uint32_t TIM9RST    :1;
        uint32_t TIM10RST   :1;
        uint32_t TIM11RST   :1;
        uint32_t res4       :13;
    } bits;
} rcc_APB2RSTR_t;



typedef union
{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t GPIOAEN    :1;
        volatile uint32_t GPIOBEN    :1;
        volatile uint32_t GPIOCEN    :1;
        volatile uint32_t GPIODEN    :1;
        volatile uint32_t GPIOEEN    :1;
        volatile uint32_t res0       :2;
        volatile uint32_t GPIOHEN    :1;
        volatile uint32_t res1       :4;
        volatile uint32_t CRCEN      :1;
        volatile uint32_t res2       :8;
        volatile uint32_t DMA1EN     :1;
        volatile uint32_t DMA2EN     :1;
        volatile uint32_t res3       :9;
    } bits;

} rcc_AHB1ENR_t;


typedef union
{   
    uint32_t reg;
    struct
    {
        uint32_t res0       :7;
        uint32_t OTGFSEN    :1;
        uint32_t res1       :24;
    } bits;
    
} rcc_AHB2ENR_t;


typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t TIM2EN    :1;
        uint32_t TIM3EN    :1;
        uint32_t TIM4EN    :1;
        uint32_t TIM5EN    :1;
        uint32_t res0      :7;
        uint32_t WWDGEN    :1;
        uint32_t res1      :2;
        uint32_t SPI2EN    :1;
        uint32_t SPI3EN    :1;
        uint32_t res2      :1;
        uint32_t USART2EN  :1;
        uint32_t res3      :3;
        uint32_t I2C1EN    :1;
        uint32_t I2C2EN    :1;
        uint32_t I2C3EN    :1;
        uint32_t res4      :4;
        uint32_t PWREN     :1;
        uint32_t res5      :3;
    } bits;
    
} rcc_APB1ENR_t;


typedef union{
    uint32_t reg;
    struct
    {
        uint32_t TIM1EN    :1;
        uint32_t res0      :3;
        uint32_t USART1EN  :1;
        uint32_t USART6EN  :1;
        uint32_t res1      :2;
        uint32_t ADC1EN    :1;
        uint32_t res2      :2;
        uint32_t SDIOEN    :1;
        uint32_t SPI1EN    :1;
        uint32_t SPI4EN    :1;
        uint32_t SYSCFGEN  :1;
        uint32_t res3      :1;
        uint32_t TIM9EN    :1;
        uint32_t TIM10EN   :1;
        uint32_t TIM11EN   :1;
        uint32_t res4      :13;
    } bits;


} rcc_APB2ENR_t;


typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t GPIOALPEN    :1;
        uint32_t GPIOBLPEN    :1;
        uint32_t GPIOCLPEN    :1;
        uint32_t GPIODLPEN    :1;
        uint32_t GPIOELPEN    :1;
        uint32_t res0         :2;
        uint32_t GPIOHLPEN    :1;
        uint32_t res1         :4;
        uint32_t CRCLPEN      :1;
        uint32_t res2         :2;
        uint32_t FLITFLPEN    :1;
        uint32_t SRAMLPEN     :1;
        uint32_t res3         :4;
        uint32_t DMA1LPEN     :1;
        uint32_t DMA2LPEN     :1;
        uint32_t res4         :9;
    } bits;
    
} rcc_AHB1LPENR_t;


typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t res0       :7;
        uint32_t OTGFSLPEN  :1;
        uint32_t res1       :24;
    } bits;
    
} rcc_AHB2LPENR_t;

typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t TIM2LPEN    :1;
        uint32_t TIM3LPEN    :1;
        uint32_t TIM4LPEN    :1;
        uint32_t TIM5LPEN    :1;
        uint32_t res0        :7;
        uint32_t WWDGLPEN    :1;
        uint32_t res1        :2;
        uint32_t SPI2LPEN    :1;
        uint32_t SPI3LPEN    :1;
        uint32_t res2        :1;
        uint32_t USART2LPEN  :1;
        uint32_t res3        :3;
        uint32_t I2C1LPEN    :1;
        uint32_t I2C2LPEN    :1;
        uint32_t I2C3LPEN    :1;
        uint32_t res4        :4;
        uint32_t PWRLPEN     :1;
        uint32_t res5        :3;
    } bits;
    
} rcc_APB1LPENR_t;


typedef union{
    uint32_t reg;
    struct
    {
        uint32_t TIM1LPEN    :1;
        uint32_t res0        :3;
        uint32_t USART1LPEN  :1;
        uint32_t USART6LPEN  :1;
        uint32_t res1        :2;
        uint32_t ADC1LPEN    :1;
        uint32_t res2        :2;
        uint32_t SDIOLPEN    :1;
        uint32_t SPI1LPEN    :1;
        uint32_t SPI4LPEN    :1;
        uint32_t SYSCFGLPEN  :1;
        uint32_t res3        :1;
        uint32_t TIM9LPEN    :1;
        uint32_t TIM10LPEN   :1;
        uint32_t TIM11LPEN   :1;
        uint32_t res4        :13;
    } bits;

} rcc_APB2LPENR_t;




typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t LSION      :1;
        uint32_t LSIRDY     :1;
        uint32_t LSEBYP     :1;
        uint32_t res0       :5;
        uint32_t RTCSEL     :2;
        uint32_t res1       :5;
        uint32_t RTCEN      :1;
        uint32_t BDRST      :1;
        uint32_t res2       :15;
    } bits;
} rcc_BDCR_t;

typedef union
{
    uint32_t reg;
    struct
    {
        uint32_t LSIRDYF     :1;
        uint32_t LSERDYF     :1;
        uint32_t res0        :22;
        uint32_t RMVF        :1;
        uint32_t BORRSTF     :1;
        uint32_t PINRSTF     :1;
        uint32_t PORRSTF     :1;
        uint32_t SFTRSTF     :1;
        uint32_t IWDGRSTF    :1;
        uint32_t WWDGRSTF    :1;
        uint32_t LPWRRSTF    :1;
    } bits;
} rcc_CSR_t;


typedef union 
{
    uint32_t reg;
    struct
    {
        uint32_t MODPER      : 13;
        uint32_t INCSTEP     : 15;
        uint32_t res0        : 2;
        uint32_t SPREADSEL   : 1;
        uint32_t SSCGON      : 1;
    } bits;
} rcc_SSCGR_t;


typedef union 
{
    uint32_t reg;
    struct
    {
        uint32_t res0       : 6; 
        uint32_t PLLI2SN0   : 1;
        uint32_t PLLI2SN1   : 1;
        uint32_t PLLI2SN2   : 1;
        uint32_t PLLI2SN3   : 1;
        uint32_t PLLI2SN4   : 1;
        uint32_t PLLI2SN5   : 1;
        uint32_t PLLI2SN6   : 1;
        uint32_t PLLI2SN7   : 1;
        uint32_t PLLI2SN8   : 1;
        uint32_t res1       : 13;
        uint32_t PLLI2SR0   : 1;
        uint32_t PLLI2SR1   : 1;
        uint32_t PLLI2SR2   : 1;
        uint32_t res2       : 1;
    } bits;
} rcc_PLLI2SCFGR_t;

typedef union 
{
    uint32_t reg;
    struct
    {
        uint32_t res0       : 24;
        uint32_t TIMPRE     : 1;
        uint32_t res1       : 7;
    } bits;
} rcc_DCKCFGR_t;



typedef struct
{
    volatile rcc_RC_t CR;                /* 0x00 */
    volatile rcc_PLLCFGR_t PLLCFGR;     /* 0x04 */
    volatile rcc_CFGR_t CFGR;           /* 0x08 */
    volatile rcc_CIR_t CIR;             /* 0x0C */
    volatile rcc_AHB1RSTR_t AHB1RSTR;   /* 0x10 */
    volatile rcc_AHB2RSTR_t AHB2RSTR;   /* 0x14 */
    uint32_t res0[2];                   /* 0x18-0x1C */
    volatile rcc_APB1RSTR_t APB1RSTR;   /* 0x20 */
    volatile rcc_APB2RSTR_t APB2RSTR;   /* 0x24 */
    uint32_t res1[2];                   /* 0x28-0x2C */
    volatile rcc_AHB1ENR_t AHB1ENR;     /* 0x30 */
    volatile rcc_AHB2ENR_t AHB2ENR;     /* 0x34 */
    uint32_t res2[2];                   /* 0x38-0x3C */
    volatile rcc_APB1ENR_t APB1ENR;     /* 0x40 */
    volatile rcc_APB2ENR_t APB2ENR;     /* 0x44 */
    uint32_t res3[2];                   /* 0x48-0x4C */
    volatile rcc_AHB1LPENR_t AHB1LPENR; /* 0x50 */
    volatile rcc_AHB2LPENR_t AHB2LPENR; /* 0x54 */
    uint32_t res4[2];                   /* 0x58-0x5C */
    volatile rcc_APB1LPENR_t APB1LPENR; /* 0x60 */
    volatile rcc_APB2LPENR_t APB2LPENR; /* 0x64 */
    uint32_t res5[2];                   /* 0x68-0x6C */
    volatile rcc_BDCR_t BDCR;           /* 0x70 */
    volatile rcc_CSR_t CSR;             /* 0x74 */
    uint32_t res6[2];                   /* 0x78-0x7C */
    volatile rcc_SSCGR_t SSCGR;         /* 0x80 */
    volatile rcc_PLLI2SCFGR_t PLLI2SCFGR;/* 0x84 */
    uint32_t res7[2];                   /* 0x88-0x8C */
    volatile uint32_t DCKCFGR;          /* 0x90 */
    volatile uint32_t CKGATENR;         /* 0x94 */
    volatile uint32_t DCKCFGR2;         /* 0x98 */
} rcc_RegDef_t;


#define RCC_BASE            (0x40023800UL)


typedef enum{
    RCC_RES_OK,
    RCC_RES_NOK,
} rcc_return_t;


typedef enum {
    RCC_CLK_HSI,
    RCC_CLK_HSE,
    RCC_CLK_PLL,
} rcc_clktype_t;

typedef enum {
    RCC_CLK_OFF,
    RCC_CLK_ON,
} rcc_clkstate_t;


typedef enum {
    PLL_HSI_SRC,
    PLL_HSE_SRC,
} rcc_PLL_src_t;


typedef struct{
    rcc_PLL_src_t src;
    uint8_t M_val;
    uint16_t N_val;
    uint8_t Q_val;
    uint8_t P_val;
} rcc_PLL_config_t;

#define BUS_AHB1  (0)
#define BUS_AHB2  (1)
#define BUS_APB1  (2)
#define BUS_APB2  (3)

#define RCC_BUS_ENCODE(bus, bit)    (((bus) << 16) | (bit))
#define RCC_GET_BUS(x)              ((uint32_t)((x) >> 16))
#define RCC_GET_BIT(x)              ((uint32_t)((x) & 0xFFFF))


typedef enum{
  // AHB1
    RCC_GPIOA = RCC_BUS_ENCODE(BUS_AHB1, 0),
    RCC_GPIOB = RCC_BUS_ENCODE(BUS_AHB1, 1),
    RCC_GPIOC = RCC_BUS_ENCODE(BUS_AHB1, 2),
    RCC_GPIOD = RCC_BUS_ENCODE(BUS_AHB1, 3),
    RCC_GPIOE = RCC_BUS_ENCODE(BUS_AHB1, 4),
    RCC_GPIOH = RCC_BUS_ENCODE(BUS_AHB1, 7),
    RCC_CRC   = RCC_BUS_ENCODE(BUS_AHB1, 12),
    RCC_DMA1  = RCC_BUS_ENCODE(BUS_AHB1, 21),
    RCC_DMA2  = RCC_BUS_ENCODE(BUS_AHB1, 22),
    // AHB2
    RCC_OTGFS = RCC_BUS_ENCODE(BUS_AHB2, 7),
    // APB1
    RRC_TIMER_2 = RCC_BUS_ENCODE(BUS_APB1, 0),
    RRC_TIMER_3 = RCC_BUS_ENCODE(BUS_APB1, 1),
    RRC_TIMER_4 = RCC_BUS_ENCODE(BUS_APB1, 2),
    RRC_TIMER_5 = RCC_BUS_ENCODE(BUS_APB1, 3),
    RRC_WWDG    = RCC_BUS_ENCODE(BUS_APB1, 11),
    RRC_SPI2    = RCC_BUS_ENCODE(BUS_APB1, 14),
    RRC_SPI3    = RCC_BUS_ENCODE(BUS_APB1, 15),
    RRC_USART2  = RCC_BUS_ENCODE(BUS_APB1, 17),
    RRC_I2C1    = RCC_BUS_ENCODE(BUS_APB1, 21),
    RRC_I2C2    = RCC_BUS_ENCODE(BUS_APB1, 22),
    RRC_I2C3    = RCC_BUS_ENCODE(BUS_APB1, 23),
    RRC_PWR     = RCC_BUS_ENCODE(BUS_APB1, 28),
    // APB2
    RRC_TIMER_1 = RCC_BUS_ENCODE(BUS_APB2, 0),
    RRC_USART1  = RCC_BUS_ENCODE(BUS_APB2, 4),
    RRC_USART6  = RCC_BUS_ENCODE(BUS_APB2, 5),
    RRC_ADC1    = RCC_BUS_ENCODE(BUS_APB2, 8),
    RRC_SDIO    = RCC_BUS_ENCODE(BUS_APB2, 11),
    RRC_SPI1    = RCC_BUS_ENCODE(BUS_APB2, 12),
    RRC_SPI4    = RCC_BUS_ENCODE(BUS_APB2, 13),
    RRC_SYSCFG  = RCC_BUS_ENCODE(BUS_APB2, 14),
    RRC_TIMER_9 = RCC_BUS_ENCODE(BUS_APB2, 16),
    RRC_TIMER_10= RCC_BUS_ENCODE(BUS_APB2, 17),
    RRC_TIMER_11= RCC_BUS_ENCODE(BUS_APB2, 18)
}rcc_Peripheral_t;


rcc_return_t rcc_set_SysTick(rcc_clktype_t rcc);

rcc_return_t rcc_ctrlClk(rcc_clktype_t rcc, rcc_clkstate_t state);

rcc_return_t rcc_PLL_config(rcc_PLL_config_t config);

rcc_return_t rcc_En_clk_preiph(rcc_Peripheral_t periph);

rcc_return_t rcc_Dis_clk_preiph(rcc_Peripheral_t periph);

rcc_return_t rcc_reset_periph(rcc_Peripheral_t periph);

void test();