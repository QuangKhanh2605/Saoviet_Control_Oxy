
#ifndef USER_APP_CTRL_OXY__
#define USER_APP_CTRL_OXY_

#define USING_APP_CTRL_OXY

#include "event_driven.h"
#include "user_util.h"

#define TIME_MAX            1000
#define TIME_MIN            2

#define SATILITY_MAX        1000
#define SATILITY_MIN        0

#define DEFAULT_RUN_OXY     60
#define DEFAULT_FREE_OXY    2

typedef enum
{
    _EVENT_CTRL_OXY_ENTRY,
    _EVENT_CTRL_OXY_GETTICK,
    _EVENT_CTRL_OXY_0_CALIB,
    _EVENT_CTRL_OXY_100_CALIB,
    
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
    _OXY_OFF = 0,
    _OXY_FREE,    //Trang thai may nghi
    _OXY_L_RUN,     //Oxy ben trai hoat dong
    _OXY_R_RUN,     //Oxy ben phai hoat dong
}eKindStateOxy;

typedef struct
{
    uint16_t RunCtrl;
    uint16_t FreeCtrl;
    uint16_t RunOxy1;
    uint16_t FreeOxy1;
    uint16_t RunOxy2;
    uint16_t FreeOxy2;
    uint16_t RunOxy3;
    uint16_t FreeOxy3;
}Struct_Time_Ctrl_Oxy;

typedef struct
{
    uint8_t StateOxy1;
    uint8_t StateOxy2;
    uint8_t StateOxy3;
    uint16_t SetupSatility;
}Struct_Parameter_Ctrl_Oxy;

typedef struct
{
    uint16_t Temp;
    uint16_t Oxy_Mg_L;
    uint16_t Oxy_Percent;
}Struct_Param_Measure;

typedef struct
{
    uint8_t Obj1;
    uint8_t Obj2;
    uint8_t Obj3;
    uint8_t Obj4;
    uint8_t Obj5;
    uint8_t Obj6;
    uint8_t Pass1;
    uint8_t Pass2;
    uint8_t Pass3;
    uint8_t Pass4;
    uint8_t Pass5;
    uint8_t Pass6;
}Struct_Password;

extern sEvent_struct       sEventAppCtrlOxy[];
extern Struct_Time_Ctrl_Oxy            sTimeCtrlOxy;
extern Struct_Param_Measure            sParamMeasure;
extern Struct_Password                 sPassword;
extern Struct_Parameter_Ctrl_Oxy       sParamCtrlOxy;
/*================= Function Handle ================*/
uint8_t     AppCtrlOxy_Task(void);
void        Init_AppCtrlOxy(void);

void        Save_Time_Ctrl_Flash(uint16_t RunCtrl, uint16_t FreeCtrl);
void        Init_Time_Ctrl_Oxy(void);
void        Save_Satility_Flash(uint16_t Satility);
void        Init_Satility(void);

void        Ctrl_Oxy1(void);
void        Ctrl_Oxy2(void);
void        Ctrl_Oxy3(void);


#endif

