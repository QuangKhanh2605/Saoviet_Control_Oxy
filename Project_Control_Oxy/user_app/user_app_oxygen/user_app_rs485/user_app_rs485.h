
#ifndef USER_APP_RS485_H__
#define USER_APP_RS485_H__

#define USING_APP_RS485

#include "user_util.h"
#include "event_driven.h"

#define TIME_INIT_UART      10*60000

#define ID_DEFAULT          0x01

typedef enum
{
    _EVENT_RS485_ENTRY,
    _EVENT_RS485_TRANSMIT,
    _EVENT_RS485_RECEIVE_HANDLE,
    _EVENT_RS485_RECEIVE_COMPLETE,
    _EVENT_RS485_INIT_UART,
    
    _EVENT_RS485_END,
}eKindEventRs485;

typedef enum
{
    _TRANSMIT_CALIB_OXY_100_CALIB,
    _TRANSMIT_CALIB_SALINITY,
    _TRANSMIT_CALIB_TEMP,
    
    _TRANSMIT_RESET_CALIB,
    
    _TRANSMIT_OPERA,
}eKindTransmit;

typedef enum
{
    _TRANSMIT_RESET_0,
    _TRANSMIT_RESET_100,
    _TRANSMIT_RESET_TEMPERATURE,
    
    _TRANSMIT_RESET_END,
}eKindReset;

extern sEvent_struct    sEventAppRs485[];
extern uint8_t          ID_Slave;
extern uint8_t         KindTrans485;
/*====================Function Handle====================*/

uint8_t    AppRs485_Task(void);
void       Init_AppRs485(void);
void       Save_IdSlave(uint8_t ID);
void       Init_IdSlave(void);

#endif

