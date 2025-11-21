
#include "RCC_interface.h"

ErrorState_t RCC_enuEnableClk(uint8_t Copy_u8Clk)
{
    ErrorState_t Local_enuErrorState_t = ES_OK;
    uint8_t time_out = 100;

    switch (Copy_u8Clk)
    {
        case RCC_HSI:
            RCC.CR.BITS.HSION = RCC_CLK_SET;
            while ((RCC.CR.BITS.HSIRDY == RCC_CLK_NOT_RDY) && time_out--);
            break;
        case RCC_HSE:
            RCC.CR.BITS.HSEON = RCC_CLK_SET;
            while ((RCC.CR.BITS.HSERDY == RCC_CLK_NOT_RDY) && time_out--);
            break;
        case RCC_PLL:
            RCC.CR.BITS.PLLON = RCC_CLK_SET;
            while ((RCC.CR.BITS.PLLRDY == RCC_CLK_NOT_RDY) && time_out--);
            break;
        case RCC_PLL_I2S:
            RCC.CR.BITS.PLLI2SON = RCC_CLK_SET;
            while ((RCC.CR.BITS.PLLI2SRDY == RCC_CLK_NOT_RDY) && time_out--);
            break;
        default:
            Local_enuErrorState_t = ES_OUTOFRANGE;
            break;
    }

    if(time_out < 0)
    {
        Local_enuErrorState_t = ES_TIMEOUT;
    }

    return Local_enuErrorState_t;
}

ErrorState_t RCC_enuDisableClk(uint8_t Copy_u8Clk)
{ 
    ErrorState_t Local_enuErrorState_t = ES_OK;
    
    if((RCC.CFGR.BITS.SW == Copy_u8Clk) || (RCC.PLLCFGR.BITS.PLLSRC == Copy_u8Clk))
    {
        Local_enuErrorState_t = ES_USERMISCONFIG;
    }
    else
    {
        switch (Copy_u8Clk)
        {
            case RCC_HSI:
                RCC.CR.BITS.HSION = RCC_CLK_RESET;
                break;
            case RCC_HSE:
                RCC.CR.BITS.HSEON = RCC_CLK_RESET;
                break;
            case RCC_PLL:
                RCC.CR.BITS.PLLON = RCC_CLK_RESET;
                break;
            case RCC_PLL_I2S:
                RCC.CR.BITS.PLLI2SON = RCC_CLK_RESET;
                break;
            default:
                Local_enuErrorState_t = ES_OUTOFRANGE;
                break;
        }
    }

    return Local_enuErrorState_t;
}

ErrorState_t RCC_enuConfigurePLL(RCC_PLLCFGR *Copy_uniPLLConfig)
{
    ErrorState_t Local_enuErrorState_t = ES_OK;
    uint16_t clock_value = 0;

    if(RCC_PLL == RCC.CFGR.BITS.SW)
    {
        // as long as RCC_PLL is SYS_CLK, either HSI, or HSE must be enabled
        if(RCC_CLK_RESET == RCC.CR.BITS.HSION)
            Local_enuErrorState_t = RCC_enuEnableClk(RCC_HSI);
        // now, HSI is the source of SYS_CLK
        Local_enuErrorState_t |= RCC_enuSetSysClk(RCC_HSI);
    }  

    // disable PLL_CLK output signal
    Local_enuErrorState_t |= RCC_enuDisableClk(RCC_PLL);

    // enabling PLL_CLK_SRC
    Local_enuErrorState_t |= RCC_enuEnableClk(Copy_uniPLLConfig->BITS.PLLSRC);

    // configure PLL_CLK
    clock_value = RCC.PLLCFGR.BITS.PLLSRC / RCC.PLLCFGR.BITS.PLLM;
    if(clock_value > 2)
    {
        Local_enuErrorState_t = ES_USERMISCONFIG;
    }

    clock_value *= RCC.PLLCFGR.BITS.PLLN;
    if((clock_value < 192) || (clock_value > 432))
    {
        Local_enuErrorState_t = ES_USERMISCONFIG;
    }
    
    if(clock_value/RCC.PLLCFGR.BITS.PLLP > 84)
    {
        Local_enuErrorState_t = ES_USERMISCONFIG;
    }

    if(clock_value/RCC.PLLCFGR.BITS.PLLQ > 48)
    {
        Local_enuErrorState_t = ES_USERMISCONFIG;
    }

    return Local_enuErrorState_t;
}

ErrorState_t RCC_enuSetSysClk(uint8_t Copy_u8Clk)
{
    ErrorState_t Local_enuErrorState_t = ES_OK;
    uint8_t time_out = 0;

    switch (Copy_u8Clk)
    {
    case RCC_HSI:
        RCC.CFGR.BITS.SW = RCC_HSI;
        while((RCC.CFGR.BITS.SWS != RCC_HSI) && time_out--);
        break;
    case RCC_HSE:
        RCC.CFGR.BITS.SW = RCC_HSE;
        while ((RCC.CFGR.BITS.SW != RCC_HSE) && time_out--);
        break;
    case RCC_PLL:
        RCC.CFGR.BITS.SW = RCC_PLL;
        while ((RCC.CFGR.BITS.SW != RCC_PLL) && time_out--);
        break;
    default:
        Local_enuErrorState_t = ES_OUTOFRANGE;
        break;
    }

    if(time_out < 0)
    {
        Local_enuErrorState_t = ES_TIMEOUT;
    }

    return Local_enuErrorState_t;
}

ErrorState_t RCC_enuSetAHB_Clk(uint8_t Copy_u8Presc);

ErrorState_t RCC_enuSetAPB_Clk(uint8_t Copy_u8Presc);

ErrorState_t RCC_enuEnablePeripheralClk(uint8_t Copy_u8Bus, uint8_t Copy_u8Peripheral)
{
    ErrorState_t Local_enuErrorState_t = ES_OK;
    if(AHB1 == Copy_u8Bus)
    {
        switch (Copy_u8Peripheral)
        {
        case GPIOA:
            RCC.AHB1ENR.BITS.GPIOAEN = RCC_PERIPH_ENABLE;
            break;
        case GPIOB:
            RCC.AHB1ENR.BITS.GPIOBEN = RCC_PERIPH_ENABLE;
            break;
        case GPIOC:
            RCC.AHB1ENR.BITS.GPIOCEN = RCC_PERIPH_ENABLE;
            break;
        case GPIOD:
            RCC.AHB1ENR.BITS.GPIODEN = RCC_PERIPH_ENABLE;
        case GPIOE:
            RCC.AHB1ENR.BITS.GPIOAEN = RCC_PERIPH_ENABLE;
            break;
        case GPIOH:
            RCC.AHB1ENR.BITS.GPIOHEN = RCC_PERIPH_ENABLE;
            break;
        case CRC:
            RCC.AHB1ENR.BITS.CRCEN = RCC_PERIPH_ENABLE;
            break;
        case DMA1:
            RCC.AHB1ENR.BITS.DMA1EN = RCC_PERIPH_ENABLE;
            break;
        case DMA2:
            RCC.AHB1ENR.BITS.DMA2EN = RCC_PERIPH_ENABLE;
            break;
        default:
            Local_enuErrorState_t = ES_OUTOFRANGE;
            break;
        }
    }
    else if(AHB2 == Copy_u8Bus)
    {
        switch (Copy_u8Peripheral)
        {
        case OTGFS:
            RCC.AHB2ENR.BITS.OTGFSEN = RCC_PERIPH_ENABLE;
            break;
        default:
            Local_enuErrorState_t = ES_OUTOFRANGE;
            break;
        }
    }
    else if(APB1 == Copy_u8Bus)
    {
        switch (Copy_u8Peripheral)
        {
        case TIM2:
            RCC.APB1ENR.BITS.TIM2EN = RCC_PERIPH_ENABLE;
            break;
        case TIM3:
            RCC.APB1ENR.BITS.TIM3EN = RCC_PERIPH_ENABLE;
            break;
        case TIM4:
            RCC.APB1ENR.BITS.TIM4EN = RCC_PERIPH_ENABLE;
            break;
        case TIM5:
            RCC.APB1ENR.BITS.TIM5EN = RCC_PERIPH_ENABLE;
            break;
        case WWDG:
            RCC.APB1ENR.BITS.WWDGEN = RCC_PERIPH_ENABLE;
            break;
        case SPI2:
            RCC.APB1ENR.BITS.SPI2EN = RCC_PERIPH_ENABLE;
            break;
        case SPI3:
            RCC.APB1ENR.BITS.SPI3EN = RCC_PERIPH_ENABLE;
            break;
        case USART2:
            RCC.APB1ENR.BITS.USART2EN = RCC_PERIPH_ENABLE;
            break;
        case I2C1:
            RCC.APB1ENR.BITS.I2C1EN = RCC_PERIPH_ENABLE;
            break;
        case I2C2:
            RCC.APB1ENR.BITS.I2C2EN = RCC_PERIPH_ENABLE;
            break;
        case I2C3:
            RCC.APB1ENR.BITS.I2C3EN = RCC_PERIPH_ENABLE;
            break;
        case PWR:
            RCC.APB1ENR.BITS.PWREN = RCC_PERIPH_ENABLE;
            break;
        default:
            Local_enuErrorState_t = ES_OUTOFRANGE;
            break;
        }
    }
    else if(APB2 == Copy_u8Bus)
    {
        switch (Copy_u8Peripheral)
        {
        case TIM1:
            RCC.APB2ENR.BITS.TIM10EN = RCC_PERIPH_ENABLE;
            break;
        case USART1:
            RCC.APB2ENR.BITS.USART1EN = RCC_PERIPH_ENABLE;
            break;
        case USART6:
            RCC.APB2ENR.BITS.USART6EN = RCC_PERIPH_ENABLE;
            break;
        case ADC1:
            RCC.APB2ENR.BITS.ADC1EN = RCC_PERIPH_ENABLE;
            break;
        case SDIO:
            RCC.APB2ENR.BITS.SDIOEN = RCC_PERIPH_ENABLE;
            break;
        case SPI1:
            RCC.APB2ENR.BITS.SPI1EN = RCC_PERIPH_ENABLE;
            break;
        case SPI4:
            RCC.APB2ENR.BITS.SPI4EN = RCC_PERIPH_ENABLE;
            break;
        case SYSCFG:
            RCC.APB2ENR.BITS.SYSCFGEN = RCC_PERIPH_ENABLE;
            break;
        case TIM9:
            RCC.APB2ENR.BITS.TIM9EN = RCC_PERIPH_ENABLE;
            break;
        case TIM10:
            RCC.APB2ENR.BITS.TIM10EN = RCC_PERIPH_ENABLE;
            break;
        case TIM11:
            RCC.APB2ENR.BITS.TIM11EN = RCC_PERIPH_ENABLE;
            break;
        default:
            Local_enuErrorState_t = ES_OUTOFRANGE;
            break;
        }
    }

    return Local_enuErrorState_t;
}

ErrorState_t RCC_enuDisablePeripheralClk(uint8_t Copy_u8Bus, uint8_t Copy_u8Peripheral)
{
    ErrorState_t Local_enuErrorState_t = ES_OK;
    if(AHB1 == Copy_u8Bus)
    {
        switch (Copy_u8Peripheral)
        {
        case GPIOA:
            RCC.AHB1ENR.BITS.GPIOAEN = RCC_PERIPH_DISABLE;
            break;
        case GPIOB:
            RCC.AHB1ENR.BITS.GPIOBEN = RCC_PERIPH_DISABLE;
            break;
        case GPIOC:
            RCC.AHB1ENR.BITS.GPIOCEN = RCC_PERIPH_DISABLE;
            break;
        case GPIOD:
            RCC.AHB1ENR.BITS.GPIODEN = RCC_PERIPH_DISABLE;
        case GPIOE:
            RCC.AHB1ENR.BITS.GPIOAEN = RCC_PERIPH_DISABLE;
            break;
        case GPIOH:
            RCC.AHB1ENR.BITS.GPIOHEN = RCC_PERIPH_DISABLE;
            break;
        case CRC:
            RCC.AHB1ENR.BITS.CRCEN = RCC_PERIPH_DISABLE;
            break;
        case DMA1:
            RCC.AHB1ENR.BITS.DMA1EN = RCC_PERIPH_DISABLE;
            break;
        case DMA2:
            RCC.AHB1ENR.BITS.DMA2EN = RCC_PERIPH_DISABLE;
            break;
        default:
            Local_enuErrorState_t = ES_OUTOFRANGE;
            break;
        }
    }
    else if(AHB2 == Copy_u8Bus)
    {
        switch (Copy_u8Peripheral)
        {
        case OTGFS:
            RCC.AHB2ENR.BITS.OTGFSEN = RCC_PERIPH_DISABLE;
            break;
        default:
            Local_enuErrorState_t = ES_OUTOFRANGE;
            break;
        }
    }
    else if(APB1 == Copy_u8Bus)
    {
        switch (Copy_u8Peripheral)
        {
        case TIM2:
            RCC.APB1ENR.BITS.TIM2EN = RCC_PERIPH_DISABLE;
            break;
        case TIM3:
            RCC.APB1ENR.BITS.TIM3EN = RCC_PERIPH_DISABLE;
            break;
        case TIM4:
            RCC.APB1ENR.BITS.TIM4EN = RCC_PERIPH_DISABLE;
            break;
        case TIM5:
            RCC.APB1ENR.BITS.TIM5EN = RCC_PERIPH_DISABLE;
            break;
        case WWDG:
            RCC.APB1ENR.BITS.WWDGEN = RCC_PERIPH_DISABLE;
            break;
        case SPI2:
            RCC.APB1ENR.BITS.SPI2EN = RCC_PERIPH_DISABLE;
            break;
        case SPI3:
            RCC.APB1ENR.BITS.SPI3EN = RCC_PERIPH_DISABLE;
            break;
        case USART2:
            RCC.APB1ENR.BITS.USART2EN = RCC_PERIPH_DISABLE;
            break;
        case I2C1:
            RCC.APB1ENR.BITS.I2C1EN = RCC_PERIPH_DISABLE;
            break;
        case I2C2:
            RCC.APB1ENR.BITS.I2C2EN = RCC_PERIPH_DISABLE;
            break;
        case I2C3:
            RCC.APB1ENR.BITS.I2C3EN = RCC_PERIPH_DISABLE;
            break;
        case PWR:
            RCC.APB1ENR.BITS.PWREN = RCC_PERIPH_DISABLE;
            break;
        default:
            Local_enuErrorState_t = ES_OUTOFRANGE;
            break;
        }
    }
    else if(APB2 == Copy_u8Bus)
    {
        switch (Copy_u8Peripheral)
        {
        case TIM1:
            RCC.APB2ENR.BITS.TIM10EN = RCC_PERIPH_DISABLE;
            break;
        case USART1:
            RCC.APB2ENR.BITS.USART1EN = RCC_PERIPH_DISABLE;
            break;
        case USART6:
            RCC.APB2ENR.BITS.USART6EN = RCC_PERIPH_DISABLE;
            break;
        case ADC1:
            RCC.APB2ENR.BITS.ADC1EN = RCC_PERIPH_DISABLE;
            break;
        case SDIO:
            RCC.APB2ENR.BITS.SDIOEN = RCC_PERIPH_DISABLE;
            break;
        case SPI1:
            RCC.APB2ENR.BITS.SPI1EN = RCC_PERIPH_DISABLE;
            break;
        case SPI4:
            RCC.APB2ENR.BITS.SPI4EN = RCC_PERIPH_DISABLE;
            break;
        case SYSCFG:
            RCC.APB2ENR.BITS.SYSCFGEN = RCC_PERIPH_DISABLE;
            break;
        case TIM9:
            RCC.APB2ENR.BITS.TIM9EN = RCC_PERIPH_DISABLE;
            break;
        case TIM10:
            RCC.APB2ENR.BITS.TIM10EN = RCC_PERIPH_DISABLE;
            break;
        case TIM11:
            RCC.APB2ENR.BITS.TIM11EN = RCC_PERIPH_DISABLE;
            break;
        default:
            Local_enuErrorState_t = ES_OUTOFRANGE;
            break;
        }
    }
    
    return Local_enuErrorState_t;
}
