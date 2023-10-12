

#ifndef USER_APP_MENU_DISPLAY_H__
#define USER_APP_MENU_DISPLAY_H__

#define USING_APP_MENU_DISPLAY


#include "event_driven.h"
#include "user_util.h"


#define TIME_BACK_MAIN_1        60000


typedef enum
{
    _EVENT_MENU_ENTRY,
    _EVENT_MENU_DISPLAY_ALTER,
    _EVENT_MENU_DISPLAY_CALIB_OXY,
    _EVENT_MENU_DISPLAY_BACK_MAIN_1,
    
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
    _MENU_LOGIN,
    _MENU_SETTING_MAIN,
    _MENU_SETTING_CYCLE,
    _MENU_SETTING_CALIB,
    _MENU_SETTING_SATILITY,
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
    _SETTING_MAIN_SATILITY,
    _SETTING_MAIN_PASSWORD,
}eKindScreenSettingMain;

typedef enum
{
    _SETTING_CYCLE_RUN_CHOOSE=0,
    _SETTING_CYCLE_FREE_CHOOSE,
    _SETTING_CYCLE_RUN_CHANGE,
    _SETTING_CYCLE_FREE_CHANGE,
}eKindScreenSettingCycle;

typedef enum
{
    _SETTING_CALIB_0_CHOOSE=0,
    _SETTING_CALIB_100_CHOOSE,
}eKindScreenSettingCalib;

typedef enum
{
    _SETTING_SATILITY_CHOOSE,
    _SETTING_SATILITY_CHANGE,
}eKIndScreenSettingSatility;

typedef enum
{
    _CALIB_COMPLETE,
    _CALIB_ON_GOING,
    _CALIB_ERROR,
}eKindStateCalib;



typedef struct
{
    uint8_t ConnectServer;
    uint8_t Screen;
    uint8_t Login;
    uint8_t SettingMain;
    uint8_t SettingCycle;
    uint8_t SettingCalib;
    uint8_t SettingSatility;
    uint8_t SettingPassword;
    uint8_t CalibOxy;
}Struct_Menu_State;

typedef struct
{
    uint16_t TimeRunCtrlOxy;
    uint16_t TimeFreeCtrlOxy;
    uint16_t Satility;
    uint8_t  Pass1;
    uint8_t  Pass2;
    uint8_t  Pass3;
    uint8_t  Pass4;
    uint8_t  Pass5;
    uint8_t  Pass6;
}Struct_Menu_Stamp;

extern sEvent_struct        sEventAppMenu[];
extern Struct_Menu_State               sMenuState;
extern Struct_Menu_Stamp               sMenuStamp;
/*================= Function Handle ================*/
uint8_t     AppMenu_Task(void);
void        Init_AppMenuDisplay(void);
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
void        BT_Menu_Login(uint8_t KindButton);
void        BT_Menu_Setting_Main(uint8_t KindButton);
void        BT_Menu_Setting_Cycle(uint8_t KindButton);
void        BT_Menu_Setting_Calib(uint8_t KindButton);
void        BT_Menu_Setting_Satility(uint8_t KindButton);
void        BT_Menu_Setting_Password(uint8_t KindButton);
void        BT_Menu_Calib_Oxy(uint8_t KindButton);



#endif
