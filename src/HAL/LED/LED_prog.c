#include "LED_interface.h"

extern const LED_cfg_t LED_astrConfig[LED_NUM];

ErrorState_t LED_enuInit()
{
    ErrorState_t Local_enuErrorState = ES_OK;

    for(uint8_t i=0; i<LED_NUM; i++)
    {
        Local_enuErrorState = GPIO_enuSetPinDir(LED_astrConfig[i].LED_PORT, LED_astrConfig[i].LED_PIN, GPIO_OUTPUT);
        if(LED_astrConfig[i].LED_isPP < GOPI_OTYPE_MAX)
        {
            // Local_enuErrorState |= gpioenu
        }
        else
        {
            Local_enuErrorState = ES_OUTOFRANGE;
        }
    }

    return Local_enuErrorState;
}

ErrorState_t LED_enuTurnON(uint8_t Copy_u8LedName);

ErrorState_t LED_enuTurnOFF(uint8_t Copy_u8LedName);

ErrorState_t LED_enuToggle(uint8_t Copy_u8LedName);