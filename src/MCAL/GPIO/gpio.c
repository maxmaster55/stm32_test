#include "MCAL/GPIO/gpio.h"




// for just one pin
gpio_return_t gpio_init(GPIO_PinConfig_t *pin_cfg){
    if(pin_cfg == NULL || pin_cfg->port == NULL) return GPIO_RES_NOK;

    uint8_t pin_index = GPIO_MASK_TO_PIN(pin_cfg->pin);

    /* OTYPER: single bit per pin */
    pin_cfg->port->OTYPER &= ~(1U << pin_index); /* clear output type */
    pin_cfg->port->OTYPER |= ((pin_cfg->output_type & 0x1U) << pin_index);  /* set output type */

    /* MODER: two bits per pin - clear then set */
    pin_cfg->port->MODER &= ~(0x03U << (pin_index * 2));
    // bit insertion
    pin_cfg->port->MODER |= ((pin_cfg->mode & 0x03U) << (pin_index * 2));  /* set mode */

    /* PUPDR: two bits per pin - clear then set */
    pin_cfg->port->PUPDR &= ~(0x03U << (pin_index * 2)); /* clear pull bits */
    pin_cfg->port->PUPDR |= ((pin_cfg->pull & 0x03U) << (pin_index * 2));  /* set pull */

    return GPIO_RES_OK;
}


gpio_return_t gpio_write(GPIO_RegDef_t *port, uint16_t pin, uint8_t value){
    if(port == NULL) return GPIO_RES_NOK;
    /* BSRR: lower half sets, upper half resets */
    port->BSRR = (value) ? pin : (pin << 16);
    return GPIO_RES_OK;
}

gpio_return_t gpio_read(GPIO_RegDef_t *port, uint16_t pin, uint8_t *value){
    if(port == NULL || value == NULL) return GPIO_RES_NOK;
    *value = (port->IDR & pin) ? 1 : 0;
    return GPIO_RES_OK;
}

gpio_return_t gpio_toggle(GPIO_RegDef_t *port, uint16_t pin){
    if(port == NULL) return GPIO_RES_NOK;
    port->ODR ^= pin;
    return GPIO_RES_OK;
}

gpio_return_t gpio_set_alt_function(GPIO_RegDef_t *port, uint16_t pin, uint8_t alt_func){
    if(port == NULL) return GPIO_RES_NOK;
    uint8_t pin_number = GPIO_MASK_TO_PIN(pin);
    uint8_t reg = pin_number / 8;               /* 0 for AFR[0], 1 for AFR[1] */
    uint8_t shift = (pin_number % 8) * 4;       /* correct shift within that register */

    port->AFR[reg] &= ~(0x0FU << shift);        /* clear 4 bits for that pin */
    port->AFR[reg] |= ((alt_func & 0x0FU) << shift);  /* set new alternate function */

    return GPIO_RES_OK;
}
