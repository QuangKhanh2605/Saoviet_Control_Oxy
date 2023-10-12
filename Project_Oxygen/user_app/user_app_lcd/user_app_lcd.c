#include "user_app_lcd.h"
#include "rtc.h"
#include "user_app_ctrl_oxy.h"
#include "user_app_menu_display.h"
/*=============== Function static ================*/
static uint8_t fevent_lcd_entry(uint8_t event);
static uint8_t fevent_lcd_get_time_rtc(uint8_t event);

/*================== Struct ===================*/
sEvent_struct       sEventAppLcd[]=
{
  {_EVENT_LCD_ENTRY,            1, 5, 0,        fevent_lcd_entry},
  {_EVENT_LCD_GET_TIME_RTC,     1, 5, 250,      fevent_lcd_get_time_rtc},
};

uint8_t ID[20] = "SVEPID23000001";
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
char cTime_RTC[MAX_BUFFER_TIME]="00/00/00-00:00:00";
/*===== Object LCD =============== MAIN Screen 1 =================*/
LCD_Char_Display                    oLCD_C_Main_1[]=
{
  {_LCD_C_MAIN_1_ID,                0,2,8,          (char*)"ID  :"},
  {_LCD_C_MAIN_1_ID_VALUE,          0,2,40,         (char*)ID},
  {_LCD_C_MAIN_1_TEMP,              0,3,8,          (char*)"Temp:        C"},
  {_LCD_C_MAIN_1_OXY_1,             0,4,8,          (char*)"Oxy :       mg/L"},
  {_LCD_C_MAIN_1_OXY_2,             0,5,80,         (char*)"%"},
  {_LCD_C_MAIN_1_STATE,             0,6,8,          (char*)"State:Oxy1|Oxy2|Oxy3"},
  {_LCD_C_MAIN_1_STATE_OXY_1_ON,    0,7,44,         (char*)"  ON|"},
  {_LCD_C_MAIN_1_STATE_OXY_1_OFF,   0,7,44,         (char*)" OFF|"},
  {_LCD_C_MAIN_1_STATE_OXY_2_ON,    0,7,74,         (char*)"  ON|"},
  {_LCD_C_MAIN_1_STATE_OXY_2_OFF,   0,7,74,         (char*)" OFF|"},
  {_LCD_C_MAIN_1_STATE_OXY_3_ON,    0,7,104,        (char*)"  ON"},
  {_LCD_C_MAIN_1_STATE_OXY_3_OFF,   0,7,104,        (char*)" OFF"},
};

LCD_Uint_Display                    oLCD_U_Main_1[]=
{
  {_LCD_U_MAIN_1_TEMP,              0,3,42,          &sParamMeasure.Temp},
  {_LCD_U_MAIN_1_OXY_MG_L,          0,4,42,          &sParamMeasure.Oxy_Mg_L},
  {_LCD_U_MAIN_1_OXY_PERCENT,       0,5,42,          &sParamMeasure.Oxy_Percent},
};

/*===== Object LCD =============== MAIN_Screen 2 =================*/
LCD_Char_Display                    oLCD_C_Main_2[]=
{
  {_LCD_C_MAIN_2_ID,                0,2,8,          (char*)"ID  :"},
  {_LCD_C_MAIN_2_ID_VALUE,          0,2,40,         (char*)ID},
  {_LCD_C_MAIN_2_CYCLE,             0,4,8,          (char*)"Cycle:      Second"},
  {_LCD_C_MAIN_2_FREE,              0,5,8,          (char*)"Free :      Second"},
  {_LCD_C_MAIN_2_STATE,             0,6,8,          (char*)"State:Oxy1|Oxy2|Oxy3"},
  {_LCD_C_MAIN_2_STATE_OXY_1_ON,    0,7,44,         (char*)"  ON|"},
  {_LCD_C_MAIN_2_STATE_OXY_1_OFF,   0,7,44,         (char*)" OFF|"},
  {_LCD_C_MAIN_2_STATE_OXY_2_ON,    0,7,74,         (char*)"  ON|"},
  {_LCD_C_MAIN_2_STATE_OXY_2_OFF,   0,7,74,         (char*)" OFF|"},
  {_LCD_C_MAIN_2_STATE_OXY_3_ON,    0,7,104,        (char*)"  ON"},
  {_LCD_C_MAIN_2_STATE_OXY_3_OFF,   0,7,104,        (char*)" OFF"},
};

LCD_Uint_Display                    oLCD_U_Main_2[]=
{
  {_LCD_U_MAIN_2_RUN,               0,4,48,          &sTimeCtrlOxy.RunCtrl},
  {_LCD_U_MAIN_2_FREE,              0,5,48,          &sTimeCtrlOxy.FreeCtrl},
};

/*===== Object LCD =============== Screen Login =================*/
LCD_Char_Display                    oLCD_C_Login[]=
{
  {_LCD_C_LOGIN_ID,                0,2,8,         (char*)"ID  :"},
  {_LCD_C_LOGIN_ID_VALUE,          0,2,40,         (char*)ID},
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
  {_LCD_U_LOGIN_PASSWORD_1,        0,4,68,         (uint16_t*)&sPassword.Obj1},
  {_LCD_U_LOGIN_PASSWORD_2,        0,4,78,         (uint16_t*)&sPassword.Obj2},
  {_LCD_U_LOGIN_PASSWORD_3,        0,4,88,         (uint16_t*)&sPassword.Obj3},
  {_LCD_U_LOGIN_PASSWORD_4,        0,4,98,         (uint16_t*)&sPassword.Obj4},
  {_LCD_U_LOGIN_PASSWORD_5,        0,4,108,        (uint16_t*)&sPassword.Obj5},
  {_LCD_U_LOGIN_PASSWORD_6,        0,4,118,        (uint16_t*)&sPassword.Obj6},
};

/*===== Object LCD ===============  Setting Main =================*/
LCD_Char_Display                    oLCD_C_Setting_Main[]=
{
  {_LCD_C_SETTING_MAIN_TITLE,          0,2,45,          (char*)"SETTING"},
  {_LCD_C_SETTING_MAIN_CYCLE,          0,3,8,           (char*)"[1] Duty cycle Oxy"},
  {_LCD_C_SETTING_MAIN_CALIB,          0,4,8,           (char*)"[2] Calib Oxy gen "},
  {_LCD_C_SETTING_MAIN_SATILITY,       0,5,8,           (char*)"[3] Satility"},
  {_LCD_C_SETTING_MAIN_PASSWORD,       0,6,8,           (char*)"[4] Change Password"},
  {_LCD_C_SETTING_MAIN_PLEASE,         0,7,22,          (char*)"Please Choose!"}
};

/*===== Object LCD ================ Setting Calib ================*/
LCD_Char_Display                    oLCD_C_Setting_Calib[]=
{
  {_LCD_C_SETTING_CALIB_TITLE,          0,2,12,          (char*)"SETTING CALIB OXY"},
  {_LCD_C_SETTING_CALIB_0,              0,4,8,           (char*)"[1] Oxy 0%"},
  {_LCD_C_SETTING_CALIB_100,            0,5,8,           (char*)"[2] Oxy 100%"},
  {_LCD_C_SETTING_CALIB_PLEASE,         0,7,22,          (char*)"Please Choose!"}
};

/*===== Object LCD ================ Setting Duty Cycle================*/
LCD_Char_Display                    oLCD_C_Setting_Cycle[]=
{
  {_LCD_C_SETTING_CYCLE_TITLE,          0,2,10,          (char*)"SETTING DUTY CYLCE"},
  {_LCD_C_SETTING_CYCLE_RUN,            0,4,8,           (char*)"[1] Cycle:      s"},
  {_LCD_C_SETTING_CYCLE_FREE,           0,5,8,           (char*)"[2] Free :      s"},
  {_LCD_C_SETTING_CALIB_PLEASE,         0,7,22,          (char*)"Please Choose!"},
  {_LCD_C_SETTING_CYCLE_CLICK,          0,7,22,          (char*)"Select Up/Down"},
};

LCD_Uint_Display                    oLCD_U_Setting_Cycle[]=
{
  {_LCD_U_SETTING_CYCLE_RUN,            0,4,70,         &sMenuStamp.TimeRunCtrlOxy},
  {_LCD_U_SETTING_CYCLE_FREE,           0,5,70,         &sMenuStamp.TimeFreeCtrlOxy},
};

/*===== Object LCD ================ Setting Satility =================*/
LCD_Char_Display                    oLCD_C_Setting_Satility[]=
{
  {_LCD_C_SETTING_SATILITY,             0,2,15,             (char*)"SETTING SATILITY"},
  {_LCD_C_SETTING_SATILITY_PARAM,       0,4,8,              (char*)"[1] Satility:"},
  {_LCD_C_SETTING_SATILITY_PLEASE,      0,7,22,             (char*)"Please Choose!"},
  {_LCD_C_SETTING_SATILITY_CLICK,       0,7,22,             (char*)"Select Up/Down"},
};

LCD_Uint_Display                    oLCD_U_Setting_Satility[]=
{
  {_LCD_U_SETTING_SATILITY_PARAM,       0,4,90,              &sMenuStamp.Satility},
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
  {_LCD_C_SETTING_PASSWORD_SAVE,        0,7,8,          (char*)"   Enter to Save  "},
  {_LCD_C_SETTING_PASSWORD_ENTER,       0,7,8,          (char*)"Enter new Password"},
  {_LCD_C_SETTING_PASSWORD_CLICK,       0,7,8,          (char*)" Click Enter/Exit "},
};

LCD_Uint_Display                    oLCD_U_Setting_Password[]=
{
  {_LCD_U_SETTING_PASSWORD_1,        0,4,68,         (uint16_t*)&sMenuStamp.Pass1},
  {_LCD_U_SETTING_PASSWORD_2,        0,4,78,         (uint16_t*)&sMenuStamp.Pass2},
  {_LCD_U_SETTING_PASSWORD_3,        0,4,88,         (uint16_t*)&sMenuStamp.Pass3},
  {_LCD_U_SETTING_PASSWORD_4,        0,4,98,         (uint16_t*)&sMenuStamp.Pass4},
  {_LCD_U_SETTING_PASSWORD_5,        0,4,108,        (uint16_t*)&sMenuStamp.Pass5},
  {_LCD_U_SETTING_PASSWORD_6,        0,4,118,        (uint16_t*)&sMenuStamp.Pass6},
};
/*===== Object LCD ================ Screen Calib Oxy ================*/
LCD_Char_Display                    oLCD_C_Calib_Oxy[]=
{
  {_LCD_C_CALIB_OXY_0,                  0,2,30,                 (char*)"CALIB OXY 0"},
  {_LCD_C_CALIB_OXY_100,                0,2,25,                 (char*)"CALIB OXY 100"},
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


void DLCD_Setting_Satility_Alter(void)
{
    static uint16_t stampSatility  = 0;
    
    if(*oLCD_U_Setting_Satility[_LCD_U_SETTING_SATILITY_PARAM].Object != stampSatility)
    {
        stampSatility = *oLCD_U_Setting_Satility[_LCD_U_SETTING_SATILITY_PARAM].Object;
        oLCD_U_Setting_Satility[_LCD_U_SETTING_SATILITY_PARAM].Status = 1;
    }
    
    DLCD_Setting_Satility();
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
    static uint8_t stampStateOxy1 = 0;
    static uint8_t stampStateOxy2 = 0;
    static uint8_t stampStateOxy3 = 0;
    
    if(stampStateOxy1 != sParamCtrlOxy.StateOxy1)
    {
        if(sParamCtrlOxy.StateOxy1 == _OXY_OFF) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_1_OFF].Status = 1;
        else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_1_ON].Status = 1;
        stampStateOxy1 = sParamCtrlOxy.StateOxy1;
    }
    
    if(stampStateOxy2 != sParamCtrlOxy.StateOxy2)
    {
        if(sParamCtrlOxy.StateOxy2 == _OXY_OFF) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_2_OFF].Status = 1;
        else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_2_ON].Status = 1;
        stampStateOxy2 = sParamCtrlOxy.StateOxy2;
    }
    
    if(stampStateOxy3 != sParamCtrlOxy.StateOxy3)
    {
        if(sParamCtrlOxy.StateOxy3 == _OXY_OFF) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_3_OFF].Status = 1;
        else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_3_ON].Status = 1;
        stampStateOxy3 = sParamCtrlOxy.StateOxy3;
    }
    
    DLCD_Main_2();
}


void DLCD_Main_1_Alter(void)
{
    static uint16_t stampTemp = 0;
    static uint16_t stampOxyMgL = 0;
    static uint16_t stampOxyPercent = 0;
    static uint8_t stampStateOxy1 = 0;
    static uint8_t stampStateOxy2 = 0;
    static uint8_t stampStateOxy3 = 0;
    
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
    
    if(stampStateOxy1 != sParamCtrlOxy.StateOxy1)
    {
        if(sParamCtrlOxy.StateOxy1 == _OXY_OFF) oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_1_OFF].Status = 1;
        else oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_1_ON].Status = 1;
        stampStateOxy1 = sParamCtrlOxy.StateOxy1;
    }
    
    if(stampStateOxy2 != sParamCtrlOxy.StateOxy2)
    {
        if(sParamCtrlOxy.StateOxy2 == _OXY_OFF) oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_2_OFF].Status = 1;
        else oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_2_ON].Status = 1;
        stampStateOxy2 = sParamCtrlOxy.StateOxy2;
    }
    
    if(stampStateOxy3 != sParamCtrlOxy.StateOxy3)
    {
        if(sParamCtrlOxy.StateOxy3 == _OXY_OFF) oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_3_OFF].Status = 1;
        else oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_3_ON].Status = 1;
        stampStateOxy3 = sParamCtrlOxy.StateOxy3;
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

void DLCD_Setting_Satility_Entry(void)
{
    oLCD_C_Setting_Satility[_LCD_C_SETTING_SATILITY].Status = 1;
    oLCD_C_Setting_Satility[_LCD_C_SETTING_SATILITY_PARAM].Status = 1;
    oLCD_C_Setting_Satility[_LCD_C_SETTING_SATILITY_PLEASE].Status = 1;
    
    oLCD_U_Setting_Satility[_LCD_U_SETTING_SATILITY_PARAM].Status = 1;
    
    DLCD_Setting_Satility();
    Display_Control_Setting_Left(4);
}

void DLCD_Setting_Cycle_Entry(void)
{
    oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_TITLE].Status = 1;
    oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_RUN].Status = 1;
    oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_FREE].Status = 1;
    oLCD_C_Setting_Cycle[_LCD_C_SETTING_CALIB_PLEASE].Status = 1;
    
    oLCD_U_Setting_Cycle[_LCD_U_SETTING_CYCLE_RUN].Status = 1;
    oLCD_U_Setting_Cycle[_LCD_U_SETTING_CYCLE_FREE].Status = 1;
    
    DLCD_Setting_Cycle();
    Display_Control_Setting_Left(4);
}

void DLCD_Setting_Calib_Entry(void)
{
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TITLE].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_0].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_100].Status = 1;
    oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_PLEASE].Status = 1;
    
    DLCD_Setting_Calib();
    Display_Control_Setting_Left(4);
}

void DLCD_Setting_Main_Entry(void)
{
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_TITLE].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_CYCLE].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_CALIB].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_SATILITY].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PASSWORD].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PLEASE].Status = 1;
    
    DLCD_Setting_Main();
    Display_Control_Setting_Left(3);
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
    oLCD_C_Main_2[_LCD_C_MAIN_2_CYCLE].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_FREE].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_STATE].Status = 1;
    if(sParamCtrlOxy.StateOxy1 == _OXY_OFF) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_1_OFF].Status = 1;
    else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_1_ON].Status = 1;
    
    if(sParamCtrlOxy.StateOxy2 == _OXY_OFF) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_2_OFF].Status = 1;
    else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_2_ON].Status = 1;

    if(sParamCtrlOxy.StateOxy3 == _OXY_OFF) oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_3_OFF].Status = 1;
    else oLCD_C_Main_2[_LCD_C_MAIN_2_STATE_OXY_3_ON].Status = 1;
    
    oLCD_U_Main_2[_LCD_U_MAIN_2_RUN].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_FREE].Status = 1;
    
    DLCD_Main_2();
}

void DLCD_Main_1_Entry(void)
{
    oLCD_C_Main_1[_LCD_C_MAIN_1_ID].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_ID_VALUE].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_TEMP].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_OXY_1].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_OXY_2].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_STATE].Status = 1;
    if(sParamCtrlOxy.StateOxy1 == _OXY_OFF) oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_1_OFF].Status = 1;
    else oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_1_ON].Status = 1;
    
    if(sParamCtrlOxy.StateOxy2 == _OXY_OFF) oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_2_OFF].Status = 1;
    else oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_2_ON].Status = 1;

    if(sParamCtrlOxy.StateOxy3 == _OXY_OFF) oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_3_OFF].Status = 1;
    else oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_OXY_3_ON].Status = 1;
    
    oLCD_U_Main_1[_LCD_U_MAIN_1_TEMP].Status = 1;
    oLCD_U_Main_1[_LCD_U_MAIN_1_OXY_MG_L].Status = 1;
    oLCD_U_Main_1[_LCD_U_MAIN_1_OXY_PERCENT].Status = 1;
    
    DLCD_Main_1();
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

void        DLCD_Setting_Satility(void)
{
    for(uint8_t i = 0; i < LCD_C_SETTING_SATILITY_END; i++)
    {
        if(oLCD_C_Setting_Satility[i].Status == 1)
        {
            GLCD_WriteString(oLCD_C_Setting_Satility[i].Object,oLCD_C_Setting_Satility[i].Rol,oLCD_C_Setting_Satility[i].Col,1);
            oLCD_C_Setting_Satility[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_SETTING_SATILITY_END; i++)
    {
        if(oLCD_U_Setting_Satility[i].Status == 1)
        {
           GLCD_Write_Float_Not_Zero(*oLCD_U_Setting_Satility[i].Object, 0,  oLCD_U_Setting_Satility[i].Rol,oLCD_U_Setting_Satility[i].Col);
           oLCD_U_Setting_Satility[i].Status = 0;
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
           GLCD_Write_Float_Not_Zero(*oLCD_U_Main_2[i].Object,1 ,  oLCD_U_Main_2[i].Rol,oLCD_U_Main_2[i].Col);
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
                GLCD_GoTo(3, 13*5 + 13 + 1);
                GLCD_WriteCharacter(0x8C, FONT5x7);
            }
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_MAIN_1_END; i++)
    {
        if(oLCD_U_Main_1[i].Status == 1)
        {
          if(oLCD_U_Main_1[i].e_name == _LCD_U_MAIN_1_OXY_MG_L)
          {
            GLCD_Write_Float_Not_Zero(*oLCD_U_Main_1[i].Object,2 ,  oLCD_U_Main_1[i].Rol,oLCD_U_Main_1[i].Col);
          }else GLCD_Write_Float_Not_Zero(*oLCD_U_Main_1[i].Object,1 ,  oLCD_U_Main_1[i].Rol,oLCD_U_Main_1[i].Col);
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
    static uint8_t StateBefore = 2;
    for(uint8_t i = 0; i < MAX_BUFFER_TIME; i++)
    {
        if(aData[i] != cTime_RTC[i])
        {
            GLCD_GoTo(0, i*5 + i);
            GLCD_WriteCharacter(cTime_RTC[i], FONT5x7);
            aData[i] = cTime_RTC[i];
        }
    }
    
    if(StateBefore != sMenuState.ConnectServer)
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
        
        StateBefore = sMenuState.ConnectServer;
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


