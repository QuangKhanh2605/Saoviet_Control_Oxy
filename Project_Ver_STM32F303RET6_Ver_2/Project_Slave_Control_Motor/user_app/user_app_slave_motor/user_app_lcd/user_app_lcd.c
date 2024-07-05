#include "user_app_lcd.h"
#include "user_define_slave_motor.h"

/*============== Function static ===============*/
static uint8_t fevent_lcd_entry(uint8_t event);
static uint8_t fevent_lcd_handle(uint8_t event);

/*=================== struct ==================*/
sEvent_struct               sEventAppLcd[] = 
{
  {_EVENT_LCD_ENTRY,        1, 5, 5,      fevent_lcd_entry},

  {_EVENT_LCD_HANDLE,       0, 5, 0,      fevent_lcd_handle},
};
CLCD_Name LCD;

/*===== Object LCD =============== MAIN Screen 1 =================*/
LCD_Char_Display                    oLCD_C_Main_1[]=
{
  {_LCD_C_MAIN_1_ID_SLAVE,              0,0,0,2,        (char*)"ID:"},
  
  {_LCD_C_MAIN_1_STATE_CONTROL_OFF,     0,0,7,10,        (char*)" OFF"},
  {_LCD_C_MAIN_1_STATE_CONTROL_ON,      0,0,7,10,        (char*)"  ON"},
  {_LCD_C_MAIN_1_STATE_CONTROL_FAIL,    0,0,7,10,        (char*)"FAIL"},
  
  {_LCD_C_MAIN_1_STATE_MACHINE_OFF,     0,0,11,15,      (char*)"| OFF"},
  {_LCD_C_MAIN_1_STATE_MACHINE_MAN,     0,0,11,15,      (char*)"|  ON"},
  {_LCD_C_MAIN_1_STATE_MACHINE_AUTO,    0,0,11,15,      (char*)"|AUTO"},
  
  {_LCD_C_MAIN_1_AMPERAGE_UNIT,         0,1,15,15,      (char*)"A"},
  
  {_LCD_C_MAIN_1_PRESSURE_1,            0,2,0,2,       (char*)"P1:"},
  {_LCD_C_MAIN_1_PRESSURE_2,            0,2,0,2,       (char*)"P2:"},
  {_LCD_C_MAIN_1_PRESSURE_UNIT,         0,2,13,15,      (char*)"Bar"},
  
  {_LCD_C_MAIN_1_STATE_AMPERAGE,        0,3,2,3,        (char*)"A:"},
  {_LCD_C_MAIN_1_STATE_AMPERAGE_OK,     0,3,4,7,        (char*)"OK  "},
  {_LCD_C_MAIN_1_STATE_AMPERAGE_FAIL,   0,3,4,7,        (char*)"FAIL"},
  
  {_LCD_C_MAIN_1_STATE_PRESSURE,        0,3,9,10,        (char*)"P:"},
  {_LCD_C_MAIN_1_STATE_PRESSURE_OK,     0,3,11,14,       (char*)"OK  "},
  {_LCD_C_MAIN_1_STATE_PRESSURE_FAIL,   0,3,11,14,       (char*)"FAIL"},
};

LCD_Uint_Display                    oLCD_U_Main_1[]=
{
  {_LCD_U_MAIN_1_ID_SLAVE,          0,0,3,3,         &sInforRs485.ID_Slave},
  {_LCD_U_MAIN_1_AMPERAGE_1,        0,1,0,4,         &sMeasureMotor.AmperagePhase_1},
  {_LCD_U_MAIN_1_AMPERAGE_2,        0,1,5,9,         &sMeasureMotor.AmperagePhase_2},
  {_LCD_U_MAIN_1_AMPERAGE_3,        0,1,10,14,       &sMeasureMotor.AmperagePhase_3},
  {_LCD_U_MAIN_1_PRESSURE,          0,2,5,9,         &sMeasureMotor.Pressure},
};

/*===== Object LCD =============== MAIN_Screen 2 =================*/
LCD_Char_Display                    oLCD_C_Main_2[]=
{
  {_LCD_C_MAIN_2_TITLE,                 0,0,0,14,               (char*)"PARAMETER SETUP"},
  {_LCD_C_MAIN_2_AMPERAGE_UPPER,        0,0,0,7,                (char*)"A_Upper:"},
  {_LCD_C_MAIN_2_AMPERAGE_UPPER_UNIT,   0,0,13,15,              (char*)"A "},
  {_LCD_C_MAIN_2_AMPERAGE_DELTA,        0,1,0,7,                (char*)"A_Delta:"},
  {_LCD_C_MAIN_2_AMPERAGE_DELTA_UNIT,   0,1,13,15,              (char*)"%  "},
  {_LCD_C_MAIN_2_PRESSURE_UPPER,        0,2,0,7,                (char*)"P_Upper:"},
  {_LCD_C_MAIN_2_PRESSURE_UPPER_UNIT,   0,2,13,15,              (char*)"Bar"},
  {_LCD_C_MAIN_2_PRESSURE_LOWER,        0,3,0,7,                (char*)"P_Lower:"},
  {_LCD_C_MAIN_2_PRESSURE_LOWER_UNIT,   0,3,13,15,              (char*)"Bar"},
};

LCD_Uint_Display                    oLCD_U_Main_2[]=
{
  {_LCD_U_MAIN_2_AMPERAGE_UPPER,         0,0,8,11,          &sParamMotor.Amperage_Upper},
  {_LCD_U_MAIN_2_AMPERAGE_DELTA,         0,1,8,11,          &sParamMotor.Amperage_Delta},
  {_LCD_U_MAIN_2_PRESSURE_UPPER,         0,2,8,11,          &sParamMotor.Pressure_Upper},
  {_LCD_U_MAIN_2_PRESSURE_LOWER,         0,3,8,11,          &sParamMotor.Pressure_Lower},
};

/*===== Object LCD =============== MAIN Screen Login =================*/
LCD_Char_Display                    oLCD_C_Login[]=
{
  {_LCD_C_LOGIN_PASSWORD,          0,0,1,13,         (char*)"LOGIN SETTING"},
  {_LCD_C_LOGIN_PASS_1,            0,2,3,13,         (char*)"*          "},
  {_LCD_C_LOGIN_PASS_2,            0,2,3,13,         (char*)"  *        "},
  {_LCD_C_LOGIN_PASS_3,            0,2,3,13,         (char*)"    *      "},
  {_LCD_C_LOGIN_PASS_4,            0,2,3,13,         (char*)"      *    "},
  {_LCD_C_LOGIN_PASS_5,            0,2,3,13,         (char*)"        *  "},
  {_LCD_C_LOGIN_PASS_6,            0,2,3,13,         (char*)"          *"},
  
  {_LCD_C_LOGIN_ENTER,             0,3,1,14,         (char*)"Enter Password"},
  {_LCD_C_LOGIN_LOGIN,             0,3,1,14,         (char*)"Enter to Login"},
  {_LCD_C_LOGIN_WRONG,             0,3,1,14,         (char*)"  Incorrect!  "},
};

LCD_Uint_Display                    oLCD_U_Login[]=
{
  {_LCD_U_LOGIN_PASSWORD_1,        0,1,3,3,           (uint16_t*)&sMenuStamp.sPassLogin.Pass1},
  {_LCD_U_LOGIN_PASSWORD_2,        0,1,5,5,           (uint16_t*)&sMenuStamp.sPassLogin.Pass2},
  {_LCD_U_LOGIN_PASSWORD_3,        0,1,7,7,           (uint16_t*)&sMenuStamp.sPassLogin.Pass3},
  {_LCD_U_LOGIN_PASSWORD_4,        0,1,9,9,           (uint16_t*)&sMenuStamp.sPassLogin.Pass4},
  {_LCD_U_LOGIN_PASSWORD_5,        0,1,11,11,         (uint16_t*)&sMenuStamp.sPassLogin.Pass5},
  {_LCD_U_LOGIN_PASSWORD_6,        0,1,13,13,         (uint16_t*)&sMenuStamp.sPassLogin.Pass6},
};

/*===== Object LCD ===============  Setting Main =================*/
LCD_Char_Display                    oLCD_C_Setting_Main[]=
{
  {_LCD_C_SETTING_MAIN_TITLE,          0,0,4,10,            (char*)"SETTING"},
  {_LCD_C_SETTING_MAIN_PARAMETER,      0,1,1,11,            (char*)"1 Parameter"},
  {_LCD_C_SETTING_MAIN_OPTION,         0,2,1,8,             (char*)"2 Option"},
  {_LCD_C_SETTING_MAIN_PASSWORD,       0,3,1,10,            (char*)"3 Password"},
};

/*===== Object LCD ================ Setting Param =================*/
LCD_Char_Display                    oLCD_C_Setting_Parameter[]=
{
  {_LCD_C_SETTING_PARAMETER,                        0,0,3,11,            (char*)"PARAMETER"},
  {_LCD_C_SETTING_PARAMETER_AMPERAGE_UPPER,         0,0,1,7,             (char*)"A_Upper"},
  {_LCD_C_SETTING_PARAMETER_AMPERAGE_UPPER_UNIT,    0,0,12,12,           (char*)"A"},
  {_LCD_C_SETTING_PARAMETER_AMPERAGE_DELTA,         0,1,1,7,             (char*)"A_Delta"},
  {_LCD_C_SETTING_PARAMETER_AMPERAGE_DELTA_UNIT,    0,1,12,12,           (char*)"%"},
  {_LCD_C_SETTING_PARAMETER_PRESSURE_UPPER,         0,2,1,7,             (char*)"P_Upper"},
  {_LCD_C_SETTING_PARAMETER_PRESSURE_UPPER_UNIT,    0,2,12,14,           (char*)"Bar"},
  {_LCD_C_SETTING_PARAMETER_PRESSURE_LOWER,         0,3,1,7,             (char*)"P_Lower"},
  {_LCD_C_SETTING_PARAMETER_PRESSURE_LOWER_UNIT,    0,3,12,14,           (char*)"Bar"},
};

LCD_Uint_Display                    oLCD_U_Setting_Parameter[]=
{
  {_LCD_U_SETTING_PARAMETER_AMPERAGE_UPPER,         0,0,8,11,              &sMenuStamp.Amperage_Upper},
  {_LCD_U_SETTING_PARAMETER_AMPERAGE_DELTA,         0,1,8,11,              &sMenuStamp.Amperage_Delta},
  {_LCD_U_SETTING_PARAMETER_PRESSURE_UPPER,         0,2,8,11,              &sMenuStamp.Pressure_Upper},
  {_LCD_U_SETTING_PARAMETER_PRESSURE_LOWER,         0,3,8,11,              &sMenuStamp.Pressure_Lower},
};


/*===== Object LCD ================ Setting Option =================*/
LCD_Char_Display                    oLCD_C_Setting_Option[]=
{
  {_LCD_C_SETTING_OPTION,               0,0,4,10,            (char*)"OPTION"},
  {_LCD_C_SETTING_OPTION_AMPERAGE,      0,0,1,8,             (char*)"Amperage"},
  {_LCD_C_SETTING_OPTION_AMPERAGE_ON,   0,0,12,14,           (char*)" ON"},
  {_LCD_C_SETTING_OPTION_AMPERAGE_OFF,  0,0,12,14,           (char*)"OFF"},
  
  {_LCD_C_SETTING_OPTION_PRESSURE,      0,1,1,8,             (char*)"Pressure"},
  {_LCD_C_SETTING_OPTION_PRESSURE_ON,   0,1,12,14,           (char*)" ON"},
  {_LCD_C_SETTING_OPTION_PRESSURE_OFF,  0,1,12,14,           (char*)"OFF"},
  
  {_LCD_C_SETTING_CONSTANT_PRESSURE,    0,2,1,8,             (char*)"Pr_Const"},
  {_LCD_C_SETTING_CONSTANT_UINIT,       0,2,13,14,           (char*)"Ba"},
  
  {_LCD_C_SETTING_ID_SLAVE,             0,3,1,2,             (char*)"ID"},
};

LCD_Uint_Display                    oLCD_U_Setting_Option[]=
{
  {_LCD_U_SETTING_CONSTANT_PRESSURE,    0,2,10,11,           &sMenuStamp.Pressure_Constant},
  {_LCD_U_SETTING_ID_SLAVE,             0,3,10,11,           &sMenuStamp.ID_Slave},
};

/*==== Object LCD ================= Setting Password ==================*/
LCD_Char_Display                    oLCD_C_Setting_Password[]=
{
  {_LCD_C_SETTING_PASSWORD_TITLE,       0,0,4,11,         (char*)"PASSWORD"},
  {_LCD_C_SETTING_PASSWORD_1,           0,2,3,13,         (char*)"*          "},
  {_LCD_C_SETTING_PASSWORD_2,           0,2,3,13,         (char*)"  *        "},
  {_LCD_C_SETTING_PASSWORD_3,           0,2,3,13,         (char*)"    *      "},
  {_LCD_C_SETTING_PASSWORD_4,           0,2,3,13,         (char*)"      *    "},
  {_LCD_C_SETTING_PASSWORD_5,           0,2,3,13,         (char*)"        *  "},
  {_LCD_C_SETTING_PASSWORD_6,           0,2,3,13,         (char*)"          *"},
  {_LCD_C_SETTING_PASSWORD_SUCCESS,     0,2,4,13,         (char*)" Success "},
  {_LCD_C_SETTING_PASSWORD_SAVE,        0,3,0,15,         (char*)"  Enter to Save "},
  {_LCD_C_SETTING_PASSWORD_ENTER,       0,3,0,15,         (char*)"  New Password  "},
  {_LCD_C_SETTING_PASSWORD_CLICK,       0,3,0,15,         (char*)"Click Enter/Exit"},
};

LCD_Uint_Display                    oLCD_U_Setting_Password[]=
{
  {_LCD_U_SETTING_PASSWORD_1,        0,1,3,3,          (uint16_t*)&sMenuStamp.sPassWord.Pass1},
  {_LCD_U_SETTING_PASSWORD_2,        0,1,5,5,          (uint16_t*)&sMenuStamp.sPassWord.Pass2},
  {_LCD_U_SETTING_PASSWORD_3,        0,1,7,7,          (uint16_t*)&sMenuStamp.sPassWord.Pass3},
  {_LCD_U_SETTING_PASSWORD_4,        0,1,9,9,          (uint16_t*)&sMenuStamp.sPassWord.Pass4},
  {_LCD_U_SETTING_PASSWORD_5,        0,1,11,11,        (uint16_t*)&sMenuStamp.sPassWord.Pass5},
  {_LCD_U_SETTING_PASSWORD_6,        0,1,13,13,        (uint16_t*)&sMenuStamp.sPassWord.Pass6},
};

/*================= Function Handle ==============*/
static uint8_t fevent_lcd_entry(uint8_t event)
{
    fevent_active(sEventAppMenu, _EVENT_MENU_ENTRY);
    return 1;
}

static uint8_t fevent_lcd_handle(uint8_t event)
{
    return 1;
}

/*================= Function DLCD ==============*/
void        DLCD_Setting_Password(void)
{
    for(uint8_t i = 0; i < _LCD_C_SETTING_PASSWORD_END; i++)
    {
        if(oLCD_C_Setting_Password[i].Status == 1)
        {
            LCD_Display_String(&LCD, oLCD_C_Setting_Password[i].Rol, oLCD_C_Setting_Password[i].Col_Begin, oLCD_C_Setting_Password[i].Col_End,
                                    ALIGN_RIGHT, (uint8_t*)oLCD_C_Setting_Password[i].Object);
            oLCD_C_Setting_Password[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_SETTING_PASSWORD_END; i++)
    {
        if(oLCD_U_Setting_Password[i].Status == 1)
        {
            LCD_Display_Uint_Point(&LCD, oLCD_U_Setting_Password[i].Rol, oLCD_U_Setting_Password[i].Col_Begin, oLCD_U_Setting_Password[i].Col_End,
                                    ALIGN_RIGHT, (int)(*oLCD_U_Setting_Password[i].Object), 0);
            oLCD_U_Setting_Password[i].Status = 0;
        }
    }
}

void        DLCD_Setting_Option(void)
{
    for(uint8_t i = 0; i < LCD_C_SETTING_OPTION_END; i++)
    {
        if(oLCD_C_Setting_Option[i].Status == 1)
        {
            LCD_Display_String(&LCD, oLCD_C_Setting_Option[i].Rol, oLCD_C_Setting_Option[i].Col_Begin, oLCD_C_Setting_Option[i].Col_End,
                                    ALIGN_RIGHT, (uint8_t*)oLCD_C_Setting_Option[i].Object);
            oLCD_C_Setting_Option[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < LCD_U_SETTING_OPTION_END; i++)
    {
        if(oLCD_U_Setting_Option[i].Status == 1)
        {
            LCD_Display_Uint_Point(&LCD, oLCD_U_Setting_Option[i].Rol, oLCD_U_Setting_Option[i].Col_Begin, oLCD_U_Setting_Option[i].Col_End,
                                    ALIGN_RIGHT, (int)(*oLCD_U_Setting_Option[i].Object), 0);
            oLCD_U_Setting_Option[i].Status = 0;
        }
    }
}


void        DLCD_Setting_Parameter(void)
{
    for(uint8_t i = 0; i < LCD_C_SETTING_PARAMETER_END; i++)
    {
        if(oLCD_C_Setting_Parameter[i].Status == 1)
        {
            LCD_Display_String(&LCD, oLCD_C_Setting_Parameter[i].Rol, oLCD_C_Setting_Parameter[i].Col_Begin, oLCD_C_Setting_Parameter[i].Col_End,
                                    ALIGN_RIGHT, (uint8_t*)oLCD_C_Setting_Parameter[i].Object);
            oLCD_C_Setting_Parameter[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_SETTING_PARAMETER_END; i++)
    {
        if(oLCD_U_Setting_Parameter[i].Status == 1)
        {
            if(i == _LCD_U_SETTING_PARAMETER_AMPERAGE_DELTA)
            {
                LCD_Display_Uint_Point(&LCD, oLCD_U_Setting_Parameter[i].Rol, oLCD_U_Setting_Parameter[i].Col_Begin, oLCD_U_Setting_Parameter[i].Col_End,
                                        ALIGN_RIGHT, (int)(*oLCD_U_Setting_Parameter[i].Object), 0);
            }
            else
            {
                LCD_Display_Uint_Point(&LCD, oLCD_U_Setting_Parameter[i].Rol, oLCD_U_Setting_Parameter[i].Col_Begin, oLCD_U_Setting_Parameter[i].Col_End,
                                        ALIGN_RIGHT, (int)(*oLCD_U_Setting_Parameter[i].Object), 1);
            }
            oLCD_U_Setting_Parameter[i].Status = 0;
        }
    }
}

void        DLCD_Setting_Main(void)
{
    for(uint8_t i = 0; i < _LCD_C_SETTING_MAIN_END; i++)
    {
        if(oLCD_C_Setting_Main[i].Status == 1)
        {
            LCD_Display_String(&LCD, oLCD_C_Setting_Main[i].Rol, oLCD_C_Setting_Main[i].Col_Begin, oLCD_C_Setting_Main[i].Col_End,
                                    ALIGN_RIGHT, (uint8_t*)oLCD_C_Setting_Main[i].Object);
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
            LCD_Display_String(&LCD, oLCD_C_Login[i].Rol, oLCD_C_Login[i].Col_Begin, oLCD_C_Login[i].Col_End,
                                    ALIGN_RIGHT, (uint8_t*)oLCD_C_Login[i].Object);
            oLCD_C_Login[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_LOGIN_END; i++)
    {
        if(oLCD_U_Login[i].Status == 1)
        {
           LCD_Display_Uint_Point(&LCD, oLCD_U_Login[i].Rol, oLCD_U_Login[i].Col_Begin, oLCD_U_Login[i].Col_End,
                                    ALIGN_RIGHT, (int)(*oLCD_U_Login[i].Object), 0);
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
            LCD_Display_String(&LCD, oLCD_C_Main_2[i].Rol, oLCD_C_Main_2[i].Col_Begin, oLCD_C_Main_2[i].Col_End,
                                    ALIGN_RIGHT, (uint8_t*)oLCD_C_Main_2[i].Object);
            oLCD_C_Main_2[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_MAIN_2_END; i++)
    {
        if(oLCD_U_Main_2[i].Status == 1)
        {
            if(i == _LCD_U_MAIN_2_AMPERAGE_DELTA)
            {
                LCD_Display_Uint_Point(&LCD, oLCD_U_Main_2[i].Rol, oLCD_U_Main_2[i].Col_Begin, oLCD_U_Main_2[i].Col_End,
                                        ALIGN_RIGHT, (int)(*oLCD_U_Main_2[i].Object), 0);
            }
            else
            {
                LCD_Display_Uint_Point(&LCD, oLCD_U_Main_2[i].Rol, oLCD_U_Main_2[i].Col_Begin, oLCD_U_Main_2[i].Col_End,
                                        ALIGN_RIGHT, (int)(*oLCD_U_Main_2[i].Object), 1);
            }
            
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
            LCD_Display_String(&LCD, oLCD_C_Main_1[i].Rol, oLCD_C_Main_1[i].Col_Begin, oLCD_C_Main_1[i].Col_End,
                                    ALIGN_RIGHT, (uint8_t*)oLCD_C_Main_1[i].Object);
            oLCD_C_Main_1[i].Status = 0;
        }
    }
    
    for(uint8_t i = 0; i < _LCD_U_MAIN_1_END; i++)
    {
        if(oLCD_U_Main_1[i].Status == 1)
        {
            if(i ==_LCD_U_MAIN_1_ID_SLAVE)
            {
                LCD_Display_Uint_Point(&LCD, oLCD_U_Main_1[i].Rol, oLCD_U_Main_1[i].Col_Begin, oLCD_U_Main_1[i].Col_End,
                                        ALIGN_LEFT, (int)(*oLCD_U_Main_1[i].Object), 0);
                oLCD_U_Main_1[i].Status = 0;
            }
            else
            {
                LCD_Display_Uint_Point(&LCD, oLCD_U_Main_1[i].Rol, oLCD_U_Main_1[i].Col_Begin, oLCD_U_Main_1[i].Col_End,
                                        ALIGN_LEFT, (int)(*oLCD_U_Main_1[i].Object), 1);
                oLCD_U_Main_1[i].Status = 0;
            }
        }
    }
}

/*================= Function DLCD Entry ==============*/
void DLCD_Setting_Password_Entry(void)
{
//    oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_TITLE].Status = 1;
    oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_1].Status = 1;
    oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_ENTER].Status = 1;

    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_1].Status = 1;
    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_2].Status = 1;
    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_3].Status = 1;
    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_4].Status = 1;
    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_5].Status = 1;
    oLCD_U_Setting_Password[_LCD_U_SETTING_PASSWORD_6].Status = 1;
    
    CLCD_Clear(&LCD);
    DLCD_Setting_Password();
    Menu_ResetState();
}

void        DLCD_Setting_Option_Entry(void)
{
//    oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION].Status = 1;
    
    oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_AMPERAGE].Status = 1;
    oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_AMPERAGE_OFF].Status = 1;
    
    oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_PRESSURE].Status = 1;
    oLCD_C_Setting_Option[_LCD_C_SETTING_CONSTANT_PRESSURE].Status = 1;
    oLCD_C_Setting_Option[_LCD_C_SETTING_CONSTANT_UINIT].Status = 1;
    
    oLCD_C_Setting_Option[_LCD_C_SETTING_ID_SLAVE].Status = 1;
    
    switch(sMenuStamp.Pressure_Option)
    {
        case _PRESSURE_OPTION_ON:
          oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_PRESSURE_ON].Status = 1;
          break;
          
        case _PRESSURE_OPTION_OFF:
          oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_PRESSURE_OFF].Status = 1;
          break;

        default:
          break;
    }
    oLCD_U_Setting_Option[_LCD_U_SETTING_CONSTANT_PRESSURE].Status = 1;
    oLCD_U_Setting_Option[_LCD_U_SETTING_ID_SLAVE].Status = 1;
  
    CLCD_Clear(&LCD);
    DLCD_Setting_Option();
    Display_Control_Setting_Left(_CONTROL_PAGE_0);
    Menu_ResetState();
}

void DLCD_Setting_Parameter_Entry(void)
{
//    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_AMPERAGE_UPPER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_AMPERAGE_UPPER_UNIT].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_AMPERAGE_DELTA].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_AMPERAGE_DELTA_UNIT].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PRESSURE_UPPER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PRESSURE_UPPER_UNIT].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PRESSURE_LOWER].Status = 1;
    oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PRESSURE_LOWER_UNIT].Status = 1;
    
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_AMPERAGE_UPPER].Status = 1;
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_AMPERAGE_DELTA].Status = 1;
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_PRESSURE_UPPER].Status = 1;
    oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_PRESSURE_LOWER].Status = 1;

    CLCD_Clear(&LCD);
    DLCD_Setting_Parameter();
    Display_Control_Setting_Left(_CONTROL_PAGE_0);
    Menu_ResetState();
}

void DLCD_Setting_Main_Entry(void)
{
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_TITLE].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PARAMETER].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PASSWORD].Status = 1;
    oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_OPTION].Status = 1;
    
    CLCD_Clear(&LCD);
    DLCD_Setting_Main();
    Display_Control_Setting_Left(_CONTROL_PAGE_1);
    Menu_ResetState();
}

void DLCD_Login_Entry(void)
{
    oLCD_C_Login[_LCD_C_LOGIN_PASSWORD].Status = 1;
    oLCD_C_Login[_LCD_C_LOGIN_PASS_1].Status = 1;
    oLCD_C_Login[_LCD_C_LOGIN_ENTER].Status = 1;

    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_1].Status = 1;
    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_2].Status = 1;
    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_3].Status = 1;
    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_4].Status = 1;
    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_5].Status = 1;
    oLCD_U_Login[_LCD_U_LOGIN_PASSWORD_6].Status = 1;
    
    CLCD_Clear(&LCD);
    DLCD_Login();
    Menu_ResetState();
}

void DLCD_Main_2_Entry(void)
{
//    oLCD_C_Main_2[_LCD_C_MAIN_2_TITLE].Status = 1;
  
    oLCD_C_Main_2[_LCD_C_MAIN_2_AMPERAGE_UPPER].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_AMPERAGE_UPPER_UNIT].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_AMPERAGE_DELTA].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_AMPERAGE_DELTA_UNIT].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_PRESSURE_UPPER].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_PRESSURE_UPPER_UNIT].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_PRESSURE_LOWER].Status = 1;
    oLCD_C_Main_2[_LCD_C_MAIN_2_PRESSURE_LOWER_UNIT].Status = 1;
    
    oLCD_U_Main_2[_LCD_U_MAIN_2_AMPERAGE_UPPER].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_AMPERAGE_DELTA].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_PRESSURE_UPPER].Status = 1;
    oLCD_U_Main_2[_LCD_U_MAIN_2_PRESSURE_LOWER].Status = 1;
    
    CLCD_Clear(&LCD);
    DLCD_Main_2();
    Menu_ResetState();
}

void        DLCD_Main_1_Entry(void)
{
    oLCD_C_Main_1[_LCD_C_MAIN_1_ID_SLAVE].Status = 1;
    
    switch(sInforRs485.State_OnOff_Motor)
    {
        case _RS485_MOTOR_ON:
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_CONTROL_ON].Status = 1;
          break;
          
        case _RS485_MOTOR_OFF:
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_CONTROL_OFF].Status = 1;
          break;
          
        case _RS485_MOTOR_FAIL:
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_CONTROL_FAIL].Status = 1;
          break;
          
        default:
          break;
    }
    
    oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_AMPERAGE].Status = 1;
    if(sParamMotor.StatusAmperage == _MOTOR_OK)
        oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_AMPERAGE_OK].Status = 1;
    else
        oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_AMPERAGE_FAIL].Status = 1;
    
    oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_PRESSURE].Status = 1;
    if(sParamMotor.StatusPressure == _MOTOR_OK)
        oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_PRESSURE_OK].Status = 1;
    else
        oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_PRESSURE_FAIL].Status = 1;
    
    switch(sParamMotor.StateMachine)
    {
        case _MACHINE_OFF:
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_MACHINE_OFF].Status = 1;
          break;
          
        case _MACHINE_MAN:
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_MACHINE_MAN].Status = 1;
          break;
          
        case _MACHINE_AUTO:
          oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_MACHINE_AUTO].Status = 1;
          break;
          
        default:
          break;
    }

    if(sMeasureMotor.Pressure_Chanel == _CHANEL_P1)
        oLCD_C_Main_1[_LCD_C_MAIN_1_PRESSURE_1].Status = 1;
    else
        oLCD_C_Main_1[_LCD_C_MAIN_1_PRESSURE_2].Status = 1;
    
    oLCD_U_Main_1[_LCD_U_MAIN_1_PRESSURE].Object = &sMeasureMotor.Pressure;

    oLCD_C_Main_1[_LCD_C_MAIN_1_PRESSURE_UNIT].Status = 1;
    oLCD_C_Main_1[_LCD_C_MAIN_1_AMPERAGE_UNIT].Status = 1;

    oLCD_U_Main_1[_LCD_U_MAIN_1_ID_SLAVE].Status = 1;
    oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_1].Status = 1;
    oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_2].Status = 1;
    oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_3].Status = 1;
    oLCD_U_Main_1[_LCD_U_MAIN_1_PRESSURE].Status = 1;

   
    CLCD_Clear(&LCD);
    DLCD_Main_1();
    Menu_ResetState();
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


void        DLCD_Setting_Option_Alter(void)
{
    static uint16_t OptionAmperage = 0;
    static uint16_t OptionPressure = 0;
    static uint16_t ConstantPressure = 0;
    static uint16_t stamp_ID_Slave = 0;
  
    if(OptionAmperage != sMenuStamp.Amperage_Option)
    {
        OptionAmperage = sMenuStamp.Amperage_Option;
        
        if(sMenuStamp.Amperage_Option == _AMPERAGE_OPTION_OFF) 
          oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_AMPERAGE_OFF].Status = 1;
        else 
          oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_AMPERAGE_ON].Status = 1;
    }
    
    if(ConstantPressure != *oLCD_U_Setting_Option[_LCD_U_SETTING_CONSTANT_PRESSURE].Object)
    {
        ConstantPressure = *oLCD_U_Setting_Option[_LCD_U_SETTING_CONSTANT_PRESSURE].Object;
        oLCD_U_Setting_Option[_LCD_U_SETTING_CONSTANT_PRESSURE].Status = 1;
    }
    
    if(OptionPressure != sMenuStamp.Pressure_Option)
    {
        OptionPressure = sMenuStamp.Pressure_Option;
        
        if(sMenuStamp.Pressure_Option == _PRESSURE_OPTION_OFF) 
          oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_PRESSURE_OFF].Status = 1;
        else 
          oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_PRESSURE_ON].Status = 1;
    }
    
    if(stamp_ID_Slave != *oLCD_U_Setting_Option[_LCD_U_SETTING_ID_SLAVE].Object)
    {
        stamp_ID_Slave = *oLCD_U_Setting_Option[_LCD_U_SETTING_ID_SLAVE].Object;
        oLCD_U_Setting_Option[_LCD_U_SETTING_ID_SLAVE].Status = 1;
    }
    
    DLCD_Setting_Option();
}

void        DLCD_Setting_Parameter_Alter(void)
{
    static uint16_t stampAmperageUpper_Setup = 0;
    static uint16_t stampAmperageDelta_Setup = 0;
    static uint16_t stampPressureUpper_Setup = 0;
    static uint16_t stampPressureLower_Setup = 0;
    
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_AMPERAGE_UPPER].Object != stampAmperageUpper_Setup)
    {
        stampAmperageUpper_Setup = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_AMPERAGE_UPPER].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_AMPERAGE_UPPER].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_AMPERAGE_DELTA].Object != stampAmperageDelta_Setup)
    {
        stampAmperageDelta_Setup = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_AMPERAGE_DELTA].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_AMPERAGE_DELTA].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_PRESSURE_UPPER].Object != stampPressureUpper_Setup)
    {
        stampPressureUpper_Setup = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_PRESSURE_UPPER].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_PRESSURE_UPPER].Status = 1;
    }
    
    if(*oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_PRESSURE_LOWER].Object != stampPressureLower_Setup)
    {
        stampPressureLower_Setup = *oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_PRESSURE_LOWER].Object;
        oLCD_U_Setting_Parameter[_LCD_U_SETTING_PARAMETER_PRESSURE_LOWER].Status = 1;
    }
    
    DLCD_Setting_Parameter();
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
    static uint16_t stampAmperageUpper = 0;
    static uint16_t stampAmperageDelta = 0;
    static uint16_t stampPressureUpper = 0;
    static uint16_t stampPressureLower = 0;
    
    if(*oLCD_U_Main_2[_LCD_U_MAIN_2_AMPERAGE_UPPER].Object != stampAmperageUpper)
    {
        stampAmperageUpper = *oLCD_U_Main_2[_LCD_U_MAIN_2_AMPERAGE_UPPER].Object;
        oLCD_U_Main_2[_LCD_U_MAIN_2_AMPERAGE_UPPER].Status = 1;
    }
    
    if(*oLCD_U_Main_2[_LCD_U_MAIN_2_AMPERAGE_DELTA].Object != stampAmperageDelta)
    {
        stampAmperageDelta = *oLCD_U_Main_2[_LCD_U_MAIN_2_AMPERAGE_DELTA].Object;
        oLCD_U_Main_2[_LCD_U_MAIN_2_AMPERAGE_DELTA].Status = 1;
    }
    
    if(*oLCD_U_Main_2[_LCD_U_MAIN_2_PRESSURE_UPPER].Object != stampPressureUpper)
    {
        stampPressureUpper = *oLCD_U_Main_2[_LCD_U_MAIN_2_PRESSURE_UPPER].Object;
        oLCD_U_Main_2[_LCD_U_MAIN_2_PRESSURE_UPPER].Status = 1;
    }
    
    if(*oLCD_U_Main_2[_LCD_U_MAIN_2_PRESSURE_LOWER].Object != stampPressureLower)
    {
        stampPressureLower = *oLCD_U_Main_2[_LCD_U_MAIN_2_PRESSURE_LOWER].Object;
        oLCD_U_Main_2[_LCD_U_MAIN_2_PRESSURE_LOWER].Status = 1;
    }
    
    DLCD_Main_2();
}

void DLCD_Main_1_Alter(void)
{
    static uint8_t stampAmperageI1 = 0;
    static uint8_t stampAmperageI2 = 0;
    static uint8_t stampAmperageI3 = 0;
    static uint8_t stampPressure = 0;
    static uint8_t stampStatusAmperage = _MOTOR_OK;
    static uint8_t stampStatusPressure = _MOTOR_OK;
    static uint8_t stampMachine = _MACHINE_OFF;
    static uint8_t stampChanelPressure = _CHANEL_P1;
    static uint16_t stampIdSlave = 0; 
    static uint8_t stampStateControl = _RS485_MOTOR_ON;
    
    if(*oLCD_U_Main_1[_LCD_U_MAIN_1_ID_SLAVE].Object != stampIdSlave)
    {
        stampIdSlave = *oLCD_U_Main_1[_LCD_U_MAIN_1_ID_SLAVE].Object;
        oLCD_U_Main_1[_LCD_U_MAIN_1_ID_SLAVE].Status = 1;
    }
    
    if(stampStatusAmperage != sParamMotor.StatusAmperage)
    {
        stampStatusAmperage = sParamMotor.StatusAmperage;
        
        if(sParamMotor.StatusAmperage == _MOTOR_OK)
            oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_AMPERAGE_OK].Status = 1;
        else
            oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_AMPERAGE_FAIL].Status = 1;
    }
    
    if(stampStatusPressure != sParamMotor.StatusPressure)
    {
        stampStatusPressure = sParamMotor.StatusPressure;
        
        if(sParamMotor.StatusPressure == _MOTOR_OK)
            oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_PRESSURE_OK].Status = 1;
        else
            oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_PRESSURE_FAIL].Status = 1;
    }
    
    if(stampMachine != sParamMotor.StateMachine)
    {
        stampMachine = sParamMotor.StateMachine;
        
        switch(sParamMotor.StateMachine)
        {
            case _MACHINE_OFF:
              oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_MACHINE_OFF].Status = 1;
              break;
              
            case _MACHINE_MAN:
              oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_MACHINE_MAN].Status = 1;
              break;
              
            case _MACHINE_AUTO:
              oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_MACHINE_AUTO].Status = 1;
              break;
              
            default:
              break;
        }
    }
    
    if(stampStateControl != sInforRs485.State_OnOff_Motor)
    {
        stampStateControl = sInforRs485.State_OnOff_Motor;
        
        switch(sInforRs485.State_OnOff_Motor)
        {
            case _RS485_MOTOR_ON:
              oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_CONTROL_ON].Status = 1;
              break;
              
            case _RS485_MOTOR_OFF:
              oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_CONTROL_OFF].Status = 1;
              break;
              
            case _RS485_MOTOR_FAIL:
              oLCD_C_Main_1[_LCD_C_MAIN_1_STATE_CONTROL_FAIL].Status = 1;
              break;
              
            default:
              break;
        }
    }
    
    if(*oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_1].Object != stampAmperageI1)
    {
        stampAmperageI1 = *oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_1].Object;
        oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_1].Status = 1;
    }
    
    if(*oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_2].Object != stampAmperageI2)
    {
        stampAmperageI2 = *oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_2].Object;
        oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_2].Status = 1;
    }
    
    if(*oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_3].Object != stampAmperageI3)
    {
        stampAmperageI3 = *oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_3].Object;
        oLCD_U_Main_1[_LCD_U_MAIN_1_AMPERAGE_3].Status = 1;
    }
    
    if(*oLCD_U_Main_1[_LCD_U_MAIN_1_PRESSURE].Object != stampPressure)
    {
        stampPressure = *oLCD_U_Main_1[_LCD_U_MAIN_1_PRESSURE].Object;
        oLCD_U_Main_1[_LCD_U_MAIN_1_PRESSURE].Status = 1;
    }
    
    if(stampChanelPressure != sMeasureMotor.Pressure_Chanel)
    {
        if(sMeasureMotor.Pressure_Chanel == _CHANEL_P1)
            oLCD_C_Main_1[_LCD_C_MAIN_1_PRESSURE_1].Status = 1;
        else
            oLCD_C_Main_1[_LCD_C_MAIN_1_PRESSURE_2].Status = 1;
        
        stampChanelPressure = sMeasureMotor.Pressure_Chanel;
    }
    
    DLCD_Main_1();
}

/*===================== Function Handle =================*/
void Display_Control_Setting_Left(uint8_t Page)
{
    if(Page <= _CONTROL_CLEAR)
    {
      for(uint8_t i = 0 ; i < _CONTROL_CLEAR; i++)
      {
        if(i == Page)
        {
            if(i != _CONTROL_CLEAR)
            {
                CLCD_SetCursor(&LCD, 0, i);
                CLCD_WriteChar(&LCD, '>');
            }
        }
        else
        {
            CLCD_SetCursor(&LCD, 0, i);
            CLCD_WriteChar(&LCD, ' '); 
        }
      }
    }
}

void Display_Control_Setting_Right(uint8_t Page)
{
    if(Page <= _CONTROL_CLEAR)
    {
      for(uint8_t i = 0 ; i < _CONTROL_CLEAR; i++)
      {
        if(i == Page)
        {
            if(i != _CONTROL_CLEAR)
            {
                CLCD_SetCursor(&LCD, 15, i);
                CLCD_WriteChar(&LCD, '<');
            }
        }
        else
        {
            CLCD_SetCursor(&LCD, 15, i);
            CLCD_WriteChar(&LCD, ' '); 
        }
      }
    }
}

/*===================== Save and Init ===================*/

void Init_LCD(void)
{
    CLCD_4BIT_Init(&LCD, 16,4, MCU_RS_GPIO_Port, MCU_RS_Pin ,MCU_EN_GPIO_Port, MCU_EN_Pin,
                               LCD_D4_GPIO_Port, LCD_D4_Pin,LCD_D5_GPIO_Port, LCD_D5_Pin,
                               LCD_D6_GPIO_Port, LCD_D6_Pin,LCD_D7_GPIO_Port, LCD_D7_Pin);
    HAL_Delay(50);
    CLCD_Clear(&LCD);
    CLCD_SetCursor(&LCD, 1, 1);
    CLCD_WriteString(&LCD, "SAO VIET .JSC"); 
    
    CLCD_SetCursor(&LCD, 1, 3);
    CLCD_WriteString(&LCD, "Please wait..."); 
}

/*===================== Task and Init ===================*/
void    Init_AppLcd(void)
{
    Init_LCD();
}

uint8_t AppLcd_Task(void)
{
    uint8_t i = 0;
    uint8_t Result = false;
    for( i = 0; i < _EVENT_LCD_END; i++)
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


