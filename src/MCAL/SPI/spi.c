
#include <MCAL/SPI/spi.h>

void spi_init(const spi_cfg_t* cfg)
{
    SPI_regs_t* spi = cfg->spi;

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




