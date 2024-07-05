#ifndef USER_APP_RS485_H_
#define USER_APP_RS485_H_

#define USING_APP_RS485

#include "event_driven.h"
#include "user_util.h"

typedef enum
{
    _EVENT_RS485_ENTRY,
    _EVENT_RS485_TRANSMIT,
    _EVENT_RS485_RECEIVE_HANDLE,
    _EVENT_RS485_RECEIVE_COMPLETE,

    _EVENT_RS485_END,
}eKindEventRs485;

extern sEvent_struct        sEventAppRs485[];
/*=============== Function handle ================*/
uint8_t     AppRs485_Task(void);
void        Init_AppRs485(void);

void        Send_RS458_Sensor(uint8_t *aData, uint16_t Length_u16) ;

#endif

