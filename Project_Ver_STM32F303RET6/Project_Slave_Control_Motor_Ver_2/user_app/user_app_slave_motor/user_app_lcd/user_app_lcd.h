#ifndef USER_APP_LCD_H_
#define USER_APP_LCD_H_

#define USING_APP_LCD

#include "event_driven.h"
#include "user_util.h"

#include "CLCD.h"

#define MCU_RS_GPIO_Port        MCU_LCD_RS_GPIO_Port
#define MCU_RS_Pin              MCU_LCD_RS_Pin
#define MCU_EN_GPIO_Port        MCU_LCD_EN_GPIO_Port
#define MCU_EN_Pin              MCU_LCD_EN_Pin
#define LCD_D4_GPIO_Port        MCU_LCD_D4_GPIO_Port
#define LCD_D4_Pin              MCU_LCD_D4_Pin
#define LCD_D5_GPIO_Port        MCU_LCD_D5_GPIO_Port
#define LCD_D5_Pin              MCU_LCD_D5_Pin
#define LCD_D6_GPIO_Port        MCU_LCD_D6_GPIO_Port
#define LCD_D6_Pin              MCU_LCD_D6_Pin
#define LCD_D7_GPIO_Port        MCU_LCD_D7_GPIO_Port
#define LCD_D7_Pin              MCU_LCD_D7_Pin
    
typedef enum
{
    _EVENT_LCD_ENTRY,
    _EVENT_LCD_HANDLE,

    _EVENT_LCD_END,
}eKindEventLCD;

typedef enum
{
    _CONTROL_PAGE_0,
    _CONTROL_PAGE_1,
    _CONTROL_PAGE_2,
    _CONTROL_PAGE_3,
    _CONTROL_CLEAR,
}eKindDisplayControlSetting;

typedef struct
{
    uint8_t e_name;             //Enum_Name Object muon hien thi
    uint8_t Status;             //Trang thai da hien thi hay chua
    uint8_t Rol;                //Dong hien thi
	uint8_t Col_Begin;          //Cot bat dau hien thi
    uint8_t Col_End;            //Cot ket thuc hien thi
    char *Object;               //Du lieu muon hien thi (char)
}LCD_Char_Display;

typedef struct
{
    uint8_t e_name;             //Enum_Name Object muon hien thi
    uint8_t  Status;            //Trang thai da hien thi hay chua
    uint8_t  Rol;               //Dong hien thi
    uint8_t  Col_Begin;         //Cot bat dau hien thi
    uint8_t  Col_End;           //Cot ket thuc hien thi
    uint16_t *Object;           //Du lieu muon hien thi (uint16_t)
}LCD_Uint_Display;

/*-----------------Enum LCD MAIN 1 --------------------*/
typedef enum
{
    _LCD_C_MAIN_1_STATE_AMPERAGE,
    _LCD_C_MAIN_1_STATE_AMPERAGE_OK,
    _LCD_C_MAIN_1_STATE_AMPERAGE_FAIL,
  
    _LCD_C_MAIN_1_STATE_PRESSURE,
    _LCD_C_MAIN_1_STATE_PRESSURE_OK,
    _LCD_C_MAIN_1_STATE_PRESSURE_FAIL,
    
    _LCD_C_MAIN_1_STATE_MACHINE_OFF,
    _LCD_C_MAIN_1_STATE_MACHINE_MAN,
    _LCD_C_MAIN_1_STATE_MACHINE_AUTO,
    
    _LCD_C_MAIN_1_COL_1,
    _LCD_C_MAIN_1_COL_2,
    _LCD_C_MAIN_1_COL_3,
//    _LCD_C_MAIN_1_COL_4,
    
    _LCD_C_MAIN_1_AMPERAGE_1,
    _LCD_C_MAIN_1_AMPERAGE_2,
    _LCD_C_MAIN_1_AMPERAGE_3,
//    _LCD_C_MAIN_1_AMPERAGE_UNIT,
//    _LCD_C_MAIN_1_AMPERAGE_OK,
//    _LCD_C_MAIN_1_AMPERAGE_WARNING,
    
    _LCD_C_MAIN_1_PRESSURE_1,
    _LCD_C_MAIN_1_PRESSURE_2,
    _LCD_C_MAIN_1_PRESSURE_UNIT,
//    _LCD_C_MAIN_1_PRESSURE_OK,
//    _LCD_C_MAIN_1_PRESSURE_WARNING,
    
    _LCD_C_MAIN_1_END,
}eKindLcd_C_Main_1;

typedef enum
{
    _LCD_U_MAIN_1_AMPERAGE_1,
    _LCD_U_MAIN_1_AMPERAGE_2,
    _LCD_U_MAIN_1_AMPERAGE_3,
    _LCD_U_MAIN_1_PRESSURE,
    
    _LCD_U_MAIN_1_END,
}eKindLcd_U_Main_1;
/*-------------------------Enum LCD Main 2---------------------*/
typedef enum
{
    _LCD_C_MAIN_2_TITLE,
    _LCD_C_MAIN_2_AMPERAGE_UPPER,
    _LCD_C_MAIN_2_AMPERAGE_UPPER_UNIT,
    _LCD_C_MAIN_2_AMPERAGE_DELTA,
    _LCD_C_MAIN_2_AMPERAGE_DELTA_UNIT,
    _LCD_C_MAIN_2_PRESSURE_UPPER,
    _LCD_C_MAIN_2_PRESSURE_UPPER_UNIT,
    _LCD_C_MAIN_2_PRESSURE_LOWER,
    _LCD_C_MAIN_2_PRESSURE_LOWER_UNIT,
    
    _LCD_C_MAIN_2_END,
}eKindLcd_C_Main_2;

typedef enum
{
    _LCD_U_MAIN_2_AMPERAGE_UPPER,
    _LCD_U_MAIN_2_AMPERAGE_DELTA,
    _LCD_U_MAIN_2_PRESSURE_UPPER,
    _LCD_U_MAIN_2_PRESSURE_LOWER,
    
    _LCD_U_MAIN_2_END,
}eKindLcd_U_Main_2;

/*---------------------------Enum LCD Login------------------------*/
typedef enum
{
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
    _LCD_C_SETTING_MAIN_PARAMETER,
    _LCD_C_SETTING_MAIN_OPTION,
    _LCD_C_SETTING_MAIN_PASSWORD,
    
    _LCD_C_SETTING_MAIN_END,
}eKindLcd_C_Setting_Main;

/*----------------------------Enum setting parameter--------------------------*/
typedef enum
{
    _LCD_C_SETTING_PARAMETER,
    _LCD_C_SETTING_PARAMETER_AMPERAGE_UPPER,
    _LCD_C_SETTING_PARAMETER_AMPERAGE_UPPER_UNIT,
    _LCD_C_SETTING_PARAMETER_AMPERAGE_DELTA,
    _LCD_C_SETTING_PARAMETER_AMPERAGE_DELTA_UNIT,
    _LCD_C_SETTING_PARAMETER_PRESSURE_UPPER,
    _LCD_C_SETTING_PARAMETER_PRESSURE_UPPER_UNIT,
    _LCD_C_SETTING_PARAMETER_PRESSURE_LOWER,
    _LCD_C_SETTING_PARAMETER_PRESSURE_LOWER_UNIT,
    
    LCD_C_SETTING_PARAMETER_END,
}eKindLcd_C_Setting_Parameter;

typedef enum
{
    _LCD_U_SETTING_PARAMETER_AMPERAGE_UPPER,
    _LCD_U_SETTING_PARAMETER_AMPERAGE_DELTA,
    _LCD_U_SETTING_PARAMETER_PRESSURE_UPPER,
    _LCD_U_SETTING_PARAMETER_PRESSURE_LOWER,
    
    _LCD_U_SETTING_PARAMETER_END,
}eKindLcd_U_Setting_Parameter;

/*----------------------------Enum setting option--------------------------*/
typedef enum
{
    _LCD_C_SETTING_OPTION,
    _LCD_C_SETTING_OPTION_AMPERAGE,
    _LCD_C_SETTING_OPTION_AMPERAGE_ON,
    _LCD_C_SETTING_OPTION_AMPERAGE_OFF,
    
    _LCD_C_SETTING_OPTION_PRESSURE,
    _LCD_C_SETTING_OPTION_PRESSURE_ON,
    _LCD_C_SETTING_OPTION_PRESSURE_OFF,
    
    _LCD_C_SETTING_CONSTANT_PRESSURE,
    _LCD_C_SETTING_CONSTANT_UINIT,
    
    LCD_C_SETTING_OPTION_END,
}eKindLcd_C_Setting_Option;

typedef enum
{
    _LCD_U_SETTING_CONSTANT_PRESSURE,
    
    LCD_U_SETTING_OPTION_END,
}eKindLcd_U_Setting_Option;

/*---------------------------Enum LCD Login------------------------*/
typedef enum
{
    _LCD_C_SETTING_PASSWORD_TITLE,
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

extern sEvent_struct        sEventAppLcd[];

extern LCD_Char_Display                    oLCD_C_Main_1[];
extern LCD_Uint_Display                    oLCD_U_Main_1[];

extern LCD_Char_Display                    oLCD_C_Main_2[];
extern LCD_Uint_Display                    oLCD_U_Main_2[];

extern LCD_Char_Display                    oLCD_C_Login[];
extern LCD_Uint_Display                    oLCD_U_Login[];

extern LCD_Char_Display                    oLCD_C_Setting_Main[];

extern LCD_Char_Display                    oLCD_C_Setting_Parameter[];
extern LCD_Uint_Display                    oLCD_U_Setting_Parameter[];

extern LCD_Char_Display                    oLCD_C_Setting_Option[];

extern LCD_Char_Display                    oLCD_C_Setting_Password[];
extern LCD_Uint_Display                    oLCD_U_Setting_Password[];
/*=============== Function handle ================*/
uint8_t     AppLcd_Task(void);
void        Init_AppLcd(void);
void        Init_LCD(void);

void        Display_Control_Setting_Left(uint8_t Page);
void        Display_Control_Setting_Right(uint8_t Page);

//--------------Function Display-----------
void        DLCD_Main_1(void);
void        DLCD_Main_2(void);
void        DLCD_Login(void);
void        DLCD_Setting_Main(void);
void        DLCD_Setting_Parameter(void);
void        DLCD_Setting_Option(void);
void        DLCD_Setting_Password(void);

//-------------Function Entry--------------
void        DLCD_Main_1_Entry(void);
void        DLCD_Main_2_Entry(void);
void        DLCD_Login_Entry(void);
void        DLCD_Setting_Main_Entry(void);
void        DLCD_Setting_Parameter_Entry(void);
void        DLCD_Setting_Option_Entry(void);
void        DLCD_Setting_Password_Entry(void);

//-------------Function Alter---------------
void        DLCD_Main_1_Alter(void);
void        DLCD_Main_2_Alter(void);
void        DLCD_Login_Alter(void);
void        DLCD_Setting_Parameter_Alter(void);
void        DLCD_Setting_Option_Alter(void);
void        DLCD_Setting_Password_Alter(void);

#endif

