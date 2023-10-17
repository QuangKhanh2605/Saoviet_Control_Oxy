
#ifndef USER_APP_RS485_H__
#define USER_APP_RS485_H__

#define USING_APP_RS485

#include "user_util.h"
#include "event_driven.h"

typedef enum
{
    _EVENT_RS485_ENTRY,
    _EVENT_RS485_TRANSMIT,
    _EVENT_RS485_RECEIVE_HANDLE,
    _EVENT_RS485_RECEIVE_COMPLETE,
    
    _EVENT_RS485_END,
}eKindEventRs485;

extern sEvent_struct    sEventAppRs485[];

/*====================Function Handle====================*/

uint8_t    AppRs485_Task(void);


#endif

