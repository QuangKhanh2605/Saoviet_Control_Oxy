
#ifndef USER_APP_RS485_H__
#define USER_APP_RS485_H__

#define USING_APP_RS485

#include "user_util.h"
#include "event_driven.h"

#define ID_DEFAULT_OXY          5
#define ID_DEFAULT_PH           3

#define SENSOR_GPIO_Port        MCU_RL7_GPIO_Port
#define SENSOR_Pin              MCU_RL7_Pin

typedef enum
{
    _EVENT_RS485_ENTRY,
    
    _EVENT_RS485_TRANSMIT,
    _EVENT_RS485_RECEIVE_HANDLE,
    _EVENT_RS485_RECEIVE_COMPLETE,
    
    _EVENT_RS485_WAIT_CALIB,
    
    _EVENT_RESET_SENSOR_OXY,
    _EVENT_WAIT_SENSOR_OXY_ON,
    
    _EVENT_RS485_END,
}eKindEventRs485;

typedef enum
{
    _RS485_CALIB_OXY_100_CALIB,
    _RS485_CALIB_SALINITY,
    _RS485_CALIB_TEMP,
    _RS485_CALIB_PH_POINT_686,
    _RS485_CALIB_PH_POINT_918,
    
    _RS485_OPERA_OXY,
    _RS485_OPERA_PH,
}eKindMode485;

typedef struct
{
    uint8_t Oxy;
    uint8_t pH;
}Struct_IdSlave;

typedef struct
{
    uint8_t Trans;
    uint8_t Recv;
}Struct_KindMode485;

extern sEvent_struct        sEventAppRs485[];
extern Struct_IdSlave       sIdSlave485;
extern Struct_KindMode485   sKindMode485;
/*====================Function Handle====================*/

uint8_t    AppRs485_Task(void);
void       Init_AppRs485(void);
void       Save_IdSlave(uint8_t ID_Oxy, uint8_t ID_pH);
void       Init_IdSlave(void);
void       Average_MeasureOxy(uint16_t Oxy_Mg_L, uint16_t Oxy_Percent);

//void       Send_RS458_Normal(uint8_t *aData, uint16_t Length_u16);
void       Send_RS458_Sensor(uint8_t *aData, uint16_t Length_u16);
#endif

