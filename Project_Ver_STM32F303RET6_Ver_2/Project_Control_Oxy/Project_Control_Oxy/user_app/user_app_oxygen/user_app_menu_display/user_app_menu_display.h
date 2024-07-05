

#ifndef USER_APP_MENU_DISPLAY_H__
#define USER_APP_MENU_DISPLAY_H__

#define USING_APP_MENU_DISPLAY


#include "event_driven.h"
#include "user_util.h"

#include "user_app_ctrl_oxy.h"

#define TIME_BACK_MAIN_1        60000


typedef enum
{
    _EVENT_MENU_ENTRY,
    _EVENT_MENU_DISPLAY_ALTER,
    _EVENT_MENU_DISPLAY_CALIB_OXY,
    _EVENT_MENU_DISPLAY_BACK_MAIN_1,
    _EVENT_DISPLAY_STATE_MEASURE,
    
    _EVENT_OFF_SPEAKER,
    
    _EVENT_MENU_END,
}eKindEventMenu;

typedef enum
{
    _BT_ENTER_CLICK,
    _BT_ENTER_HOLD_CYCLE,
    _BT_EXIT_CLICK,
    _BT_EXIT_HOLD_CYCLE,
    _BT_UP_CLICK,
    _BT_UP_HOLD_CYCLE,
    _BT_DOWN_CLICK,
    _BT_DOWN_HOLD_CYCLE,
    
    _BT_ENTER_HOLD_ONCE,
    _BT_EXIT_HOLD_ONCE,
    _BT_UP_HOLD_ONCE,
    _BT_DOWN_HOLD_ONCE,
}eKindButtonMenu;

typedef enum
{
    _MENU_MAIN_1=0,
    _MENU_MAIN_2,
    _MENU_MAIN_2_TAB_2,
    _MENU_MAIN_3,
    _MENU_LOGIN,
    _MENU_SETTING_MAIN,
    _MENU_SETTING_CYCLE,
    _MENU_SETTING_CALIB,
    _MENU_SETTING_PARAMETER,
    _MENU_SETTING_PARAMETER_TAB_2,
    _MENU_SETTING_PASSWORD,
    _MENU_CALIB_OXY,
}eKindScreenMenu;

typedef enum
{
    _LOGIN_PASS_1=0,
    _LOGIN_PASS_2,
    _LOGIN_PASS_3,
    _LOGIN_PASS_4,
    _LOGIN_PASS_5,
    _LOGIN_PASS_6,
}eKindScreenLogin;

typedef enum
{
    _CHANGE_PASS_1=0,
    _CHANGE_PASS_2,
    _CHANGE_PASS_3,
    _CHANGE_PASS_4,
    _CHANGE_PASS_5,
    _CHANGE_PASS_6,
    _SUCCESS_PASS,
}eKindScreenSettingPassword;

typedef enum
{
    _SETTING_MAIN_CYCLE=0,
    _SETTING_MAIN_CALIB,
    _SETTING_MAIN_PARAMETER,
    _SETTING_MAIN_PASSWORD,
}eKindScreenSettingMain;

typedef enum
{
    _SETTING_CYCLE_NUMBER_MOTOR_CHOOSE=0,
    _SETTING_CYCLE_RUN_CHOOSE,
    _SETTING_CYCLE_FREE_CHOOSE,
    
    _SETTING_CYCLE_NUMBER_MOTOR_CHANGE,
    _SETTING_CYCLE_RUN_CHANGE,
    _SETTING_CYCLE_FREE_CHANGE,
    _SETTING_CYCLE_SEND_SLAVE,
}eKindScreenSettingCycle;

typedef enum
{
    _SETTING_CALIB_SALINITY_CHOOSE=0,
    _SETTING_CALIB_100_CALIB_CHOOSE,
    _SETTING_CALIB_PH_POINT_686_CHOOSE,
    _SETTING_CALIB_PH_POINT_918_CHOOSE,
//    _SETTING_CALIB_TEMPERATURE_CHOOSE,
    _SETTING_CALIB_SALINITY_CHANGE,
    _SETTING_CALIB_100_CALIB_ENTER,
    _SETTING_CALIB_PH_POINT_686_ENTER,
    _SETTING_CALIB_PH_POINT_918_ENTER,
//    _SETTING_CALIB_TEMPERATURE_CHANGE,
}eKindScreenSettingCalib;

typedef enum
{
    _SETTING_OXY_UPPER_CHOOSE,
    _SETTING_OXY_LOWER_CHOOSE,
    _SETTING_OXY_UPPER_W_CHOOSE,
    _SETTING_OXY_LOWER_W_CHOOSE,
    
    _SETTING_OXY_UPPER_CHANGE,
    _SETTING_OXY_LOWER_CHANGE,
    _SETTING_OXY_UPPER_W_CHANGE,
    _SETTING_OXY_LOWER_W_CHANGE,
}eKIndScreenSettingTimeDelay;

typedef enum
{
    _SETTING_PARAMETER_TAB_2_PH_UPPER_CHOOSE,
    _SETTING_PARAMETER_TAB_2_PH_LOWER_CHOOSE,
    _SETTING_PARAMETER_TAB_2_PH_TIME_PUMP_CHOOSE,
    _SETTING_PARAMETER_TAB_2_PH_TIME_SAMPLING_CHOOSE,
    
    _SETTING_PARAMETER_TAB_2_PH_UPPER_CHANGE,
    _SETTING_PARAMETER_TAB_2_PH_LOWER_CHANGE,
    _SETTING_PARAMETER_TAB_2_PH_TIME_PUMP_CHANGE,
    _SETTING_PARAMETER_TAB_2_PH_TIME_SAMPLING_CHANGE,
}eKindScreenSettingParameterTab2;

typedef enum
{
    _CALIB_COMPLETE,
    _CALIB_ON_GOING,
    _CALIB_ERROR,
}eKindStateCalib;

typedef struct
{
    uint16_t Pass1;
    uint16_t Pass2;
    uint16_t Pass3;
    uint16_t Pass4;
    uint16_t Pass5;
    uint16_t Pass6;
}Struct_Password;

typedef struct
{
    uint8_t ConnectServer;              //Trang thai ket noi server
    uint8_t Screen;                     //Menu man hinh chinh
    uint8_t Login;                      //Menu Login
    uint8_t SettingMain;                //Menu Man hinh chinh setting
    uint8_t SettingCycle;               //Menu Setting Cycle
    uint8_t SettingCalib;               //Menu Setting Calib
    uint8_t SettingParameter;           //Menu Setting Parameter
    uint8_t SettingParameter_Tab_2;     //Menu Setting Parameter Tab 2
    uint8_t SettingPassword;            //Menu Setting Password
    uint8_t CalibSensor;                //Menu Screen Calib sensor
}Struct_Menu_State;

typedef struct
{
    uint16_t Salinity;                  //Stamp Salinity
    uint16_t Temperature;               //Stamp Temperature
//    uint16_t TimeDelay;                 //Stamp Time Delay
    uint16_t OxyUpper;                  //Stamp Oxy Upper
    uint16_t OxyLower;                  //Stamp Oxy Lower
    uint16_t OxyUpperWarning;           //Stamp Oxy Upper Warning
    uint16_t OxyLowerWarning;           //Stamp Oxy Lower Warning
    uint16_t pH_Upper;                  //Stamp pH Upper 
    uint16_t pH_Lower;                  //Stamp pH Lower
    uint16_t pH_TimePump;               //Stamp pH Time Pump
    uint16_t pH_TimeSampling;           //Stamp pH Time Sampling
    
    uint16_t NumberMotorSlave;
    
    Struct_TimeSlave        sTimeSlave;
    Struct_Password         sPassLogin; 
    Struct_Password         sPassWord; 
}Struct_Menu_Stamp;

extern sEvent_struct        sEventAppMenu[];
extern Struct_Menu_State               sMenuState;
extern Struct_Menu_Stamp               sMenuStamp;
extern Struct_Password                 sPassword;
/*================= Function Handle ================*/
uint8_t     AppMenu_Task(void);
void        Init_AppMenuDisplay(void);
void        Init_StartMenu(void);
void        Menu_ResetState(void);
void        Save_Password(void);
void        Init_Password(void);
uint8_t     Check_Password(void);
void        Stamp_Menu_Exit(void);


void        User_Button_Enter_Click(void);
void        User_Button_Exit_Click(void);
void        User_Button_Up_Click(void);
void        User_Button_Down_Click(void);

void        User_Button_Enter_Hold_Cycle(void);
void        User_Button_Exit_Hold_Cycle(void);
void        User_Button_Up_Hold_Cycle(void);
void        User_Button_Down_Hold_Cycle(void);

void        User_Button_Enter_Hold_Once(void);
void        User_Button_Exit_Hold_Once(void);
void        User_Button_Up_Hold_Once(void);
void        User_Button_Down_Hold_Once(void);

void        Handle_Menu_Button(uint8_t KindButton);
void        BT_Menu_Main_1(uint8_t KindButton);
void        BT_Menu_Main_2(uint8_t KindButton);
void        BT_Menu_Main_2_Tab_2(uint8_t KindButton);
void        BT_Menu_Main_3(uint8_t KindButton);
void        BT_Menu_Login(uint8_t KindButton);
void        BT_Menu_Setting_Main(uint8_t KindButton);
void        BT_Menu_Setting_Cycle(uint8_t KindButton);
void        BT_Menu_Setting_Calib(uint8_t KindButton);
void        BT_Menu_Setting_Parameter(uint8_t KindButton);
void        BT_Menu_Setting_Parameter_Tab_2(uint8_t KindButton);
void        BT_Menu_Setting_Password(uint8_t KindButton);
void        BT_Menu_Calib_Oxy(uint8_t KindButton);

void        On_Speaker(uint16_t TimeOn);

#endif
