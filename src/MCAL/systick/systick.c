#include <MCAL/systick/systick.h>

volatile nvic_callback_t G_callback = NULL;
volatile uint64_t G_ms_ticks = 0;
uint64_t G_clock_freq = 0;

void SysTick_Handler(void)
{
    G_ms_ticks++;   // counts every 1ms
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

    // ✔ set LOAD once for 1ms resolution
    uint32_t load = (clock_freq / 1000) - 1;
    SYSTICK->LOAD = load;
    SYSTICK->VAL = 0;

    return SYSTICK_OK;
}

systick_ret_t systick_configure_callback(nvic_callback_t callback)
{
    G_callback = callback;
    return SYSTICK_OK;
}

// blocking ms delay
systick_ret_t systick_wait(uint32_t ms)
{
    uint64_t start = G_ms_ticks;
    while ((G_ms_ticks - start) < ms);  // wait until ms passed
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
