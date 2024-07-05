#include "user_app_menu_display.h"
#include "user_define.h"

/*============== Function static ===============*/
static uint8_t fevent_menu_entry(uint8_t event);
static uint8_t fevent_menu_display_alter(uint8_t event);
static uint8_t fevent_menu_display_back_main_1(uint8_t event);
static uint8_t fevent_off_speaker(uint8_t event);
/*=================== struct ==================*/
sEvent_struct               sEventAppMenu[] = 
{
  {_EVENT_MENU_ENTRY,                   1, 5, 0,                    fevent_menu_entry},
  {_EVENT_MENU_DISPLAY_ALTER,           0, 5, 20,                   fevent_menu_display_alter},

  {_EVENT_MENU_DISPLAY_BACK_MAIN_1,     0, 5, TIME_BACK_MAIN_1,     fevent_menu_display_back_main_1},
  
  {_EVENT_OFF_SPEAKER,                  0, 5, 0,                    fevent_off_speaker},
};

Struct_Menu_State               sMenuState={0};
Struct_Menu_Stamp               sMenuStamp={0};
Struct_Password                 sPassword ={0};
/*================= Function Handle ==============*/
static uint8_t fevent_menu_entry(uint8_t event)
{
    DLCD_Main_1_Entry();
    fevent_active(sEventAppMenu, _EVENT_MENU_DISPLAY_ALTER);
    return 1;
}

static uint8_t fevent_menu_display_alter(uint8_t event)
{
    switch(sMenuState.Screen)
    {
        case _MENU_MAIN_1:
          DLCD_Main_1_Alter();
          break;
          
        case _MENU_MAIN_2:
          DLCD_Main_2_Alter();
          break;
          
        case _MENU_LOGIN:
          DLCD_Login_Alter();
          break;     
          
        case _MENU_SETTING_PARAMETER:
          DLCD_Setting_Parameter_Alter();
          break;   
          
        case _MENU_SETTING_OPTION:
          DLCD_Setting_Option_Alter();
          break;
          
        case _MENU_SETTING_PASSWORD:
          DLCD_Setting_Password_Alter();
          break;  
            
        default:
          break;
    }
    fevent_enable(sEventAppMenu, event);
    return 1;
}


static uint8_t fevent_menu_display_back_main_1(uint8_t event)
{
    if(sMenuState.Screen != _MENU_MAIN_1)
    {
          sMenuState.Screen = _MENU_MAIN_1;
          DLCD_Main_1_Entry();
          
          Stamp_Menu_Exit();
//          Menu_ResetState();
          Check_Password();
    }
    return 1;
}

static uint8_t fevent_off_speaker(uint8_t event)
{
    HAL_GPIO_WritePin(Speaker_GPIO_Port, Speaker_Pin, GPIO_PIN_RESET);
    return 1;
}

/*====================== Function Use Button Click =======================*/
void User_Button_Enter_Click(void)
{
    On_Speaker(50);
    Handle_Menu_Button(_BT_ENTER_CLICK);
}

void User_Button_Exit_Click(void)
{
    On_Speaker(50);
    Handle_Menu_Button(_BT_EXIT_CLICK);
}

void User_Button_Up_Click(void)
{
    On_Speaker(50);
    Handle_Menu_Button(_BT_UP_CLICK);
}

void User_Button_Down_Click(void)
{
    On_Speaker(50);
    Handle_Menu_Button(_BT_DOWN_CLICK);
}
/*====================== Function Use Button Hold Cycle =======================*/
void User_Button_Enter_Hold_Cycle(void)
{
    Handle_Menu_Button(_BT_ENTER_HOLD_CYCLE);
}

void User_Button_Exit_Hold_Cycle(void)
{
    Handle_Menu_Button(_BT_EXIT_HOLD_CYCLE);
}

void User_Button_Up_Hold_Cycle(void)
{
    Handle_Menu_Button(_BT_UP_HOLD_CYCLE);
}

void User_Button_Down_Hold_Cycle(void)
{
    Handle_Menu_Button(_BT_DOWN_HOLD_CYCLE);
}
/*====================== Function Use Button Hold Once =======================*/
void User_Button_Enter_Hold_Once(void)
{
    On_Speaker(50);
    Handle_Menu_Button(_BT_ENTER_HOLD_ONCE);
}

void User_Button_Exit_Hold_Once(void)
{
    On_Speaker(50);
    Handle_Menu_Button(_BT_EXIT_HOLD_ONCE);
}

void User_Button_Up_Hold_Once(void)
{
    On_Speaker(50);
    Handle_Menu_Button(_BT_UP_HOLD_ONCE);
}

void User_Button_Down_Hold_Once(void)
{
    On_Speaker(50);
    Handle_Menu_Button(_BT_DOWN_HOLD_ONCE);
}

/*============================ Function Handle =========================*/
void    Handle_Menu_Button(uint8_t KindButton)
{
    fevent_enable(sEventAppMenu, _EVENT_MENU_DISPLAY_BACK_MAIN_1);
    switch(sMenuState.Screen)
    {
        case _MENU_MAIN_1:
          BT_Menu_Main_1(KindButton);
          break;
          
        case _MENU_MAIN_2:
          BT_Menu_Main_2(KindButton);
          break;
          
        case _MENU_LOGIN:
          BT_Menu_Login(KindButton);
          break;
          
        case _MENU_SETTING_MAIN:
          BT_Menu_Setting_Main(KindButton);
          break;

        case _MENU_SETTING_PARAMETER:
          BT_Menu_Setting_Parameter(KindButton);
          break;
          
        case _MENU_SETTING_OPTION:
          BT_Menu_Setting_Option(KindButton);
          break;
          
        case _MENU_SETTING_PASSWORD:
          BT_Menu_Setting_Password(KindButton);
          break;
          
        default:
          break;
    }
}

void        BT_Menu_Main_1(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          sMenuState.Screen = _MENU_MAIN_2;
          DLCD_Main_2_Entry();
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
        case _BT_EXIT_HOLD_CYCLE:
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          break;
    
        case _BT_ENTER_HOLD_ONCE:
          sMenuState.Screen = _MENU_LOGIN;
          DLCD_Login_Entry();
          break;
          
        case _BT_EXIT_HOLD_ONCE:
          break;
          
        case _BT_UP_HOLD_ONCE:
        case _BT_DOWN_HOLD_ONCE:
//          if(sParamMotor.StateMotor == _MOTOR_OFF)
//            fevent_active(sEventAppMotor, _EVENT_ON_MOTOR);
          break;
        
        default:
          break;
    }
}
void        BT_Menu_Main_2(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          sMenuState.Screen = _MENU_MAIN_1;
          DLCD_Main_1_Entry();
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
          sMenuState.Screen = _MENU_MAIN_1;
          DLCD_Main_1_Entry();
          break;
          
        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          break;
    
        case _BT_ENTER_HOLD_ONCE:
          sMenuState.Screen = _MENU_LOGIN;
          DLCD_Login_Entry();
          break;
          
        case _BT_EXIT_HOLD_ONCE:
        case _BT_UP_HOLD_ONCE:
        case _BT_DOWN_HOLD_ONCE:
          break;
        
        default:
          break;
    }
}
void        BT_Menu_Login(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          switch(sMenuState.Login)
          {
            case _LOGIN_PASS_1:
                sMenuState.Login = _LOGIN_PASS_2;
                oLCD_C_Login[_LCD_C_LOGIN_PASS_2].Status = 1;
                break;
                
            case _LOGIN_PASS_2:
                sMenuState.Login = _LOGIN_PASS_3;
                oLCD_C_Login[_LCD_C_LOGIN_PASS_3].Status = 1;
                break;
                
            case _LOGIN_PASS_3:
                sMenuState.Login = _LOGIN_PASS_4;
                oLCD_C_Login[_LCD_C_LOGIN_PASS_4].Status = 1;
                break;
                
            case _LOGIN_PASS_4:
                sMenuState.Login = _LOGIN_PASS_5;
                oLCD_C_Login[_LCD_C_LOGIN_PASS_5].Status = 1;
                break;
                
            case _LOGIN_PASS_5:
                sMenuState.Login = _LOGIN_PASS_6;
                oLCD_C_Login[_LCD_C_LOGIN_PASS_6].Status = 1;
                oLCD_C_Login[_LCD_C_LOGIN_LOGIN].Status = 1;
                break;
                
            case _LOGIN_PASS_6:
                if(Check_Password() == 1)   
                {
                  sMenuState.Screen = _MENU_SETTING_MAIN;
                  DLCD_Setting_Main_Entry();
                }
                else
                {
                  DLCD_Login_Entry();
                  oLCD_C_Login[_LCD_C_LOGIN_WRONG].Status = 1;
                }
                break;
                
              default: 
                break;
          }
          DLCD_Login();
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
          switch(sMenuState.Login)
          {
              case _LOGIN_PASS_1:
                sMenuState.Screen = _MENU_MAIN_1;
                Check_Password();
                DLCD_Main_1_Entry();
                break;
                
              case _LOGIN_PASS_2:
                sMenuState.Login = _LOGIN_PASS_1;
                oLCD_C_Login[_LCD_C_LOGIN_PASS_1].Status = 1;
                break;
                
              case _LOGIN_PASS_3:
                sMenuState.Login = _LOGIN_PASS_2;
                oLCD_C_Login[_LCD_C_LOGIN_PASS_2].Status = 1;
                break;
                
              case _LOGIN_PASS_4:
                sMenuState.Login = _LOGIN_PASS_3;
                oLCD_C_Login[_LCD_C_LOGIN_PASS_3].Status = 1;
                break;
                
              case _LOGIN_PASS_5:
                sMenuState.Login = _LOGIN_PASS_4;
                oLCD_C_Login[_LCD_C_LOGIN_PASS_4].Status = 1;
                break;
                
              case _LOGIN_PASS_6:
                sMenuState.Login = _LOGIN_PASS_5;
                oLCD_C_Login[_LCD_C_LOGIN_PASS_5].Status = 1;
                oLCD_C_Login[_LCD_C_LOGIN_ENTER].Status = 1;
                break;
          }
          DLCD_Login();
          break;
          
        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          switch(sMenuState.Login)
          {
            case _LOGIN_PASS_1:
              if(sMenuStamp.sPassLogin.Pass1 <9)  sMenuStamp.sPassLogin.Pass1++;
              break;
            
            case _LOGIN_PASS_2:
              if(sMenuStamp.sPassLogin.Pass2 <9)  sMenuStamp.sPassLogin.Pass2++;
              break;
              
            case _LOGIN_PASS_3:
              if(sMenuStamp.sPassLogin.Pass3 <9)  sMenuStamp.sPassLogin.Pass3++;
              break;
              
            case _LOGIN_PASS_4:
              if(sMenuStamp.sPassLogin.Pass4 <9)  sMenuStamp.sPassLogin.Pass4++;
              break;
              
            case _LOGIN_PASS_5:
              if(sMenuStamp.sPassLogin.Pass5 <9)  sMenuStamp.sPassLogin.Pass5++;
              break;
              
            case _LOGIN_PASS_6:
              if(sMenuStamp.sPassLogin.Pass6 <9)  sMenuStamp.sPassLogin.Pass6++;
              break;
              
              default:
                break;
          }
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          switch(sMenuState.Login)
          {
            case _LOGIN_PASS_1:
              if(sMenuStamp.sPassLogin.Pass1 > 0)    sMenuStamp.sPassLogin.Pass1--;
              break;
            
            case _LOGIN_PASS_2:
              if(sMenuStamp.sPassLogin.Pass2 > 0)    sMenuStamp.sPassLogin.Pass2--;
              break;
              
            case _LOGIN_PASS_3:
              if(sMenuStamp.sPassLogin.Pass3 > 0)    sMenuStamp.sPassLogin.Pass3--;
              break;
              
            case _LOGIN_PASS_4:
              if(sMenuStamp.sPassLogin.Pass4 > 0)    sMenuStamp.sPassLogin.Pass4--;
              break;
              
            case _LOGIN_PASS_5:
              if(sMenuStamp.sPassLogin.Pass5 > 0)    sMenuStamp.sPassLogin.Pass5--;
              break;
              
            case _LOGIN_PASS_6:
              if(sMenuStamp.sPassLogin.Pass6 > 0)    sMenuStamp.sPassLogin.Pass6--;
              break;
              
              default:
                break;
          }
          break;
    
        case _BT_ENTER_HOLD_ONCE:
        case _BT_EXIT_HOLD_ONCE:
          Reset_Chip();
          break;
          
        case _BT_UP_HOLD_ONCE:
        case _BT_DOWN_HOLD_ONCE:
          break;
        
        default:
          break;
    }
}

void        BT_Menu_Setting_Main(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          switch(sMenuState.SettingMain)
          {
            case _SETTING_MAIN_PARAMETER:
                sMenuState.Screen = _MENU_SETTING_PARAMETER;
                DLCD_Setting_Parameter_Entry();
                break;
               
                
            case _SETTING_MAIN_OPTION:
                sMenuState.Screen = _MENU_SETTING_OPTION;
                DLCD_Setting_Option_Entry();
                break;
                
            case _SETTING_MAIN_PASSWORD:
                sMenuState.Screen = _MENU_SETTING_PASSWORD;
                DLCD_Setting_Password_Entry();
                break;
                
              default: 
                break;
          }
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
          sMenuState.Screen = _MENU_MAIN_1;
          DLCD_Main_1_Entry();
          break;
          
        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          switch(sMenuState.SettingMain)
          {
              case _SETTING_MAIN_OPTION:
                sMenuState.SettingMain = _SETTING_MAIN_PARAMETER;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PARAMETER].Rol);
                break;
                
              case _SETTING_MAIN_PASSWORD:
                sMenuState.SettingMain = _SETTING_MAIN_OPTION;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_OPTION].Rol);
                break;

              default:
                break;
          }
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          switch(sMenuState.SettingMain)
          {
              case _SETTING_MAIN_PARAMETER:
                sMenuState.SettingMain = _SETTING_MAIN_OPTION;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_OPTION].Rol);
                break;
                
              case _SETTING_MAIN_OPTION:
                sMenuState.SettingMain = _SETTING_MAIN_PASSWORD;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PASSWORD].Rol);
                break;

              default:
                break;
          }
          break;
          
        case _BT_ENTER_HOLD_ONCE:
        case _BT_EXIT_HOLD_ONCE:
        case _BT_UP_HOLD_ONCE:
        case _BT_DOWN_HOLD_ONCE:
          break;
        
        default:
          break;
    }
}

void        BT_Menu_Setting_Parameter(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          switch(sMenuState.SettingParameter)
          {     
              case _SETTING_AMPERAGE_UPPER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_AMPERAGE_UPPER_CHANGE;
                Display_Control_Setting_Right(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_AMPERAGE_UPPER].Rol);
                break;
                
              case _SETTING_AMPERAGE_DELTA_CHOOSE:
                sMenuState.SettingParameter = _SETTING_AMPERAGE_DELTA_CHANGE;
                Display_Control_Setting_Right(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_AMPERAGE_DELTA].Rol);
                break;
                
              case _SETTING_PRESSURE_UPPER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_PRESSURE_UPPER_CHANGE;
                Display_Control_Setting_Right(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PRESSURE_UPPER].Rol);
                break;
                
              case _SETTING_PRESSURE_LOWER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_PRESSURE_LOWER_CHANGE;
                Display_Control_Setting_Right(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PRESSURE_LOWER].Rol);
                break;

              case _SETTING_AMPERAGE_UPPER_CHANGE:
                sMenuState.SettingParameter = _SETTING_AMPERAGE_UPPER_CHOOSE;
                Save_AmperageWarning(sMenuStamp.Amperage_Upper, sParamMotor.Amperage_Delta);
                Display_Control_Setting_Right(_CONTROL_CLEAR);
                break;
                
              case _SETTING_AMPERAGE_DELTA_CHANGE:
                sMenuState.SettingParameter = _SETTING_AMPERAGE_DELTA_CHOOSE;
                Save_AmperageWarning(sParamMotor.Amperage_Upper, sMenuStamp.Amperage_Delta);
                Display_Control_Setting_Right(_CONTROL_CLEAR);
                break;
                
              case _SETTING_PRESSURE_UPPER_CHANGE:
                sMenuState.SettingParameter = _SETTING_PRESSURE_UPPER_CHOOSE;
                Save_PressureWarning(sMenuStamp.Pressure_Upper, sParamMotor.Pressure_Lower);
                Display_Control_Setting_Right(_CONTROL_CLEAR);
                break;
                
              case _SETTING_PRESSURE_LOWER_CHANGE:
                sMenuState.SettingParameter = _SETTING_PRESSURE_LOWER_CHOOSE;
                Save_PressureWarning(sParamMotor.Pressure_Upper, sMenuStamp.Pressure_Lower);
                Display_Control_Setting_Right(_CONTROL_CLEAR);
                break;

              default:
                break;
          }
          DLCD_Setting_Parameter();
            break;

          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
          switch(sMenuState.SettingParameter)
          {
              case _SETTING_AMPERAGE_UPPER_CHOOSE:
              case _SETTING_AMPERAGE_DELTA_CHOOSE:
              case _SETTING_PRESSURE_UPPER_CHOOSE:
              case _SETTING_PRESSURE_LOWER_CHOOSE:
                sMenuState.Screen = _MENU_SETTING_MAIN;
                DLCD_Setting_Main_Entry();
                break;
                
              case _SETTING_AMPERAGE_UPPER_CHANGE:
              case _SETTING_AMPERAGE_DELTA_CHANGE:
              case _SETTING_PRESSURE_UPPER_CHANGE:
              case _SETTING_PRESSURE_LOWER_CHANGE:
                switch(sMenuState.SettingParameter)
                {
                  case _SETTING_AMPERAGE_UPPER_CHANGE:
                    sMenuState.SettingParameter = _SETTING_AMPERAGE_UPPER_CHOOSE;
                    break;
                    
                  case _SETTING_AMPERAGE_DELTA_CHANGE:
                    sMenuState.SettingParameter = _SETTING_AMPERAGE_DELTA_CHOOSE;
                    break;
                    
                  case _SETTING_PRESSURE_UPPER_CHANGE:
                    sMenuState.SettingParameter = _SETTING_PRESSURE_UPPER_CHOOSE;
                    break;
                    
                  case _SETTING_PRESSURE_LOWER_CHANGE:
                    sMenuState.SettingParameter = _SETTING_PRESSURE_LOWER_CHOOSE;
                    break;

                    default:
                      break;
                }
                
                Stamp_Menu_Exit();
                DLCD_Setting_Parameter();
                Display_Control_Setting_Right(_CONTROL_CLEAR);
                break;
            
              default:
                break;
          }
            break;
          
        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          switch(sMenuState.SettingParameter)
          {
              case _SETTING_AMPERAGE_DELTA_CHOOSE:
                sMenuState.SettingParameter = _SETTING_AMPERAGE_UPPER_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_AMPERAGE_UPPER].Rol);
                break;
            
              case _SETTING_PRESSURE_UPPER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_AMPERAGE_DELTA_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_AMPERAGE_DELTA].Rol);
                break;
                
              case _SETTING_PRESSURE_LOWER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_PRESSURE_UPPER_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PRESSURE_UPPER].Rol);
                break;
                
              case _SETTING_AMPERAGE_UPPER_CHANGE:
                if(sMenuStamp.Amperage_Upper < AMPERAGE_UPPER_MAX) 
                  sMenuStamp.Amperage_Upper++;
                break;
                
              case _SETTING_AMPERAGE_DELTA_CHANGE:
                if(sMenuStamp.Amperage_Delta < AMPERAGE_DELTA_MAX) 
                  sMenuStamp.Amperage_Delta++;
                break;
                
              case _SETTING_PRESSURE_UPPER_CHANGE:
                if(sMenuStamp.Pressure_Upper < PRESSURE_UPPER_MAX) 
                  sMenuStamp.Pressure_Upper++;
                break;

              case _SETTING_PRESSURE_LOWER_CHANGE:
                if(sMenuStamp.Pressure_Lower < sParamMotor.Pressure_Upper - 1) 
                    sMenuStamp.Pressure_Lower++;
                break;
                
              default:
                break;
          }
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          switch(sMenuState.SettingParameter)
          {
              case _SETTING_AMPERAGE_UPPER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_AMPERAGE_DELTA_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_AMPERAGE_DELTA].Rol);
                break;
            
              case _SETTING_AMPERAGE_DELTA_CHOOSE:
                sMenuState.SettingParameter = _SETTING_PRESSURE_UPPER_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PRESSURE_UPPER].Rol);
                break;
                
              case _SETTING_PRESSURE_UPPER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_PRESSURE_LOWER_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PRESSURE_LOWER].Rol);
                break;
                
              case _SETTING_AMPERAGE_UPPER_CHANGE:
                if(sMenuStamp.Amperage_Upper > AMPERGAE_UPPER_MIN) 
                    sMenuStamp.Amperage_Upper--;
                break;
                
              case _SETTING_AMPERAGE_DELTA_CHANGE:
                if(sMenuStamp.Amperage_Delta > AMPERGAE_DELTA_MIN) 
                    sMenuStamp.Amperage_Delta--;
                break;
                
              case _SETTING_PRESSURE_UPPER_CHANGE:
                if(sMenuStamp.Pressure_Upper > sParamMotor.Pressure_Lower + 1) 
                    sMenuStamp.Pressure_Upper--;
                break;
                
              case _SETTING_PRESSURE_LOWER_CHANGE:
                if(sMenuStamp.Pressure_Lower > PRESSURE_LOWER_MIN) 
                    sMenuStamp.Pressure_Lower--;
                break;

              default:
                break;
          }
          break;
          
        case _BT_ENTER_HOLD_ONCE:
        case _BT_EXIT_HOLD_ONCE:
        case _BT_UP_HOLD_ONCE:
        case _BT_DOWN_HOLD_ONCE:
          break;
        
        default:
          break;
    }
}

void        BT_Menu_Setting_Option(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          switch(sMenuState.SettingOption)
          {     
              case _SETTING_AMPERAGE_OPTION_CHOOSE:
                sMenuState.SettingOption = _SETTING_AMPERAGE_OPTION_CHANGE;
                Display_Control_Setting_Right(oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_AMPERAGE].Rol);
                break;
                
              case _SETTING_PRESSURE_OPTION_CHOOSE:
                sMenuState.SettingOption = _SETTING_PRESSURE_OPTION_CHANGE;
                Display_Control_Setting_Right(oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_PRESSURE].Rol);
                break;
                
              case _SETTING_CONSTANT_PRESSURE_CHOOSE:
                sMenuState.SettingOption = _SETTING_CONSTANT_PRESSURE_CHANGE;
                Display_Control_Setting_Right(oLCD_C_Setting_Option[_LCD_C_SETTING_CONSTANT_PRESSURE].Rol);
                break;
                
              case _SETTING_ID_SLAVE_CHOOSE:
                sMenuState.SettingOption = _SETTING_ID_SLAVE_CHANGE;
                Display_Control_Setting_Right(oLCD_C_Setting_Option[_LCD_C_SETTING_ID_SLAVE].Rol);
                break;
                
              case _SETTING_AMPERAGE_OPTION_CHANGE:
                sMenuState.SettingOption = _SETTING_AMPERAGE_OPTION_CHOOSE;
                Save_OptionWarning(sMenuStamp.Amperage_Option, sParamMotor.Pressure_Option, sParamMotor.Pressure_Constant);
                Display_Control_Setting_Right(_CONTROL_CLEAR);
                break;
                
              case _SETTING_PRESSURE_OPTION_CHANGE:
                sMenuState.SettingOption = _SETTING_PRESSURE_OPTION_CHOOSE;
                Save_OptionWarning(sParamMotor.Amperage_Option, sMenuStamp.Pressure_Option, sParamMotor.Pressure_Constant);
                Display_Control_Setting_Right(_CONTROL_CLEAR);
                break;
                
              case _SETTING_CONSTANT_PRESSURE_CHANGE:
                sMenuState.SettingOption = _SETTING_CONSTANT_PRESSURE_CHOOSE;
                Save_OptionWarning(sParamMotor.Amperage_Option, sParamMotor.Pressure_Option, sMenuStamp.Pressure_Constant);
                Display_Control_Setting_Right(_CONTROL_CLEAR);
                break;
                
              case _SETTING_ID_SLAVE_CHANGE:
                sMenuState.SettingOption = _SETTING_ID_SLAVE_CHOOSE;
                Save_IDSlave(sMenuStamp.ID_Slave);
                Display_Control_Setting_Right(_CONTROL_CLEAR);
                break;
                
              default:
                break;
          }
          DLCD_Setting_Option();
            break;

          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
          switch(sMenuState.SettingOption)
          {
              case _SETTING_AMPERAGE_OPTION_CHOOSE:
              case _SETTING_PRESSURE_OPTION_CHOOSE:
              case _SETTING_CONSTANT_PRESSURE_CHOOSE:
              case _SETTING_ID_SLAVE_CHOOSE:
                sMenuState.Screen = _MENU_SETTING_MAIN;
                DLCD_Setting_Main_Entry();
                break;
                
              case _SETTING_AMPERAGE_OPTION_CHANGE:
              case _SETTING_PRESSURE_OPTION_CHANGE:
              case _SETTING_CONSTANT_PRESSURE_CHANGE:
              case _SETTING_ID_SLAVE_CHANGE:
                switch(sMenuState.SettingOption)
                {
                  case _SETTING_AMPERAGE_OPTION_CHANGE:
                    sMenuState.SettingOption = _SETTING_AMPERAGE_OPTION_CHOOSE;
                    break;
                    
                  case _SETTING_PRESSURE_OPTION_CHANGE:
                    sMenuState.SettingOption = _SETTING_PRESSURE_OPTION_CHOOSE;
                    break;
                    
                  case _SETTING_CONSTANT_PRESSURE_CHANGE:
                    sMenuState.SettingOption = _SETTING_CONSTANT_PRESSURE_CHOOSE;
                    break;
                    
                  case _SETTING_ID_SLAVE_CHANGE:
                    sMenuState.SettingOption = _SETTING_ID_SLAVE_CHOOSE;
                    break;
                    
                    default:
                      break;
                }
                
                Stamp_Menu_Exit();
                DLCD_Setting_Option();
                Display_Control_Setting_Right(_CONTROL_CLEAR);
                break;
            
              default:
                break;
          }
            break;
          
        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          switch(sMenuState.SettingOption)
          {
              case _SETTING_PRESSURE_OPTION_CHOOSE:
                sMenuState.SettingOption = _SETTING_AMPERAGE_OPTION_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_AMPERAGE].Rol);
                break;
            
              case _SETTING_CONSTANT_PRESSURE_CHOOSE:
                sMenuState.SettingOption = _SETTING_PRESSURE_OPTION_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_PRESSURE].Rol);
                break;
                
              case _SETTING_ID_SLAVE_CHOOSE:
                sMenuState.SettingOption = _SETTING_CONSTANT_PRESSURE_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Option[_LCD_C_SETTING_CONSTANT_PRESSURE].Rol);
                break;
                
              case _SETTING_AMPERAGE_OPTION_CHANGE:
                switch(sMenuStamp.Amperage_Option)
                {
                    case _AMPERAGE_OPTION_OFF:
                      sMenuStamp.Amperage_Option = _AMPERAGE_OPTION_ON;
                      break;
                      
                    case _AMPERAGE_OPTION_ON:
                      sMenuStamp.Amperage_Option = _AMPERAGE_OPTION_OFF;
                      break;
                      
                    default:
                      break;
                }   
                break;
                
              case _SETTING_PRESSURE_OPTION_CHANGE:
                switch(sMenuStamp.Pressure_Option)
                {
                    case _PRESSURE_OPTION_OFF:
                      sMenuStamp.Pressure_Option = _PRESSURE_OPTION_ON;
                      break;
                      
                    case _PRESSURE_OPTION_ON:
                      sMenuStamp.Pressure_Option = _PRESSURE_OPTION_OFF;
                      break;
                      
//                    case _PRESSURE_OPTION_P2:
//                      sMenuStamp.Pressure_Option = _PRESSURE_OPTION_P1;
//                      break;
                      
//                    case _PRESSURE_OPTION_P3:
//                      sMenuStamp.Pressure_Option = _PRESSURE_OPTION_P4;
//                      break;
//                      
//                    case _PRESSURE_OPTION_P4:
//                      sMenuStamp.Pressure_Option = _PRESSURE_OPTION_OFF;
//                      break;
                      
                    default:
                      break;
                }   
                break;
                
              case _SETTING_CONSTANT_PRESSURE_CHANGE:
                if(sMenuStamp.Pressure_Constant < PRESSURE_CONSTANT_MAX)
                  sMenuStamp.Pressure_Constant++;
                    
                break;
                
              case _SETTING_ID_SLAVE_CHANGE:
                if(sMenuStamp.ID_Slave < SLAVE_ID_MAX)
                  sMenuStamp.ID_Slave++;
                    
                break;
 
              default:
                break;
          }
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          switch(sMenuState.SettingOption)
          {
              case _SETTING_AMPERAGE_OPTION_CHOOSE:
                sMenuState.SettingOption = _SETTING_PRESSURE_OPTION_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Option[_LCD_C_SETTING_OPTION_PRESSURE].Rol);
                break;
                
              case _SETTING_PRESSURE_OPTION_CHOOSE:
                sMenuState.SettingOption = _SETTING_CONSTANT_PRESSURE_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Option[_LCD_C_SETTING_CONSTANT_PRESSURE].Rol);
                break;
                
              case _SETTING_CONSTANT_PRESSURE_CHOOSE:
                sMenuState.SettingOption = _SETTING_ID_SLAVE_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Option[_LCD_C_SETTING_ID_SLAVE].Rol);
                break;
                
              case _SETTING_AMPERAGE_OPTION_CHANGE:
                switch(sMenuStamp.Amperage_Option)
                {
                    case _AMPERAGE_OPTION_OFF:
                      sMenuStamp.Amperage_Option = _AMPERAGE_OPTION_ON;
                      break;
                      
                    case _AMPERAGE_OPTION_ON:
                      sMenuStamp.Amperage_Option = _AMPERAGE_OPTION_OFF;
                      break;
                      
                    default:
                      break;
                }   
                break;
                
              case _SETTING_PRESSURE_OPTION_CHANGE:
                switch(sMenuStamp.Pressure_Option)
                {
                    case _PRESSURE_OPTION_OFF:
                      sMenuStamp.Pressure_Option = _PRESSURE_OPTION_ON;
                      break;
                      
                    case _PRESSURE_OPTION_ON:
                      sMenuStamp.Pressure_Option = _PRESSURE_OPTION_OFF;
                      break;
                      
//                    case _PRESSURE_OPTION_P2:
//                      sMenuStamp.Pressure_Option = _PRESSURE_OPTION_OFF;
//                      break;
                      
//                    case _PRESSURE_OPTION_P3:
//                      sMenuStamp.Pressure_Option = _PRESSURE_OPTION_P4;
//                      break;
//                      
//                    case _PRESSURE_OPTION_P4:
//                      sMenuStamp.Pressure_Option = _PRESSURE_OPTION_OFF;
//                      break;
                      
                    default:
                      break;
                }   
                break;
 
              case _SETTING_CONSTANT_PRESSURE_CHANGE:
                if(sMenuStamp.Pressure_Constant > PRESSURE_CONSTANT_MIN)
                    sMenuStamp.Pressure_Constant--;
                
                break;
                
              case _SETTING_ID_SLAVE_CHANGE:
                if(sMenuStamp.ID_Slave > 1)
                    sMenuStamp.ID_Slave--;
                
                break;
                
              default:
                break;
          }
          break;
          
        case _BT_ENTER_HOLD_ONCE:
        case _BT_EXIT_HOLD_ONCE:
        case _BT_UP_HOLD_ONCE:
        case _BT_DOWN_HOLD_ONCE:
          break;
        
        default:
          break;
    }
}

void        BT_Menu_Setting_Password(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          switch(sMenuState.SettingPassword)
          {
            case _CHANGE_PASS_1:
                sMenuState.SettingPassword = _CHANGE_PASS_2;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_2].Status = 1;
                break;
                
            case _CHANGE_PASS_2:
                sMenuState.SettingPassword = _CHANGE_PASS_3;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_3].Status = 1;
                break;
                
            case _CHANGE_PASS_3:
                sMenuState.SettingPassword = _CHANGE_PASS_4;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_4].Status = 1;
                break;
                
            case _CHANGE_PASS_4:
                sMenuState.SettingPassword = _CHANGE_PASS_5;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_5].Status = 1;
                break;
                
            case _CHANGE_PASS_5:
                sMenuState.SettingPassword = _CHANGE_PASS_6;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_6].Status = 1;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_SAVE].Status = 1;
                break;
                
            case _CHANGE_PASS_6:
                Save_Password();
                sMenuState.SettingPassword = _SUCCESS_PASS;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_SUCCESS].Status = 1;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_CLICK].Status = 1;
                break;
                
            case _SUCCESS_PASS:
                sMenuState.Screen = _MENU_MAIN_1;
                DLCD_Main_1_Entry();
                
              default: 
                break;
          }
          DLCD_Setting_Password();
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
          switch(sMenuState.SettingPassword)
          {
              case _CHANGE_PASS_1:
                Stamp_Menu_Exit();
                sMenuState.Screen = _MENU_SETTING_MAIN;
                DLCD_Setting_Main_Entry();
                break;
                
              case _CHANGE_PASS_2:
                sMenuState.SettingPassword = _CHANGE_PASS_1;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_1].Status = 1;
                break;
                
              case _CHANGE_PASS_3:
                sMenuState.SettingPassword = _CHANGE_PASS_2;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_2].Status = 1;
                break;
                
              case _CHANGE_PASS_4:
                sMenuState.SettingPassword = _CHANGE_PASS_3;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_3].Status = 1;
                break;
                
              case _CHANGE_PASS_5:
                sMenuState.SettingPassword = _CHANGE_PASS_4;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_4].Status = 1;
                break;
                
              case _CHANGE_PASS_6:
                sMenuState.SettingPassword = _CHANGE_PASS_5;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_5].Status = 1;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_ENTER].Status = 1;
                break;
                
              case _SUCCESS_PASS:
                sMenuState.Screen = _MENU_MAIN_1;
                Stamp_Menu_Exit();
                DLCD_Main_1_Entry();
          }
          DLCD_Setting_Password();
          break;
          
        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          switch(sMenuState.SettingPassword)
          {
            case _CHANGE_PASS_1:
              if(sMenuStamp.sPassWord.Pass1 <9)  sMenuStamp.sPassWord.Pass1++;
              break;
            
            case _CHANGE_PASS_2:
              if(sMenuStamp.sPassWord.Pass2 <9)  sMenuStamp.sPassWord.Pass2++;
              break;
              
            case _CHANGE_PASS_3:
              if(sMenuStamp.sPassWord.Pass3 <9)  sMenuStamp.sPassWord.Pass3++;
              break;
              
            case _CHANGE_PASS_4:
              if(sMenuStamp.sPassWord.Pass4 <9)  sMenuStamp.sPassWord.Pass4++;
              break;
              
            case _CHANGE_PASS_5:
              if(sMenuStamp.sPassWord.Pass5 <9)  sMenuStamp.sPassWord.Pass5++;
              break;
              
            case _CHANGE_PASS_6:
              if(sMenuStamp.sPassWord.Pass6 <9)  sMenuStamp.sPassWord.Pass6++;
              break;
              
              default:
                break;
          }
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          switch(sMenuState.SettingPassword)
          {
            case _CHANGE_PASS_1:
              if(sMenuStamp.sPassWord.Pass1 > 0)    sMenuStamp.sPassWord.Pass1--;
              break;
            
            case _CHANGE_PASS_2:
              if(sMenuStamp.sPassWord.Pass2 > 0)    sMenuStamp.sPassWord.Pass2--;
              break;
              
            case _CHANGE_PASS_3:
              if(sMenuStamp.sPassWord.Pass3 > 0)    sMenuStamp.sPassWord.Pass3--;
              break;
              
            case _CHANGE_PASS_4:
              if(sMenuStamp.sPassWord.Pass4 > 0)    sMenuStamp.sPassWord.Pass4--;
              break;
              
            case _CHANGE_PASS_5:
              if(sMenuStamp.sPassWord.Pass5 > 0)    sMenuStamp.sPassWord.Pass5--;
              break;
              
            case _CHANGE_PASS_6:
              if(sMenuStamp.sPassWord.Pass6 > 0)    sMenuStamp.sPassWord.Pass6--;
              break;
              
              default:
                break;
          }
          break;
    
        case _BT_ENTER_HOLD_ONCE:
        case _BT_EXIT_HOLD_ONCE:
        case _BT_UP_HOLD_ONCE:
        case _BT_DOWN_HOLD_ONCE:
          break;
        
        default:
          break;
    }
}
/*============================ Function Handle =========================*/
void Menu_ResetState(void)
{
    sMenuState.Login = 0;
    sMenuState.SettingMain = 0;
    sMenuState.SettingOption = 0;
    sMenuState.SettingParameter = 0;
    sMenuState.SettingPassword = 0;
}
/*==========================Function Handle=========================*/
void On_Speaker(uint16_t TimeOn)
{
    HAL_GPIO_WritePin(Speaker_GPIO_Port, Speaker_Pin, GPIO_PIN_SET);
    sEventAppMenu[_EVENT_OFF_SPEAKER].e_period = TimeOn;
    fevent_enable(sEventAppMenu, _EVENT_OFF_SPEAKER);
}

/*=========================  Function Handle LCD =======================*/
uint8_t Check_Password(void)
{
    uint8_t Result = 0;
    if(sMenuStamp.sPassLogin.Pass1 == sPassword.Pass1 &&
       sMenuStamp.sPassLogin.Pass2 == sPassword.Pass2 &&
       sMenuStamp.sPassLogin.Pass3 == sPassword.Pass3 &&
       sMenuStamp.sPassLogin.Pass4 == sPassword.Pass4 &&
       sMenuStamp.sPassLogin.Pass5 == sPassword.Pass5 &&
       sMenuStamp.sPassLogin.Pass6 == sPassword.Pass6 )
    {
        Result = 1;
    }
    
    if(sMenuStamp.sPassLogin.Pass1 == 2 &&
       sMenuStamp.sPassLogin.Pass2 == 6 &&
       sMenuStamp.sPassLogin.Pass3 == 0 &&
       sMenuStamp.sPassLogin.Pass4 == 5 &&
       sMenuStamp.sPassLogin.Pass5 == 0 &&
       sMenuStamp.sPassLogin.Pass6 == 0 )
    {
        Result = 1;
    }
    
    sMenuStamp.sPassLogin.Pass1 = 0;
    sMenuStamp.sPassLogin.Pass2 = 0;
    sMenuStamp.sPassLogin.Pass3 = 0; 
    sMenuStamp.sPassLogin.Pass4 = 0;
    sMenuStamp.sPassLogin.Pass5 = 0;
    sMenuStamp.sPassLogin.Pass6 = 0; 
    
    return Result;
}

void Save_Password(void)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    if(sMenuStamp.sPassWord.Pass1 <= 0x09 && sMenuStamp.sPassWord.Pass2 <= 0x09 && 
       sMenuStamp.sPassWord.Pass3 <= 0x09 && sMenuStamp.sPassWord.Pass4 <= 0x09 && 
       sMenuStamp.sPassWord.Pass5 <= 0x09 && sMenuStamp.sPassWord.Pass6 <= 0x09 )
    {
        sPassword.Pass1 = sMenuStamp.sPassWord.Pass1;
        sPassword.Pass2 = sMenuStamp.sPassWord.Pass2;
        sPassword.Pass3 = sMenuStamp.sPassWord.Pass3;
        sPassword.Pass4 = sMenuStamp.sPassWord.Pass4;
        sPassword.Pass5 = sMenuStamp.sPassWord.Pass5;
        sPassword.Pass6 = sMenuStamp.sPassWord.Pass6;
        aData[length++] = sPassword.Pass1;
        aData[length++] = sPassword.Pass2;
        aData[length++] = sPassword.Pass3;
        aData[length++] = sPassword.Pass4;
        aData[length++] = sPassword.Pass5;
        aData[length++] = sPassword.Pass6;
        Save_Array(ADDR_PASSWORD_SETTING, aData, length);
    }
}

void Init_Password(void)
{
    if(*(__IO uint8_t*)(ADDR_PASSWORD_SETTING) == BYTE_TEMP_FIRST)
    {
        sPassword.Pass1  = *(__IO uint8_t*)(ADDR_PASSWORD_SETTING+2) ;
        sPassword.Pass2  = *(__IO uint8_t*)(ADDR_PASSWORD_SETTING+3) ;
        sPassword.Pass3  = *(__IO uint8_t*)(ADDR_PASSWORD_SETTING+4) ;
        sPassword.Pass4  = *(__IO uint8_t*)(ADDR_PASSWORD_SETTING+5) ;
        sPassword.Pass5  = *(__IO uint8_t*)(ADDR_PASSWORD_SETTING+6) ;
        sPassword.Pass6  = *(__IO uint8_t*)(ADDR_PASSWORD_SETTING+7) ;
    }
    if(sPassword.Pass1 > 0x09 || sPassword.Pass2 > 0x09 || 
       sPassword.Pass3 > 0x09 || sPassword.Pass4 > 0x09 ||
       sPassword.Pass5 > 0x09 || sPassword.Pass6 > 0x09)
    {
        sPassword.Pass1 = 0x00;
        sPassword.Pass2 = 0x00;
        sPassword.Pass3 = 0x00;
        sPassword.Pass4 = 0x00;
        sPassword.Pass5 = 0x00;
        sPassword.Pass6 = 0x00;
        Save_Password();
    }
    Stamp_Menu_Exit();
}

void Stamp_Menu_Exit(void)
{
    sMenuStamp.sPassWord.Pass1 = sPassword.Pass1;
    sMenuStamp.sPassWord.Pass2 = sPassword.Pass2;
    sMenuStamp.sPassWord.Pass3 = sPassword.Pass3;
    sMenuStamp.sPassWord.Pass4 = sPassword.Pass4;
    sMenuStamp.sPassWord.Pass5 = sPassword.Pass5;
    sMenuStamp.sPassWord.Pass6 = sPassword.Pass6;
    
    sMenuStamp.Amperage_Upper  = sParamMotor.Amperage_Upper;
    sMenuStamp.Amperage_Delta  = sParamMotor.Amperage_Delta;
    sMenuStamp.Amperage_Option = sParamMotor.Amperage_Option;
    
    sMenuStamp.Pressure_Option = sParamMotor.Pressure_Option;
    sMenuStamp.Pressure_Upper  = sParamMotor.Pressure_Upper;
    sMenuStamp.Pressure_Lower  = sParamMotor.Pressure_Lower;
    
    sMenuStamp.Pressure_Constant = sParamMotor.Pressure_Constant;
    sMenuStamp.ID_Slave = sInforRs485.ID_Slave;
}

/*===================== Task and Init ===================*/
void    Init_AppMenu(void)
{
    Init_Password();
}

uint8_t AppMenu_Task(void)
{
    uint8_t i = 0;
    uint8_t Result = false;
    for( i = 0; i < _EVENT_MENU_DISPLAY_END; i++)
    {
        if(sEventAppMenu[i].e_status == 1)
        {
            Result = true;
            if((sEventAppMenu[i].e_systick == 0) ||
                ((HAL_GetTick() - sEventAppMenu[i].e_systick) >= sEventAppMenu[i].e_period))
            {
                sEventAppMenu[i].e_status = 0; //Disable event
                sEventAppMenu[i].e_systick= HAL_GetTick();
                sEventAppMenu[i].e_function_handler(i);
            }
        }
    }
    
    return Result;
}

