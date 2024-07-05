#ifndef USER_APP_RS485_H_
#define USER_APP_RS485_H_

#define USING_APP_RS485

#include "event_driven.h"
#include "user_util.h"

#define SLAVE_ID_DEFAULT        1
#define SLAVE_ID_MAX            8

typedef enum
{
    _EVENT_RS485_ENTRY,
    _EVENT_RS485_TRANSMIT,
    _EVENT_RS485_RECEIVE_HANDLE,
    _EVENT_RS485_RECEIVE_COMPLETE,
    
    _EVENT_RS485_CHECK_CONNECT,
    _EVENT_RS485_DEINIT_UART,

    _EVENT_RS485_END,
}eKindEventRs485;

typedef enum
{
    _RS485_MOTOR_OFF,
    _RS485_MOTOR_ON,
    _RS485_MOTOR_FAIL,
}eKindControlMotor;

typedef struct
{
    uint16_t ID_Slave;
    uint8_t  State_OnOff_Motor;
}structInforRs485;

extern sEvent_struct        sEventAppRs485[];
extern structInforRs485     sInforRs485;
/*=============== Function handle ================*/
uint8_t     AppRs485_Task(void);
void        Init_AppRs485(void);

void        Send_RS458_Sensor(uint8_t *aData, uint16_t Length_u16) ;
void        Init_IDSlave(void);
void        Save_IDSlave(uint16_t ID);
#endif

