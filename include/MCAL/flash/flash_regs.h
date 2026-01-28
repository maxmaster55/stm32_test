#pragma once

#include "glob.h"

typedef struct
{
    union
    {
        volatile uint32_t reg;

        struct
        {
            volatile uint32_t LATENCY           :4;
            volatile uint32_t res0              :4;
            volatile uint32_t PRFTEN            :1;
            volatile uint32_t ICEN              :1;
            volatile uint32_t DCEN              :1;
            volatile uint32_t ICRST             :1;
            volatile uint32_t DCRST             :1;
            volatile uint32_t res1              :19;
        };
        
    } ACR;


    union
    {
        volatile uint32_t reg;

        struct
        {
            volatile uint32_t KEY               :32;
        };
        
    } KEYR;
    

    union
    {
        volatile uint32_t reg;

        struct
        {
            volatile uint32_t OPTKEYR           :32;
        };
        
    } OPTKEYR;

    union
    {
        volatile uint32_t reg;

        struct
        {
            volatile uint32_t EOP               :1;
            volatile uint32_t OPERR             :1;
            volatile uint32_t res0              :2;
            volatile uint32_t WRPERR            :1;
            volatile uint32_t PGAERR            :1;
            volatile uint32_t PGPERR            :1;
            volatile uint32_t PGSERR            :1;
            volatile uint32_t RDERR             :1;
            volatile uint32_t res1              :7;
            volatile uint32_t BSY               :1;
            volatile uint32_t res2              :15;
        };
        
    } SR;

    union
    {
        volatile uint32_t reg;

        struct
        {
            volatile uint32_t PG                :1;
            volatile uint32_t SER               :1;
            volatile uint32_t MER               :1;
            volatile uint32_t SNB               :4;
            volatile uint32_t res0              :1;
            volatile uint32_t PSIZE             :2;
            volatile uint32_t res1              :6;
            volatile uint32_t STRT              :1;
            volatile uint32_t res2              :7;
            volatile uint32_t EOPIE             :1;
            volatile uint32_t ERRIE             :1;
            volatile uint32_t res3              :5;
            volatile uint32_t LOCK              :1;
        };
        
    } CR;


    union
    {
        volatile uint32_t reg;

        struct
        {
            volatile uint32_t OPTLOCK           :1;
            volatile uint32_t OPTSTRT           :1;
            volatile uint32_t BOR_LEV           :2;
            volatile uint32_t res0              :1;
            volatile uint32_t WDG_SW            :1;
            volatile uint32_t nRST_STOP         :1;
            volatile uint32_t nRST_STDBY        :1;
            volatile uint32_t RDP               :8;
            volatile uint32_t nWRP              :8;
            volatile uint32_t res1              :7;
            volatile uint32_t SPRMOD            :1;
        };
        
    } OPTCR;



} flash_regs_t;
