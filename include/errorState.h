#ifndef ERROR_STATE_H
#define ERROR_STATE_H

typedef enum
{
    ES_OK,
    ES_NOK,
    ES_OUTOFRANGE,
    ES_NULLPTR,
    ES_TIMEOUT,
    ES_USERMISCONFIG,
}ErrorState_t;

#endif // ERROR_STATE_H