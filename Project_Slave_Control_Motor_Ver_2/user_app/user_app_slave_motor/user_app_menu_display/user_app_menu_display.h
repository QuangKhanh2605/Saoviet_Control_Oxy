#ifndef USER_APP_MENU_DISPLAY_H_
#define USER_APP_MENU_DISPLAY_H_

#define USING_APP_MENU_DISPLAY

#include "event_driven.h"
#include "user_util.h"

#define TIME_BACK_MAIN_1        60000

typedef enum
{
    _EVENT_MENU_ENTRY,
    _EVENT_MENU_DISPLAY_ALTER,
    _EVENT_MENU_DISPLAY_BACK_MAIN_1,
    
    _EVENT_OFF_SPEAKER,

    _EVENT_MENU_DISPLAY_END,
}eKindEventMenuDisplay;

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
    _MENU_LOGIN,
    _MENU_SETTING_MAIN,
    _MENU_SETTING_PARAMETER,
    _MENU_SETTING_OPTION,
    _MENU_SETTING_PASSWORD,
}eKindScreenMenu;

typedef enum
{
    _SETTING_AMPERAGE_UPPER_CHOOSE,
    _SETTING_AMPERAGE_DELTA_CHOOSE,
    _SETTING_PRESSURE_UPPER_CHOOSE,
    _SETTING_PRESSURE_LOWER_CHOOSE,
    
    _SETTING_AMPERAGE_UPPER_CHANGE,
    _SETTING_AMPERAGE_DELTA_CHANGE,
    _SETTING_PRESSURE_UPPER_CHANGE,
    _SETTING_PRESSURE_LOWER_CHANGE,
}eKIndScreenSettingParameter;

typedef enum
{
    _SETTING_AMPERAGE_OPTION_CHOOSE,
    _SETTING_PRESSURE_OPTION_CHOOSE,
    _SETTING_CONSTANT_PRESSURE_CHOOSE,
    
    _SETTING_AMPERAGE_OPTION_CHANGE,
    _SETTING_PRESSURE_OPTION_CHANGE,
    _SETTING_CONSTANT_PRESSURE_CHANGE,
}eKIndScreenSettingOption;

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
    _SETTING_MAIN_PARAMETER=0,
    _SETTING_MAIN_OPTION,
    _SETTING_MAIN_PASSWORD,
}eKindScreenSettingMain;

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
    uint16_t Amperage_Upper;
    uint16_t Amperage_Delta;
    uint16_t Pressure_Upper;
    uint16_t Pressure_Lower;
    
    uint16_t Amperage_Option;
    uint16_t Pressure_Option;
    
    uint16_t Pressure_Constant;
  
    Struct_Password         sPassLogin; 
    Struct_Password         sPassWord; 
}Struct_Menu_Stamp;

typedef struct
{
    uint8_t Screen;                     //Menu man hinh chinh
    uint8_t Login;                      //Menu Login
    uint8_t SettingMain;                //Menu Man hinh chinh setting
    uint8_t SettingParameter;           //Menu Setting Parameter
    uint8_t SettingOption;     //Menu Setting Parameter Tab 2
    uint8_t SettingPassword;            //Menu Setting Password
}Struct_Menu_State;

extern sEvent_struct        sEventAppMenu[];
extern Struct_Menu_Stamp               sMenuStamp;
extern Struct_Password                 sPassword;
/*=============== Function handle ================*/
uint8_t     AppMenu_Task(void);
void        Init_AppMenu(void);

void        Menu_ResetState(void);
uint8_t     Check_Password(void);
void        Save_Password(void);
void        Init_Password(void);
void        Stamp_Menu_Exit(void);
/*--------------- Handle Button Menu --------------*/
void        On_Speaker(uint16_t TimeOn);

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
void        BT_Menu_Login(uint8_t KindButton);
void        BT_Menu_Setting_Main(uint8_t KindButton);
void        BT_Menu_Setting_Parameter(uint8_t KindButton);
void        BT_Menu_Setting_Option(uint8_t KindButton);
void        BT_Menu_Setting_Password(uint8_t KindButton);

#endif

