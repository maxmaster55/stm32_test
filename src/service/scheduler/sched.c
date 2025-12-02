#include <service/scheduler/sched.h>
#include <MCAL/systick/systick.h>

#define CLK 16000000UL

static runnable_t* runnable_list[RUNNABLE_MAX];
extern uint64_t G_ticks;

volatile int trig = 0;

sched_ret_t sched_start(){

    systick_start();
     // G_ticks = 0;
    while (1)
    {
        if (trig == 1)
        {
            trig = 0;
            for (int i = 0; i < RUNNABLE_MAX; i++)
            {
                if (runnable_list[i] != NULL)
                {
                    uint32_t every = runnable_list[i]->every;
                    uint32_t first = runnable_list[i]->first_delay;

                    if (G_ticks >= first)
                    {
                        if ((G_ticks - first) % every == 0)
                        {
                            runnable_list[i]->callback(runnable_list[i]->args);
                        }
                    }
                }
            }
        }
        
    }
    
}

sched_ret_t sched_init(uint32_t time){
    // init the systick timer
    systick_init(CLK, SYSTICK_PRESCALER_NO);
    systick_set_val(time);
    systick_configure_callback(sched_exec);
    //systick_start();
}

sched_ret_t sched_register(runnable_t* runnable){
    if (runnable == NULL || runnable->callback == NULL)
    {
        return SCHED_RET_NOK;
    }
    
    if (runnable_list[runnable->priority] == NULL)
    {
        runnable_list[runnable->priority] = runnable;
    }
    return SCHED_RET_OK;
}

sched_ret_t sched_exec(void){
    trig = 1;
}
