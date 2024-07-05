

#ifndef USER_APP_CTRL_PH_H_
#define USER_APP_CTRL_PH_H_

#define USING_APP_CTRL_PH

#define USING_APP_CTRL_PH_DEBUG_

#include "event_driven.h"
#include "user_util.h"

#define NUMBER_SAMPLING_PH      15
#define PH_TIME_RESEND_WARNING  60000


#define PH_STEP_WARNING_DEFAULT 80
#define PH_STEP_WARNING_MAX     200
#define PH_STEP_WARNING_MIN     20

#define PH_UPPER_DEFAULT    760
#define PH_LOWER_DEFAULT    740
#define PH_UPPER_MAX        900
#define PH_LOWER_MIN        600
#define PH_CYCLE_UP_LOW     10

#define PH_TIME_CONTROL_MAX         60
#define PH_TIME_CONTROL_MIN         1
#define PH_TIME_PUMP_DEFAULT        5
#define PH_TIME_SAMPLING_DEFAULT    15

typedef enum
{
    _EVENT_CTRL_PH_ENTRY,
    
    _EVENT_CTRL_PH_CHECK_POWER,
    _EVENT_CTRL_PH_STATE_SLAVE,
    _EVENT_CTRL_PH_WARNING,
    
    _EVENT_CTRL_PH_SAMPLING,
    _EVENT_CTRL_PH_PUMP,

    _EVENT_CTRL_PH_END,
}eKindEventCtrlPH;

typedef enum
{
    _PH_DISCONNECT,
    _PH_CONNECT,
}eKindStateSensorpH;

typedef enum
{
    _PH_LOW,
    _PH_HIGH,
    _PH_BALANCE,
}eKindStatePHLow;

typedef enum
{
    _NONE_SENSOR_PH,
    _ONE_SENSOR_PH,
}eKindChooseOptionPH;

typedef enum
{
    _PH_SAMPLING,
    _PH_PUMP,
}eKindStateCtrlPH_Pump;

typedef enum
{
    _PH_STT_OK,
    _PH_STT_FAIL,
}eKindStateWarningPH;

typedef struct
{
    uint8_t ChooseOptionPH;         //Thiet bi co su dung cam bien pH k 
    uint8_t StateSensorPH;          //Trang thai ket noi vs Sensor PH
    uint8_t StatePH;                //Trang thai PH hien tai
    
    uint16_t pH_Measure;            //Gia tri pH do duoc
    uint16_t pH_Upper;              //Gia tri pH dieu khien nguong tren
    uint16_t pH_Lower;              //Gia tri pH dieu khien nguong duoi
    uint16_t pH_StepWarning;        //Gia tri pH step canh bao
    
    uint8_t  StateCtrl;             //Trang thai dieu khien pH
    uint8_t  StatePump;
    uint16_t pH_TimePump;           //Thoi gian bom 
    uint16_t pH_TimeSampling;       //Thoi gian lay mau 
    uint16_t pH_AverageSampling;    //Gia tri lay mau trung binh
    uint32_t pH_GettickMsCtrl;      //Get tick Ms dieu khien
    
    uint8_t StateWarningPH_Connect;
    uint8_t StateWarningPH_Measure;
}Struct_ParamCtrlPH;



extern sEvent_struct        sEventAppCtrlPH[];
extern Struct_ParamCtrlPH   sParamCtrlPH;
/*=============== Function handle ================*/

uint8_t     AppCtrlPH_Task(void);
void        Init_AppCtrlPH(void);

void        Save_pH_UpperLowerWar(uint16_t Upper, uint16_t Lower, uint16_t StepWarning);
void        Init_pH_UpperLowerWar(void);

void        Save_pH_TimeControl(uint16_t TimePump, uint16_t TimeSampling);
void        Init_pH_TimeControl(void);

void        Save_pH_ChooseOption(uint8_t StateChoose);
void        Init_pH_ChooseOption(void);

void        Save_pH_StateCtrl(void);
void        Init_pH_StateCtrl(void);

void        Control_Pump(eKindStateCtrlPH_Pump Kind_Ctrl);
#endif
