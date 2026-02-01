#pragma once

#include "glob.h"


typedef struct
{

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t CPHA          :1;
            volatile uint32_t CPOL          :1;
            volatile uint32_t MSTR          :1;
            volatile uint32_t BR            :3;
            volatile uint32_t SPE           :1;
            volatile uint32_t LSBFIRST      :1;
            volatile uint32_t SSI           :1;
            volatile uint32_t SSM           :1;
            volatile uint32_t RXONLY        :1;
            volatile uint32_t DFF           :1;
            volatile uint32_t CRCNEXT       :1;
            volatile uint32_t CRCEN         :1;
            volatile uint32_t BIDIOE        :1;
            volatile uint32_t BIDIMODE      :1;
            volatile uint32_t res0          :16;
        };
    }CR_1;

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t RXDMAEN       :1;
            volatile uint32_t TXDMAEN       :1;
            volatile uint32_t SSOE          :1;
            volatile uint32_t res0          :1;
            volatile uint32_t FRF           :1;
            volatile uint32_t ERRIE         :1;
            volatile uint32_t RXNEIE        :1;
            volatile uint32_t TXNEIE        :1;
            volatile uint32_t res1          :24;
        };
    }CR_2;
    
    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t RXNE          :1;
            volatile uint32_t TXE           :1;
            volatile uint32_t CHSIDE        :1;
            volatile uint32_t UDR           :1;
            volatile uint32_t CRCERR        :1;
            volatile uint32_t MODF          :1;
            volatile uint32_t OVR           :1;
            volatile uint32_t BSY           :1;
            volatile uint32_t FRE           :1;
            volatile uint32_t res0          :23;
        };
    }SR;

        
    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t DR            :16;
            volatile uint32_t res0          :16;
        };
    }DR;


    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t CRCPOLY       :16;
            volatile uint32_t res0          :16;
        };
    }CRCPR;

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t RxCRC         :16;
            volatile uint32_t res0          :16;
        };
    }RXCRCR;

    union
    {
        volatile uint32_t reg;
        struct
        {
            volatile uint32_t TxCRC         :16;
            volatile uint32_t res0          :16;
        };
    }TXCRCR;


    // I2S not used
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
} SPI_regs_t;
