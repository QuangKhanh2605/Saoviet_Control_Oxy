
#ifndef USER_APP_LCD__
#define USER_APP_LCD__

#define USING_APP_LCD

#include "event_driven.h"
#include "user_util.h"

#include "GLCD192x64.h"
#define MAX_BUFFER_TIME     17

typedef enum
{
    _EVENT_LCD_ENTRY,
    _EVENT_LCD_GET_TIME_RTC,
    
    _EVENT_LCD_END,
}eKindEventLcd;

typedef struct
{
    uint8_t  e_name;
    uint8_t Status;
    uint8_t Rol;
	uint8_t Col;
    char *Object;
}LCD_Char_Display;

typedef struct
{
    uint8_t e_name;
    uint8_t  Status;
    uint8_t  Rol;
    uint8_t  Col;
    uint16_t *Object;
}LCD_Uint_Display;

typedef struct
{
    uint8_t e_name;
    uint8_t Status;
    uint8_t Rol;
    uint8_t Col;
    uint8_t Object;
}LCD_Uint_Icon;
/*-----------------Enum LCD MAIN 1 --------------------*/
typedef enum
{
    _LCD_C_MAIN_1_ID,
    _LCD_C_MAIN_1_ID_VALUE,
    _LCD_C_MAIN_1_PH_WATER,
    _LCD_C_MAIN_1_TEMP,
    _LCD_C_MAIN_1_OXY_1,
    _LCD_C_MAIN_1_OXY_2,
    _LCD_C_MAIN_1_STATE_OXY_1_ON,
    _LCD_C_MAIN_1_STATE_OXY_1_OFF,
    _LCD_C_MAIN_1_STATE_OXY_2_ON,
    _LCD_C_MAIN_1_STATE_OXY_2_OFF,
    _LCD_C_MAIN_1_STATE_OXY_3_ON,
    _LCD_C_MAIN_1_STATE_OXY_3_OFF,
    _LCD_C_MAIN_1_STATE_OXY_4_ON,
    _LCD_C_MAIN_1_STATE_OXY_4_OFF,
    
    _LCD_C_MAIN_1_END,
}eKindLcd_C_Main_1;

typedef enum
{
    _LCD_U_MAIN_1_PH_WATER,
    _LCD_U_MAIN_1_TEMP,
    _LCD_U_MAIN_1_OXY_MG_L,
    _LCD_U_MAIN_1_OXY_PERCENT,
    
    _LCD_U_MAIN_1_END,
}eKindLcd_U_Main_1;

/*-------------------------Enum LCD Main 2---------------------*/
typedef enum
{
    _LCD_C_MAIN_2_OXY_UPPER,
    _LCD_C_MAIN_2_OXY_LOWER,
    _LCD_C_MAIN_2_SALINITY,
    _LCD_C_MAIN_2_OXY_UPPER_WARNING,
    _LCD_C_MAIN_2_OXY_LOWER_WARNING,
    _LCD_C_MAIN_2_TIME_DELAY,
    
    _LCD_C_MAIN_2_END,
}eKindLcd_C_Main_2;

typedef enum
{
    _LCD_U_MAIN_2_OXY_UPPER,
    _LCD_U_MAIN_2_OXY_LOWER,
    _LCD_U_MAIN_2_SALINITY,
    _LCD_U_MAIN_2_OXY_UPPER_WARNING,
    _LCD_U_MAIN_2_OXY_LOWER_WARNING,
    _LCD_U_MAIN_2_TIME_DELAY,
    
    _LCD_U_MAIN_2_END,
}eKindLcd_U_Main_2;

/*---------------------------Enum LCD Login------------------------*/
typedef enum
{
    _LCD_C_LOGIN_ID,
    _LCD_C_LOGIN_ID_VALUE,
    _LCD_C_LOGIN_PASSWORD,
    _LCD_C_LOGIN_PASS_1,
    _LCD_C_LOGIN_PASS_2,
    _LCD_C_LOGIN_PASS_3,
    _LCD_C_LOGIN_PASS_4,
    _LCD_C_LOGIN_PASS_5,
    _LCD_C_LOGIN_PASS_6,
    _LCD_C_LOGIN_ENTER,
    _LCD_C_LOGIN_LOGIN,
    _LCD_C_LOGIN_WRONG,
    
    _LCD_C_LOGIN_END,
}eKindLcd_C_Login;

typedef enum
{
    _LCD_U_LOGIN_PASSWORD_1,
    _LCD_U_LOGIN_PASSWORD_2,
    _LCD_U_LOGIN_PASSWORD_3,
    _LCD_U_LOGIN_PASSWORD_4,
    _LCD_U_LOGIN_PASSWORD_5,
    _LCD_U_LOGIN_PASSWORD_6,
    
    _LCD_U_LOGIN_END,
}eKindLcd_U_Login;

/*----------------------------Enum setting main--------------------------*/
typedef enum
{
    _LCD_C_SETTING_MAIN_TITLE,
    _LCD_C_SETTING_MAIN_CYCLE,
    _LCD_C_SETTING_MAIN_CALIB,
    _LCD_C_SETTING_MAIN_PARAMETER,
    _LCD_C_SETTING_MAIN_PASSWORD,
    _LCD_C_SETTING_MAIN_PLEASE,
    
    _LCD_C_SETTING_MAIN_END,
}eKindLcd_C_Setting_Main;

/*----------------------------Enum setting calib--------------------------*/
typedef enum
{
    _LCD_C_SETTING_CALIB_TITLE,
    _LCD_C_SETTING_CALIB_100_CALIB,
    _LCD_C_SETTING_CALIB_SALINITY,
    _LCD_C_SETTING_CALIB_TEMPERATURE,
    _LCD_C_SETTING_CALIB_PLEASE,
    _LCD_C_SETTING_CALIB_SELECT,
    _LCD_C_SETTING_CALIB_ENTER_CALIB,
    _LCD_C_SETTING_CALIB_CONFIRM_RESET,
    
    _LCD_C_SETTING_CALIB_END,
}eKindLcd_C_Setting_Calib;

typedef enum
{
    _LCD_U_SETTING_CALIB_SALINITY,
    _LCD_U_SETTING_CALIB_TEMPERATURE,
    
    _LCD_U_SETTING_CALIB_END,
}eKindLcd_U_Setting_Calib;

/*---------------------------Enum setting cycle---------------------------*/
typedef enum
{
    _LCD_C_SETTING_CYCLE_TITLE,
    _LCD_C_SETTING_CYCLE_RUN,
    _LCD_C_SETTING_CYCLE_FREE,
    _LCD_C_SETTING_CYCLE_SLAVE,
    _LCD_C_SETTING_CYCLE_PLEASE,
    _LCD_C_SETTING_CYCLE_CLICK,
    _LCD_C_SETTING_CYCLE_SUCCESS,
    
    _LCD_C_SETTING_CYCLE_END,
}eKindLcd_C_Setting_Cycle;

typedef enum
{
    _LCD_U_SETTING_CYCLE_RUN,
    _LCD_U_SETTING_CYCLE_FREE,
    
    _LCD_U_SETTING_CYCLE_END,
}eKindLcd_U_Setting_Cycle;

/*----------------------------Enum setting cycle--------------------------*/
typedef enum
{
    _LCD_C_SETTING_PARAMETER,
    _LCD_C_SETTING_PARAMETER_OXY_UPPER,
    _LCD_C_SETTING_PARAMETER_OXY_LOWER,
    _LCD_C_SETTING_PARAMETER_TIME_DELAY,
    _LCD_C_SETTING_PARAMETER_OXY_UPPER_W,
    _LCD_C_SETTING_PARAMETER_OXY_LOWER_W,
    
    LCD_C_SETTING_PARAMETER_END,
}eKindLcd_C_Setting_Parameter;

typedef enum
{
    _LCD_U_SETTING_PARAMETER_OXY_UPPER,
    _LCD_U_SETTING_PARAMETER_OXY_LOWER,
    _LCD_U_SETTING_PARAMETER_TIME_DELAY,
    _LCD_U_SETTING_PARAMETER_OXY_UPPER_W,
    _LCD_U_SETTING_PARAMETER_OXY_LOWER_W,
    
    _LCD_U_SETTING_PARAMETER_END,
}eKindLcd_U_Setting_Parameter;
/*---------------------------Enum LCD Login------------------------*/
typedef enum
{
    _LCD_C_SETTING_PASSWORD_TITLE,
    _LCD_C_SETTING_PASSWORD,
    _LCD_C_SETTING_PASSWORD_1,
    _LCD_C_SETTING_PASSWORD_2,
    _LCD_C_SETTING_PASSWORD_3,
    _LCD_C_SETTING_PASSWORD_4,
    _LCD_C_SETTING_PASSWORD_5,
    _LCD_C_SETTING_PASSWORD_6,
    _LCD_C_SETTING_PASSWORD_SUCCESS,
    _LCD_C_SETTING_PASSWORD_SAVE,
    _LCD_C_SETTING_PASSWORD_ENTER,
    _LCD_C_SETTING_PASSWORD_CLICK,
    
    _LCD_C_SETTING_PASSWORD_END,
}eKindLcd_C_Setting_PassWord;

typedef enum
{
    _LCD_U_SETTING_PASSWORD_1,
    _LCD_U_SETTING_PASSWORD_2,
    _LCD_U_SETTING_PASSWORD_3,
    _LCD_U_SETTING_PASSWORD_4,
    _LCD_U_SETTING_PASSWORD_5,
    _LCD_U_SETTING_PASSWORD_6,
    
    _LCD_U_SETTING_PASSWORD_END,
}eKindLcd_U_Setting_Password;


/*--------------------------------Enum calib oxy---------------------------*/
typedef enum
{
    _LCD_C_CALIB_OXY_100,
    _LCD_C_CALIB_OXY_SALINITY,
    _LCD_C_CALIB_OXY_TEMPERATURE,
    _LCD_C_CALIB_OXY_RESET,
    _LCD_C_CALIB_OXY_STATE_LOAD,
    _LCD_C_CALIB_OXY_STATE_DONE,
    _LCD_C_CALIB_OXY_STATE_ERROR,
    _LCD_C_CALIB_OXY_PLEASE_WAIT,
    _LCD_C_CALIB_OXY_PLEASE_CLICK,
    
    _LCD_C_CALIB_OXY_END,
}eKindLcd_C_Calib_Oxy;

/*-------------------------------------------------------------*/
typedef enum
{
    _RUN_DISPLAY_CHAR_PARAM,
    _RUN_DISPLAY_CHAR_OXY_1,
    _RUN_DISPLAY_CHAR_OXY_2,
    _RUN_DISPLAY_CHAR_OXY_3,

    _RUN_DISPLAY_CHAR_END,
}eKindLcdRunDisplayChar;

typedef enum
{
    _RUN_DISPLAY_UINT_OXY_1_RUN,
    _RUN_DISPLAY_UINT_OXY_1_FREE,
    
    _RUN_DISPLAY_UINT_OXY_2_RUN,
    _RUN_DISPLAY_UINT_OXY_2_FREE,
    
    _RUN_DISPLAY_UINT_OXY_3_RUN,
    _RUN_DISPLAY_UINT_OXY_3_FREE,

    _RUN_DISPLAY_UINT_END,
}eKindLcdRunDisplayUint;

typedef enum
{
    _SETUP_DISPLAY_CHAR_PARAM,
    _SETUP_DISPLAY_CHAR_1,
    _SETUP_DISPLAY_CHAR_2,
    _SETUP_DISPLAY_CHAR_3,

    _SETUP_DISPLAY_CHAR_END,
}eKindLcdSetupDisplayChar;

typedef enum
{
    _SETUP_DISPLAY_UINT_1,
    _SETUP_DISPLAY_UINT_2,

    _SETUP_DISPLAY_UINT_END,
}eKindLcdSetupDisplayUint;


typedef enum
{
    _DIS_CHAR,
    _DIS_UINT,
}eKindDisplayLcd;

extern sEvent_struct       sEventAppVibSensor[];

extern LCD_Char_Display                    oLCD_C_Main_1[];
extern LCD_Uint_Display                    oLCD_U_Main_1[];

extern LCD_Char_Display                    oLCD_C_Main_2[];
extern LCD_Uint_Display                    oLCD_U_Main_2[];

extern LCD_Char_Display                    oLCD_C_Login[];
extern LCD_Uint_Display                    oLCD_U_Login[];

extern LCD_Char_Display                    oLCD_C_Setting_Main[];

extern LCD_Char_Display                    oLCD_C_Setting_Calib[];
extern LCD_Uint_Display                    oLCD_U_Setting_Calib[];

extern LCD_Char_Display                    oLCD_C_Setting_Cycle[];
extern LCD_Uint_Display                    oLCD_U_Setting_Cycle[];

extern LCD_Char_Display                    oLCD_C_Setting_Parameter[];
extern LCD_Uint_Display                    oLCD_U_Setting_Parameter[];

extern LCD_Char_Display                    oLCD_C_Setting_Password[];
extern LCD_Uint_Display                    oLCD_U_Setting_Password[];
extern LCD_Char_Display                    oLCD_C_Calib_Oxy[];
/*================= Function Handle ================*/
uint8_t     AppLcd_Task(void);
void        Init_AppLcd(void);

void        Display_Time(void);
void        Uint_To_Char_Location(char time[], uint32_t stamp, uint16_t *location);
void        Time_To_Char(char News[]);
void        Display_Control_Setting_Left(uint8_t Page);
void        Display_Control_Setting_Right(uint8_t Page);
void        Clear_Rol_LCD(uint8_t PageStart, uint8_t PageEnd);

//--------------Function Display-----------
void        DLCD_Main_1(void);
void        DLCD_Main_2(void);
void        DLCD_Login(void);
void        DLCD_Setting_Main(void);
void        DLCD_Setting_Calib(void);
void        DLCD_Setting_Cycle(void);
void        DLCD_Setting_Parameter(void);
void        DLCD_Setting_Password(void);
void        DLCD_Calib_Oxy(void);

//-------------Function Entry--------------
void        DLCD_Main_1_Entry(void);
void        DLCD_Main_2_Entry(void);
void        DLCD_Login_Entry(void);
void        DLCD_Setting_Main_Entry(void);
void        DLCD_Setting_Cycle_Entry(void);
void        DLCD_Setting_Calib_Entry(void);
void        DLCD_Setting_Parameter_Entry(void);
void        DLCD_Setting_Password_Entry(void);
void        DLCD_Calib_Oxy_Entry(void);

//-------------Function Alter---------------
void        DLCD_Main_1_Alter(void);
void        DLCD_Main_2_Alter(void);
void        DLCD_Login_Alter(void);
void        DLCD_Setting_Cycle_Alter(void);
void        DLCD_Setting_Calib_Alter(void);
void        DLCD_Setting_Parameter_Alter(void);
void        DLCD_Setting_Password_Alter(void);

#endif

