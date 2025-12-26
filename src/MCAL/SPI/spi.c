
#include <MCAL/SPI/spi.h>

void spi_init(const spi_cfg_t* cfg)
{
    SPI_regs_t* spi = NULL;
    switch (cfg->spi_num)
    {
    case SPI_NUM_1:
        spi = SPI1;
        break;
    case SPI_NUM_2:
        spi = SPI2;
        break;
    case SPI_NUM_3:
        spi = SPI3;
        break;
    case SPI_NUM_4:
        spi = SPI4;
        break;
    default:
        return;
    }

    spi->CR_1.SPE = 0;

    // master/slave
    spi->CR_1.MSTR = (cfg->mode == SPI_MODE_MASTER);

    // clock rate
    spi->CR_1.BR = cfg->speed;

    // clock polarity and phase
    spi->CR_1.CPOL = cfg->polarity;
    spi->CR_1.CPHA = cfg->phase;

    // data size
    spi->CR_1.DFF = (cfg->data_size == SPI_DATASIZE_16BIT);

    // erame format
    spi->CR_1.LSBFIRST = (cfg->frame_format == SPI_FRAME_LSB_FIRST);

    // NSS management
    if (cfg->nss == SPI_NSS_SOFT) {
        spi->CR_1.SSM = 1;
        spi->CR_1.SSI = 1; // internal high
    } else if (cfg->nss == SPI_NSS_HARD_OUTPUT) {
        spi->CR_2.SSOE = 1; // output enabled
    } else {
        spi->CR_1.SSM = 0; // hardware input
    }

    // protocol
    spi->CR_2.FRF = (cfg->protocol == SPI_PROTOCOL_TI);

    // full duplex
    spi->CR_1.BIDIMODE = 0;
    spi->CR_1.RXONLY = 0;

    // enable
    spi->CR_1.SPE = 1;
}




void spi_write(const spi_cfg_t* cfg, uint8_t data)
{
    SPI_regs_t* spi = NULL;
    switch(cfg->spi_num) {
        case SPI_NUM_1: spi = SPI1; break;
        case SPI_NUM_2: spi = SPI2; break;
        case SPI_NUM_3: spi = SPI3; break;
        case SPI_NUM_4: spi = SPI4; break;
        default: return;
    }
    spi->DR.DR = data;
}

void spi_read(const spi_cfg_t* cfg, uint8_t* data)
{
    SPI_regs_t* spi = NULL;
    switch(cfg->spi_num) {
        case SPI_NUM_1: spi = SPI1; break;
        case SPI_NUM_2: spi = SPI2; break;
        case SPI_NUM_3: spi = SPI3; break;
        case SPI_NUM_4: spi = SPI4; break;
        default: return;
    }
    *data = spi->DR.DR;
}



void spi_DMA_enable(spi_num_t spi_num)
{
    SPI_regs_t* spi = NULL;
    switch (spi_num)
    {
        case SPI_NUM_1: spi = SPI1; break;
        case SPI_NUM_2: spi = SPI2; break;
        case SPI_NUM_3: spi = SPI3; break;
        case SPI_NUM_4: spi = SPI4; break;
        default: return;
    }
    spi->CR_2.RXDMAEN = 1; // Enable RX DMA
    spi->CR_2.TXDMAEN = 1; // Enable TX DMA
}

void spi_DMA_disable(spi_num_t spi_num)
{
    SPI_regs_t* spi = NULL;
    switch (spi_num)
    {
        case SPI_NUM_1: spi = SPI1; break;
        case SPI_NUM_2: spi = SPI2; break;
        case SPI_NUM_3: spi = SPI3; break;
        case SPI_NUM_4: spi = SPI4; break;
        default: return;
    }

    spi->CR_2.RXDMAEN = 0; // Disable RX DMA
    spi->CR_2.TXDMAEN = 0; // Disable TX DMA
}