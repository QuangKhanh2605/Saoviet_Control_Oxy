#ifndef INC_USER_DEFINE_SLAVE_MOTOR_H_
#define INC_USER_DEFINE_SLAVE_MOTOR_H_

#define USING_APP_SLAVE_MOTOR

#define INC_APP_BUTTON
#ifdef INC_APP_BUTTON
    #include "user_app_button.h"
#endif

#define INC_APP_LCD
#ifdef INC_APP_LCD
    #include "user_app_lcd.h"
#endif

#define INC_APP_MENU_DISPLAY
#ifdef INC_APP_MENU_DISPLAY
    #include "user_app_menu_display.h"
#endif

#define INC_APP_MOTOR
#ifdef INC_APP_MOTOR
    #include "user_app_motor.h"
#endif

#define INC_APP_RS485
#ifdef INC_APP_RS485
    #include "user_app_rs485.h"
#endif


/*==================Function Handle=================*/
void SlaveMotor_Task(void);
void SlaveMotor_Init(void);

#endif

