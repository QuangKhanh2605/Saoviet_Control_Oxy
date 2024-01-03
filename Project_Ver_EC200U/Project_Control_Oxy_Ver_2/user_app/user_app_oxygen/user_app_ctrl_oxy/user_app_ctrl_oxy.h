
#ifndef USER_APP_CTRL_OXY__
#define USER_APP_CTRL_OXY__

#define USING_APP_CTRL_OXY

#include "event_driven.h"
#include "user_util.h"

#define NUMBER_OXY          4

#define OXY_UPPER_W_DEFAULT   1300
#define OXY_LOWER_W_DEFAULT   550
#define OXY_UPPER_W_MAX       3000
#define OXY_LOWER_W_MIN       400

#define OXY_UPPER_DEFAULT   850
#define OXY_LOWER_DEFAULT   650
#define OXY_UPPER_MAX       3000
#define OXY_LOWER_MIN       450
#define OXY_CYCLE_UP_LOW    10

#define TIME_MAX            1000
#define TIME_MIN            2


#define TIMECHANGE_DEFAULT  15
#define TIMEWARNING_DEFAULT 15
#define TIMEWARNING_MAX     1000
#define TIMEWARNING_MIN     1

#define TIMEDELAY_DEFAULT   1
#define TIMEDELAY_MAX       1000
#define TIMEDELAY_MIN       1

#define SALINITY_MAX        5000
#define SALINITY_MIN        0

#define TEMPERATURE_MAX     5000
#define TEMPERATURE_MIN     0

#define RUN_OXY_DEFAULT     66
#define FREE_OXY_DEFAULT    6

#define FREQ_SEND_NOTIFY    1

#define MAX_NUMBER_USER_SIM 5

#define TIME_RESEND_WARNING 1

typedef enum
{
    _EVENT_CTRL_OXY_ENTRY,
    _EVENT_ENTRY_TSVH,
    _EVENT_CTRL_OXY_IWDG,
    _EVENT_SEND_SLAVE_CYCLE,
    _EVENT_CTRL_OXY_WAIT_CALIB,
    
    _EVENT_CTRL_OXY_LOG_TSVH,
//    _EVENT_CTRL_OXY_SW_NOTIFY,
    _EVENT_CTRL_OXY_PW_DETECT,
    _EVENT_CTRL_OXY_STATE_SLAVE,
    _EVENT_CTRL_OXY_WARNING,
//    _EVENT_CTRL_OXY_FREQ_NOTIFY,
    _EVENT_CTRL_OXY_RESET_DCU,
    _EVENT_CTRL_OXY_SW_MAN_AUTO,
    
    _EVENT_CTRL_OXY_LOG_EEPROM,
    _EVENT_SEND_LOG_EEPROM,
//    _EVENT_TEST_EEPROM,
    
    _EVENT_CTRL_OXY_END,
}eKindEventCtrlOxy;

typedef enum
{
    _ENTER_RUN,
    _ENTER_SETUP,
    _ENTER_SETUP_RUN,
    _ENTER_SETUP_FREE,
    
    _ENTER_END,
}eKindStateButtonEnter;

typedef enum
{
    _OFF_OXY = 0,
    _ON_OXY,
}eKindStateOxy;

typedef enum
{
    _POWER_OFF,
    _POWER_ON,
}eKindStatePower;

typedef enum
{
    _OXY_1,
    _OXY_2,
    _OXY_3,
    _OXY_4,
    
    _OXY_END,
}eKindNumberOxy;

typedef enum
{
    _MACHINE_MAN,
    _MACHINE_AUTO,
    _MACHINE_OFF,
}eKindStateMachine;

typedef enum
{
    _OXY_DISCONNECT,
    _OXY_CONNECT,
}eKindStateSensorOxy;

typedef enum
{
    _OXY_LOW,
    _OXY_HIGH,
    _OXY_BALANCE,
}eKindStateOxyLow;

typedef enum
{
    _CALIB_100,
    _CALIB_SALINITY,
    _CALIB_TEMPERATURE,
}eKindCalibOxy;

typedef enum
{
    _RESPOND_NOTIFY_OXY_LOW = 1,            //Thong bao luong oxy thap
    _RESPOND_NOTIFY_POWER_OFF,              //Thong bao mat dien 
    _RESPOND_NOTIFY_POWER_ON,               //Thong bao co dien
    _RESPOND_NOTIFY_SLAVE_DISCONNECT,       //Thong bao mat ket noi slave
    _RESPOND_NOTIFY_SLAVE_CONNECT,          //Thong bao co ket noi slave
}eKindRespondNotify;

typedef struct
{
    uint16_t RunCtrl;               //Thoi gian chay Oxy nap vao Slave
    uint16_t FreeCtrl;              //Thoi gian nghi Oxy nap vao Slave   
}Struct_TimeSlave;

typedef struct
{
    uint8_t StateDCU;               //Trang thai DCU san sang gui TSVH
    uint8_t StateMachine;           //Trang thai Auto hoac Manual
    uint8_t StateSensorPH;          //Trang thai ket noi vs Sensor PH
    uint8_t StateSensorOxy;         //Trang thai ket noi vs Sensor OXY
    uint8_t StatePower;             //Trang thai nguon cap
    uint8_t StateOxyLower;          //Trang thai canh bao Oxy thap
    uint8_t StateSendOpera;
    
    uint8_t *aStateOxy;             //Trang thai Oxy
}Struct_StateCtrlOxy;

typedef struct
{
    uint16_t Oxy_Upper;             //Nguong tren dieu khien oxy
    uint16_t Oxy_Lower;             //Nguong duoi dieu khien oxy
    uint16_t TimeDelay;             //Thoi gian wait giua cac Oxy
    uint16_t TimeChange;            //Thoi gian thay doi Oxy
    uint16_t Oxy_Upper_Warning;     //Nguong tren canh bao
    uint16_t Oxy_Lower_Warning;     //Nguong duoi canh bao
}Struct_ParamCtrlOxy;

typedef struct
{
    uint16_t Temp;                  //Nhiet Do Sensor
    uint16_t Salinity;              //Do man Sensor
    uint16_t Oxy_Mg_L;              //Oxy Mg L Sensor
    uint16_t Oxy_Percent;           //Oxy % Sensor
    uint16_t pH_Water;
}Struct_ParamMeasure;

typedef struct
{
    uint16_t Addr_Oxy_Log_Start;
    uint16_t Addr_Oxy_Log_End;
    uint16_t KindSendLog;
    uint16_t SizePage;
    uint16_t SizeRecord;
}Struct_OxyLogEEPROM;

typedef struct
{
    uint32_t GettickMs;
    uint32_t Oxy_Delay;
    uint32_t Oxy_Change;
}Struct_GettickMs;

extern sEvent_struct                    sEventAppCtrlOxy[];
extern Struct_TimeSlave                 sTimeSlave;
extern Struct_ParamMeasure              sParamMeasure;
extern Struct_StateCtrlOxy              sStateCtrlOxy;
extern Struct_ParamCtrlOxy              sParamCtrlOxy;
extern Struct_OxyLogEEPROM              sOxyLogEEPROM;
extern Struct_GettickMs                 sGettickMs;
/*================= Function Handle ================*/
uint8_t     AppCtrlOxy_Task(void);
void        Init_AppCtrlOxy(void);

void        Save_TimeCtrlOxy(uint16_t TimeDelay, uint16_t TimeChange);
void        Init_TimeCtrlOxy(void);

void        Save_StateOxy(void);
void        Init_StateOxy(void);

void        Init_FlagDCU(void);
void        Save_GetTickMs(void);
void        Init_GetTickMs(void);

void        AppCtrlOxy_Log_Data_TSVH (void);
uint8_t     AppCtrlOxy_Packet_TSVH (uint8_t *pData);

void        Init_QueueCtrlOxy(void);
uint8_t     QueueRun_To_QueueFree(void);
uint8_t     QueueFree_To_QueueRun(void);
uint8_t     QueueRun_Change_QueueFree(void);

void        Save_TimeSlave(uint16_t RunCtrl, uint16_t FreeCtrl);
void        Init_TimeSlave(void);

void        Save_OxyUpperLower(uint16_t Upper, uint16_t Lower, uint16_t Upper_Warning, uint16_t Lower_Warning);
void        Init_OxyUpperLower(void);

void        Handle_Machine_Auto(void);
void        Handle_Machine_Manual(void);

void        Control_Oxy(uint8_t KindOxy, uint8_t State);
void        Reset_CountFreeOxy(void);
void        Change_State_Oxy(void);
void        OnOff_Warning(uint8_t State);
void        AppCtrlOxy_Packet_Data (uint8_t *pTarget, uint16_t *LenTarget, uint8_t Obis, 
                             void *pData, uint8_t LenData, uint8_t Scale);
uint32_t    Calculator_Scale(uint8_t Scale);
uint32_t    Calculator_Value_Scale(uint32_t Value, uint8_t Scale, uint8_t Scale_Default);

void        Compensation_Salinity(uint16_t *Oxy_Concentration, uint16_t Sal_x100, uint16_t Temp_x100);
//void        DCU_Respond_ID_Server(uint32_t ID_Server);
void        DCU_Respond_Debug(uint8_t *data, uint16_t length);

uint8_t     Calculator_Addr_Read_Write_EEPROM(uint16_t CountRec, uint16_t *AddrRec);
void        Log_EventWarnig(uint8_t Obis, uint8_t State);
#endif

