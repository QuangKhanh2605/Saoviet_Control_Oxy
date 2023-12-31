#include "user_app_menu_display.h"
#include "user_app_lcd.h"
#include "user_app_ctrl_oxy.h"
#include "user_internal_mem.h"
#include "user_sim.h"
#include "user_modem.h"
/*=============== Function static ================*/
static uint8_t fevent_menu_entry(uint8_t event);
static uint8_t fevent_menu_display_alter(uint8_t event);
static uint8_t fevent_menu_display_calib_oxy(uint8_t event);
static uint8_t fevent_menu_display_back_main_1(uint8_t event);
/*================== Struct ===================*/
sEvent_struct       sEventAppMenu[]=
{
  {_EVENT_MENU_ENTRY,               0, 5, 5,                fevent_menu_entry},
  {_EVENT_MENU_DISPLAY_ALTER,       0, 5, 20,               fevent_menu_display_alter},
  
  {_EVENT_MENU_DISPLAY_CALIB_OXY,   0, 5, 20,               fevent_menu_display_calib_oxy},
  {_EVENT_MENU_DISPLAY_BACK_MAIN_1, 0, 5, TIME_BACK_MAIN_1, fevent_menu_display_back_main_1},
};

Struct_Menu_State               sMenuState={0};
Struct_Menu_Stamp               sMenuStamp={0};
Struct_Password                 sPassword ={0};
/*================== Function Handle ===================*/
static uint8_t fevent_menu_entry(uint8_t event)
{
    DLCD_Main_1_Entry();
    fevent_active(sEventAppMenu, _EVENT_MENU_DISPLAY_ALTER);
    return 1;
}

static uint8_t fevent_menu_display_alter(uint8_t event)
{
    sMenuState.ConnectServer = sSimVar.ConnSerStatus_u8;
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
          
        case _MENU_SETTING_CYCLE:
          DLCD_Setting_Cycle_Alter();
          break;  
          
        case _MENU_SETTING_CALIB:
          DLCD_Setting_Calib_Alter();
          break;  
          
        case _MENU_SETTING_PARAMETER:
          DLCD_Setting_Parameter_Alter();
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

static uint8_t fevent_menu_display_calib_oxy(uint8_t event)
{
  switch(sMenuState.CalibSensor)
  {
      case _CALIB_ON_GOING:
        fevent_enable(sEventAppMenu, event);
        break;
        
      case _CALIB_COMPLETE:
      case _CALIB_ERROR:
        if(sMenuState.CalibSensor == _CALIB_COMPLETE)
            oLCD_C_Calib_Oxy[_LCD_C_CALIB_OXY_STATE_DONE].Status = 1;
        else
            oLCD_C_Calib_Oxy[_LCD_C_CALIB_OXY_STATE_ERROR].Status = 1;
        
        oLCD_C_Calib_Oxy[_LCD_C_CALIB_OXY_PLEASE_CLICK].Status = 1;
        DLCD_Calib_Oxy();
        
        oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Object = &sParamMeasure.Temp;
        oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Object = &sParamMeasure.Salinity;
        Stamp_Menu_Exit();
        break;
        
      default:
        break;
  }
  return 1;
}

static uint8_t fevent_menu_display_back_main_1(uint8_t event)
{
    if(sMenuState.Screen != _MENU_MAIN_1)
    {
          sMenuState.Screen = _MENU_MAIN_1;
          Clear_Rol_LCD(2, 7);
          DLCD_Main_1_Entry();
          
          Stamp_Menu_Exit();
          Menu_ResetState();
          Check_Password();
    }
    return 1;
}
/*====================== Function Use Button Click =======================*/
void User_Button_Enter_Click(void)
{
    Handle_Menu_Button(_BT_ENTER_CLICK);
}

void User_Button_Exit_Click(void)
{
    Handle_Menu_Button(_BT_EXIT_CLICK);
}

void User_Button_Up_Click(void)
{
    Handle_Menu_Button(_BT_UP_CLICK);
}

void User_Button_Down_Click(void)
{
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
    Handle_Menu_Button(_BT_ENTER_HOLD_ONCE);
}

void User_Button_Exit_Hold_Once(void)
{
    Handle_Menu_Button(_BT_EXIT_HOLD_ONCE);
    switch(sMenuState.Screen)
    {
        //Exit Hold Once Back Main 1
        case _MENU_MAIN_2:
        case _MENU_LOGIN:
        case _MENU_SETTING_MAIN:
        case _MENU_SETTING_CYCLE:
        case _MENU_SETTING_CALIB:
        case _MENU_SETTING_PARAMETER:
        case _MENU_SETTING_PASSWORD:
          fevent_active(sEventAppMenu, _EVENT_MENU_DISPLAY_BACK_MAIN_1);
          break;
          
        default:
          break;
    }
}

void User_Button_Up_Hold_Once(void)
{
    Handle_Menu_Button(_BT_UP_HOLD_ONCE);
}

void User_Button_Down_Hold_Once(void)
{
    Handle_Menu_Button(_BT_DOWN_HOLD_ONCE);
}
/*========================= Function Handle Menu =======================*/
void    BT_Menu_Main_1(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          sMenuState.Screen = _MENU_MAIN_2;
          Clear_Rol_LCD(3, 7);
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
          Menu_ResetState();
          Clear_Rol_LCD(3, 7);
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

void    BT_Menu_Main_2(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          sMenuState.Screen = _MENU_MAIN_1;
          Clear_Rol_LCD(3, 7);
          DLCD_Main_1_Entry();
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
          sMenuState.Screen = _MENU_MAIN_1;
          Clear_Rol_LCD(3, 7);
          DLCD_Main_1_Entry();
          break;
          
        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          break;
    
        case _BT_ENTER_HOLD_ONCE:
          sMenuState.Screen = _MENU_LOGIN;
          Menu_ResetState();
          Clear_Rol_LCD(3, 7);
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

void    BT_Menu_Login(uint8_t KindButton)
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
                Menu_ResetState();
                if(Check_Password() == 1)   
                {
                  DCU_Respond_Debug((uint8_t*)"Login_Setting",13);
                  UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Login_SETTING!\r\n");
                  sMenuState.Screen = _MENU_SETTING_MAIN;
                  Clear_Rol_LCD(2, 7);
                  DLCD_Setting_Main_Entry();
                }
                else
                {
                  DCU_Respond_Debug((uint8_t*)"Login_Fail",13);
                  UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Login_Fail!\r\n");
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
                Menu_ResetState();
                
                Clear_Rol_LCD(3, 7);
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

void    BT_Menu_Setting_Main(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          switch(sMenuState.SettingMain)
          {
            case _SETTING_MAIN_CYCLE:
                sMenuState.Screen = _MENU_SETTING_CYCLE;
                Clear_Rol_LCD(2, 6);
                DLCD_Setting_Cycle_Entry();
                break;
                
            case _SETTING_MAIN_CALIB:
                sMenuState.Screen = _MENU_SETTING_CALIB;
                Clear_Rol_LCD(2, 6);
                
                DLCD_Setting_Calib_Entry();
                break;
                
            case _SETTING_MAIN_PARAMETER:
                sMenuState.Screen = _MENU_SETTING_PARAMETER;
                Clear_Rol_LCD(2, 6);
                
                DLCD_Setting_Parameter_Entry();
                break;
                
            case _SETTING_MAIN_PASSWORD:
                sMenuState.Screen = _MENU_SETTING_PASSWORD;
                Clear_Rol_LCD(2, 7);
                
                DLCD_Setting_Password_Entry();
                break;
                
              default: 
                break;
          }
          Menu_ResetState();
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
          sMenuState.Screen = _MENU_MAIN_1;
          Menu_ResetState();
          Clear_Rol_LCD(2, 7);
          DLCD_Main_1_Entry();
          break;
          
        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          switch(sMenuState.SettingMain)
          {
              case _SETTING_MAIN_CYCLE:
                sMenuState.SettingMain = _SETTING_MAIN_PASSWORD;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PASSWORD].Rol);
                break;
                
              case _SETTING_MAIN_CALIB:
                sMenuState.SettingMain = _SETTING_MAIN_CYCLE;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_CYCLE].Rol);
                break;
                
              case _SETTING_MAIN_PARAMETER:
                sMenuState.SettingMain = _SETTING_MAIN_CALIB;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_CALIB].Rol);
                break;
                
              case _SETTING_MAIN_PASSWORD:
                sMenuState.SettingMain = _SETTING_MAIN_PARAMETER;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PARAMETER].Rol);
                break;
                
              default:
                break;
          }
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          switch(sMenuState.SettingMain)
          {
              case _SETTING_MAIN_CYCLE:
                sMenuState.SettingMain = _SETTING_MAIN_CALIB;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_CALIB].Rol);
                break;
                
              case _SETTING_MAIN_CALIB:
                sMenuState.SettingMain = _SETTING_MAIN_PARAMETER;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PARAMETER].Rol);
                break;
                
              case _SETTING_MAIN_PARAMETER:
                sMenuState.SettingMain = _SETTING_MAIN_PASSWORD;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_PASSWORD].Rol);
                break;
                
              case _SETTING_MAIN_PASSWORD:
                sMenuState.SettingMain = _SETTING_MAIN_CYCLE;
                Display_Control_Setting_Left(oLCD_C_Setting_Main[_LCD_C_SETTING_MAIN_CYCLE].Rol);
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

void    BT_Menu_Setting_Cycle(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          switch(sMenuState.SettingCycle)
          {
            case _SETTING_CYCLE_RUN_CHOOSE:
                sMenuState.SettingCycle = _SETTING_CYCLE_RUN_CHANGE;
                oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_CLICK].Status = 1;
                Display_Control_Setting_Right(oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_RUN].Rol);
                break;
                
            case _SETTING_CYCLE_FREE_CHOOSE:
                sMenuState.SettingCycle = _SETTING_CYCLE_FREE_CHANGE;
                oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_CLICK].Status = 1;
                Display_Control_Setting_Right(oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_FREE].Rol);
                break;
                
            case _SETTING_CYCLE_SEND_SLAVE:
                oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_SUCCESS].Status = 1;
                fevent_active(sEventAppCtrlOxy, _EVENT_SEND_SLAVE_CYCLE);
                break;
                
            case _SETTING_CYCLE_RUN_CHANGE:
            case _SETTING_CYCLE_FREE_CHANGE:
                if(sMenuState.SettingCycle == _SETTING_CYCLE_RUN_CHANGE)
                  sMenuState.SettingCycle = _SETTING_CYCLE_RUN_CHOOSE;
                else
                  sMenuState.SettingCycle = _SETTING_CYCLE_FREE_CHOOSE;
                oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_PLEASE].Status = 1;
                Save_TimeSlave(sMenuStamp.sTimeSlave.RunCtrl, sMenuStamp.sTimeSlave.FreeCtrl);
                GLCD_ClearArea(3, 120, 6, 127);
                break;
                
              default: 
                break;
          }
          DLCD_Setting_Cycle();
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
          switch(sMenuState.SettingCycle)
          {
            case _SETTING_CYCLE_RUN_CHOOSE:
            case _SETTING_CYCLE_FREE_CHOOSE:
            case _SETTING_CYCLE_SEND_SLAVE:
              sMenuState.Screen = _MENU_SETTING_MAIN;
              Menu_ResetState();
              Clear_Rol_LCD(2, 6);
              DLCD_Setting_Main_Entry();
              break;
              
            case _SETTING_CYCLE_RUN_CHANGE:
            case _SETTING_CYCLE_FREE_CHANGE:
              Stamp_Menu_Exit();
              if(sMenuState.SettingCycle == _SETTING_CYCLE_RUN_CHANGE)
                sMenuState.SettingCycle = _SETTING_CYCLE_RUN_CHOOSE;
              else
                sMenuState.SettingCycle = _SETTING_CYCLE_FREE_CHOOSE;
              oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_PLEASE].Status = 1;
              DLCD_Setting_Cycle();
              GLCD_ClearArea(3, 120, 6, 127);
              break;
              
              default: 
                break;
          }
          break;
          
        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          switch(sMenuState.SettingCycle)
          {
              case _SETTING_CYCLE_RUN_CHOOSE:
                sMenuState.SettingCycle = _SETTING_CYCLE_SEND_SLAVE;
                Display_Control_Setting_Left(oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_SLAVE].Rol);
                break;
                
              case _SETTING_CYCLE_FREE_CHOOSE:
                sMenuState.SettingCycle = _SETTING_CYCLE_RUN_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_RUN].Rol);
                break;
                
              case _SETTING_CYCLE_SEND_SLAVE:
                sMenuState.SettingCycle = _SETTING_CYCLE_FREE_CHOOSE;
                oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_PLEASE].Status = 1;
                Display_Control_Setting_Left(oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_FREE].Rol);
                break;
                
              case _SETTING_CYCLE_RUN_CHANGE:
                if(sMenuStamp.sTimeSlave.RunCtrl < TIME_MAX)     sMenuStamp.sTimeSlave.RunCtrl++;
                break;
                
              case _SETTING_CYCLE_FREE_CHANGE:
                if(sMenuStamp.sTimeSlave.FreeCtrl < TIME_MAX)    sMenuStamp.sTimeSlave.FreeCtrl++;
                break;
                
              default:
                break;
          }
          DLCD_Setting_Cycle();
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          switch(sMenuState.SettingCycle)
          {
              case _SETTING_CYCLE_RUN_CHOOSE:
                sMenuState.SettingCycle = _SETTING_CYCLE_FREE_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_FREE].Rol);
                break;
                
              case _SETTING_CYCLE_FREE_CHOOSE:
                sMenuState.SettingCycle = _SETTING_CYCLE_SEND_SLAVE;
                Display_Control_Setting_Left(oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_SLAVE].Rol);
                break;
                
              case _SETTING_CYCLE_SEND_SLAVE:
                sMenuState.SettingCycle = _SETTING_CYCLE_RUN_CHOOSE;
                oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_PLEASE].Status = 1;
                Display_Control_Setting_Left(oLCD_C_Setting_Cycle[_LCD_C_SETTING_CYCLE_RUN].Rol);
                break;
                
              case _SETTING_CYCLE_RUN_CHANGE:
                if(sMenuStamp.sTimeSlave.RunCtrl > TIME_MIN)     sMenuStamp.sTimeSlave.RunCtrl--;
                break;
                
              case _SETTING_CYCLE_FREE_CHANGE:
                if(sMenuStamp.sTimeSlave.FreeCtrl > TIME_MIN)     sMenuStamp.sTimeSlave.FreeCtrl--;
                break;
                
              default:
                break;
          }
          DLCD_Setting_Cycle();
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

void    BT_Menu_Setting_Calib(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          switch(sMenuState.SettingCalib)
          {
            case _SETTING_CALIB_100_CALIB_CHOOSE:
                sMenuState.SettingCalib = _SETTING_CALIB_100_CALIB_ENTER;
                oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_ENTER_CALIB].Status = 1;
                DLCD_Setting_Calib();
                break;
            
            case _SETTING_CALIB_100_CALIB_ENTER:
            case _SETTING_CALIB_SALINITY_CHANGE:
            case _SETTING_CALIB_TEMPERATURE_CHANGE:
                sMenuState.Screen = _MENU_CALIB_OXY;
                sMenuState.CalibSensor = _CALIB_ON_GOING;
                switch(sMenuState.SettingCalib)
                {
                    case _SETTING_CALIB_100_CALIB_ENTER:
                      sKindMode485.Trans = _RS485_CALIB_OXY_100_CALIB;
                      oLCD_C_Calib_Oxy[_LCD_C_CALIB_OXY_100].Status = 1;
                      UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Calib_OXY_100!\r\n");
                      DCU_Respond_Debug((uint8_t*)"Calib_Oxy_100",13);
                      break; 
                      
                    case _SETTING_CALIB_SALINITY_CHANGE:
                      sKindMode485.Trans = _RS485_CALIB_SALINITY;
                      oLCD_C_Calib_Oxy[_LCD_C_CALIB_OXY_SALINITY].Status = 1;
                      UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Calib_SALINITY!\r\n");
                      DCU_Respond_Debug((uint8_t*)"Calib_Salinity",14);
                      break;
                      
                    case _SETTING_CALIB_TEMPERATURE_CHANGE:
                      sKindMode485.Trans = _RS485_CALIB_TEMP;
                      oLCD_C_Calib_Oxy[_LCD_C_CALIB_OXY_TEMPERATURE].Status = 1;
                      UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Calib_TEMPERATURE!\r\n");
                      DCU_Respond_Debug((uint8_t*)"Calib_Temperature",17);
                      break;

                    default:
                      break;
                }
                fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_WAIT_CALIB);
                fevent_active(sEventAppMenu, _EVENT_MENU_DISPLAY_CALIB_OXY);
                Clear_Rol_LCD(2, 7);
                DLCD_Calib_Oxy_Entry();
                break;
                
            case _SETTING_CALIB_SALINITY_CHOOSE:
                Stamp_Menu_Exit();
                oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Object = &sMenuStamp.Salinity;
                oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_SELECT].Status = 1;
                DLCD_Setting_Calib();
                
                sMenuState.SettingCalib = _SETTING_CALIB_SALINITY_CHANGE;
                fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_WAIT_CALIB);
                Display_Control_Setting_Right(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_SALINITY].Rol);
                break;
                
            case _SETTING_CALIB_TEMPERATURE_CHOOSE:
                Stamp_Menu_Exit();
                oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Object = &sMenuStamp.Temperature;
                oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_SELECT].Status = 1;
                DLCD_Setting_Calib();
                
                sMenuState.SettingCalib = _SETTING_CALIB_TEMPERATURE_CHANGE;
                fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_WAIT_CALIB);
                Display_Control_Setting_Right(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TEMPERATURE].Rol);
                break;
    
              default: 
                break;
          }
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
          break;
          
        case _BT_EXIT_CLICK:
          switch(sMenuState.SettingCalib)
          {
              case _SETTING_CALIB_100_CALIB_CHOOSE:
              case _SETTING_CALIB_SALINITY_CHOOSE:
              case _SETTING_CALIB_TEMPERATURE_CHOOSE:
                sMenuState.Screen = _MENU_SETTING_MAIN;
                Menu_ResetState();
                Clear_Rol_LCD(2, 7);
                DLCD_Setting_Main_Entry();
                break;
                
            case _SETTING_CALIB_100_CALIB_ENTER:
                sMenuState.SettingCalib = _SETTING_CALIB_100_CALIB_CHOOSE;
                oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_PLEASE].Status = 1;
                DLCD_Setting_Calib();
                break;
                
              case _SETTING_CALIB_SALINITY_CHANGE:
                Stamp_Menu_Exit();
                oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_SALINITY].Object = &sParamMeasure.Salinity;
                sMenuState.SettingCalib = _SETTING_CALIB_SALINITY_CHOOSE;
                
                oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_PLEASE].Status = 1;
                DLCD_Setting_Calib();
                GLCD_ClearArea(3, 120, 6, 127);
                Display_Control_Setting_Left(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_SALINITY].Rol);
                break;
                
              case _SETTING_CALIB_TEMPERATURE_CHANGE:
                Stamp_Menu_Exit();
                oLCD_U_Setting_Calib[_LCD_U_SETTING_CALIB_TEMPERATURE].Object = &sParamMeasure.Temp;
                sMenuState.SettingCalib = _SETTING_CALIB_TEMPERATURE_CHOOSE;
                
                oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_PLEASE].Status = 1;
                DLCD_Setting_Calib();
                GLCD_ClearArea(3, 120, 6, 127);
                Display_Control_Setting_Left(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TEMPERATURE].Rol);
                break;
                
              default:
                break;
          }
          break;
          
        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          switch(sMenuState.SettingCalib)
          {
              case _SETTING_CALIB_100_CALIB_CHOOSE:
                sMenuState.SettingCalib = _SETTING_CALIB_TEMPERATURE_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TEMPERATURE].Rol);
                break;
                
              case _SETTING_CALIB_SALINITY_CHOOSE:
                sMenuState.SettingCalib = _SETTING_CALIB_100_CALIB_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_100_CALIB].Rol);
                break;
                
              case _SETTING_CALIB_TEMPERATURE_CHOOSE:
                sMenuState.SettingCalib = _SETTING_CALIB_SALINITY_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_SALINITY].Rol);
                break;
                
              case _SETTING_CALIB_SALINITY_CHANGE:
                if(sMenuStamp.Salinity <= SALINITY_MAX - 100)          
                  sMenuStamp.Salinity +=100;
                else 
                  sMenuStamp.Salinity = SALINITY_MAX;
                break;
                
              case _SETTING_CALIB_TEMPERATURE_CHANGE:
                if(sMenuStamp.Temperature <= TEMPERATURE_MAX - 10)    
                  sMenuStamp.Temperature +=10;
                else    
                  sMenuStamp.Temperature = TEMPERATURE_MAX;
                break;

              default:
                break;
          }
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          switch(sMenuState.SettingCalib)
          {
              case _SETTING_CALIB_100_CALIB_CHOOSE:
                sMenuState.SettingCalib = _SETTING_CALIB_SALINITY_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_SALINITY].Rol);
                break;
                
              case _SETTING_CALIB_SALINITY_CHOOSE:
                sMenuState.SettingCalib = _SETTING_CALIB_TEMPERATURE_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_TEMPERATURE].Rol);
                break;
                
              case _SETTING_CALIB_TEMPERATURE_CHOOSE:
                sMenuState.SettingCalib = _SETTING_CALIB_100_CALIB_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Calib[_LCD_C_SETTING_CALIB_100_CALIB].Rol);
                break;
                
              case _SETTING_CALIB_SALINITY_CHANGE:
                if(sMenuStamp.Salinity >= SALINITY_MIN + 100)          
                  sMenuStamp.Salinity-=100;
                else    
                  sMenuStamp.Salinity = SALINITY_MIN;
                break;
                
              case _SETTING_CALIB_TEMPERATURE_CHANGE:
                if(sMenuStamp.Temperature > TEMPERATURE_MIN + 10)    
                  sMenuStamp.Temperature-=10;
                else    
                  sMenuStamp.Temperature = TEMPERATURE_MIN;
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

void    BT_Menu_Setting_Parameter(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          switch(sMenuState.SettingParameter)
          {     
              case _SETTING_OXY_UPPER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_OXY_UPPER_CHANGE;
                oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_CLICK].Status = 1;
                Display_Control_Setting_Right(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_UPPER].Rol);
                break;
                
              case _SETTING_OXY_LOWER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_OXY_LOWER_CHANGE;
                oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_CLICK].Status = 1;
                Display_Control_Setting_Right(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_LOWER].Rol);
                break;
                
              case _SETTING_TIMEDELAY_CHOOSE:
                sMenuState.SettingParameter = _SETTING_TIMEDELAY_CHANGE;
                oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_CLICK].Status = 1;
                Display_Control_Setting_Right(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_TIME_DELAY].Rol);
                break;
                
              case _SETTING_TIMEWARNING_CHOOSE:
                sMenuState.SettingParameter = _SETTING_TIMEWARNING_CHANGE;
                oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_CLICK].Status = 1;
                Display_Control_Setting_Right(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_WARNING].Rol);
                break;
                
              case _SETTING_OXY_UPPER_CHANGE:
                #ifdef USING_APP_SIM  
                    sMQTT.aMARK_MESS_PENDING[DATA_HANDSHAKE] = TRUE;
                #endif
                Save_OxyUpperLower(sMenuStamp.OxyUpper, sParamCtrlOxy.Oxy_Lower);
                sMenuState.SettingParameter = _SETTING_OXY_UPPER_CHOOSE;
                oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PLEASE].Status = 1;
                GLCD_ClearArea(3, 120, 6, 127);
                DCU_Respond_Debug((uint8_t*)"Setup_OxyUpper",14);
                break;
                
              case _SETTING_OXY_LOWER_CHANGE:
                #ifdef USING_APP_SIM  
                    sMQTT.aMARK_MESS_PENDING[DATA_HANDSHAKE] = TRUE;
                #endif
                Save_OxyUpperLower(sParamCtrlOxy.Oxy_Upper, sMenuStamp.OxyLower);
                sMenuState.SettingParameter = _SETTING_OXY_LOWER_CHOOSE;
                oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PLEASE].Status = 1;
                GLCD_ClearArea(3, 120, 6, 127);
                DCU_Respond_Debug((uint8_t*)"Setup_OxyLower",14);
                break;
                
              case _SETTING_TIMEDELAY_CHANGE:
                Save_TimeCtrlOxy(sMenuStamp.TimeDelay, sParamCtrlOxy.TimeChange,sParamCtrlOxy.TimeWarning);
                sMenuState.SettingParameter = _SETTING_TIMEDELAY_CHOOSE;
                oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PLEASE].Status = 1;
                GLCD_ClearArea(3, 120, 6, 127);
                DCU_Respond_Debug((uint8_t*)"Setup_TimeDelay",15);
                break;
                
              case _SETTING_TIMEWARNING_CHANGE:
                Save_TimeCtrlOxy(sParamCtrlOxy.TimeDelay, sParamCtrlOxy.TimeChange,sMenuStamp.TimeWarning);
                sMenuState.SettingParameter = _SETTING_TIMEWARNING_CHOOSE;
                oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PLEASE].Status = 1;
                GLCD_ClearArea(3, 120, 6, 127);
                DCU_Respond_Debug((uint8_t*)"Setup_TimeWarning",17);
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
              case _SETTING_OXY_UPPER_CHOOSE:
              case _SETTING_OXY_LOWER_CHOOSE:
              case _SETTING_TIMEDELAY_CHOOSE:
              case _SETTING_TIMEWARNING_CHOOSE:
                sMenuState.Screen = _MENU_SETTING_MAIN;
                Menu_ResetState();
                Clear_Rol_LCD(2, 6);
                DLCD_Setting_Main_Entry();
                break;
                
              case _SETTING_OXY_UPPER_CHANGE:
              case _SETTING_OXY_LOWER_CHANGE:
              case _SETTING_TIMEDELAY_CHANGE:
              case _SETTING_TIMEWARNING_CHANGE:
                switch(sMenuState.SettingParameter)
                {
                  case _SETTING_OXY_UPPER_CHANGE:
                    sMenuState.SettingParameter = _SETTING_OXY_UPPER_CHOOSE;
                    break;
                    
                  case _SETTING_OXY_LOWER_CHANGE:
                    sMenuState.SettingParameter = _SETTING_OXY_LOWER_CHOOSE;
                    break;
                    
                  case _SETTING_TIMEDELAY_CHANGE:
                    sMenuState.SettingParameter = _SETTING_TIMEDELAY_CHOOSE;
                    break;
                    
                  case _SETTING_TIMEWARNING_CHANGE:
                    sMenuState.SettingParameter = _SETTING_TIMEWARNING_CHOOSE;
                    break;
                    
                    default:
                      break;
                }

                Stamp_Menu_Exit();
                oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_PLEASE].Status = 1;
                DLCD_Setting_Parameter();
                GLCD_ClearArea(3, 120, 6, 127);
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
              case _SETTING_OXY_UPPER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_TIMEWARNING_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_WARNING].Rol);
                break;
                
              case _SETTING_OXY_LOWER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_OXY_UPPER_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_UPPER].Rol);
                break;
                
              case _SETTING_TIMEDELAY_CHOOSE:
                sMenuState.SettingParameter = _SETTING_OXY_LOWER_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_LOWER].Rol);
                break;
                
              case _SETTING_TIMEWARNING_CHOOSE:
                sMenuState.SettingParameter = _SETTING_TIMEDELAY_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_TIME_DELAY].Rol);
                break;
                
              case _SETTING_OXY_UPPER_CHANGE:
                if(sMenuStamp.OxyUpper < OXY_UPPER_MAX - 10) 
                {
                  if(sMenuStamp.OxyUpper % 10 == 0)
                    sMenuStamp.OxyUpper +=10;
                  else
                    sMenuStamp.OxyUpper = (sMenuStamp.OxyUpper/10+1)*10;
                }
                else 
                  sMenuStamp.OxyUpper = OXY_UPPER_MAX;
                break;
                
              case _SETTING_OXY_LOWER_CHANGE:
                if(sMenuStamp.OxyLower < sParamCtrlOxy.Oxy_Upper - OXY_CYCLE_UP_LOW - 10) 
                {
                  if(sMenuStamp.OxyLower % 10 == 0)
                    sMenuStamp.OxyLower +=10;
                  else
                    sMenuStamp.OxyLower = (sMenuStamp.OxyLower/10+1)*10;
                }
                else
                  sMenuStamp.OxyLower = sParamCtrlOxy.Oxy_Upper - OXY_CYCLE_UP_LOW;
                break;
                
              case _SETTING_TIMEDELAY_CHANGE:
                if(sMenuStamp.TimeDelay < TIMEDELAY_MAX) sMenuStamp.TimeDelay++;
                break;
                
              case _SETTING_TIMEWARNING_CHANGE:
                if(sMenuStamp.TimeWarning < TIMEWARNING_MAX) sMenuStamp.TimeWarning++;
                break;
                
              default:
                break;
          }
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          switch(sMenuState.SettingParameter)
          {
              case _SETTING_OXY_UPPER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_OXY_LOWER_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_LOWER].Rol);
                break;
                
              case _SETTING_OXY_LOWER_CHOOSE:
                sMenuState.SettingParameter = _SETTING_TIMEDELAY_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_TIME_DELAY].Rol);
                break;
                
              case _SETTING_TIMEDELAY_CHOOSE:
                sMenuState.SettingParameter = _SETTING_TIMEWARNING_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_WARNING].Rol);
                break;

              case _SETTING_TIMEWARNING_CHOOSE:
                sMenuState.SettingParameter = _SETTING_OXY_UPPER_CHOOSE;
                Display_Control_Setting_Left(oLCD_C_Setting_Parameter[_LCD_C_SETTING_PARAMETER_OXY_UPPER].Rol);
                break;
                
              case _SETTING_OXY_UPPER_CHANGE:
                if(sMenuStamp.OxyUpper > sParamCtrlOxy.Oxy_Lower + OXY_CYCLE_UP_LOW + 10) 
                {
                  if(sMenuStamp.OxyUpper % 10 == 0)
                    sMenuStamp.OxyUpper -=10;
                  else
                    sMenuStamp.OxyUpper = (sMenuStamp.OxyUpper/10)*10;
                }
                else 
                  sMenuStamp.OxyUpper = sParamCtrlOxy.Oxy_Lower + OXY_CYCLE_UP_LOW;
                break;
                
              case _SETTING_OXY_LOWER_CHANGE:
                if(sMenuStamp.OxyLower > OXY_LOWER_MIN +10) 
                {
                  if(sMenuStamp.OxyLower % 10 == 0)
                    sMenuStamp.OxyLower -=10;
                  else
                    sMenuStamp.OxyLower = (sMenuStamp.OxyLower/10)*10;
                }
                else    
                  sMenuStamp.OxyLower = OXY_LOWER_MIN;
                break;
                
              case _SETTING_TIMEDELAY_CHANGE:
                if(sMenuStamp.TimeDelay > TIMEDELAY_MIN) sMenuStamp.TimeDelay--;
                break;
                
              case _SETTING_TIMEWARNING_CHANGE:
                if(sMenuStamp.TimeWarning > TIMEWARNING_MIN) sMenuStamp.TimeWarning--;
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

void    BT_Menu_Setting_Password(uint8_t KindButton)
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
                UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Change_Password!\r\n");
                DCU_Respond_Debug((uint8_t*)"Change_Password",15);
                Save_Password();
                sMenuState.SettingPassword = _SUCCESS_PASS;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_SUCCESS].Status = 1;
                oLCD_C_Setting_Password[_LCD_C_SETTING_PASSWORD_CLICK].Status = 1;
                break;
                
            case _SUCCESS_PASS:
                sMenuState.Screen = _MENU_MAIN_1;
                Menu_ResetState();
                Clear_Rol_LCD(2, 7);
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
                Menu_ResetState();
                Clear_Rol_LCD(2, 7);
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
                Menu_ResetState();
                Clear_Rol_LCD(2, 7);
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

void    BT_Menu_Calib_Oxy(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
        case _BT_ENTER_HOLD_CYCLE:
        case _BT_EXIT_CLICK:
        case _BT_EXIT_HOLD_CYCLE:
            if(sMenuState.CalibSensor != _CALIB_ON_GOING)
            {
                sMenuState.SettingCalib = _SETTING_CALIB_100_CALIB_CHOOSE;
                sMenuState.Screen = _MENU_SETTING_CALIB;
                Clear_Rol_LCD(2, 7);
                DLCD_Setting_Calib_Entry();
            }
            break;
            
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
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
/*========================  Function Handle Menu =======================*/
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
          
        case _MENU_SETTING_CYCLE:
          BT_Menu_Setting_Cycle(KindButton);
          break;
          
        case _MENU_SETTING_CALIB:
          BT_Menu_Setting_Calib(KindButton);
          break;
          
        case _MENU_SETTING_PARAMETER:
          BT_Menu_Setting_Parameter(KindButton);
          break;
          
        case _MENU_SETTING_PASSWORD:
          BT_Menu_Setting_Password(KindButton);
          break;
          
        case _MENU_CALIB_OXY:
          BT_Menu_Calib_Oxy(KindButton);
          break;
          
        default:
          break;
    }
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
        sPassword.Pass3  = *(__IO uint8_t*)(ADDR_PASSWORD_SETTING+5) ;
        sPassword.Pass3  = *(__IO uint8_t*)(ADDR_PASSWORD_SETTING+6) ;
        sPassword.Pass3  = *(__IO uint8_t*)(ADDR_PASSWORD_SETTING+7) ;
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
    
    sMenuStamp.Salinity    = sParamMeasure.Salinity;
    sMenuStamp.Temperature = sParamMeasure.Temp;
    
    sMenuStamp.TimeDelay    = sParamCtrlOxy.TimeDelay;
    sMenuStamp.TimeWarning  = sParamCtrlOxy.TimeWarning;
    sMenuStamp.OxyUpper     = sParamCtrlOxy.Oxy_Upper;
    sMenuStamp.OxyLower     = sParamCtrlOxy.Oxy_Lower;
    
    sMenuStamp.sTimeSlave.FreeCtrl = sTimeSlave.FreeCtrl;
    sMenuStamp.sTimeSlave.RunCtrl  = sTimeSlave.RunCtrl;
}

/*============================ Function Handle =========================*/
void Menu_ResetState(void)
{
    sMenuState.Login = 0;
    sMenuState.SettingMain = 0;
    sMenuState.SettingCycle = 0;
    sMenuState.SettingCalib = 0;
    sMenuState.SettingPassword = 0;
    sMenuState.SettingParameter = 0;
}

/*==========================Function Handle=========================*/
void Init_StartMenu(void)
{
    GLCD_ClearScreen();
    GLCD_WriteString((char*)"MACHINE CONTROL OXY",0,7,1);
    GLCD_WriteString((char*)"SAO VIET",4,42,1);
//    GLCD_WriteString((char*)"Loading",5,45,1);
    GLCD_WriteString((char*)"Please wait...",7,25,1);
}

void Init_AppMenuDisplay(void)
{
    Init_Password();
    Init_StartMenu();
}

uint8_t AppMenu_Task(void)
{
    uint8_t i = 0;
    uint8_t Result =  false;
    
    for(i = 0; i < _EVENT_MENU_END; i++)
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

