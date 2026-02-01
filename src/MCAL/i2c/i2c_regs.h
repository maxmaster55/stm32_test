#pragma once
#include "glob.h"


typedef struct
{

    union
    {
        volatile uint32_t reg;
        volatile struct
        {
            uint32_t PE           :1;
            uint32_t SMBUS           :1;
            uint32_t res0           :1;
            uint32_t SMBTYPE           :1;
            uint32_t ENARP           :1;
            uint32_t ENPEC           :1;
            uint32_t ENGC           :1;
            uint32_t NOSTRETCH           :1;
            uint32_t START           :1;
            uint32_t STOP           :1;
            uint32_t ACK           :1;
            uint32_t POS           :1;
            uint32_t PEC           :1;
            uint32_t ALERT           :1;
            uint32_t res1           :1;
            uint32_t SWRST           :1;

        };
        
    }CR1;
    

    union
    {
        volatile uint32_t reg;
        volatile struct
        {
            uint32_t FREQ           :6;
            uint32_t res0           :2;
            uint32_t ITERREN           :1;
            uint32_t ITEVTEN           :1;
            uint32_t ITBUFEN           :1;
            uint32_t DMAEN           :1;
            uint32_t LAST           :1;

        };
        
    }CR2;
    
    union
    {
        volatile uint32_t reg;
        volatile struct
        {
            uint32_t ADD0             :1;
            uint32_t ADD1_7           :7;
            uint32_t ADD8_9           :2;
            uint32_t ENDUAL           :1;
            uint32_t res0             :5;
            uint32_t ADDMODE          :1;
        };
        
    }OAR1;

    union
    {
        volatile uint32_t reg;
        volatile struct
        {
            uint32_t ENDUAL           :1;
            uint32_t ADD2           :7;

        };
        
    }OAR2;

    union
    {
        volatile uint32_t reg;
        volatile struct
        {
            uint32_t DR           :8;
        };
        
    }DR;

    union
    {
        volatile uint32_t reg;
        volatile struct
        {
            uint32_t SB           :1;
            uint32_t ADDR           :1;
            uint32_t BTF           :1;
            uint32_t ADD10           :1;
            uint32_t STOPF           :1;
            uint32_t res0           :1;
            uint32_t RxNE           :1;
            uint32_t TxE           :1;
            uint32_t BERR           :1;
            uint32_t ARLO           :1;
            uint32_t AF           :1;
            uint32_t OVR           :1;
            uint32_t PECERR           :1;
            uint32_t res1           :1;
            uint32_t TIMEOUT           :1;
            uint32_t SMBALERT           :1;
        };
        
    }SR1;

    union
    {
        volatile uint32_t reg;
        volatile struct
        {
            uint32_t MSL           :1;
            uint32_t BUSY           :1;
            uint32_t TRA           :1;
            uint32_t res0           :1;
            uint32_t GENCALL           :1;
            uint32_t SMBDEFAULT           :1;
            uint32_t SMBHOST           :1;
            uint32_t DUALF           :1;
            uint32_t PEC           :8;

        };
        
    }SR2;


    union
    {
        volatile uint32_t reg;
        volatile struct
        {
            uint32_t CCR            :12;
            uint32_t res0           :2;
            uint32_t DUTY           :1;
            uint32_t F_S            :1;
        };
        
    }CCR;


    union
    {
        volatile uint32_t reg;
        volatile struct
        {
            uint32_t TRISE          :6;
        };
        
    }TRISE;


    union
    {
        volatile uint32_t reg;
        volatile struct
        {
            uint32_t DNF            :4; 
            uint32_t ANOFF          :4; 
        };
        
    }FLTR;

} i2c_regs_t;
