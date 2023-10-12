#include "user_app_comm.h"
#include "user_uart.h"
#include "user_app_lcd.h"
#include "user_app_ctrl_oxy.h"
#include "user_app_relay.h"
#include "user_app_button.h"
#include "user_app_rs485.h"
#include "user_app_menu_display.h"
/*=========================== Func App Main ========================*/

void SysApp_Init (void)
{
    Init_AppLcd();
    Init_AppCtrlOxy();
    Init_AppMenuDisplay();
}

void SysApp_Setting (void)
{
    AppComm_Init();
}

/*
    Func: Init Queue trong main
*/
uint8_t TaskStatus_u8 = 0;
void Main_Task (void)
{
  SysApp_Init();
  SysApp_Setting();
  HAL_Delay(20);
  for(;;)
  {
#ifdef USING_APP_LCD
    AppLcd_Task();
#endif
    
#ifdef USING_APP_CTRL_OXY
    AppCtrlOxy_Task();
#endif
    
#ifdef USING_APP_RELAY
    AppRelay_Task();
#endif
    
#ifdef USING_APP_BUTTON
    AppButton_Task();
#endif
    
#ifdef USING_APP_RS485
    AppRs485_Task();
#endif
        
#ifdef USING_APP_MENU_DISPLAY
    AppMenu_Task();
#endif
  }
}


/*============= Function App Main ===============*/

void AppComm_Init (void)
{
    Init_Uart_Module();
}




