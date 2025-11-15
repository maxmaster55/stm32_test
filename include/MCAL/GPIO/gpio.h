#pragma once

#include "glob.h"
#include "gpio_cfg.h"
#include <stdlib.h>
#include <stdint.h>

// 0x4002 1C00 - 0x4002 1FFF
#define GPIOH_BASE 0x40021C00UL
// 0x4002 1000 - 0x4002 13FF
#define GPIOE_BASE 0x40021000UL
// 0x4002 0C00 - 0x4002 0FFF
#define GPIOD_BASE 0x40020C00UL
// 0x4002 0800 - 0x4002 0BFF
#define GPIOC_BASE 0x40020800UL
// 0x4002 0400 - 0x4002 07FF
#define GPIOB_BASE 0x40020400UL
// 0x4002 0000 - 0x4002 03FF
#define GPIOA_BASE 0x40020000UL


typedef struct {
    volatile uint32_t MODER;    // GPIO port mode register,               Address offset: 0x00
    volatile uint32_t OTYPER;   // GPIO port output type register,        Address offset: 0x04
    volatile uint32_t OSPEEDR;  // GPIO port output speed register,       Address offset: 0x08
    volatile uint32_t PUPDR;    // GPIO port pull-up/pull-down register,  Address offset: 0x0C
    volatile uint32_t IDR;      // GPIO port input data register,         Address offset: 0x10
    volatile uint32_t ODR;      // GPIO port output data register,        Address offset: 0x14
    volatile uint32_t BSRR;     // GPIO port bit set/reset register,      Address offset: 0x18
    volatile uint32_t LCKR;     // GPIO port configuration lock register, Address offset: 0x1C
    volatile uint32_t AFR[2];   // GPIO alternate function registers,     Address offset: 0x20-0x24
} GPIO_RegDef_t;


typedef struct
{  
    GPIO_RegDef_t *port;
    uint32_t pin;
    uint8_t mode;
    uint8_t pull;
    uint8_t alt_function;
    uint8_t speed;
    uint8_t output_type;
} GPIO_PinConfig_t;



#define GPIOA     ((GPIO_RegDef_t *) GPIOA_BASE)
#define GPIOB     ((GPIO_RegDef_t *) GPIOB_BASE)
#define GPIOC     ((GPIO_RegDef_t *) GPIOC_BASE)
#define GPIOD     ((GPIO_RegDef_t *) GPIOD_BASE)
#define GPIOE     ((GPIO_RegDef_t *) GPIOE_BASE)
#define GPIOH     ((GPIO_RegDef_t *) GPIOH_BASE)



// pin mask to pin number
#define GPIO_MASK_TO_PIN(mask) __builtin_ctz(mask)


// GPIO Pin definitions
#define    GPIO_PIN_0      (0x0001)
#define    GPIO_PIN_1      (0x0002)
#define    GPIO_PIN_2      (0x0004)
#define    GPIO_PIN_3      (0x0008)
#define    GPIO_PIN_4      (0x0010)
#define    GPIO_PIN_5      (0x0020)
#define    GPIO_PIN_6      (0x0040)
#define    GPIO_PIN_7      (0x0080)
#define    GPIO_PIN_8      (0x0100)
#define    GPIO_PIN_9      (0x0200)
#define    GPIO_PIN_10     (0x0400)
#define    GPIO_PIN_11     (0x0800)
#define    GPIO_PIN_12     (0x1000)
#define    GPIO_PIN_13     (0x2000)
#define    GPIO_PIN_14     (0x4000)
#define    GPIO_PIN_15     (0x8000)


// GPIO Mode definitions
#define GPIO_MODE_INPUT        (0x00)
#define GPIO_MODE_OUTPUT       (0x01)
#define GPIO_MODE_ALTFN        (0x02)
#define GPIO_MODE_ANALOG       (0x03)


// GPIO Pull-Up/Pull-Down definitions
#define GPIO_PULL_NO          (0x00)
#define GPIO_PULL_UP          (0x01)
#define GPIO_PULL_DOWN        (0x02)


// GPIO Alternate Function definitions
#define GPIO_AF0_SYSTEM      (0x00)
#define GPIO_AF1_TIM1_2      (0x01)
#define GPIO_AF2_TIM3_5      (0x02)
#define GPIO_AF3_TIM9_11     (0x03)
#define GPIO_AF4_I2C1_3      (0x04)
#define GPIO_AF5_SPI1_2      (0x05)
#define GPIO_AF6_SPI3        (0x06)
#define GPIO_AF7_USART1_2    (0x07)
#define GPIO_AF8_USART6      (0x08)
#define GPIO_AF9_I2C2_3      (0x09)
#define GPIO_AF10_OTG_FS     (0x0A)
#define GPIO_AF11_ETH        (0x0B)
#define GPIO_AF12_SDIO       (0x0C)
#define GPIO_AF13            (0x0D)
#define GPIO_AF14            (0x0E)
#define GPIO_AF15_EVENTOUT   (0x0F)


// GPIO Speed definitions
#define GPIO_SPEED_LOW       (0x00)
#define GPIO_SPEED_MEDIUM    (0x01)
#define GPIO_SPEED_FAST      (0x02)
#define GPIO_SPEED_HIGH      (0x03)

// GPIO Output Type definitions
#define GPIO_OUTPUT_PUSHPULL    (0x00)
#define GPIO_OUTPUT_OPENDRAIN   (0x01)



typedef enum {
    GPIO_RES_OK,
    GPIO_RES_NOK,
    GPIO_RES_NOT_INITIALIZED
} gpio_return_t;



/**
 * @brief Initialize GPIO pin
 * 
 * @param pin_cfg pointer to GPIO_PinConfig_t structure
 * @return gpio_return_t 
 */
gpio_return_t gpio_init(GPIO_PinConfig_t *pin_cfg);

/**
 * @brief Write value to GPIO pin
 * 
 * @param port use GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOH defines
 * @param pin use GPIO_PIN_x defines
 * @param value 0 or 1
 * @return gpio_return_t 
 */
gpio_return_t gpio_write(GPIO_RegDef_t *port, uint16_t pin, uint8_t value);

/**
 * @brief Read value from GPIO pin
 * 
 * @param port use GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOH defines
 * @param pin use GPIO_PIN_x defines
 * @param value pointer to store the read value
 * @return gpio_return_t 
 */
gpio_return_t gpio_read(GPIO_RegDef_t *port, uint16_t pin, uint8_t *value);

/**
 * @brief Toggle GPIO pin
 * 
 * @param port use GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOH defines
 * @param pin use GPIO_PIN_x defines
 * @return gpio_return_t 
 */
gpio_return_t gpio_toggle(GPIO_RegDef_t *port, uint16_t pin);

/**
 * @brief Set alternate function for GPIO pin
 * 
 * @param port use GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOH defines
 * @param pin use GPIO_PIN_x defines
 * @param alt_func use GPIO_AFx defines
 * @return gpio_return_t 
 */
gpio_return_t gpio_set_alt_function(GPIO_RegDef_t *port, uint16_t pin, uint8_t alt_func);


gpio_return_t gpio_set_port_value(GPIO_RegDef_t *port, uint16_t value);

gpio_return_t gpio_get_port_value(GPIO_RegDef_t *port, uint16_t *value);