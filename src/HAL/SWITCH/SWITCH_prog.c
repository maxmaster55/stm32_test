
#include "SWITCH_interface.h"

extern SW_Congig_t SW_astrConfig[SWITHC_NUM];

ErrorState_t SW_enuInit()
{
    ErrorState_t Local_enuErrorState = ES_OK;

    for(uint8_t i=0; i<SWITHC_NUM; i++)
    {
        Local_enuErrorState = GPIO_enuSetPinDir(SW_astrConfig[i].SW_u8Port, SW_astrConfig[i].SW_u8Pin, GPIO_INPUT);

        if(SW_astrConfig[i].SW_u8ResConfig < GPIO_PUPD_NUM)
        {
            Local_enuErrorState |= GPIO_enuEnablePinPUPD(SW_astrConfig[i].SW_u8Port, SW_astrConfig[i].SW_u8Pin, SW_astrConfig[i].SW_u8ResConfig);
        }
        else if(SW_astrConfig[i].SW_u8ResConfig < SW_RESCNFG_NUM)
        {
            // external pullup/pulldown
        }
        else
        {
            Local_enuErrorState = ES_OUTOFRANGE;
        }
    }

    return Local_enuErrorState;
}

ErrorState_t SW_enuReadState(uint8_t Copy_u8SwitchName, uint8_t *Copy_u8Value)
{
    ErrorState_t Local_enuErrorState = ES_OK;

    if(Copy_u8SwitchName < SWITHC_NUM)
    {
        Local_enuErrorState = GPIO_enuGetPinValue(SW_astrConfig[Copy_u8SwitchName].SW_u8Port, SW_astrConfig[Copy_u8SwitchName].SW_u8Pin, Copy_u8Value);
        // delay to overcome debouncing
        for(uint32_t i=0; i<500000; i++);
        Local_enuErrorState = GPIO_enuGetPinValue(SW_astrConfig[Copy_u8SwitchName].SW_u8Port, SW_astrConfig[Copy_u8SwitchName].SW_u8Pin, Copy_u8Value);
    }
    else
    {
        Local_enuErrorState = ES_OUTOFRANGE;
    }

    return Local_enuErrorState;
}