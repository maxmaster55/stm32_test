#include <MCAL/systick/systick.h>

volatile nvic_callback_t G_callback = NULL;
volatile uint64_t G_ticks = 0;
uint64_t G_clock_freq = 0;

void SysTick_Handler(void)
{
    G_ticks++;   // counts every tick
    if (G_callback != NULL)
        G_callback();
}

// clock freq in HZ
systick_ret_t systick_init(uint64_t clock_freq, systick_prescaler_t prescaler)
{
    // enable interrupt
    SYSTICK->CTRL |= (1 << SYSTICK_REG_TICK_INT);

    // prescaler
    if (prescaler == SYSTICK_PRESCALER_NO)
    {
        SYSTICK->CTRL |= (1 << SYSTICK_REG_CLKSOURCE);
    }
    else
    {
        SYSTICK->CTRL &= ~(1 << SYSTICK_REG_CLKSOURCE);
    }

    // set global clock
    G_clock_freq = clock_freq;

    return SYSTICK_OK;
}

systick_ret_t systick_configure_callback(nvic_callback_t callback)
{
    G_callback = callback;
    return SYSTICK_OK;
}


systick_ret_t systick_set_val(uint32_t ms){
    if (G_clock_freq == 0)
        return SYSTICK_NOK; // make sure clock is set

    // convert ms to ticks
    uint64_t ticks = ((uint64_t)G_clock_freq * ms) / 1000;

    // SysTick LOAD is 24-bit
    if (ticks == 0 || ticks > 0xFFFFFF)
        return SYSTICK_NOK;

    SYSTICK->LOAD = (uint32_t)(ticks - 1);
    SYSTICK->VAL  = 0; // reset current value
    return SYSTICK_OK;
}

// blocking ms delay
systick_ret_t systick_wait(uint32_t ms)
{
    uint64_t start = G_ticks;
    while ((G_ticks - start) < ms);  // wait until ms passed
    return SYSTICK_OK;
}

systick_ret_t systick_start()
{
    SYSTICK->CTRL |= (1 << SYSTICK_REG_ENABLE);
    return SYSTICK_OK;
}

systick_ret_t systick_stop()
{
    SYSTICK->CTRL &= ~(1 << SYSTICK_REG_ENABLE);
    return SYSTICK_OK;
}
