#include "glob.h"

typedef union 
{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t FEIF0 : 1;
        volatile uint32_t res0 : 1;
        // DMEIF0 TEIF0 HTIF0 TCIF0 FEIF1
        volatile uint32_t DMEIF0 : 1;
        volatile uint32_t TEIF0 : 1;
        volatile uint32_t HTIF0 : 1;
        volatile uint32_t TCIF0 : 1;
        volatile uint32_t FEIF1 : 1;
        volatile uint32_t res1 : 1;
        // DMEIF1 TEIF1 HTIF1 TCIF1
        volatile uint32_t DMEIF1 : 1;
        volatile uint32_t TEIF1 : 1;
        volatile uint32_t HTIF1 : 1;
        volatile uint32_t TCIF1 : 1;
        volatile uint32_t res2 : 4;
        volatile uint32_t FEIF2 : 1;
        volatile uint32_t res3 : 1;
        // DMEIF2 TEIF2 HTIF2 TCIF2 FEIF3
        volatile uint32_t DMEIF2 : 1;
        volatile uint32_t TEIF2 : 1;
        volatile uint32_t HTIF2 : 1;
        volatile uint32_t TCIF2 : 1;
        volatile uint32_t FEIF3 : 1;
        volatile uint32_t res4 : 1;
        // DMEIF3 TEIF3 HTIF3 TCIF3
        volatile uint32_t DMEIF3 : 1;
        volatile uint32_t TEIF3 : 1;
        volatile uint32_t HTIF3 : 1;
        volatile uint32_t TCIF3 : 1;
        volatile uint32_t res5 : 4;
    } bits;
} dma_LISR_reg_t;


typedef union{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t FEIF4 : 1;
        volatile uint32_t res0 : 1;
        // DMEIF4 TEIF4 HTIF4 TCIF4 FEIF5
        volatile uint32_t DMEIF4 : 1;
        volatile uint32_t TEIF4 : 1;
        volatile uint32_t HTIF4 : 1;
        volatile uint32_t TCIF4 : 1;
        volatile uint32_t FEIF5 : 1;
        volatile uint32_t res1 : 1;
        // DMEIF5 TEIF5 HTIF5 TCIF5
        volatile uint32_t DMEIF5 : 1;
        volatile uint32_t TEIF5 : 1;
        volatile uint32_t HTIF5 : 1;
        volatile uint32_t TCIF5 : 1;
        volatile uint32_t res2 : 4;
        volatile uint32_t FEIF6 : 1;
        volatile uint32_t res3 : 1;
        // DMEIF6 TEIF6 HTIF6 TCIF6 FEIF7
        volatile uint32_t DMEIF6 : 1;
        volatile uint32_t TEIF6 : 1;
        volatile uint32_t HTIF6 : 1;
        volatile uint32_t TCIF6 : 1;
        volatile uint32_t FEIF7 : 1;
        volatile uint32_t res4 : 1;
        // DMEIF7 TEIF7 HTIF7 TCIF7
        volatile uint32_t DMEIF7 : 1;
        volatile uint32_t TEIF7 : 1;
        volatile uint32_t HTIF7 : 1;
        volatile uint32_t TCIF7 : 1;
        volatile uint32_t res5 : 4;
    } bits;
} dma_HISR_reg_t;


typedef union{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t CFEIF0 : 1;
        volatile uint32_t res0 : 1;
        // CDMEIF0 CTEIF0 CHTIF0 CTCIF0 CFEIF1
        volatile uint32_t CDMEIF0 : 1;
        volatile uint32_t CTEIF0 : 1;
        volatile uint32_t CHTIF0 : 1;
        volatile uint32_t CTCIF0 : 1;
        volatile uint32_t CFEIF1 : 1;
        volatile uint32_t res1 : 1;
        // CDMEIF1 CTEIF1 CHTIF1 CTCIF1
        volatile uint32_t CDMEIF1 : 1;
        volatile uint32_t CTEIF1 : 1;
        volatile uint32_t CHTIF1 : 1;
        volatile uint32_t CTCIF1 : 1;
        volatile uint32_t res2 : 4;
        volatile uint32_t CFEIF2 : 1;
        volatile uint32_t res3 : 1;
        // CDMEIF2 CTEIF2 CHTIF2 CTCIF2 CFEIF3
        volatile uint32_t CDMEIF2 : 1;
        volatile uint32_t CTEIF2 : 1;
        volatile uint32_t CHTIF2 : 1;
        volatile uint32_t CTCIF2 : 1;
        volatile uint32_t CFEIF3 : 1;
        volatile uint32_t res4 : 1;
        // CDMEIF3 CTEIF3 CHTIF3 CTCIF3
        volatile uint32_t CDMEIF3 : 1;
        volatile uint32_t CTEIF3 : 1;
        volatile uint32_t CHTIF3 : 1;
        volatile uint32_t CTCIF3 : 1;
        volatile uint32_t res5 : 4;
    } bits;
} dma_LIFCR_reg_t;



typedef union{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t CFEIF4 : 1;
        volatile uint32_t res0 : 1;
         // CDMEIF4 CTEIF4 CHTIF4 CTCIF4 CFEIF5
        volatile uint32_t CDMEIF4 : 1;
        volatile uint32_t CTEIF4 : 1;
        volatile uint32_t CHTIF4 : 1;
        volatile uint32_t CTCIF4 : 1;
        volatile uint32_t CFEIF5 : 1;
        volatile uint32_t res1 : 1;
         // CDMEIF5 CTEIF5 CHTIF5 CTCIF5
        volatile uint32_t CDMEIF5 : 1;
        volatile uint32_t CTEIF5 : 1;
        volatile uint32_t CHTIF5 : 1;
        volatile uint32_t CTCIF5 : 1;
        volatile uint32_t res2 : 4;
        volatile uint32_t CFEIF6 : 1;
        volatile uint32_t res3 : 1;
         // CDMEIF6 CTEIF6 CHTIF6 CTCIF6 CFEIF7
        volatile uint32_t CDMEIF6 : 1;
        volatile uint32_t CTEIF6 : 1;
        volatile uint32_t CHTIF6 : 1;
        volatile uint32_t CTCIF6 : 1;
        volatile uint32_t CFEIF7 : 1;
        volatile uint32_t res4 : 1;
         // CDMEIF7 CTEIF7 CHTIF7 CTCIF7
        volatile uint32_t CDMEIF7 : 1;
        volatile uint32_t CTEIF7 : 1;
        volatile uint32_t CHTIF7 : 1;
        volatile uint32_t CTCIF7 : 1;
        volatile uint32_t res5 : 4;
    } bits;
} dma_HIFCR_reg_t;



typedef union{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t EN : 1;
        volatile uint32_t DMEIE : 1;
        volatile uint32_t TEIE : 1;
        volatile uint32_t HTIE : 1;
        volatile uint32_t TCIE : 1;
        volatile uint32_t PFCTRL : 1;
        volatile uint32_t DIR : 2;
        volatile uint32_t CIRC : 1;
        volatile uint32_t PINC : 1;
        volatile uint32_t MINC : 1;
        volatile uint32_t PSIZE : 2;
        volatile uint32_t MSIZE : 2;
        volatile uint32_t PINCOS : 1;
        volatile uint32_t PL : 2;
        volatile uint32_t DBM : 1;
        volatile uint32_t CT : 1;
        volatile uint32_t res0 : 1;
        volatile uint32_t PBURST : 2;
        volatile uint32_t MBURST : 2;
        volatile uint32_t CHSEL : 3;
        volatile uint32_t res1 : 4;
    } bits;
} dma_SxCR_reg_t;


typedef union{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t NDT : 16;
        volatile uint32_t res : 16;
    } bits;
} dma_SxNDTR_reg_t;


typedef union{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t PAR : 32;
    } bits;
} dma_SxPAR_reg_t;


typedef union{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t MAR : 32;
    } bits;
} dma_SxM0AR_reg_t;


typedef union{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t MAR : 32;
    } bits;
} dma_SxM1AR_reg_t;


typedef union{
    volatile uint32_t reg;
    struct
    {
        volatile uint32_t FTH : 2;
        volatile uint32_t DMDIS : 1;
        volatile uint32_t FS : 3;
        volatile uint32_t res0 : 1;
        volatile uint32_t FEIE : 1;
        volatile uint32_t res1 : 24;
    } bits;
} dma_SxFCR_reg_t;


typedef struct 
{
    dma_SxCR_reg_t SxCR;       // 0x00
    dma_SxNDTR_reg_t SxNDTR;   // 0x04
    dma_SxPAR_reg_t SxPAR;     // 0x08
    dma_SxM0AR_reg_t SxM0AR;   // 0x0C
    dma_SxM1AR_reg_t SxM1AR;   // 0x10
    dma_SxFCR_reg_t SxFCR;      // 0x14
} dma_stream_regs_t;



typedef struct
{
    dma_LISR_reg_t LISR;       // 0x00
    dma_HISR_reg_t HISR;       // 0x04
    dma_LIFCR_reg_t LIFCR;     // 0x08
    dma_HIFCR_reg_t HIFCR;     // 0x0C
    
    dma_stream_regs_t STREAM[8]; // 0x10 - 0x60

} dma_regs_t;
