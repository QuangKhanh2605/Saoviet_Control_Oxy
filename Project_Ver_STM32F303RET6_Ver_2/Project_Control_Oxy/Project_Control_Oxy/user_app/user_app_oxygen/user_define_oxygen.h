#ifndef INC_USER_DEFINE_OXYGEN_H_
#define INC_USER_DEFINE_OXYGEN_H_

#define USING_APP_OXYGEN

// Inclue App Button
#define INC_APP_BUTTON
#ifdef INC_APP_BUTTON
    #include "user_app_button.h"
#endif

//Include App Ctrl Oxy
#define INC_APP_CTRL_OXY
#ifdef INC_APP_CTRL_OXY
    #include "user_app_ctrl_oxy.h"
#endif

//Include App LCD
#define INC_APP_LCD
#ifdef INC_APP_LCD
    #include "user_app_lcd.h"
#endif

//Include App Menu Display
#define INC_APP_MENU_DISPLAY
#ifdef INC_APP_MENU_DISPLAY
    #include "user_app_menu_display.h"
#endif

//Include App Relay
#define INC_APP_RELAY
#ifdef INC_APP_RELAY
    #include "user_app_relay.h"
#endif

//Include App Rs485
#define INC_APP_RS485
#ifdef INC_APP_RS485
    #include "user_app_rs485.h"
#endif

//Include App Rs485
#define INC_APP_RS485_SLAVE
#ifdef INC_APP_RS485_SLAVE
    #include "user_app_rs485_slave.h"
#endif

//Include App Ctrl PH
#define INC_APP_CTRL_PH
#ifdef INC_APP_CTRL_PH
    #include "user_app_ctrl_ph.h"
#endif

/*==================Function Handle=================*/
void Oxygen_Task(void);
void Oxygen_Init(void);



#endif
