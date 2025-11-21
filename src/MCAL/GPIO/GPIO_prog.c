/*
* During and just after reset, the alternate functions are not active and the I/O ports are 
* configured in input floating mode.
*/

#include "GPIO_interface.h"
#include "GPIO_private.h"

extern const GPIO_CONFG_t GPIO_astrConfig[GPIO_u8PORT_MAX];

ErrorState_t GPIO_enuInit()
{
    ErrorState_t Local_enuErrorState = ES_OK;
    for(uint8_t Local_u8Iter=0; Local_u8Iter<GPIO_u8PORT_MAX; Local_u8Iter++)
    {
        // Local_enuErrorState = 
    }

    return Local_enuErrorState;
}

ErrorState_t GPIO_enuSetPortDir(uint8_t Copyu8PortID, uint8_t Copy_u8Dir)
{
    ErrorState_t Local_enuErrorState = ES_OK;
    
    if((Copyu8PortID < GPIO_u8PORT_MAX))
    {
        switch (Copy_u8Dir)
        {
        case GPIO_INPUT:
            *(GPIO_aptrReg[Copyu8PortID]).MODER = 0X00000000;
            break;
        case GPIO_OUTPUT:
            *(GPIO_aptrReg[Copyu8PortID]).MODER = 0X55555555;
            break;
        case GPIO_ALTFNC:
            *(GPIO_aptrReg[Copyu8PortID]).MODER = 0XAAAAAAAA;
            break;
        case GPIO_ANALOG:
            *(GPIO_aptrReg[Copyu8PortID]).MODER = 0XFFFFFFFF;
            break;
        default:
            Local_enuErrorState = ES_OUTOFRANGE;
            break;
        };
    }
    else
    {
        Local_enuErrorState = ES_NULLPTR;
    }

    return Local_enuErrorState;
}

ErrorState_t GPIO_enuSetPortValue(uint8_t Copyu8PortID, uint8_t Copy_u8Value)
{
    ErrorState_t Local_enuErrorState = ES_OK;
    
    if((Copyu8PortID < GPIO_u8PORT_MAX) && (Copy_u8Value < 65536))
    {
        *(GPIO_aptrReg[Copyu8PortID]).ODR = Copy_u8Value;            
    }
    else
    {
        Local_enuErrorState = ES_OUTOFRANGE;
    }

    return Local_enuErrorState;
}

ErrorState_t GPIO_enuFlipPortValue(uint8_t Copyu8PortID)
{
    ErrorState_t Local_enuErrorState = ES_OK;
    
    if(Copyu8PortID < GPIO_u8PORT_MAX)
    {
        *(GPIO_aptrReg[Copyu8PortID]).ODR ^= 0XFFFFFFFF;          
    }
    else
    {
        Local_enuErrorState = ES_NULLPTR;
    }

    return Local_enuErrorState;
}


ErrorState_t GPIO_enuSetPinDir(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8Dir)
{
    ErrorState_t Local_enuErrorState;

    if((Copyu8PortID < GPIO_u8PORT_MAX) && (Copy_u8Pin < GPIO_u8PIN_MAX))
    {
        Copy_u8Pin *= 2;
        *(GPIO_aptrReg[Copyu8PortID]).MODER &= ~(0b11 << Copy_u8Pin); // clear
        switch (Copy_u8Dir)
        {
            case GPIO_INPUT:
                *(GPIO_aptrReg[Copyu8PortID]).MODER |= GPIO_INPUT << Copy_u8Pin;
                break;
            case GPIO_OUTPUT:
                *(GPIO_aptrReg[Copyu8PortID]).MODER |= GPIO_OUTPUT << Copy_u8Pin;
                break;
            case GPIO_ALTFNC:
                *(GPIO_aptrReg[Copyu8PortID]).MODER |= GPIO_ALTFNC << Copy_u8Pin;
                break;
            case GPIO_ANALOG:
                *(GPIO_aptrReg[Copyu8PortID]).MODER |= GPIO_ANALOG << Copy_u8Pin;
                break;
            default:
                Local_enuErrorState = ES_OUTOFRANGE;
                break;
        }             
    }
    else
    {
        Local_enuErrorState = ES_OUTOFRANGE;
    }

    return Local_enuErrorState;
}

ErrorState_t GPIO_enuSetPinValue(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8Value)
{
    ErrorState_t Local_enuErrorState;

    if((Copyu8PortID < GPIO_u8PORT_MAX) && (Copy_u8Pin < GPIO_u8PIN_MAX) && (Copy_u8Value < GPIO_STATE_MAX))
    {     
        // clearing
        *(GPIO_aptrReg[Copyu8PortID]).ODR &= ~(1 << Copy_u8Pin);
        // setting
        *(GPIO_aptrReg[Copyu8PortID]).ODR |= (Copy_u8Value << Copy_u8Pin);         
    }
    else
    {
        Local_enuErrorState = ES_NULLPTR;
    }

    return Local_enuErrorState;
}

ErrorState_t GPIO_enuFlipPinValue(uint8_t Copyu8PortID, uint8_t Copy_u8Pin)
{
    ErrorState_t Local_enuErrorState = ES_OK;
    
    if((Copyu8PortID < GPIO_u8PORT_MAX) && (Copy_u8Pin < GPIO_u8PIN_MAX))
    {
        *(GPIO_aptrReg[Copyu8PortID]).ODR ^= (1 << Copy_u8Pin);          
    }
    else
    {
        Local_enuErrorState = ES_NULLPTR;
    }

    return Local_enuErrorState;
}

ErrorState_t GPIO_enuGetPinValue(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t *Copy_u8Value)
{
    ErrorState_t Local_enuErrorState = ES_OK;
    
    if((Copyu8PortID < GPIO_u8PORT_MAX) && (Copy_u8Pin < GPIO_u8PIN_MAX))
    {
        uint32_t Local_u32PortValue = *(GPIO_aptrReg[Copyu8PortID]).ODR;
        *Copy_u8Value = (Local_u32PortValue >> Copy_u8Pin) & 1;
    }
    else
    {
        Local_enuErrorState = ES_NULLPTR;
    }

    return Local_enuErrorState;
}

ErrorState_t GPIO_enuSetOutputType(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8Type)
{
    ErrorState_t Local_enuErrorState = ES_OK;
    
    if((Copyu8PortID < GPIO_u8PORT_MAX) && (Copy_u8Pin < GPIO_u8PIN_MAX) && (Copy_u8Type < GOPI_OTYPE_MAX))
    {
        // clear
        *(GPIO_aptrReg[Copyu8PortID]).OTYPER &= ~(1 << Copy_u8Pin);
        // set
        *(GPIO_aptrReg[Copyu8PortID]).OTYPER |= Copy_u8Type << Copy_u8Pin;
    }
    else
    {
        Local_enuErrorState = ES_NULLPTR;
    }

    return Local_enuErrorState;
}

ErrorState_t GPIO_enuSetPinSpeed(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8Speed)
{
    ErrorState_t Local_enuErrorState = ES_OK;
    
    Copy_u8Pin *= 2;
    if((Copyu8PortID < GPIO_u8PORT_MAX) && (Copy_u8Pin < GPIO_u8PIN_MAX) && (Copy_u8Speed < GPIO_OSPEED_MAX))
    {
        // clear
        *(GPIO_aptrReg[Copyu8PortID]).OSPEEDR &= ~(1 << Copy_u8Pin);
        // set
        *(GPIO_aptrReg[Copyu8PortID]).OSPEEDR |= Copy_u8Speed << Copy_u8Pin;
    }
    else
    {
        Local_enuErrorState = ES_NULLPTR;
    }

    return Local_enuErrorState;
}

ErrorState_t GPIO_enuSetPinPUPD(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8PullType)
{
    ErrorState_t Local_enuErrorState = ES_OK;
    
    Copy_u8Pin *= 2;
    if((Copyu8PortID < GPIO_u8PORT_MAX) && (Copy_u8Pin < GPIO_u8PIN_MAX) && (Copy_u8PullType < GPIO_PUPD_MAX))
    {
        // clear
        *(GPIO_aptrReg[Copyu8PortID]).OSPEEDR &= ~(1 << Copy_u8Pin);
        // set
        *(GPIO_aptrReg[Copyu8PortID]).OSPEEDR |= Copy_u8PullType << Copy_u8Pin;
    }
    else
    {
        Local_enuErrorState = ES_NULLPTR;
    }

    return Local_enuErrorState;
}

ErrorState_t GPIO_enuCnfgAltFunc(uint8_t Copyu8PortID, uint8_t Copy_u8Pin, uint8_t Copy_u8AltFunc);