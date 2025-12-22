#include "MCAL/DMA/dma.h"

/* =========================
 * DMA INIT
 * ========================= */
dma_ret_t dma_init(dma_cfg_t *cfg)
{
    if (cfg == NULL) return DMA_NOK;

    /* Reset stream completely */
    dma_stream_reset(cfg->dma, cfg->stream_number);

    /* -------- Select channel -------- */
    cfg->dma->STREAM[cfg->stream_number].SxCR.bits.CHSEL = cfg->channel;

    /* -------- Set data length -------- */
    cfg->dma->STREAM[cfg->stream_number].SxNDTR.bits.NDT = cfg->data_length;

    /* -------- Direction & addressing -------- */
    switch (cfg->direction)
    {
    case DMA_DIR_MEM_TO_MEM:
        cfg->dma->STREAM[cfg->stream_number].SxCR.bits.DIR = DMA_DIR_MEM_TO_MEM;

        cfg->dma->STREAM[cfg->stream_number].SxPAR.reg  = (uint32_t)cfg->from;
        cfg->dma->STREAM[cfg->stream_number].SxM0AR.reg = (uint32_t)cfg->to;

        cfg->dma->STREAM[cfg->stream_number].SxCR.bits.MINC = cfg->from_increment;
        cfg->dma->STREAM[cfg->stream_number].SxCR.bits.PINC = cfg->to_increment;
        break;

    case DMA_DIR_MEM_TO_PERIPH:
        cfg->dma->STREAM[cfg->stream_number].SxCR.bits.DIR = DMA_DIR_MEM_TO_PERIPH;

        cfg->dma->STREAM[cfg->stream_number].SxPAR.reg  = (uint32_t)cfg->to;
        cfg->dma->STREAM[cfg->stream_number].SxM0AR.reg = (uint32_t)cfg->from;

        cfg->dma->STREAM[cfg->stream_number].SxCR.bits.MINC = cfg->from_increment;
        cfg->dma->STREAM[cfg->stream_number].SxCR.bits.PINC = 0;
        break;

    case DMA_DIR_PERIPH_TO_MEM:
        cfg->dma->STREAM[cfg->stream_number].SxCR.bits.DIR = DMA_DIR_PERIPH_TO_MEM;

        cfg->dma->STREAM[cfg->stream_number].SxPAR.reg  = (uint32_t)cfg->from;
        cfg->dma->STREAM[cfg->stream_number].SxM0AR.reg = (uint32_t)cfg->to;

        cfg->dma->STREAM[cfg->stream_number].SxCR.bits.MINC = cfg->to_increment;
        cfg->dma->STREAM[cfg->stream_number].SxCR.bits.PINC = 0;
        break;
    }

    /* -------- Data size -------- */
    cfg->dma->STREAM[cfg->stream_number].SxCR.bits.MSIZE = cfg->element_size;
    cfg->dma->STREAM[cfg->stream_number].SxCR.bits.PSIZE = cfg->element_size;

    /* -------- Priority -------- */
    cfg->dma->STREAM[cfg->stream_number].SxCR.bits.PL = cfg->Priority;

    /* -------- Enable interrupts -------- */
    cfg->dma->STREAM[cfg->stream_number].SxCR.bits.TCIE = 1; // Transfer complete
    cfg->dma->STREAM[cfg->stream_number].SxCR.bits.TEIE = 1; // Transfer error

    return DMA_OK;
}

/* =========================
 * DMA START
 * ========================= */
dma_ret_t dma_start(dma_regs_t *dma, dma_stream_t stream_number)
{
    if (dma == NULL) return DMA_NOK;

    dma->STREAM[stream_number].SxCR.bits.EN = 1;
    return DMA_OK;
}

/* =========================
 * DMA STREAM DISABLE
 * ========================= */
dma_ret_t dma_stream_disable(dma_regs_t *dma, dma_stream_t stream_number)
{
    if (dma == NULL) return DMA_NOK;

    dma->STREAM[stream_number].SxCR.bits.EN = 0;
    while (dma->STREAM[stream_number].SxCR.bits.EN);

    return DMA_OK;
}

/* =========================
 * DMA STREAM RESET
 * ========================= */
dma_ret_t dma_stream_reset(dma_regs_t *dma, dma_stream_t stream_number)
{
    if (dma == NULL) return DMA_NOK;

    /* Disable stream first */
    dma_stream_disable(dma, stream_number);

    /* -------- Clear interrupt flags (6-bit layout) -------- */
    uint32_t shift =
        (stream_number <= 3) ?
        (stream_number * 6) :
        ((stream_number - 4) * 6);

    uint32_t mask =
          (1 << (shift + 0))  /* FEIF  */
        | (1 << (shift + 2))  /* DMEIF */
        | (1 << (shift + 3))  /* TEIF  */
        | (1 << (shift + 4))  /* HTIF  */
        | (1 << (shift + 5)); /* TCIF  */

    if (stream_number <= 3)
        dma->LIFCR.reg = mask;
    else
        dma->HIFCR.reg = mask;

    /* -------- Reset registers -------- */
    dma->STREAM[stream_number].SxCR.reg   = 0;
    dma->STREAM[stream_number].SxNDTR.reg = 0;
    dma->STREAM[stream_number].SxPAR.reg  = 0;
    dma->STREAM[stream_number].SxM0AR.reg = 0;
    dma->STREAM[stream_number].SxM1AR.reg = 0;
    dma->STREAM[stream_number].SxFCR.reg  = 0x21; // reset value

    return DMA_OK;
}

/* =========================
 * DMA MEMCPY (TEST)
 * ========================= */
dma_ret_t dma_memcpy(uint32_t *src, uint32_t *dst, uint32_t size)
{
    if (!src || !dst || size == 0) return DMA_NOK;

    dma_stream_reset(DMA2, DMA_STREAM_0);

    DMA2->STREAM[0].SxCR.bits.CHSEL = 0;
    DMA2->STREAM[0].SxCR.bits.DIR   = DMA_DIR_MEM_TO_MEM;

    DMA2->STREAM[0].SxCR.bits.MINC = 1;
    DMA2->STREAM[0].SxCR.bits.PINC = 1;

    DMA2->STREAM[0].SxCR.bits.MSIZE = 1;
    DMA2->STREAM[0].SxCR.bits.PSIZE = 1;

    DMA2->STREAM[0].SxPAR.reg  = (uint32_t)src;
    DMA2->STREAM[0].SxM0AR.reg = (uint32_t)dst;
    DMA2->STREAM[0].SxNDTR.bits.NDT = size;

    DMA2->STREAM[0].SxCR.bits.TCIE = 1;
    DMA2->STREAM[0].SxCR.bits.EN   = 1;

    return DMA_OK;
}
