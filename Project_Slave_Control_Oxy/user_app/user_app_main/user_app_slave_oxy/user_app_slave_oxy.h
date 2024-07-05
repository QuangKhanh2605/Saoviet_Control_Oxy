
#ifndef USER_APP_SLAVE_OXY_H__
#define USER_APP_SLAVE_OXY_H__

#define USING_APP_SLAVE_OXY

#include "event_driven.h"
#include "user_util.h"

#define DEFAULT_BAUDRATE_SLAVE      9600

#define DEFAULT_TIMERUN_SLAVE       66
#define DEFAULT_TIMEFREE_SLAVE      5

#define TIMECYCLE_SLAVE_MAX         999
#define TIMECYCLE_SLAVE_MIN         1

#define ADDR_FLASH_BAUDRATE         0x08000000 + 1024*30
#define ADDR_FLASH_TIMECYCLE        0x08000000 + 1024*31

#define RELAY_OXY_L_GPIO            K2_GPIO_Port
#define RELAY_OXY_L_PIN             K2_Pin

#define RELAY_OXY_R_GPIO            K1_GPIO_Port
#define RELAY_OXY_R_PIN             K1_Pin

#define STATE_GPIO_OFF              GPIO_PIN_RESET
#define STATE_GPIO_ON               GPIO_PIN_SET

typedef enum
{
    _EVENT_SLAVE_OXY_ENTRY,
    _EVENT_SLAVE_OXY_IWDG,
    
    _EVENT_SLAVE_OXY_END,
}eKindEventSlaveOxy;

typedef enum
{
    _OXY_FREE=0,
    _OXY_RUN,
    _OXY_OFF,
}eKindStateOxy;

typedef enum
{
    _OXY_L_RUN,
    _OXY_R_RUN,
}eKindOxyRun;

typedef struct
{
    uint8_t TimeRun;            //Thoi gian chay (Parameter)
    uint8_t TimeFree;           //Thoi gian nghi (Parameter)
    uint32_t Baudrate;          //Baudrate 
}Struct_ParamSlaveOxy;

typedef struct
{
    uint16_t TimeRun;           //Thoi gian chay (dieu khien)
    uint16_t TimeFree;          //Thoi gian nghi (dieu khien)
    uint8_t StateCtrl;          //Trang thai dieu khien
    uint8_t KindOxyRun;         //Oxy dang hoat dong (left or right)
}Struct_CtrlOxy;

extern sEvent_struct               sEventAppSlaveOxy[];
extern Struct_ParamSlaveOxy        sParamSlave;
extern Struct_CtrlOxy              sCtrlOxy;
/*================= Function Handle ================*/
uint8_t     AppSlaveOxy_Task(void);
void        Init_AppSlaveOxy(void);

void        Ctrl_Slave_Oxy(void);


uint8_t     Save_TimeCycleSlave(uint16_t TimeRun, uint16_t TimeFree);
void        Init_TimeCycleSlave(void);

uint8_t     Save_BaudrateSlave(uint32_t Baudrate);
void        Init_BaudrateSlave(void);

void        Init_UartBaudrate(uint32_t Baudrate);

#endif

