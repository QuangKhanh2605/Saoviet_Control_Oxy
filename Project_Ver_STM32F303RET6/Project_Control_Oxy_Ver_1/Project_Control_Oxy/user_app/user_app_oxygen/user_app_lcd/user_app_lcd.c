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
  {_EVENT_LCD_ENTRY,            1, 5, 5,        fevent_lcd_entry},
  {_EVENT_LCD_GET_TIME_RTC,     0, 5, 250,      fevent_lcd_get_time_rtc},
};

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
char cTime_RTC[MAX_BUFFER_TIME]="00/00/00-00:00:00";
/*===== Object LCD =============== MAIN Screen 1 =================*/
LCD_Char_Display                    oLCD_C_Main_1[]=
{
  {_LCD_C_MAIN_1_ID,                0,2,8,          (char*)"ID  : "},
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
  {_LCD_U_MAIN_1_PH_WATER,          0,3,48,          &sParamCtrlPH.pH_Measure},
  {_LCD_U_MAIN_1_TEMP,              0,4,48,          &sParamMeasure.Temp},
  {_LCD_U_MAIN_1_OXY_MG_L,          0,5,48,          &sParamMeasure.Oxy_Mg_L},
  {_LCD_U_MAIN_1_OXY_PERCENT,       0,6,42,          &sParamMeasure.Oxy_Percent},
};

/*===== Object LCD =============== MAIN_Screen 2 =================*/
LCD_Char_Display                    oLCD_C_Main_2[]=
{
  {_LCD_C_MAIN_2_TITLE,             0,2,10,         (char*)"PARAMETER SETTINGS  "},
  {_LCD_C_MAIN_2_SALINITY,          0,3,0,          (char*)"1.Salinity:      ppt "},
  {_LCD_C_MAIN_2_OXY_UPPER,         0,4,0,          (char*)"2.Oxy Up  :      mg/L"},
  {_LCD_C_MAIN_2_OXY_LOWER,         0,5,0,          (char*)"3.Oxy Low :      mg/L"},
  {_LCD_C_MAIN_2_OXY_UPPER_WARNING, 0,6,0,          (char*)"4.Upper Wa:      mg/L"},
  {_LCD_C_MAIN_2_OXY_LOWER_WARNING, 0,7,0,          (char*)"5.Lower Wa:      mg/L"},
};

LCD_Uint_Display                    oLCD_U_Main_2[]=
{
  {_LCD_U_MAIN_2_SALINITY,          0,3,66,          &sParamMeasure.Salinity},
  {_LCD_U_MAIN_2_OXY_UPPER,         0,4,66,          &sParamCtrlOxy.Oxy_Upper},
  {_LCD_U_MAIN_2_OXY_LOWER,         0,5,66,          &sParamCtrlOxy.Oxy_Lower},
  {_LCD_U_MAIN_2_OXY_UPPER_WARNING, 0,6,66,          &sParamCtrlOxy.Oxy_Upper_Warning},
  {_LCD_U_MAIN_2_OXY_LOWER_WARNING, 0,7,66,          &sParamCtrlOxy.Oxy_Lower_Warning},
};

/*===== Object LCD ============== MAIN Screen 2 tab 2 =============*/
LCD_Char_Display                    oLCD_C_Main_2_Tab_2[]=
{
  {_LCD_C_MAIN_2_TAB_2_PH_UPPER,    0,3,0,          (char*)"6.PoH Up  :      pH  "},
  {_LCD_C_MAIN_2_TAB_2_PH_LOWER,    0,4,0,          (char*)"7.PoH Low :      pH  "},
};

LCD_Uint_Display                    oLCD_U_Main_2_Tab_2[]=
{
  {_LCD_U_MAIN_2_TAB_2_PH_UPPER,    0,3,66,          &sParamCtrlPH.pH_Upper},
  {_LCD_U_MAIN_2_TAB_2_PH_LOWER,    0,4,66,          &sParamCtrlPH.pH_Lower},
};

/*===== Object LCD =============== Screen Screen 3 =================*/
LCD_Char_Display                    oLCD_C_Main_3[]=
{
  {_LCD_C_MAIN_3_TITLE,             0,2,10,         (char*)"  STATUS WARNING   "},
  {_LCD_C_MAIN_3_OXY_CONNECT,       0,3,0,          (char*)"1.Oxy_Connect:      "},
  {_LCD_C_MAIN_3_OXY_MEASURE,       0,4,0,          (char*)"2.Oxy_Measure:      "},
  {_LCD_C_MAIN_3_OXY_PUMP,          0,5,0,          (char*)"3.Oxy_Pump   :      "},
  {_LCD_C_MAIN_3_PH_CONNECT,        0,6,0,          (char*)"4.pH_Connect :      "},
  {_LCD_C_MAIN_3_PH_MEASURE,        0,7,0,          (char*)"5.pH_Measure :      "},
};

/*===== Object LCD =============== Screen Login =================*/
LCD_Char_Display                    oLCD_C_Login[]=
{
  {_LCD_C_LOGIN_ID,                0,2,8,          (char*)"ID  : "},
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
  {_LCD_C_SETTING_CALIB_SALINITY,       0,3,8,           (char*)"[1] Sali:       ppt"},
  {_LCD_C_SETTING_CALIB_100_CALIB,      0,4,8,           (char*)"[2] Calib Oxy 100%"},
  {_LCD_C_SETTING_CALIB_PH_POINT_686,   0,5,8,           (char*)"[3] Calib pH 6.86"},
  {_LCD_C_SETTING_CALIB_PH_POINT_918,   0,6,8,           (char*)"[4] Calib pH 9.18"},
//  {_LCD_C_SETTING_CALIB_TEMPERATURE,    0,5,8,           (char*)"[3] Temp:        C"},
  {_LCD_C_SETTING_CALIB_PLEASE,         0,7,24,          (char*)"Please Choose!"},
  {_LCD_C_SETTING_CALIB_SELECT,         0,7,24,          (char*)"Select Up/Down"},
  {_LCD_C_SETTING_CALIB_ENTER_CALIB,    0,7,24,          (char*)"Enter to Calib"},
  {_LCD_C_SETTING_CALIB_CONFIRM_RESET,  0,7,24,          (char*)"Enter to Reset"},
};

LCD_Uint_Display                    oLCD_U_Setting_Calib[]=
{
  {_LCD_U_SETTING_CALIB_SALINITY,       0,3,65,          &sParamMeasure.Salinity},
//  {_LCD_U_SETTING_CALIB_TEMPERATURE,    0,5,65,          &sParamMeasure.Temp},
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

/*===== Object LCD ================ Setting Param =================*/
LCD_Char_Display                    oLCD_C_Setting_Parameter[]=
{
  {_LCD_C_SETTING_PARAMETER,             0,2,10,            (char*)"SETTING PARAMETER"},
  {_LCD_C_SETTING_PARAMETER_OXY_UPPER,   0,3,8,             (char*)"[1] O_Up :     mg/L"},
  {_LCD_C_SETTING_PARAMETER_OXY_LOWER,   0,4,8,             (char*)"[2] O_Low:     mg/L"},
  {_LCD_C_SETTING_PARAMETER_OXY_UPPER_W, 0,5,8,             (char*)"[3] Up_W :     mg/L"},
  {_LCD_C_SETTING_PARAMETER_OXY_LOWER_W, 0,6,8,             (char*)"[4] Low_W:     mg/L"},
  {_LCD_C_SETTING_PARAMETER_OXY_PLEASE,  0,7,24,            (char*)"Please Choose!"},
  {_LCD_C_SETTING_PARAMETER_OXY_SELECT,  0,7,24,            (char*)"Select Up/Down"},
};

LCD_Uint_Display                    oLCD_U_Setting_Parameter[]=
{
  {_LCD_U_SETTING_PARAMETER_OXY_UPPER,  0,3,67,              &sMenuStamp.OxyUpper},
  {_LCD_U_SETTING_PARAMETER_OXY_LOWER,  0,4,67,              &sMenuStamp.OxyLower},
  {_LCD_U_SETTING_PARAMETER_OXY_UPPER_W,0,5,67,              &sMenuStamp.OxyUpperWarning},
  {_LCD_U_SETTING_PARAMETER_OXY_LOWER_W,0,6,67,              &sMenuStamp.OxyLowerWarning},
};

/*===== Object LCD ================ Setting Param Tab 2=================*/
LCD_Char_Display                    oLCD_C_Setting_Parameter_Tab_2[]=
{
  {_LCD_C_SETTING_PARAMETER_TAB_2_PH_UPPER,         0,3,8,         (char*)"[5] pH_Up:      pH "},
  {_LCD_C_SETTING_PARAMETER_TAB_2_PH_LOWER,         0,4,8,         (char*)"[6] pH_Lo:      pH "},
  {_LCD_C_SETTING_PARAMETER_TAB_2_PH_TIME_PUMP,     0,5,8,         (char*)"[7] T_Pump:      M "},
  {_LCD_C_SETTING_PARAMETER_TAB_2_PH_TIME_SAMPLING, 0,6,8,         (char*)"[8] T_Samp:      M "},
};

LCD_Uint_Display                    oLCD_U_Setting_Parameter_Tab_2[]=
{
  {_LCD_U_SETTING_PARAMETER_TAB_2_PH_UPPER,         0,3,67,              &sMenuStamp.pH_Upper},
  {_LCD_U_SETTING_PARAMETER_TAB_2_PH_LOWER,         0,4,67,              &sMenuStamp.pH_Lower},
  {_LCD_U_SETTING_PARAMETER_TAB_2_PH_TIME_PUMP,     0,5,73,              &sMenuStamp.pH_TimePump},
  {_LCD_U_SETTING_PARAMETER_TAB_2_PH_TIME_SAMPLING, 0,6,73,              &sMenuStamp.pH_TimeSampling},
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
  {_LCD_C_CALIB_PH_POINT_686,           0,2,25,                 (char*)"CALIB PH 6.86"},
  {_LCD_C_CALIB_PH_POINT_918,           0,2,25,                 (char*)"CALIB PH 9.18"},
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
    fevent_active(sEventAppLcd, _EVENT_LCD_GET_TIME_RTC);
    fevent_active(sEventAppMenu, _EVENT_MENU_ENTRY);
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
    
    if(aDCU_ID[0] != 'S' || aDCU_ID[1] != 'V'  || aDCU_ID[2] != 'W' || aDCU_ID[3] != 'M')
    {
        OnchipFlashPageErase(ADDR_DCUID);
        HAL_Delay(1000);
        Reset_Chip();
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

void DLCD_Setting_Parameter_Tab_2_Alter(void)
{
    static uint16_t stamp_pH_Upper = 0;
    static uint16_t stamp_pH_Lower = 0;
    static uint16_t stamp_pH_TimePump = 0;
    static uint16_t stamp_pH_TimeSampling = 0;
    
    if(*oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_UPPER].Object != stamp_pH_Upper)
    {
        stamp_pH_Upper = *oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_UPPER].Object;
        oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_UPPER].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_LOWER].Object != stamp_pH_Lower)
    {
        stamp_pH_Lower = *oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_LOWER].Object;
        oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_LOWER].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_TIME_PUMP].Object != stamp_pH_TimePump)
    {
        stamp_pH_TimePump = *oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_TIME_PUMP].Object;
        oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_TIME_PUMP].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_TIME_SAMPLING].Object != stamp_pH_TimeSampling)
    {
        stamp_pH_TimeSampling = *oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_TIME_SAMPLING].Object;
        oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_TIME_SAMPLING].Status = 1;
    }
    
    DLCD_Setting_Parameter_Tab_2();
}

void DLCD_Setting_Parameter_Alter(void)
{
    static uint16_t stampOxyUpper = 0;
    static uint16_t stampOxyLower = 0;
    static uint16_t stampOxyUpperWarning = 0;
    static uint16_t stampOxyLowerWarning = 0;

    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER].Object != stampOxyUpper)
    {
        stampOxyUpper = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER].Object != stampOxyLower)
    {
        stampOxyLower = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER_W].Object != stampOxyUpperWarning)
    {
        stampOxyUpperWarning = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER_W].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER_W].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER_W].Object != stampOxyLowerWarning)
    {
        stampOxyLowerWarning = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER_W].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER_W].Status = 1;
    }
    
    DLCD_Setting_Parameter();
}

void        DLCD_Setting_Calib_Alter(void)
{
    static uint16_t stampSalinity = 0;
//    static uint16_t stampTemperature = 0;
    
    if(*oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Object != stampSalinity)
    {
        stampSalinity = *oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Object;
        oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Status = 1;
    }
    
//    if(*oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Object != stampTemperature)
//    {
//        stampTemperature = *oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Object;
//        oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Status = 1;
//    }
    
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

void        DLCD_Main_3_Alter(void)
{
    static uint8_t stampWarningOxyConnect = 0;
    static uint8_t stampWarningOxyMeasure = 0;
    static uint8_t stampWarningOxyPump    = 0;
    static uint8_t stampWarningPHConnect  = 0;
    static uint8_t stampWarningPHMeasure  = 0;
      
    if(stampWarningOxyConnect != sStateCtrlOxy.StateWarningOxy_Connect)
    {
        if(sStateCtrlOxy.StateWarningOxy_Connect == _OXY_STT_OK)
          GLCD_WriteString("  OK",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_CONNECT].Rol, 100, 1);
        else
          GLCD_WriteString("FAIL",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_CONNECT].Rol, 100, 1);
    
        stampWarningOxyConnect = sStateCtrlOxy.StateWarningOxy_Connect;
    }
    
    if(stampWarningOxyMeasure != sStateCtrlOxy.StateWarningOxy_Measure)
    {
        if(sStateCtrlOxy.StateWarningOxy_Measure == _OXY_STT_OK)
          GLCD_WriteString("  OK",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_MEASURE].Rol, 100, 1);
        else
          GLCD_WriteString("FAIL",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_MEASURE].Rol, 100, 1);
    
        stampWarningOxyMeasure = sStateCtrlOxy.StateWarningOxy_Measure;
    }
    
    if(stampWarningOxyPump != sStateCtrlOxy.StateWarningOxy_Pump)
    {
        if(sStateCtrlOxy.StateWarningOxy_Pump == _OXY_STT_OK)
          GLCD_WriteString("  OK",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_PUMP].Rol, 100, 1);
        else
          GLCD_WriteString("FAIL",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_PUMP].Rol, 100, 1);
      
        stampWarningOxyPump = sStateCtrlOxy.StateWarningOxy_Pump;
    }
    
    if(stampWarningPHConnect != sParamCtrlPH.StateWarningPH_Connect)
    {
        if(sParamCtrlPH.StateWarningPH_Connect == _PH_STT_OK)
          GLCD_WriteString("  OK",oLCD_C_Main_3[_LCD_C_MAIN_3_PH_CONNECT].Rol, 100, 1);
        else
          GLCD_WriteString("FAIL",oLCD_C_Main_3[_LCD_C_MAIN_3_PH_CONNECT].Rol, 100, 1);
    
        stampWarningPHConnect = sParamCtrlPH.StateWarningPH_Connect;
    }
    
    if(stampWarningPHMeasure != sParamCtrlPH.StateWarningPH_Measure)
    {
        if(sParamCtrlPH.StateWarningPH_Measure == _PH_STT_OK)
          GLCD_WriteString("  OK",oLCD_C_Main_3[_LCD_C_MAIN_3_PH_MEASURE].Rol, 100, 1);
        else
          GLCD_WriteString("FAIL",oLCD_C_Main_3[_LCD_C_MAIN_3_PH_MEASURE].Rol, 100, 1);
      
        stampWarningPHMeasure = sParamCtrlPH.StateWarningPH_Measure;
    }
}

void DLCD_Main_2_Tab_2_Alter(void)
{
    static uint8_t stamp_pH_Upper = 0;
    static uint8_t stamp_pH_Lower = 0;
    
    if(stamp_pH_Upper != sParamCtrlPH.pH_Upper)
    {
        oLCD_U_Main_2_Tab_2[_LCD_U_MAIN_2_TAB_2_PH_UPPER].Status = 1;
        stamp_pH_Upper = sParamCtrlPH.pH_Upper;
    }
    
    if(stamp_pH_Lower != sParamCtrlPH.pH_Lower)
    {
        oLCD_U_Main_2_Tab_2[_LCD_U_MAIN_2_TAB_2_PH_LOWER].Status = 1;
        stamp_pH_Lower = sParamCtrlPH.pH_Lower;
    }
    
    DLCD_Main_2_Tab_2();
}

void DLCD_Main_2_Alter(void)
{
    static uint8_t stampSalinity = 0;
    static uint8_t stampOxyUpper = 0;
    static uint8_t stampOxyLower = 0;
//    static uint8_t stampTimeDelay = 0;
    static uint8_t stampOxyUpperWarning = 0;
    static uint8_t stampOxyLowerWarning = 0;
    
    
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
    
//    if(stampTimeDelay != sParamCtrlOxy.TimeDelay)
//    {
//        oLCD_U_Main_2[_LCD_U_MAIN_2_TIME_DELAY].Status = 1;
//        stampTimeDelay = sParamCtrlOxy.TimeDelay;
//    }
    
    if(stampOxyUpperWarning != sParamCtrlOxy.Oxy_Upper_Warning)
    {
        oLCD_U_Main_2[_LCD_U_MAIN_2_OXY_UPPER_WARNING].Status = 1;
        stampOxyUpperWarning = sParamCtrlOxy.Oxy_Upper_Warning;
    }
    
    if(stampOxyLowerWarning != sParamCtrlOxy.Oxy_Lower_Warning)
    {
        oLCD_U_Main_2[_LCD_U_MAIN_2_OXY_LOWER_WARNING].Status = 1;
        stampOxyLowerWarning = sParamCtrlOxy.Oxy_Lower_Warning;
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
    
    Menu_ResetState();
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
    
    Menu_ResetState();
}

void DLCD_Setting_Parameter_Tab_2_Entry(void)
{
    oLCD_C_Setting_Parameter_Tab_2[_LCD_C_SETTING_PARAMETER_TAB_2_PH_UPPER].Status = 1;
    oLCD_C_Setting_Parameter_Tab_2[_LCD_C_SETTING_PARAMETER_TAB_2_PH_LOWER].Status = 1;
//    oLCD_C_Setting_Parameter_Tab_2[_LCD_C_SETTING_PARAMETER_TAB_2_TIME_DELAY].Status = 1;
    oLCD_C_Setting_Parameter_Tab_2[_LCD_C_SETTING_PARAMETER_TAB_2_PH_TIME_PUMP].Status = 1;
    oLCD_C_Setting_Parameter_Tab_2[_LCD_C_SETTING_PARAMETER_TAB_2_PH_TIME_SAMPLING].Status = 1;
    
    oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_UPPER].Status = 1;
    oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_LOWER].Status = 1;
//    oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_TIME_DELAY].Status = 1;
    oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_TIME_PUMP].Status = 1;
    oLCD_U_Setting_Parameter_Tab_2[_LCD_U_SETTING_PARAMETER_TAB_2_PH_TIME_SAMPLING].Status = 1;

    DLCD_Setting_Parameter_Tab_2();
    Display_Control_Setting_Left(oLCD_C_Setting_Parameter_Tab_2[_LCD_C_SETTING_PARAMETER_TAB_2_PH_UPPER].Rol);
    
    Menu_ResetState();
}

void DLCD_Setting_Parameter_Entry(void)
{
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_UPPER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_LOWER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_UPPER_W].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_LOWER_W].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_PLEASE].Status = 1;
    
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER].Status = 1;
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER].Status = 1;
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_UPPER_W].Status = 1;
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_OXY_LOWER_W].Status = 1;

    DLCD_Setting_Parameter();
    Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_UPPER].Rol);
    
    Menu_ResetState();
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
    
    Menu_ResetState();
}

void DLCD_Setting_Calib_Entry(void)
{
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TITLE].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_100_CALIB].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_SALINITY].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_PH_POINT_686].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_PH_POINT_918].Status = 1;
//    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TEMPERATURE].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_PLEASE].Status = 1;
    
    oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Status = 1;
//    oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Status = 1;
    
    oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Object = &sParamMeasure.Salinity;
//    oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Object = &sParamMeasure.Temp;
    
    DLCD_Setting_Calib();
    Display_Control_Setting_Left(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_SALINITY].Rol);
    
    Menu_ResetState();
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
    
    Menu_ResetState();
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
    
    Menu_ResetState();
}

void        DLCD_Main_3_Entry(void)
{
    oLCD_C_Main_3[_LCD_C_MAIN_3_TITLE].Status = 1;
    oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_CONNECT].Status = 1;
    oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_MEASURE].Status = 1;
    oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_PUMP].Status = 1;
    oLCD_C_Main_3[_LCD_C_MAIN_3_PH_CONNECT].Status = 1;
    oLCD_C_Main_3[_LCD_C_MAIN_3_PH_MEASURE].Status = 1;

    DLCD_Main_3();
    
    if(sStateCtrlOxy.StateWarningOxy_Connect == _OXY_STT_OK)
      GLCD_WriteString("  OK",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_CONNECT].Rol, 100, 1);
    else
      GLCD_WriteString("FAIL",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_CONNECT].Rol, 100, 1);
                       
    if(sStateCtrlOxy.StateWarningOxy_Measure == _OXY_STT_OK)
      GLCD_WriteString("  OK",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_MEASURE].Rol, 100, 1);
    else
      GLCD_WriteString("FAIL",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_MEASURE].Rol, 100, 1);
                       
    if(sStateCtrlOxy.StateWarningOxy_Pump == _OXY_STT_OK)
      GLCD_WriteString("  OK",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_PUMP].Rol, 100, 1);
    else
      GLCD_WriteString("FAIL",oLCD_C_Main_3[_LCD_C_MAIN_3_OXY_PUMP].Rol, 100, 1);
                       
    if(sParamCtrlPH.StateWarningPH_Connect == _PH_STT_OK)
      GLCD_WriteString("  OK",oLCD_C_Main_3[_LCD_C_MAIN_3_PH_CONNECT].Rol, 100, 1);
    else
      GLCD_WriteString("FAIL",oLCD_C_Main_3[_LCD_C_MAIN_3_PH_CONNECT].Rol, 100, 1);
    
    if(sParamCtrlPH.StateWarningPH_Measure == _PH_STT_OK)
      GLCD_WriteString("  OK",oLCD_C_Main_3[_LCD_C_MAIN_3_PH_MEASURE].Rol, 100, 1);
    else
      GLCD_WriteString("FAIL",oLCD_C_Main_3[_LCD_C_MAIN_3_PH_MEASURE].Rol, 100, 1);
                       
    Menu_ResetState();
}

void DLCD_Main_2_Tab_2_Entry(void)
{
    oLCD_C_Main_2_Tab_2[_LCD_C_MAIN_2_TAB_2_PH_UPPER].Status = 1;
    oLCD_C_Main_2_Tab_2[_LCD_C_MAIN_2_TAB_2_PH_LOWER].Status = 1;
    
    oLCD_U_Main_2_Tab_2[_LCD_U_MAIN_2_TAB_2_PH_UPPER].Status = 1;
    oLCD_U_Main_2_Tab_2[_LCD_U_MAIN_2_TAB_2_PH_LOWER].Status = 1;
    
    DLCD_Main_2_Tab_2();
    
    Menu_ResetState();
}

void DLCD_Main_2_Entry(void)
{
    oLCD_C_Main_2[_LCD_C_MAIN_2_TITLE].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_OXY_UPPER].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_OXY_LOWER].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_SALINITY].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_OXY_UPPER_WARNING].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_OXY_LOWER_WARNING].Status = 1;
//    oLCD_C_Main_2[_LCD_C_MAIN_2_TIME_DELAY].Status = 1;
    
    oLCD_U_Main_2[_LCD_U_MAIN_2_OXY_UPPER].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_OXY_LOWER].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_SALINITY].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_OXY_UPPER_WARNING].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_OXY_LOWER_WARNING].Status = 1;
//    oLCD_U_Main_2[_LCD_U_MAIN_2_TIME_DELAY].Status = 1;
    
    DLCD_Main_2();
    
    Menu_ResetState();
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
    
    Menu_ResetState();
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

void        DLCD_Setting_Parameter_Tab_2(void)
{
    for(uint8_t i = 0; i < _LCD_C_SETTING_PARAMETER_TAB_2_END; i++)
    {
        if(oLCD_C_Setting_Parameter_Tab_2[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Setting_Parameter_Tab_2[i].Object,oLCD_C_Setting_Parameter_Tab_2[i].Rol,oLCD_C_Setting_Parameter_Tab_2[i].Col,1);
            oLCD_C_Setting_Parameter_Tab_2[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_SETTING_PARAMETER_TAB_2_END; i++)
    {
        if(oLCD_U_Setting_Parameter_Tab_2[i].Status == 1)
        {
           if(i == _LCD_C_SETTING_PARAMETER_TAB_2_PH_TIME_PUMP || i == _LCD_C_SETTING_PARAMETER_TAB_2_PH_TIME_SAMPLING)
           {
            GLCD_Write_Float_Not_Zero(*oLCD_U_Setting_Parameter_Tab_2[i].Object, 0,  oLCD_U_Setting_Parameter_Tab_2[i].Rol,oLCD_U_Setting_Parameter_Tab_2[i].Col);
           }
           else
           {
            GLCD_Write_Float_Not_Zero(*oLCD_U_Setting_Parameter_Tab_2[i].Object, 2,  oLCD_U_Setting_Parameter_Tab_2[i].Rol,oLCD_U_Setting_Parameter_Tab_2[i].Col);
           }
            oLCD_U_Setting_Parameter_Tab_2[i].Status = 0;
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
            GLCD_Write_Float_Not_Zero(*oLCD_U_Setting_Parameter[i].Object, 2,  oLCD_U_Setting_Parameter[i].Rol,oLCD_U_Setting_Parameter[i].Col);

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
//        if(i == _LCD_C_SETTING_CALIB_TEMPERATURE)
//        {
//            GLCD_GoTo(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TEMPERATURE].Rol, 17*5 + 17 + 1);
//            GLCD_WriteCharacter(0x8C, FONT5x7);
//        }
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

void        DLCD_Main_3(void)
{
    for(uint8_t i = 0; i < _LCD_C_MAIN_3_END; i++)
    {
        if(oLCD_C_Main_3[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Main_3[i].Object,oLCD_C_Main_3[i].Rol,oLCD_C_Main_3[i].Col,1);
            oLCD_C_Main_3[i].Status = 0;
        }
    }
}

void        DLCD_Main_2_Tab_2(void)
{
    for(uint8_t i = 0; i < _LCD_C_MAIN_2_TAB_2_END; i++)
    {
        if(oLCD_C_Main_2_Tab_2[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Main_2_Tab_2[i].Object,oLCD_C_Main_2_Tab_2[i].Rol,oLCD_C_Main_2_Tab_2[i].Col,1);
            oLCD_C_Main_2_Tab_2[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_MAIN_2_TAB_2_END; i++)
    {
        if(oLCD_U_Main_2_Tab_2[i].Status == 1)
        {
           GLCD_Write_Float_Not_Zero(*oLCD_U_Main_2_Tab_2[i].Object,2 ,  oLCD_U_Main_2_Tab_2[i].Rol,oLCD_U_Main_2_Tab_2[i].Col);
           oLCD_U_Main_2_Tab_2[i].Status = 0;
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
                uint16_t stamp = *oLCD_U_Main_1[i].Object/10;
                GLCD_Write_Float_Not_Zero(stamp,1 ,  oLCD_U_Main_1[i].Rol,oLCD_U_Main_1[i].Col);
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
    GLCD_ClearArea(3, 0, 7, 7);
    GLCD_GoTo(Page, 0);
    GLCD_WriteCharacter(0x7E, FONT5x7);
}

void Display_Control_Setting_Right(uint8_t Page)
{
    GLCD_ClearArea(3, 120, 7, 127);
    GLCD_GoTo(Page, 120);
    GLCD_WriteCharacter(0x7F, FONT5x7);
}

void Display_Time(void)
{
    static char aData[MAX_BUFFER_TIME]="00/00/00-00:00:00";
    static uint8_t StateBeforeRSSI  = 0;
    static uint8_t StateBeforeServer = 2;
    static uint8_t StateBeforeMachine = 3;
    static uint8_t StateBeforeWarning = 0;
    static uint8_t StateCurrentWarning = 0;
    
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
            if(sSimInfor.RSSI_u8 <= 60)
            {
                GLCD_GoTo(0, 20*5 + 20);
                GLCD_WriteCharacter(0x9D, FONT5x7);
            }
            else if(sSimInfor.RSSI_u8 <= 70)
            {
                GLCD_GoTo(0, 20*5 + 20);
                GLCD_WriteCharacter(0x9C, FONT5x7);
            }
            else if(sSimInfor.RSSI_u8 <= 80)
            {
                GLCD_GoTo(0, 20*5 + 20);
                GLCD_WriteCharacter(0x9B, FONT5x7);
            }
            else if(sSimInfor.RSSI_u8 <= 90)
            {
                GLCD_GoTo(0, 20*5 + 20);
                GLCD_WriteCharacter(0x9A, FONT5x7);
            }
            else
            {
                GLCD_GoTo(0, 20*5 + 20);
                GLCD_WriteCharacter(0x99, FONT5x7);
            }
        }
        else if(sMenuState.ConnectServer == 0)
        {
            GLCD_GoTo(0, 20*5 + 20);
            GLCD_WriteCharacter(0x98, FONT5x7);
        }
        else
        {
            GLCD_GoTo(0, 20*5 + 20);
            GLCD_WriteCharacter(0x96, FONT5x7);
        }
        
        StateBeforeServer = sMenuState.ConnectServer;
        StateBeforeRSSI = sSimInfor.RSSI_u8;
    }     
    else
    {
        if(sMenuState.ConnectServer == 1)
        {
            if(StateBeforeRSSI != sSimInfor.RSSI_u8)
            {
                if(sSimInfor.RSSI_u8 <= 60)
                {
                    GLCD_GoTo(0, 20*5 + 20);
                    GLCD_WriteCharacter(0x9D, FONT5x7);
                }
                else if(sSimInfor.RSSI_u8 <= 70)
                {
                    GLCD_GoTo(0, 20*5 + 20);
                    GLCD_WriteCharacter(0x9C, FONT5x7);
                }
                else if(sSimInfor.RSSI_u8 <= 80)
                {
                    GLCD_GoTo(0, 20*5 + 20);
                    GLCD_WriteCharacter(0x9B, FONT5x7);
                }
                else if(sSimInfor.RSSI_u8 <= 90)
                {
                    GLCD_GoTo(0, 20*5 + 20);
                    GLCD_WriteCharacter(0x9A, FONT5x7);
                }
                else
                {
                    GLCD_GoTo(0, 20*5 + 20);
                    GLCD_WriteCharacter(0x99, FONT5x7);
                }
            }
        }
        StateBeforeServer = sMenuState.ConnectServer;
        StateBeforeRSSI = sSimInfor.RSSI_u8;
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
    
//    if(StateBeforeSensor != sStateCtrlOxy.StateSensorOxy)
//    {
//        if(sStateCtrlOxy.StateSensorOxy == _OXY_CONNECT)
//        {
//            GLCD_GoTo(0, 18*5 + 18);
//            GLCD_WriteCharacter(0x90, FONT5x7);
//        }
//        else 
//        {
//            GLCD_GoTo(0, 18*5 + 18);
//            GLCD_WriteCharacter(0x91, FONT5x7);
//        }
//        
//        StateBeforeSensor = sStateCtrlOxy.StateSensorOxy;
//    }
    
    if(sParamCtrlPH.StateWarningPH_Connect == _PH_STT_OK && sParamCtrlPH.StateWarningPH_Measure == _PH_STT_OK &&\
       sStateCtrlOxy.StateWarningOxy_Connect == _OXY_STT_OK && sStateCtrlOxy.StateWarningOxy_Measure == _OXY_STT_OK && \
       sStateCtrlOxy.StateWarningOxy_Pump == _OXY_STT_OK)
    {
        StateCurrentWarning = 0;
        if(StateBeforeWarning != StateCurrentWarning)
        {
            GLCD_GoTo(0, 18*5 + 18);
            GLCD_WriteCharacter(0x20, FONT5x7);
            StateBeforeWarning = StateCurrentWarning;
        }
    }
    else
    {
        StateCurrentWarning = 1;
        if(StateBeforeWarning != StateCurrentWarning)
        {
            GLCD_GoTo(0, 18*5 + 18);
            GLCD_WriteCharacter(0x97, FONT5x7);
            StateBeforeWarning = StateCurrentWarning;
        }
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

/*========================Handle Task and Init app======================*/
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


