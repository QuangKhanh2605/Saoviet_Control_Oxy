#include "user_define_oxygen.h"

void Oxygen_Task(void)
{
    #ifdef USING_APP_CTRL_OXY
        AppCtrlOxy_Task();
    #endif
  
    #ifdef USING_APP_LCD
        AppLcd_Task();
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

void Oxygen_Init(void)
{
    Init_AppLcd();
    Init_AppCtrlOxy();
    Init_AppMenuDisplay();
    Init_AppRs485();
}


