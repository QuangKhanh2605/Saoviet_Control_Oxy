
#ifndef USER_APP_RS485_SLAVE_H__
#define USER_APP_RS485_SLAVE_H__

#define USING_APP_RS485_SLAVE

#include "user_util.h"
#include "event_driven.h"

#define NUMBER_SLAVE_MOTOR_MAX     8 
#define NUMBER_SLAVE_DEFAULT       2

#define NUMBER_CHECK_DISCONNECT_SLAVE 5

typedef enum
{
    _EVENT_RS485_SLAVE_ENTRY,
    
    _EVENT_RS485_SLAVE_HANDLE_COMPLETE,
    _EVENT_RS485_SLAVE_TRANSMIT,
    
    _EVENT_RS485_SLAVE_DEINIT_UART,
    _EVENT_RS485_SEND_SLAVE,
    
    _EVENT_RS485_SLAVE_END,
}eKindEventRs485Slave;

typedef struct
{
    uint8_t Trans;
    uint8_t Recv;
}Struct_KindMode485Slave;

typedef enum
{
    _RS485_USER_SLAVE,
    _RS485_USER_DEBUG,
}eKindUserRs485Slave;

typedef struct
{
    uint8_t State_Machine;
    uint8_t Stata_OnOff;
    uint8_t Error_Amperage;
    uint8_t Error_Pressure;
    uint8_t Count_DisConnect;
}StructStateMotor;

extern sEvent_struct        sEventAppRs485Slave[];
extern uint8_t              Flag_User_Rs485Slave;
extern uint16_t             Number_Motor_Slave;
extern StructStateMotor           sStateSlaveMotor[];
/*====================Function Handle====================*/

uint8_t    AppRs485Slave_Task(void);
void       Init_AppRs485Slave(void);
uint8_t    Calculator_Crc_U16(uint16_t *crc, uint8_t* buf, uint16_t len);

void       Send_RS458_Normal(uint8_t *aData, uint16_t Length_u16);
void       Send_RS458_Slave(uint8_t *aData, uint16_t Length_u16);
void       Send_Slave_Crc(uint8_t aData[], uint16_t Length);

void       Save_NumberMotorSlave(uint16_t Number);
void       Init_NumberMotorSlave(void);

#endif

