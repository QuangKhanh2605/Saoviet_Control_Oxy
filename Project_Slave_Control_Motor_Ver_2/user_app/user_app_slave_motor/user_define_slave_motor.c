#include "user_define_slave_motor.h"

void SlaveMotor_Task(void)
{
    #ifdef USING_APP_BUTTON
        AppButton_Task();
    #endif
        
    #ifdef USING_APP_LCD
        AppLcd_Task();
    #endif
        
    #ifdef USING_APP_MENU_DISPLAY
        AppMenu_Task();
    #endif
        
    #ifdef USING_APP_MOTOR
        AppMotor_Task();
    #endif
        
    #ifdef USING_APP_RS485
        AppRs485_Task();
    #endif
}


void SlaveMotor_Init(void)
{
    #ifdef USING_APP_LCD
        Init_AppLcd();
    #endif
        
    #ifdef USING_APP_MOTOR
        Init_AppMotor();
    #endif  
  
    #ifdef USING_APP_MENU_DISPLAY
        Init_AppMenu();
    #endif
}

