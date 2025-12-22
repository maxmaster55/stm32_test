#pragma once
#include"glob.h"
#include "dma_regs.h"

// register structures
#define DMA1_BASE        0x40026000
#define DMA2_BASE        0x40026400

#define DMA1            ((dma_regs_t *)DMA1_BASE)
#define DMA2            ((dma_regs_t *)DMA2_BASE)


typedef enum
{
    DMA_STREAM_0 = 0,
    DMA_STREAM_1 = 1,
    DMA_STREAM_2 = 2,
    DMA_STREAM_3 = 3,
    DMA_STREAM_4 = 4,
    DMA_STREAM_5 = 5,
    DMA_STREAM_6 = 6,
    DMA_STREAM_7 = 7,
} dma_stream_t;

typedef enum
{
    DMA_CHANNEL_0 = 0,
    DMA_CHANNEL_1 = 1,
    DMA_CHANNEL_2 = 2,
    DMA_CHANNEL_3 = 3,
    DMA_CHANNEL_4 = 4,
    DMA_CHANNEL_5 = 5,
    DMA_CHANNEL_6 = 6,
    DMA_CHANNEL_7 = 7,
} dma_channel_t;

typedef enum
{
    DMA_DIR_PERIPH_TO_MEM = 0,
    DMA_DIR_MEM_TO_PERIPH = 1,
    DMA_DIR_MEM_TO_MEM = 2,
} dma_data_direction_t;


typedef enum
{
    ELEMENT_SIZE_8_BITS = 0,
    ELEMENT_SIZE_16_BITS = 1,
    ELEMENT_SIZE_32_BITS = 2,
} element_size_t;


typedef enum
{
    DMA_PRIORITY_LOW = 0,
    DMA_PRIORITY_MEDIUM = 1,
    DMA_PRIORITY_HIGH = 2,
    DMA_PRIORITY_VERY_HIGH = 3,
} dma_priority_t;


// only direct mode is supported for now
typedef struct
{
    dma_regs_t *dma;
    uint8_t stream_number;
    dma_channel_t channel;
    dma_data_direction_t direction;
    void *from;
    uint8_t from_increment;
    void *to;
    uint8_t to_increment;
    element_size_t element_size;
    uint16_t data_length;
    uint32_t Priority; // not that important for now
} dma_cfg_t;



typedef enum {
    DMA_OK,
    DMA_NOK
} dma_ret_t;

dma_ret_t dma_init(dma_cfg_t *cfg);
dma_ret_t dma_start(dma_regs_t *dma, dma_stream_t stream_number);
dma_ret_t dma_stream_disable(dma_regs_t *dma, dma_stream_t stream_number);
dma_ret_t dma_stream_reset(dma_regs_t *dma, dma_stream_t stream_number);

dma_ret_t dma_memcpy(uint32_t *src, uint32_t *dst, uint32_t size);
