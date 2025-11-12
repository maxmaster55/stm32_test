#include "MCAL/GPIO/gpio.h"






gpio_return_t gpio_init(GPIO_RegDef_t *port, uint16_t pin, uint8_t mode, uint8_t pull){
    port->MODER &= ~(0x03 << (GPIO_MASK_TO_PIN(pin) * 2)); // clear mode bits
    port->MODER |= (mode << (GPIO_MASK_TO_PIN(pin) * 2));  // set mode
    port->PUPDR &= ~(0x03 << (GPIO_MASK_TO_PIN(pin) * 2)); // clear pull bits
    port->PUPDR |= (pull << (GPIO_MASK_TO_PIN(pin) * 2));  // set pull
    return GPIO_RES_OK;
}

gpio_return_t gpio_write(GPIO_RegDef_t *port, uint16_t pin, uint8_t value){
    port->BSRR = (value) ? pin : (pin << 16);
    return GPIO_RES_OK;
}

gpio_return_t gpio_read(GPIO_RegDef_t *port, uint16_t pin, uint8_t *value){
    if(value == NULL) return GPIO_RES_NOK;
    *value = (port->IDR & pin) ? 1 : 0;
    return GPIO_RES_OK;
}

gpio_return_t gpio_toggle(GPIO_RegDef_t *port, uint16_t pin){
    port->ODR ^= pin;
    return GPIO_RES_OK;
}

gpio_return_t gpio_set_alt_function(GPIO_RegDef_t *port, uint16_t pin, uint8_t alt_func){
    uint8_t pin_number = GPIO_MASK_TO_PIN(pin);
    uint8_t reg = pin_number / 8;               // 0 for AFR[0], 1 for AFR[1]
    uint8_t shift = (pin_number % 8) * 4;       // correct shift within that register

    port->AFR[reg] &= ~(0x0F << shift);        // clear 4 bits for that pin
    port->AFR[reg] |= ((alt_func & 0x0F) << shift);  // set new alternate function

    return GPIO_RES_OK;
}
