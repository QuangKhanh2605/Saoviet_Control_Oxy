#include "user_app_lcd.h"
#include "rtc.h"
#include "user_app_ctrl_oxy.h"
#include "user_app_menu_display.h"
#include "user_modem.h"
/*=============== Function static ================*/
static uint8_t fevent_lcd_entry(uint8_t event);
static uint8_t fevent_lcd_get_time_rtc(uint8_t event);

/*================== Struct ===================*/
sEvent_struct       sEventAppLcd[]=
{
  {_EVENT_LCD_ENTRY,            1, 5, 0,        fevent_lcd_entry},
  {_EVENT_LCD_GET_TIME_RTC,     1, 5, 250,      fevent_lcd_get_time_rtc},
};

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
char cTime_RTC[MAX_BUFFER_TIME]="00/00/00-00:00:00";
/*===== Object LCD =============== MAIN Screen 1 =================*/
LCD_Char_Display                    oLCD_C_Main_1[]=
{
  {_LCD_C_MAIN_1_ID,                0,2,8,          (char*)"ID  :"},
  {_LCD_C_MAIN_1_ID_VALUE,          0,2,40,         (char*)aDCU_ID},
  {_LCD_C_MAIN_1_PH_WATER,          0,3,8,          (char*)"PoH :        pH"},
  {_LCD_C_MAIN_1_TEMP,              0,4,8,          (char*)"Temp:         C"},
  {_LCD_C_MAIN_1_OXY_1,             0,5,8,          (char*)"Oxy :        mg/L"},
  {_LCD_C_MAIN_1_OXY_2,             0,6,86,         (char*)"%"},
  {_LCD_C_MAIN_1_STATE_OXY_1_ON,    0,7,7,          (char*)"ON "},
  {_LCD_C_MAIN_1_STATE_OXY_1_OFF,   0,7,7,          (char*)"OFF"},
  {_LCD_C_MAIN_1_STATE_OXY_2_ON,    0,7,40,         (char*)"ON "},
  {_LCD_C_MAIN_1_STATE_OXY_2_OFF,   0,7,40,         (char*)"OFF"},
  {_LCD_C_MAIN_1_STATE_OXY_3_ON,    0,7,75,         (char*)"ON "},
  {_LCD_C_MAIN_1_STATE_OXY_3_OFF,   0,7,75,         (char*)"OFF"},
  {_LCD_C_MAIN_1_STATE_OXY_4_ON,    0,7,109,        (char*)"ON "},
  {_LCD_C_MAIN_1_STATE_OXY_4_OFF,   0,7,109,        (char*)"OFF"},
};
LCD_Uint_Display                    oLCD_U_Main_1[]=
{
  {_LCD_U_MAIN_1_PH_WATER,          0,3,48,          &sParamMeasure.pH_Water},
  {_LCD_U_MAIN_1_TEMP,              0,4,48,          &sParamMeasure.Temp},
  {_LCD_U_MAIN_1_OXY_MG_L,          0,5,48,          &sParamMeasure.Oxy_Mg_L},
  {_LCD_U_MAIN_1_OXY_PERCENT,       0,6,48,          &sParamMeasure.Oxy_Percent},
};

/*===== Object LCD =============== MAIN_Screen 2 =================*/
LCD_Char_Display                    oLCD_C_Main_2[]=
{
  {_LCD_C_MAIN_2_ID,                0,2,8,          (char*)"ID  :"},
  {_LCD_C_MAIN_2_ID_VALUE,          0,2,40,         (char*)aDCU_ID},
  {_LCD_C_MAIN_2_OXY_UPPER,         0,3,8,          (char*)"Oxy Up  :      mg/L"},
  {_LCD_C_MAIN_2_OXY_LOWER,         0,4,8,          (char*)"Oxy Low :      mg/L"},
  {_LCD_C_MAIN_2_SALINITY,          0,5,8,          (char*)"Salinity:      ppt"},
  {_LCD_C_MAIN_2_TIME_DELAY,        0,6,8,          (char*)"Delay   :      Min"},
  {_LCD_C_MAIN_2_TIME_WARNING,      0,7,8,          (char*)"Led_W   :      Min"},
//  {_LCD_C_MAIN_2_STATE,             0,6,8,          (char*)" Oxy1|Oxy2|Oxy3|Oxy4"},
//  {_LCD_C_MAIN_2_STATE_OXY_1_ON,    0,7,14,         (char*)"  ON|"},
//  {_LCD_C_MAIN_2_STATE_OXY_1_OFF,   0,7,14,         (char*)" OFF|"},
//  {_LCD_C_MAIN_2_STATE_OXY_2_ON,    0,7,44,         (char*)"  ON|"},
//  {_LCD_C_MAIN_2_STATE_OXY_2_OFF,   0,7,44,         (char*)" OFF|"},
//  {_LCD_C_MAIN_2_STATE_OXY_3_ON,    0,7,74,         (char*)"  ON|"},
//  {_LCD_C_MAIN_2_STATE_OXY_3_OFF,   0,7,74,         (char*)" OFF|"},
//  {_LCD_C_MAIN_2_STATE_OXY_4_ON,    0,7,104,        (char*)"  ON"},
//  {_LCD_C_MAIN_2_STATE_OXY_4_OFF,   0,7,104,        (char*)" OFF"},
};

LCD_Uint_Display                    oLCD_U_Main_2[]=
{
  {_LCD_U_MAIN_2_OXY_UPPER,         0,3,63,          &sParamCtrlOxy.Oxy_Upper},
  {_LCD_U_MAIN_2_OXY_LOWER,         0,4,63,          &sParamCtrlOxy.Oxy_Lower},
  {_LCD_U_MAIN_2_SALINITY,          0,5,63,          &sParamMeasure.Salinity},
  {_LCD_U_MAIN_2_TIME_DELAY,        0,6,63,          &sParamCtrlOxy.TimeDelay},
  {_LCD_U_MAIN_2_TIME_WARNING,      0,7,63,          &sParamCtrlOxy.TimeWarning},
};

/*===== Object LCD =============== Screen Login =================*/
LCD_Char_Display                    oLCD_C_Login[]=
{
  {_LCD_C_LOGIN_ID,                0,2,8,          (char*)"ID  :"},
  {_LCD_C_LOGIN_ID_VALUE,          0,2,40,         (char*)aDCU_ID},
  {_LCD_C_LOGIN_PASSWORD,          0,4,8,          (char*)"Password:"},
  {_LCD_C_LOGIN_PASS_1,            0,5,68,         (char*)"*               "},
  {_LCD_C_LOGIN_PASS_2,            0,5,60,         (char*)"   *            "},
  {_LCD_C_LOGIN_PASS_3,            0,5,52,         (char*)"      *         "},
  {_LCD_C_LOGIN_PASS_4,            0,5,44,         (char*)"         *      "},
  {_LCD_C_LOGIN_PASS_5,            0,5,36,         (char*)"            *   "},
  {_LCD_C_LOGIN_PASS_6,            0,5,28,         (char*)"               *"},
  
  {_LCD_C_LOGIN_ENTER,             0,7,23,         (char*)"Enter Password"},
  {_LCD_C_LOGIN_LOGIN,             0,7,23,         (char*)"Enter to Login"},
  {_LCD_C_LOGIN_WRONG,             0,7,23,         (char*)"  Incorrect!  "},
};

LCD_Uint_Display                    oLCD_U_Login[]=
{
  {_LCD_U_LOGIN_PASSWORD_1,        0,4,68,         (uint16_t*)&sMenuStamp.sPassLogin.Pass1},
  {_LCD_U_LOGIN_PASSWORD_2,        0,4,78,         (uint16_t*)&sMenuStamp.sPassLogin.Pass2},
  {_LCD_U_LOGIN_PASSWORD_3,        0,4,88,         (uint16_t*)&sMenuStamp.sPassLogin.Pass3},
  {_LCD_U_LOGIN_PASSWORD_4,        0,4,98,         (uint16_t*)&sMenuStamp.sPassLogin.Pass4},
  {_LCD_U_LOGIN_PASSWORD_5,        0,4,108,        (uint16_t*)&sMenuStamp.sPassLogin.Pass5},
  {_LCD_U_LOGIN_PASSWORD_6,        0,4,118,        (uint16_t*)&sMenuStamp.sPassLogin.Pass6},
};

/*===== Object LCD ===============  Setting Main =================*/
LCD_Char_Display                    oLCD_C_Setting_Main[]=
{
  {_LCD_C_SETTING_MAIN_TITLE,          0,2,45,          (char*)"SETTING"},
  {_LCD_C_SETTING_MAIN_CYCLE,          0,3,8,           (char*)"[1] Duty Cycle "},
  {_LCD_C_SETTING_MAIN_CALIB,          0,4,8,           (char*)"[2] Calib Sensor "},
  {_LCD_C_SETTING_MAIN_PARAMETER,      0,5,8,           (char*)"[3] Param Control"},
  {_LCD_C_SETTING_MAIN_PASSWORD,       0,6,8,           (char*)"[4] Change Password"},
  {_LCD_C_SETTING_MAIN_PLEASE,         0,7,24,          (char*)"Please Choose!"}
};

/*===== Object LCD ================ Setting Calib ================*/
LCD_Char_Display                    oLCD_C_Setting_Calib[]=
{
  {_LCD_C_SETTING_CALIB_TITLE,          0,2,12,          (char*)"SETTING CALIB OXY"},
  {_LCD_C_SETTING_CALIB_100_CALIB,      0,3,8,           (char*)"[1] Calib Oxy 100%"},
  {_LCD_C_SETTING_CALIB_SALINITY,       0,4,8,           (char*)"[2] Sali:       ppt"},
  {_LCD_C_SETTING_CALIB_TEMPERATURE,    0,5,8,           (char*)"[3] Temp:        C"},
  {_LCD_C_SETTING_CALIB_RESET_CALIB,    0,6,8,           (char*)"[4] Reset Calib"},
  {_LCD_C_SETTING_CALIB_PLEASE,         0,7,24,          (char*)"Please Choose!"},
  {_LCD_C_SETTING_CALIB_SELECT,         0,7,24,          (char*)"Select Up/Down"},
  {_LCD_C_SETTING_CALIB_ENTER_CALIB,    0,7,24,          (char*)"Enter to Calib"},
  {_LCD_C_SETTING_CALIB_CONFIRM_RESET,  0,7,24,          (char*)"Enter to Reset"},
};

LCD_Uint_Display                    oLCD_U_Setting_Calib[]=
{
  {_LCD_U_SETTING_CALIB_SALINITY,       0,4,65,          &sParamMeasure.Salinity},
  {_LCD_U_SETTING_CALIB_TEMPERATURE,    0,5,65,          &sParamMeasure.Temp},
};
/*===== Object LCD ================ Setting Duty Cycle================*/
LCD_Char_Display                    oLCD_C_Setting_Cycle[]=
{
  {_LCD_C_SETTING_CYCLE_TITLE,          0,2,10,          (char*)"SETTING DUTY CYLCE"},
  {_LCD_C_SETTING_CYCLE_RUN,            0,3,8,           (char*)"[1] Cycle:      s"},
  {_LCD_C_SETTING_CYCLE_FREE,           0,4,8,           (char*)"[2] Free :      s"},
  {_LCD_C_SETTING_CYCLE_SLAVE,          0,5,8,           (char*)"[3] Send Slave   "},
  {_LCD_C_SETTING_CYCLE_PLEASE,         0,7,24,          (char*)"Please Choose!"},
  {_LCD_C_SETTING_CYCLE_CLICK,          0,7,24,          (char*)"Select Up/Down"},
  {_LCD_C_SETTING_CYCLE_SUCCESS,        0,7,24,          (char*)" Send Success "},
};

LCD_Uint_Display                    oLCD_U_Setting_Cycle[]=
{
  {_LCD_U_SETTING_CYCLE_RUN,            0,3,70,         &sMenuStamp.sTimeSlave.RunCtrl},
  {_LCD_U_SETTING_CYCLE_FREE,           0,4,70,         &sMenuStamp.sTimeSlave.FreeCtrl},
};

/*===== Object LCD ================ Setting Salinity =================*/
LCD_Char_Display                    oLCD_C_Setting_Parameter[]=
{
  {_LCD_C_SETTING_PARAMETER,             0,2,10,            (char*)"SETTING PARAMETER"},
  {_LCD_C_SETTING_PARAMETER_OXY_UPPER,   0,3,8,             (char*)"[1] O_Up :     mg/L"},
  {_LCD_C_SETTING_PARAMETER_OXY_LOWER,   0,4,8,             (char*)"[2] O_Low:     mg/L"},
  {_LCD_C_SETTING_PARAMETER_TIME_DELAY,  0,5,8,             (char*)"[3] Delay:     Min"},
  {_LCD_C_SETTING_PARAMETER_WARNING,     0,6,8,             (char*)"[4] Led_W:     Min"},
  {_LCD_C_SETTING_PARAMETER_PLEASE,      0,7,24,            (char*)"Please Choose!"},
  {_LCD_C_SETTING_PARAMETER_CLICK,       0,7,24,            (char*)"Select Up/Down"},
};

LCD_Uint_Display                    oLCD_U_Setting_Parameter[]=
{
  {_LCD_U_SETTING_PARAMETER_OXY_UPPER,  0,3,67,              &sMenuStamp.OxyUpper},
  {_LCD_U_SETTING_PARAMETER_OXY_LOWER,  0,4,67,              &sMenuStamp.OxyLower},
  {_LCD_U_SETTING_PARAMETER_TIME_DELAY, 0,5,67,              &sMenuStamp.TimeDelay}, 
  {_LCD_U_SETTING_PARAMETER_WARNING,    0,6,67,              &sMenuStamp.TimeWarning},
};
/*==== Object LCD ================= Setting Password ==================*/
LCD_Char_Display                    oLCD_C_Setting_Password[]=
{
  {_LCD_C_SETTING_PASSWORD_TITLE,       0,2,15,         (char*)"SETTING PASSWORD"},
  {_LCD_C_SETTING_PASSWORD,             0,4,8,          (char*)"Password:"},
  {_LCD_C_SETTING_PASSWORD_1,           0,5,68,         (char*)"*               "},
  {_LCD_C_SETTING_PASSWORD_2,           0,5,60,         (char*)"   *            "},
  {_LCD_C_SETTING_PASSWORD_3,           0,5,52,         (char*)"      *         "},
  {_LCD_C_SETTING_PASSWORD_4,           0,5,44,         (char*)"         *      "},
  {_LCD_C_SETTING_PASSWORD_5,           0,5,36,         (char*)"            *   "},
  {_LCD_C_SETTING_PASSWORD_6,           0,5,28,         (char*)"               *"},
  {_LCD_C_SETTING_PASSWORD_SUCCESS,     0,5,57,         (char*)"   Success  "},
  {_LCD_C_SETTING_PASSWORD_SAVE,        0,7,10,         (char*)"   Enter to Save  "},
  {_LCD_C_SETTING_PASSWORD_ENTER,       0,7,10,         (char*)"Enter new Password"},
  {_LCD_C_SETTING_PASSWORD_CLICK,       0,7,10,         (char*)" Click Enter/Exit "},
};

LCD_Uint_Display                    oLCD_U_Setting_Password[]=
{
  {_LCD_U_SETTING_PASSWORD_1,        0,4,68,         (uint16_t*)&sMenuStamp.sPassWord.Pass1},
  {_LCD_U_SETTING_PASSWORD_2,        0,4,78,         (uint16_t*)&sMenuStamp.sPassWord.Pass2},
  {_LCD_U_SETTING_PASSWORD_3,        0,4,88,         (uint16_t*)&sMenuStamp.sPassWord.Pass3},
  {_LCD_U_SETTING_PASSWORD_4,        0,4,98,         (uint16_t*)&sMenuStamp.sPassWord.Pass4},
  {_LCD_U_SETTING_PASSWORD_5,        0,4,108,        (uint16_t*)&sMenuStamp.sPassWord.Pass5},
  {_LCD_U_SETTING_PASSWORD_6,        0,4,118,        (uint16_t*)&sMenuStamp.sPassWord.Pass6},
};
/*===== Object LCD ================ Screen Calib Oxy ================*/
LCD_Char_Display                    oLCD_C_Calib_Oxy[]=
{
  {_LCD_C_CALIB_OXY_100,                0,2,25,                 (char*)"CALIB OXY 100"},
  {_LCD_C_CALIB_OXY_SALINITY,           0,2,20,                 (char*)"CALIB SALINITY"},
  {_LCD_C_CALIB_OXY_TEMPERATURE,        0,2,13,                 (char*)"CALIB TEMPERATURE"},
  {_LCD_C_CALIB_OXY_RESET,              0,2,30,                 (char*)"RESET CALIB"},
  {_LCD_C_CALIB_OXY_STATE_LOAD,         0,4,35,                 (char*)"Loading..."},
  {_LCD_C_CALIB_OXY_STATE_DONE,         0,4,35,                 (char*)"   Done!  "},
  {_LCD_C_CALIB_OXY_STATE_ERROR,        0,4,35,                 (char*)"  Error!  "},
  {_LCD_C_CALIB_OXY_PLEASE_WAIT,        0,7,25,                 (char*)"Please Wait! "},
  {_LCD_C_CALIB_OXY_PLEASE_CLICK,       0,7,15,                 (char*)"Click Enter/Exit"},
};

/*============================ Function Struct ===============================*/

static uint8_t fevent_lcd_entry(uint8_t event)
{
    GLCD_ClearScreen();
    GLCD_WriteString(cTime_RTC,0,0,1);
    LCD128X64_DrawLine(11,0,11,127,1);
    LCD128X64_DrawLine(0,107,10,107,1);
    return 1;
}

static uint8_t fevent_lcd_get_time_rtc(uint8_t event)
{
    static uint8_t check_RTC = 0;
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	if(sTime.Seconds != check_RTC)
	{
        check_RTC = sTime.Seconds;
        Time_To_Char(cTime_RTC);
        Display_Time();
    }
    fevent_enable(sEventAppLcd, event);
    return 1;
}
/*============== Function Display Check Uint ===========*/
void DLCD_Setting_Password_Alter(void)
{
    static uint16_t stampPass1 = 0;
    static uint16_t stampPass2 = 0;
    static uint16_t stampPass3 = 0;
    static uint16_t stampPass4 = 0;
    static uint16_t stampPass5 = 0;
    static uint16_t stampPass6 = 0; 
    
    if(*oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_1].Object != stampPass1)
    {
        stampPass1 = *oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_1].Object;
        oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_1].Status = 1;
    }
    
    if(*oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_2].Object != stampPass2)
    {
        stampPass2 = *oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_2].Object;
        oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_2].Status = 1;
    }
    
    if(*oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_3].Object != stampPass3)
    {
        stampPass3 = *oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_3].Object;
        oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_3].Status = 1;
    }
    
    if(*oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_4].Object != stampPass4)
    {
        stampPass4 = *oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_4].Object;
        oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_4].Status = 1;
    }
    
    if(*oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_5].Object != stampPass5)
    {
        stampPass5 = *oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_5].Object;
        oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_5].Status = 1;
    }
    
    if(*oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_6].Object != stampPass6)
    {
        stampPass6 = *oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_6].Object;
        oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_6].Status = 1;
    }
    
    DLCD_Setting_Password();
}


void DLCD_Setting_Parameter_Alter(void)
{
    static uint16_t stampTimeDelay  = 0;
    static uint16_t stampTimeWarning= 0;
    static uint16_t stampOxyUpper = 0;
    static uint16_t stampOxyLower = 0;
    
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_TIME_DELAY].Object != stampTimeDelay)
    {
        stampTimeDelay = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_TIME_DELAY].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_TIME_DELAY].Status = 1;
    }
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_WARNING].Object != stampTimeWarning)
    {
        stampTimeWarning = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_WARNING].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_WARNING].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER].Object != stampOxyUpper)
    {
        stampTimeDelay = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER].Object != stampOxyLower)
    {
        stampTimeDelay = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER].Status = 1;
    }
    
    DLCD_Setting_Parameter();
}

void        DLCD_Setting_Calib_Alter(void)
{
    static uint16_t stampSalinity = 0;
    static uint16_t stampTemperature = 0;
    
    if(*oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Object != stampSalinity)
    {
        stampSalinity = *oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Object;
        oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Status = 1;
    }
    
    if(*oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Object != stampTemperature)
    {
        stampTemperature = *oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Object;
        oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Status = 1;
    }
    
    DLCD_Setting_Calib();
}

void DLCD_Setting_Cycle_Alter(void)
{
    static uint16_t stampRun = 0;
    static uint16_t stampFree = 0;
    
    if(*oLCD_U_Setting_Cycle[_LCD_U_SETTING_CYCLE_RUN].Object != stampRun)
    {
        stampRun = *oLCD_U_Setting_Cycle[_LCD_U_SETTING_CYCLE_RUN].Object;
        oLCD_U_Setting_Cycle[_LCD_U_SETTING_CYCLE_RUN].Status = 1;
    }
    
    if(*oLCD_U_Setting_Cycle[_LCD_U_SETTING_CYCLE_FREE].Object != stampFree)
    {
        stampFree = *oLCD_U_Setting_Cycle[_LCD_U_SETTING_CYCLE_FREE].Object;
        oLCD_U_Setting_Cycle[_LCD_U_SETTING_CYCLE_FREE].Status = 1;
    }
    
    DLCD_Setting_Cycle();
}


void DLCD_Login_Alter(void)
{
    static uint16_t stampPass1 = 0;
    static uint16_t stampPass2 = 0;
    static uint16_t stampPass3 = 0;
    static uint16_t stampPass4 = 0;
    static uint16_t stampPass5 = 0;
    static uint16_t stampPass6 = 0;
    
    if(*oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_1].Object != stampPass1)
    {
        stampPass1 = *oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_1].Object;
        oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_1].Status = 1;
    }
    
    if(*oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_2].Object != stampPass2)
    {
        stampPass2 = *oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_2].Object;
        oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_2].Status = 1;
    }
    
    if(*oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_3].Object != stampPass3)
    {
        stampPass3 = *oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_3].Object;
        oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_3].Status = 1;
    }
    
    if(*oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_4].Object != stampPass4)
    {
        stampPass4 = *oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_4].Object;
        oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_4].Status = 1;
    }
    
    if(*oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_5].Object != stampPass5)
    {
        stampPass5 = *oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_5].Object;
        oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_5].Status = 1;
    }
    
    if(*oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_6].Object != stampPass6)
    {
        stampPass6 = *oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_6].Object;
        oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_6].Status = 1;
    }
    
    DLCD_Login();
}

void DLCD_Main_2_Alter(void)
{
//    static uint8_t stampStateOxy1 = 0;
//    static uint8_t stampStateOxy2 = 0;
//    static uint8_t stampStateOxy3 = 0;
//    static uint8_t stampStateOxy4 = 0;
    static uint8_t stampSalinity = 0;
    static uint8_t stampOxyUpper = 0;
    static uint8_t stampOxyLower = 0;
    static uint8_t stampTimeDelay = 0;
    static uint8_t stampTimeWarning = 0;
    
//    if(stampStateOxy1 != sStateCtrlOxy.aStateOxy[_OXY_1])
//    {
//        if(sStateCtrlOxy.aStateOxy[_OXY_1] == _OFF_OXY) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_1_OFF].Status = 1;
//        else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_1_ON].Status = 1;
//        stampStateOxy1 = sStateCtrlOxy.aStateOxy[_OXY_1];
//    }
//    
//    if(stampStateOxy2 != sStateCtrlOxy.aStateOxy[_OXY_2])
//    {
//        if(sStateCtrlOxy.aStateOxy[_OXY_2] == _OFF_OXY) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_2_OFF].Status = 1;
//        else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_2_ON].Status = 1;
//        stampStateOxy2 = sStateCtrlOxy.aStateOxy[_OXY_2];
//    }
//    
//    if(stampStateOxy3 != sStateCtrlOxy.aStateOxy[_OXY_3])
//    {
//        if(sStateCtrlOxy.aStateOxy[_OXY_3] == _OFF_OXY) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_3_OFF].Status = 1;
//        else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_3_ON].Status = 1;
//        stampStateOxy3 = sStateCtrlOxy.aStateOxy[_OXY_3];
//    }
//    
//    if(stampStateOxy4 != sStateCtrlOxy.aStateOxy[_OXY_4])
//    {
//        if(sStateCtrlOxy.aStateOxy[_OXY_4] == _OFF_OXY) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_4_OFF].Status = 1;
//        else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_4_ON].Status = 1;
//        stampStateOxy4 = sStateCtrlOxy.aStateOxy[_OXY_4];
//    }
    
    if(stampSalinity != sParamMeasure.Salinity)
    {
        oLCD_U_Main_2[_LCD_U_MAIN_2_SALINITY].Status = 1;
        stampSalinity = sParamMeasure.Salinity;
    }
    
    if(stampOxyUpper != sParamCtrlOxy.Oxy_Upper)
    {
        oLCD_U_Main_2[_LCD_U_MAIN_2_OXY_UPPER].Status = 1;
        stampOxyUpper = sParamCtrlOxy.Oxy_Upper;
    }
    
    if(stampOxyLower != sParamCtrlOxy.Oxy_Lower)
    {
        oLCD_U_Main_2[_LCD_U_MAIN_2_OXY_LOWER].Status = 1;
        stampOxyLower = sParamCtrlOxy.Oxy_Lower;
    }
    
    if(stampTimeDelay != sParamCtrlOxy.TimeDelay)
    {
        oLCD_U_Main_2[_LCD_U_MAIN_2_TIME_DELAY].Status = 1;
        stampTimeDelay = sParamCtrlOxy.TimeDelay;
    }

    if(stampTimeWarning != sParamCtrlOxy.TimeWarning)
    {
        oLCD_U_Main_2[_LCD_U_MAIN_2_TIME_WARNING].Status = 1;
        stampTimeWarning = sParamCtrlOxy.TimeWarning;
    }
    
    DLCD_Main_2();
}


void DLCD_Main_1_Alter(void)
{
    static uint16_t stampTemp = 0;
    static uint16_t stampOxyMgL = 0;
    static uint16_t stampOxyPercent = 0;
    static uint16_t stamppHWater = 0;
    static uint8_t stampStateOxy1 = 0;
    static uint8_t stampStateOxy2 = 0;
    static uint8_t stampStateOxy3 = 0;
    static uint8_t stampStateOxy4 = 0;
    
    if(*oLCD_U_Main_1[_LCD_U_MAIN_1_TEMP].Object != stampTemp)
    {
        stampTemp = *oLCD_U_Main_1[_LCD_U_MAIN_1_TEMP].Object;
        oLCD_U_Main_1[_LCD_U_MAIN_1_TEMP].Status = 1;
    }
    
    if(*oLCD_U_Main_1[_LCD_U_MAIN_1_OXY_MG_L].Object != stampOxyMgL)
    {
        stampOxyMgL = *oLCD_U_Main_1[_LCD_U_MAIN_1_OXY_MG_L].Object;
        oLCD_U_Main_1[_LCD_U_MAIN_1_OXY_MG_L].Status = 1;
    }
    
    if(*oLCD_U_Main_1[_LCD_U_MAIN_1_OXY_PERCENT].Object != stampOxyPercent)
    {
        stampOxyPercent = *oLCD_U_Main_1[_LCD_U_MAIN_1_OXY_PERCENT].Object;
        oLCD_U_Main_1[_LCD_U_MAIN_1_OXY_PERCENT].Status = 1;
    }
    
    if(*oLCD_U_Main_1[_LCD_U_MAIN_1_PH_WATER].Object != stamppHWater)
    {
        stamppHWater = *oLCD_U_Main_1[_LCD_U_MAIN_1_PH_WATER].Object;
        oLCD_U_Main_1[_LCD_U_MAIN_1_PH_WATER].Status = 1;
    }
    
    if(stampStateOxy1 != sStateCtrlOxy.aStateOxy[_OXY_1])
    {
        stampStateOxy1 = sStateCtrlOxy.aStateOxy[_OXY_1];
        
        if(sStateCtrlOxy.aStateOxy[_OXY_1] == _OFF_OXY) 
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_1_OFF].Status = 1;
        else 
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_1_ON].Status = 1;
    }
    
    if(stampStateOxy2 != sStateCtrlOxy.aStateOxy[_OXY_2])
    {
        stampStateOxy2 = sStateCtrlOxy.aStateOxy[_OXY_2];
        
        if(sStateCtrlOxy.aStateOxy[_OXY_2] == _OFF_OXY) 
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_2_OFF].Status = 1;
        else 
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_2_ON].Status = 1;
    }
    
    if(stampStateOxy3 != sStateCtrlOxy.aStateOxy[_OXY_3])
    {
        stampStateOxy3 = sStateCtrlOxy.aStateOxy[_OXY_3];
        
        if(sStateCtrlOxy.aStateOxy[_OXY_3] == _OFF_OXY) 
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_3_OFF].Status = 1;
        else 
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_3_ON].Status = 1;
    }
    
    if(stampStateOxy4 != sStateCtrlOxy.aStateOxy[_OXY_4])
    {
        stampStateOxy4 = sStateCtrlOxy.aStateOxy[_OXY_4];
        
        if(sStateCtrlOxy.aStateOxy[_OXY_4] == _OFF_OXY) 
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_4_OFF].Status = 1;
        else 
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_4_ON].Status = 1;
    }
    
    DLCD_Main_1();
}

/*============== Function Display Entry ============*/
void DLCD_Calib_Oxy_Entry(void)
{
    oLCD_C_Calib_Oxy[_LCD_C_CALIB_OXY_STATE_LOAD].Status = 1;
    oLCD_C_Calib_Oxy[_LCD_C_CALIB_OXY_PLEASE_WAIT].Status = 1;
    
    DLCD_Calib_Oxy();
}

void DLCD_Setting_Password_Entry(void)
{
    oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_TITLE].Status = 1;
    oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD].Status = 1;
    oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_1].Status = 1;
    oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_ENTER].Status = 1;

    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_1].Status = 1;
    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_2].Status = 1;
    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_3].Status = 1;
    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_4].Status = 1;
    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_5].Status = 1;
    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_6].Status = 1;
    
    DLCD_Setting_Password();
}

void DLCD_Setting_Parameter_Entry(void)
{
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_UPPER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_LOWER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_TIME_DELAY].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_WARNING].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PLEASE].Status = 1;
    
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER].Status = 1;
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER].Status = 1;
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_TIME_DELAY].Status = 1;
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_WARNING].Status = 1;

    DLCD_Setting_Parameter();
    Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_UPPER].Rol);
}

void DLCD_Setting_Cycle_Entry(void)
{
    oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_TITLE].Status = 1;
    oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_RUN].Status = 1;
    oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_FREE].Status = 1;
    oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_SLAVE].Status = 1;
    oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_PLEASE].Status = 1;
    
    oLCD_U_Setting_Cycle[_LCD_U_SETTING_CYCLE_RUN].Status = 1;
    oLCD_U_Setting_Cycle[_LCD_U_SETTING_CYCLE_FREE].Status = 1;
    
    DLCD_Setting_Cycle();
    Display_Control_Setting_Left(oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_RUN].Rol);
}

void DLCD_Setting_Calib_Entry(void)
{
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TITLE].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_100_CALIB].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_SALINITY].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TEMPERATURE].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_RESET_CALIB].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_PLEASE].Status = 1;
    
    oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Status = 1;
    oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Status = 1;
    
    oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Object = &sParamMeasure.Salinity;
    oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Object = &sParamMeasure.Temp;
    
    DLCD_Setting_Calib();
    Display_Control_Setting_Left(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_100_CALIB].Rol);
}

void DLCD_Setting_Main_Entry(void)
{
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_TITLE].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_CYCLE].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_CALIB].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PARAMETER].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PASSWORD].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PLEASE].Status = 1;
    
    DLCD_Setting_Main();
    Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_CYCLE].Rol);
}


void DLCD_Login_Entry(void)
{
    oLCD_C_Login[_LCD_C_LOGIN_ID].Status = 1;
    oLCD_C_Login[_LCD_C_LOGIN_ID_VALUE].Status = 1;
    oLCD_C_Login[_LCD_C_LOGIN_PASSWORD].Status = 1;
    oLCD_C_Login[_LCD_C_LOGIN_PASS_1].Status = 1;
    oLCD_C_Login[_LCD_C_LOGIN_ENTER].Status = 1;

    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_1].Status = 1;
    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_2].Status = 1;
    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_3].Status = 1;
    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_4].Status = 1;
    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_5].Status = 1;
    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_6].Status = 1;
    
    DLCD_Login();
}


void DLCD_Main_2_Entry(void)
{
    oLCD_C_Main_2[_LCD_C_MAIN_2_ID].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_ID_VALUE].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_OXY_UPPER].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_OXY_LOWER].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_SALINITY].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_TIME_DELAY].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_TIME_WARNING].Status = 1;
//    if(sStateCtrlOxy.aStateOxy[_OXY_1] == _OFF_OXY) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_1_OFF].Status = 1;
//    else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_1_ON].Status = 1;
//    
//    if(sStateCtrlOxy.aStateOxy[_OXY_2] == _OFF_OXY) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_2_OFF].Status = 1;
//    else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_2_ON].Status = 1;
//
//    if(sStateCtrlOxy.aStateOxy[_OXY_3] == _OFF_OXY) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_3_OFF].Status = 1;
//    else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_3_ON].Status = 1;
//    
//    if(sStateCtrlOxy.aStateOxy[_OXY_4] == _OFF_OXY) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_4_OFF].Status = 1;
//    else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_4_ON].Status = 1;
    
    oLCD_U_Main_2[_LCD_U_MAIN_2_OXY_UPPER].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_OXY_LOWER].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_SALINITY].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_TIME_DELAY].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_TIME_WARNING].Status = 1;
    
    DLCD_Main_2();
}

void DLCD_Main_1_Entry(void)
{
    oLCD_C_Main_1[_LCD_C_MAIN_1_ID].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_ID_VALUE].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_PH_WATER].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_TEMP].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_OXY_1].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_OXY_2].Status = 1;
    
    if(sStateCtrlOxy.aStateOxy[_OXY_1] == _OFF_OXY) 
      oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_1_OFF].Status = 1;
    else 
      oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_1_ON].Status = 1;
    
    if(sStateCtrlOxy.aStateOxy[_OXY_2] == _OFF_OXY) 
      oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_2_OFF].Status = 1;
    else 
      oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_2_ON].Status = 1;

    if(sStateCtrlOxy.aStateOxy[_OXY_3] == _OFF_OXY) 
      oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_3_OFF].Status = 1;
    else 
      oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_3_ON].Status = 1;
    
    if(sStateCtrlOxy.aStateOxy[_OXY_4] == _OFF_OXY) 
      oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_4_OFF].Status = 1;
    else 
      oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_4_ON].Status = 1;
    
    oLCD_U_Main_1[_LCD_U_MAIN_1_TEMP].Status = 1;
    oLCD_U_Main_1[_LCD_U_MAIN_1_OXY_MG_L].Status = 1;
    oLCD_U_Main_1[_LCD_U_MAIN_1_OXY_PERCENT].Status = 1;
    oLCD_U_Main_1[_LCD_U_MAIN_1_PH_WATER].Status = 1;

    DLCD_Main_1();
    
    GLCD_GoTo(7, 0);
    GLCD_WriteCharacter(0x92, FONT5x7);
    GLCD_GoTo(7, 33);
    GLCD_WriteCharacter(0x93, FONT5x7);
    GLCD_GoTo(7, 68);
    GLCD_WriteCharacter(0x94, FONT5x7);
    GLCD_GoTo(7, 102);
    GLCD_WriteCharacter(0x95, FONT5x7);
}

/*============== Function Handle ==============*/

void        DLCD_Calib_Oxy(void)
{
    for(uint8_t i = 0; i < _LCD_C_CALIB_OXY_END; i++)
    {
        if(oLCD_C_Calib_Oxy[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Calib_Oxy[i].Object,oLCD_C_Calib_Oxy[i].Rol,oLCD_C_Calib_Oxy[i].Col,1);
            oLCD_C_Calib_Oxy[i].Status = 0;
        }
    }
}

void        DLCD_Setting_Password(void)
{
    for(uint8_t i = 0; i < _LCD_C_SETTING_PASSWORD_END; i++)
    {
        if(oLCD_C_Setting_Password[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Setting_Password[i].Object,oLCD_C_Setting_Password[i].Rol,oLCD_C_Setting_Password[i].Col,1);
            oLCD_C_Setting_Password[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_SETTING_PASSWORD_END; i++)
    {
        if(oLCD_U_Setting_Password[i].Status == 1)
        {
           GLCD_Write_Integer_Not_Zero(*oLCD_U_Setting_Password[i].Object,  oLCD_U_Setting_Password[i].Rol,oLCD_U_Setting_Password[i].Col);
           oLCD_U_Setting_Password[i].Status = 0;
        }
    }
}

void        DLCD_Setting_Parameter(void)
{
    for(uint8_t i = 0; i < LCD_C_SETTING_PARAMETER_END; i++)
    {
        if(oLCD_C_Setting_Parameter[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Setting_Parameter[i].Object,oLCD_C_Setting_Parameter[i].Rol,oLCD_C_Setting_Parameter[i].Col,1);
            oLCD_C_Setting_Parameter[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_SETTING_PARAMETER_END; i++)
    {
        if(oLCD_U_Setting_Parameter[i].Status == 1)
        {
           if(i == _LCD_U_SETTING_PARAMETER_TIME_DELAY || i == _LCD_U_SETTING_PARAMETER_WARNING)
           {
            GLCD_Write_Float_Not_Zero(*oLCD_U_Setting_Parameter[i].Object, 0,  oLCD_U_Setting_Parameter[i].Rol,oLCD_U_Setting_Parameter[i].Col);
           }
           else
           {
            GLCD_Write_Float_Not_Zero(*oLCD_U_Setting_Parameter[i].Object, 2,  oLCD_U_Setting_Parameter[i].Rol,oLCD_U_Setting_Parameter[i].Col);
           }
            oLCD_U_Setting_Parameter[i].Status = 0;
        }
    }
}

void        DLCD_Setting_Cycle(void)
{
    for(uint8_t i = 0; i < _LCD_C_SETTING_CYCLE_END; i++)
    {
        if(oLCD_C_Setting_Cycle[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Setting_Cycle[i].Object,oLCD_C_Setting_Cycle[i].Rol,oLCD_C_Setting_Cycle[i].Col,1);
            oLCD_C_Setting_Cycle[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_SETTING_CYCLE_END; i++)
    {
        if(oLCD_U_Setting_Cycle[i].Status == 1)
        {
           GLCD_Write_Float_Not_Zero(*oLCD_U_Setting_Cycle[i].Object,1,  oLCD_U_Setting_Cycle[i].Rol,oLCD_U_Setting_Cycle[i].Col);
           oLCD_U_Setting_Cycle[i].Status = 0;
        }
    }
}

void        DLCD_Setting_Calib(void)
{
    for(uint8_t i = 0; i < _LCD_C_SETTING_CALIB_END; i++)
    {
        if(oLCD_C_Setting_Calib[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Setting_Calib[i].Object,oLCD_C_Setting_Calib[i].Rol,oLCD_C_Setting_Calib[i].Col,1);
            oLCD_C_Setting_Calib[i].Status = 0;
        }
        if(i == _LCD_C_SETTING_CALIB_TEMPERATURE)
        {
            GLCD_GoTo(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TEMPERATURE].Rol, 17*5 + 17 + 1);
            GLCD_WriteCharacter(0x8C, FONT5x7);
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_SETTING_CALIB_END; i++)
    {
        if(oLCD_U_Setting_Calib[i].Status == 1)
        {
             GLCD_Write_Float_Not_Zero(*oLCD_U_Setting_Calib[i].Object,2,  oLCD_U_Setting_Calib[i].Rol,oLCD_U_Setting_Calib[i].Col);
             oLCD_U_Setting_Calib[i].Status = 0;
        }
    }
}


void        DLCD_Setting_Main(void)
{
    for(uint8_t i = 0; i < _LCD_C_SETTING_MAIN_END; i++)
    {
        if(oLCD_C_Setting_Main[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Setting_Main[i].Object,oLCD_C_Setting_Main[i].Rol,oLCD_C_Setting_Main[i].Col,1);
            oLCD_C_Setting_Main[i].Status = 0;
        }
    }
}

void        DLCD_Login(void)
{
    for(uint8_t i = 0; i < _LCD_C_LOGIN_END; i++)
    {
        if(oLCD_C_Login[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Login[i].Object,oLCD_C_Login[i].Rol,oLCD_C_Login[i].Col,1);
            oLCD_C_Login[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_LOGIN_END; i++)
    {
        if(oLCD_U_Login[i].Status == 1)
        {
           GLCD_Write_Integer_Not_Zero(*oLCD_U_Login[i].Object,  oLCD_U_Login[i].Rol,oLCD_U_Login[i].Col);
           oLCD_U_Login[i].Status = 0;
        }
    }
}


void        DLCD_Main_2(void)
{
    for(uint8_t i = 0; i < _LCD_C_MAIN_2_END; i++)
    {
        if(oLCD_C_Main_2[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Main_2[i].Object,oLCD_C_Main_2[i].Rol,oLCD_C_Main_2[i].Col,1);
            oLCD_C_Main_2[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_MAIN_2_END; i++)
    {
        if(oLCD_U_Main_2[i].Status == 1)
        {
           if(i == _LCD_U_MAIN_2_TIME_DELAY || i == _LCD_U_MAIN_2_TIME_WARNING)
                GLCD_Write_Float_Not_Zero(*oLCD_U_Main_2[i].Object,0 ,  oLCD_U_Main_2[i].Rol,oLCD_U_Main_2[i].Col);
           else
                GLCD_Write_Float_Not_Zero(*oLCD_U_Main_2[i].Object,2 ,  oLCD_U_Main_2[i].Rol,oLCD_U_Main_2[i].Col);
           oLCD_U_Main_2[i].Status = 0;
        }
    }
}

void        DLCD_Main_1(void)
{
    for(uint8_t i = 0; i < _LCD_C_MAIN_1_END; i++)
    {
        if(oLCD_C_Main_1[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Main_1[i].Object,oLCD_C_Main_1[i].Rol,oLCD_C_Main_1[i].Col,1);
            oLCD_C_Main_1[i].Status = 0;
            if(i==_LCD_C_MAIN_1_TEMP)
            {
                GLCD_GoTo(oLCD_C_Main_1[_LCD_C_MAIN_1_TEMP].Rol, 14*5 + 14 + 1);
                GLCD_WriteCharacter(0x8C, FONT5x7);
            }
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_MAIN_1_END; i++)
    {
        if(oLCD_U_Main_1[i].Status == 1)
        {
            if(i == _LCD_U_MAIN_1_OXY_PERCENT)
            {
              if(*oLCD_U_Main_1[i].Object > 9999)
              {
                uint16_t stamp = *oLCD_U_Main_1[i].Object/10;
                GLCD_Write_Float_Not_Zero(stamp,1 ,  oLCD_U_Main_1[i].Rol,oLCD_U_Main_1[i].Col);
              }
              else
              {
                GLCD_Write_Float_Not_Zero(*oLCD_U_Main_1[i].Object,2 ,  oLCD_U_Main_1[i].Rol,oLCD_U_Main_1[i].Col);
              }
            }
            else
            {
                GLCD_Write_Float_Not_Zero(*oLCD_U_Main_1[i].Object,2 ,  oLCD_U_Main_1[i].Rol,oLCD_U_Main_1[i].Col);
            }
            oLCD_U_Main_1[i].Status = 0;
        }
    }
}
/*=============================== Function Handle ===============================*/
void Clear_Rol_LCD(uint8_t PageStart, uint8_t PageEnd)
{
    GLCD_ClearArea(PageStart, 0, PageEnd, 127);
}

void Display_Control_Setting_Left(uint8_t Page)
{
    GLCD_ClearArea(3, 0, 6, 7);
    GLCD_GoTo(Page, 0);
    GLCD_WriteCharacter(0x7E, FONT5x7);
}

void Display_Control_Setting_Right(uint8_t Page)
{
    GLCD_ClearArea(3, 120, 6, 127);
    GLCD_GoTo(Page, 120);
    GLCD_WriteCharacter(0x7F, FONT5x7);
}

void Display_Time(void)
{
    static char aData[MAX_BUFFER_TIME]="00/00/00-00:00:00";
    static uint8_t StateBeforeServer = 2;
    static uint8_t StateBeforeMachine = 3;
    static uint8_t StateBeforeSensor = 2;
    for(uint8_t i = 0; i < MAX_BUFFER_TIME; i++)
    {
        if(aData[i] != cTime_RTC[i])
        {
            GLCD_GoTo(0, i*5 + i);
            GLCD_WriteCharacter(cTime_RTC[i], FONT5x7);
            aData[i] = cTime_RTC[i];
        }
    }

    if(StateBeforeServer != sMenuState.ConnectServer)
    {
        if(sMenuState.ConnectServer == 1)
        {
            GLCD_GoTo(0, 20*5 + 20);
            GLCD_WriteCharacter(0x8A, FONT5x7);
        }
        else
        {
            GLCD_GoTo(0, 20*5 + 20);
            GLCD_WriteCharacter(0x8B, FONT5x7);
        }
        
        StateBeforeServer = sMenuState.ConnectServer;
    }        
    
    if(StateBeforeMachine != sStateCtrlOxy.StateMachine)
    {
        if(sStateCtrlOxy.StateMachine == _MACHINE_AUTO)
        {
            GLCD_GoTo(0, 19*5 + 19);
            GLCD_WriteCharacter(0x8D, FONT5x7);
        }
        else if(sStateCtrlOxy.StateMachine == _MACHINE_MAN)
        {
            GLCD_GoTo(0, 19*5 + 19);
            GLCD_WriteCharacter(0x8E, FONT5x7);
        }
        else
        {
            GLCD_GoTo(0, 19*5 + 19);
            GLCD_WriteCharacter(0x8F, FONT5x7);
        }
        
        StateBeforeMachine = sStateCtrlOxy.StateMachine;
    }
    
    if(StateBeforeSensor != sStateCtrlOxy.StateSensorOxy)
    {
        if(sStateCtrlOxy.StateSensorOxy == _OXY_CONNECT)
        {
            GLCD_GoTo(0, 18*5 + 18);
            GLCD_WriteCharacter(0x90, FONT5x7);
        }
        else 
        {
            GLCD_GoTo(0, 18*5 + 18);
            GLCD_WriteCharacter(0x91, FONT5x7);
        }
        
        StateBeforeSensor = sStateCtrlOxy.StateSensorOxy;
    }
}
void Time_To_Char(char cData[])
{
	uint16_t Length=0;
//	if(RTC_Current->Year<10)
//	{
//		cData[Length]='0';
//	}
	Uint_To_Char_Location(cData, sDate.Date, &Length);
	cData[Length++]='/';
	
//	if(RTC_Current->Month<10)
//	{
//		cData[Length]='0';
//	}
	Uint_To_Char_Location(cData, sDate.Month, &Length);
	cData[Length++]='/';
	
//	if(RTC_Current->Date<10)
//	{
//		cData[Length]='0';
//	}
	Uint_To_Char_Location(cData, sDate.Year, &Length);
    
    //	cData[Length++]=' ';
    cData[Length++]='|';
//    cData[Length++]=' ';
	if(sTime.Hours<10)
	{
		cData[Length++]='0';
	}
	Uint_To_Char_Location(cData, sTime.Hours, &Length);
	cData[Length++]=':';

	if(sTime.Minutes<10)
	{
		cData[Length++]='0';
	}
	Uint_To_Char_Location(cData, sTime.Minutes, &Length);
	cData[Length++]=':';

	if(sTime.Seconds<10)
	{
		cData[Length++]='0';
	}
    Uint_To_Char_Location(cData, sTime.Seconds, &Length);
    

    
	while(Length<MAX_BUFFER_TIME)
    {
        cData[Length++] = ' ';
    }
}

void Uint_To_Char_Location(char time[], uint32_t stamp, uint16_t *location)
{
	uint16_t lengthStamp=1;
	uint32_t Division=10;
	while(stamp/Division>=1)
	{
		Division=Division*10;
		(lengthStamp)++;
	}
	int j=lengthStamp+*location -1;
	for(;j>=*location;j--)
	{
	time[j]=stamp%10+48;
	stamp=stamp/10;
	}
	*location=*location + lengthStamp;
}

void Init_AppLcd(void)
{
    GLCD_Initialize();
}

uint8_t AppLcd_Task(void)
{
    uint8_t i = 0;
    uint8_t Result =  false;
    
    for(i = 0; i < _EVENT_LCD_END; i++)
    {
        if(sEventAppLcd[i].e_status == 1)
        {
            Result = true; 
            
            if((sEventAppLcd[i].e_systick == 0) ||
               ((HAL_GetTick() - sEventAppLcd[i].e_systick) >= sEventAppLcd[i].e_period))
            {
                sEventAppLcd[i].e_status = 0; //Disable event
                sEventAppLcd[i].e_systick= HAL_GetTick();
                sEventAppLcd[i].e_function_handler(i);
            }
        }
    }
    
    
    return Result;
}


