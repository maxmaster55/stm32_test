#include "glob.h"
#include "MCAL/RCC/rcc.h"

#define SYSTICK_BASE_ADDR (0xE000E010UL)


typedef struct
{
    uint32_t CTRL;
    uint32_t LOAD;
    uint32_t VAL;
    uint32_t CALIB;
} systick_reg_t;

#define SYSTICK ((systick_reg_t*) SYSTICK_BASE_ADDR)

typedef enum{
    SYSTICK_OK,
    SYSTICK_NOK
}systick_ret_t;

typedef enum{
    SYSTICK_PRESCALER_NO,
    SYSTICK_PRESCALER_8
}systick_prescaler_t;

#define SYSTICK_REG_ENABLE           (0)
#define SYSTICK_REG_TICK_INT         (1)
#define SYSTICK_REG_CLKSOURCE        (2)
#define SYSTICK_REG_COUNT_FLAG       (16)


typedef void (*nvic_callback_t) (void);

systick_ret_t systick_init(uint64_t clock_val, systick_prescaler_t prescaler);
systick_ret_t systick_configure_callback(nvic_callback_t callback);
systick_ret_t systick_set_val(uint32_t ms);
systick_ret_t systick_wait(uint32_t ms);
systick_ret_t systick_start();
systick_ret_t systick_stop();



