
#ifndef USER_APP_BUTTON_H__
#define USER_APP_BUTTON_H__

#define USING_APP_BUTTON

#include "user_util.h"
#include "event_driven.h"
#include "user_app_menu_display.h"

#define NUMBER_BUTTON               4
#define BUTTON_ON                   GPIO_PIN_RESET
#define BUTTON_OFF                  GPIO_PIN_SET
#define TIME_HOLD_DELAY             100     //Gia tri phai khac time Hold Button

/*----------------------Define Parameter--------------------*/
#define USING_BUTTON_ENTER   
#define USING_BUTTON_EXIT
#define USING_BUTTON_UP
#define USING_BUTTON_DOWN

#ifdef USING_BUTTON_ENTER
    #define BUTTON_ENTER_PORT           BT_ENTER_GPIO_Port
    #define BUTTON_ENTER_PIN            BT_ENTER_Pin
    #define TIME_HOLD_BUTTON_ENTER      3000

    #define User_BT_Enter_Click         User_Button_Enter_Click
    #define User_BT_Enter_Hold_Cycle    User_Button_Enter_Hold_Cycle
    #define User_BT_Enter_Hold_Once     User_Button_Enter_Hold_Once
#endif 

#ifdef USING_BUTTON_EXIT
    #define BUTTON_EXIT_PORT            BT_EXIT_GPIO_Port
    #define BUTTON_EXIT_PIN             BT_EXIT_Pin
    #define TIME_HOLD_BUTTON_EXIT       2000

    #define User_BT_Exit_Click          User_Button_Exit_Click
    #define User_BT_Exit_Hold_Cycle     User_Button_Exit_Hold_Cycle
    #define User_BT_Exit_Hold_Once      User_Button_Exit_Hold_Once
#endif

#ifdef USING_BUTTON_UP
    #define BUTTON_UP_PORT              BT_UP_GPIO_Port
    #define BUTTON_UP_PIN               BT_UP_Pin
    #define TIME_HOLD_BUTTON_UP         1000

    #define User_BT_Up_Click            User_Button_Up_Click
    #define User_BT_Up_Hold_Cycle       User_Button_Up_Hold_Cycle
    #define User_BT_Up_Hold_Once        User_Button_Up_Hold_Once
#endif

#ifdef USING_BUTTON_DOWN
    #define BUTTON_DOWN_PORT            BT_DOWN_GPIO_Port
    #define BUTTON_DOWN_PIN             BT_DOWN_Pin
    #define TIME_HOLD_BUTTON_DOWN       1000

    #define User_BT_Down_Click          User_Button_Down_Click
    #define User_BT_Down_Hold_Cycle     User_Button_Down_Hold_Cycle
    #define User_BT_Down_Hold_Once      User_Button_Down_Hold_Once
#endif

typedef enum
{
    _EVENT_BUTTON_ENTRY,
    _EVENT_BUTTON_CLICK_ENTER,
    _EVENT_BUTTON_HOLD_ENTER,
    _EVENT_BUTTON_CLICK_UP,
    _EVENT_BUTTON_HOLD_UP,
    _EVENT_BUTTON_CLICK_DOWN,
    _EVENT_BUTTON_HOLD_DOWN,
    _EVENT_BUTTON_CLICK_EXIT,
    _EVENT_BUTTON_HOLD_EXIT,
    
    _EVENT_BUTTON_END,
}eKindEventButton;

typedef enum
{
    BT_ENTER = 0,
    BT_UP,
    BT_DOWN,
    BT_EXIT
}eKindButton;

extern sEvent_struct       sEventAppButton[];
/*================= Function Handle ================*/
uint8_t     AppButton_Task(void);


#endif

