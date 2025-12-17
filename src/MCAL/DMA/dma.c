#include "MCAL/DMA/dma.h"




dma_ret_t dma_init(dma_cfg_t *cfg){
    if (cfg == NULL) return DMA_NOK;

    dma_SxCR_reg_t *stream = get_stream(DMA2, cfg->stream_number);
    if (stream == NULL) return DMA_NOK;


    // disable first
    stream->bits.EN = 0; 
    while (stream->bits.EN);


    // slect channel
    stream->bits.CHSEL = cfg->channel;

    // set data length
    cfg->dma->STREAM[cfg->stream_number].SxNDTR.bits.NDT = cfg->data_length;


    // set direction & increment
    switch (cfg->direction)
    {
    case DMA_DIR_MEM_TO_MEM:
        // direction
        stream->bits.DIR = DMA_DIR_MEM_TO_MEM; // memory to memory

        // data addresses
        cfg->dma->STREAM[cfg->stream_number].SxPAR.reg = (uint32_t)cfg->from; 
        cfg->dma->STREAM[cfg->stream_number].SxM0AR.reg = (uint32_t)cfg->to; // memory address

        // increment
        stream->bits.MINC = cfg->from_increment;
        stream->bits.PINC = cfg->to_increment;
        break;
    
    case DMA_DIR_MEM_TO_PERIPH:
        // direction
        stream->bits.DIR = DMA_DIR_MEM_TO_PERIPH;
        
        // data addresses    
        cfg->dma->STREAM[cfg->stream_number].SxPAR.reg = (uint32_t)cfg->to; // peripheral address
        cfg->dma->STREAM[cfg->stream_number].SxM0AR.reg = (uint32_t)cfg->from; 

        // increment
        stream->bits.MINC = cfg->to_increment;
        stream->bits.PINC = cfg->from_increment;
        break;

    case DMA_DIR_PERIPH_TO_MEM:
        // direction
        stream->bits.DIR = DMA_DIR_PERIPH_TO_MEM;

        // data addresses
        cfg->dma->STREAM[cfg->stream_number].SxPAR.reg = (uint32_t)cfg->from; // peripheral address
        cfg->dma->STREAM[cfg->stream_number].SxM0AR.reg = (uint32_t)cfg->to;

        stream->bits.MINC = cfg->to_increment;
        stream->bits.PINC = cfg->from_increment;
        break;
    }


    // data size (only for direct mode)
    stream->bits.MSIZE = cfg->element_size;
    stream->bits.PSIZE = cfg->element_size;

    // priority
    stream->bits.PL = cfg->Priority;



    return DMA_OK;
}


dma_ret_t dma_start(dma_regs_t *dma, dma_stream_t stream_number){
    if (dma == NULL) return DMA_NOK;
    dma_SxCR_reg_t *stream = get_stream(dma, stream_number);
    if (stream == NULL) return DMA_NOK;

    stream->bits.EN = 1;

    return DMA_OK;
}

dma_ret_t dma_memcpy(uint32_t *src, uint32_t *dst, uint32_t size){
    // test dma memcpy using dma2 stream 0

    // test read feif0


    DMA2->STREAM[0].SxCR.bits.EN = 0; 
    while (DMA2->STREAM[0].SxCR.bits.EN);

    // enable increment
    DMA2->STREAM[0].SxCR.bits.MINC = 1;
    DMA2->STREAM[0].SxCR.bits.MSIZE = 1; // 16-bit
    DMA2->STREAM[0].SxCR.bits.PSIZE = 1; // 16-bit
    DMA2->STREAM[0].SxCR.bits.PINC = 1;

    DMA2->STREAM[0].SxCR .bits.CHSEL = 0; // channel 0
    DMA2->STREAM[0].SxCR.bits.DIR = 2; // memory to memory


    DMA2->STREAM[0].SxPAR.reg = (uint32_t)src; 
    DMA2->STREAM[0].SxM0AR.reg = (uint32_t)dst; // memory address
    DMA2->STREAM[0].SxNDTR.bits.NDT = 8; // number of data items
    
    DMA2->STREAM[0].SxCR.bits.EN = 1;

}