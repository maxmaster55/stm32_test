#pragma once

#include "glob.h"


#define RUNNABLE_MAX (10UL)

typedef void (*runnable_cb_t) (void * args);

typedef struct
{
    char name[20]; // useless shit
    runnable_cb_t callback;
    uint32_t every;
    uint32_t first_delay;
    uint8_t priority;
    void* args;
}runnable_t;


typedef enum{
    SCHED_RET_OK,
    SCHED_RET_NOK
} sched_ret_t;




sched_ret_t sched_init(uint32_t time);

sched_ret_t sched_register(runnable_t* runnable);

sched_ret_t sched_exec(void);

sched_ret_t sched_start(void);



