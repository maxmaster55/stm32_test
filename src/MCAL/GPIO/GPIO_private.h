#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_

#include "stdint.h"

#define GPIOA  (*(volatile GPIOA_REG_t *)0x40020000)
#define GPIOB  (*(volatile GPIOA_REG_t *)0x40020400)
#define GPIOC  (*(volatile GPIOA_REG_t *)0x40020800)
#define GPIOD  (*(volatile GPIOA_REG_t *)0x40020C00)
#define GPIOE  (*(volatile GPIOA_REG_t *)0x40021000)
#define GPIOH  (*(volatile GPIOA_REG_t *)0x40021C00)

/* -------------------- BASIC REG DEFINITION -------------------- */

typedef struct
{
    uint32_t MODER;
    uint32_t OTYPER;
    uint32_t OSPEEDR;
    uint32_t PUPDR;
    uint32_t IDR;
    uint32_t ODR;
}GPIOA_REG_t;


volatile GPIOA_REG_t * GPIO_aptrReg[GPIO_u8PORT_MAX] = {
    0x40020000,
    0x40020400,
    0x40020800,
    0x40020C00,
    0x40021000,
    0x40021C00,
};

typedef enum
{
    GPIO_u8PORTA,
    GPIO_u8PORTB,
    GPIO_u8PORTC,
    GPIO_u8PORTD,
    GPIO_u8PORTE,
    GPIO_u8PORTH,
    GPIO_u8PORT_MAX,
}GPIO_PORT_t;

typedef enum
{
    GPIO_u8PIN0,
    GPIO_u8PIN1,
    GPIO_u8PIN2,
    GPIO_u8PIN3,
    GPIO_u8PIN4,
    GPIO_u8PIN5,
    GPIO_u8PIN6,
    GPIO_u8PIN7,
    GPIO_u8PIN8,
    GPIO_u8PIN9,
    GPIO_u8PIN10,
    GPIO_u8PIN11,
    GPIO_u8PIN12,
    GPIO_u8PIN13,
    GPIO_u8PIN14,
    GPIO_u8PIN15,
    GPIO_u8PIN_MAX,
}GPIO_PIN_t;

typedef enum
{
    GPIO_INPUT,
    GPIO_OUTPUT,
    GPIO_ALTFNC,
    GPIO_ANALOG,
    GPIO_MODE_MAX,
}GPIO_MODE_t;

typedef enum
{
    GPIO_PUSHPULL,
    GPIO_OPENDRAIN,
    GOPI_OTYPE_MAX,
}GPIO_OTYPE_t;

typedef enum
{
    GPIO_LOWSPEED,
    GPIO_MEDSPEED,
    GPIO_HIGHSPEED,
    GPIO_VERYHIGHSPEED,
    GPIO_OSPEED_MAX,
}GPIO_OSPEED_t;

typedef enum
{
    GPIO_FLOATING,
    GPIO_PULLUP,
    GPIO_PULLDOWN,
    GPIO_PUPD_MAX,
}GPIO_PUPD_t;

typedef enum
{
    GPIO_u8LOW, 
    GPIO_u8HIGH,
    GPIO_STATE_MAX,
}GPIO_STATE_t;
/* -------------------- SUB REG DEFINITION -------------------- */

typedef union
{
    uint32_t WORD;
    struct
    {
        uint32_t MODER0 : 2;
        uint32_t MODER1 : 2;
        uint32_t MODER2 : 2;
        uint32_t MODER3 : 2;
        uint32_t MODER4 : 2;
        uint32_t MODER5 : 2;
        uint32_t MODER6 : 2;
        uint32_t MODER7 : 2;
        uint32_t MODER8 : 2;
        uint32_t MODER9 : 2;
        uint32_t MODER10 : 2;
        uint32_t MODER11 : 2;
        uint32_t MODER12 : 2;
        uint32_t MODER13 : 2;
        uint32_t MODER14 : 2;
        uint32_t MODER15 : 2;
    }BITS;
}GPIO_MODER_t;

typedef union
{
    uint32_t WORD;
    struct
    {
        uint32_t OT0 : 1;
        uint32_t OT1 : 1;
        uint32_t OT2 : 1;
        uint32_t OT3 : 1;
        uint32_t OT4 : 1;
        uint32_t OT5 : 1;
        uint32_t OT6 : 1;
        uint32_t OT7 : 1;
        uint32_t OT8 : 1;
        uint32_t OT9 : 1;
        uint32_t OT10 : 1;
        uint32_t OT11 : 1;
        uint32_t OT12 : 1;
        uint32_t OT13 : 1;
        uint32_t OT14 : 1;
        uint32_t OT15 : 1;
        uint32_t RSRVD : 16;
    }BITS;
}GPIO_OTYPER_t;

typedef union
{
    uint32_t WORD;
    struct 
    {
        uint32_t OSPEEDR0 : 2;
        uint32_t OSPEEDR1 : 2;
        uint32_t OSPEEDR2 : 2;
        uint32_t OSPEEDR3 : 2;
        uint32_t OSPEEDR4 : 2;
        uint32_t OSPEEDR5 : 2;
        uint32_t OSPEEDR6 : 2;
        uint32_t OSPEEDR7 : 2;
        uint32_t OSPEEDR8 : 2;
        uint32_t OSPEEDR9 : 2;
        uint32_t OSPEEDR10 : 2;
        uint32_t OSPEEDR11 : 2;
        uint32_t OSPEEDR12 : 2;
        uint32_t OSPEEDR13 : 2;
        uint32_t OSPEEDR14 : 2;
        uint32_t OSPEEDR15 : 2;
    }BITS;
}GPIO_OSPEEDR_t;

typedef union
{
    uint32_t WORD;
    struct
    {
        uint32_t PUPDR0 : 2;
        uint32_t PUPDR1 : 2;
        uint32_t PUPDR2 : 2;
        uint32_t PUPDR3 : 2;
        uint32_t PUPDR4 : 2;
        uint32_t PUPDR5 : 2;
        uint32_t PUPDR6 : 2;
        uint32_t PUPDR7 : 2;
        uint32_t PUPDR8 : 2;
        uint32_t PUPDR9 : 2;
        uint32_t PUPDR10 : 2;
        uint32_t PUPDR11 : 2;
        uint32_t PUPDR12 : 2;
        uint32_t PUPDR13 : 2;
        uint32_t PUPDR14 : 2;
        uint32_t PUPDR15 : 2;
    }BITS;
}GPIO_PUPDR_t;

typedef union
{
    uint32_t WORD;
    struct
    {
        uint32_t IDR0 : 1;
        uint32_t IDR1 : 1;
        uint32_t IDR2 : 1;
        uint32_t IDR3 : 1;
        uint32_t IDR4 : 1;
        uint32_t IDR5 : 1;
        uint32_t IDR6 : 1;
        uint32_t IDR7 : 1;
        uint32_t IDR8 : 1;
        uint32_t IDR9 : 1;
        uint32_t IDR10 : 1;
        uint32_t IDR11 : 1;
        uint32_t IDR12 : 1;
        uint32_t IDR13 : 1;
        uint32_t IDR14 : 1;
        uint32_t IDR15 : 1;
        uint32_t RSRVD : 16;
    }BITS;
}GPIO_IDR_t;

typedef union
{
    uint32_t WORD;
    struct
    {
        uint32_t ODR0 : 1;
        uint32_t ODR1 : 1;
        uint32_t ODR2 : 1;
        uint32_t ODR3 : 1;
        uint32_t ODR4 : 1;
        uint32_t ODR5 : 1;
        uint32_t ODR6 : 1;
        uint32_t ODR7 : 1;
        uint32_t ODR8 : 1;
        uint32_t ODR9 : 1;
        uint32_t ODR10 : 1;
        uint32_t ODR11 : 1;
        uint32_t ODR12 : 1;
        uint32_t ODR13 : 1;
        uint32_t ODR14 : 1;
        uint32_t ODR15 : 1;
        uint32_t RSRVD : 16;  
    }BITS;
}GPIO_ODR_t;


#endif // GPIO_PRIVATE_H_