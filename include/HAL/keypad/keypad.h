#include <MCAL/GPIO/gpio.h>


typedef enum {
    BTN_RETURN_OK,
    BTN_RETURN_NOK,
} keypad_return_t;

typedef enum {
    BTN_PRESSED,
    BTN_NOT_PRESSED
} keypad_state_t;


typedef struct
{
    GPIO_RegDef_t* port;
    uint32_t pin;
} keypad_port_pin_t;


typedef struct
{
    keypad_port_pin_t rows[4];
    keypad_port_pin_t cols[4];
} keypad_t;




keypad_return_t keypad_init(keypad_t *keypad_cfg);
//keypad_return_t keypad_read_state();
keypad_return_t keypad_is_pressed(uint8_t btn);

char keypad_get_last_pressed();