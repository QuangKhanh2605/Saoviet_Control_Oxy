
#ifndef USER_APP_LED7SEG_H__
#define USER_APP_LED7SEG_H__

#define USING_APP_LED7SEG

#include "event_driven.h"
#include "user_util.h"

#define NUMBER_LED7SEG_MAX      999
#define NONE_POINT_LED7SEG      0xAA

typedef enum
{
    _EVENT_LED7SEG_ENTRY,
    _EVENT_LED7SEG_DISPLAY,
    _EVENT_LED7SEG_TEST,
    
    _EVENT_LED7SEG_BACK_MAIN,
    
    _EVENT_LED7SEG_END,
}eKindEventLeg7seg;

typedef enum
{
    _BT_ENTER_CLICK,
    _BT_ENTER_HOLD_CYCLE,
//    _BT_EXIT_CLICK,
//    _BT_EXIT_HOLD_CYCLE,
    _BT_UP_CLICK,
    _BT_UP_HOLD_CYCLE,
    _BT_DOWN_CLICK,
    _BT_DOWN_HOLD_CYCLE,
    
    _BT_ENTER_HOLD_ONCE,
//    _BT_EXIT_HOLD_ONCE,
    _BT_UP_HOLD_ONCE,
    _BT_DOWN_HOLD_ONCE,
}eKindButtonMenu;

typedef enum
{
    _MENU_MAIN=0,
    _MENU_SETUP_RUN,
    _MENU_SETUP_FREE,
}eKindStateMenu;

typedef enum
{
    _LOCAL_DIS_1 = 1,
    _LOCAL_DIS_2,
    _LOCAL_DIS_3,
    
    _LOCAL_DIS_END,
}eKindLocationDisplay;


typedef struct
{
    uint8_t  StateMenu;
    uint16_t Stamp_TimeRun;
    uint16_t Stamp_TimeFree;    
}Struct_MenuStamp;

typedef struct
{
    uint16_t NumberDis; 
    uint8_t PointDis;
    uint8_t Location;
    uint8_t RunDis;
}Struct_ParamLed7seg;

extern sEvent_struct                sEventAppLed7seg[];
extern Struct_ParamLed7seg          sParamLed7seg;
extern Struct_MenuStamp             sParamMenu;

extern uint8_t front_led7seg_cathode_NDOT[10];
extern uint8_t front_led7seg_cathode_DOT[10];

extern uint8_t front_led7seg_anode_NDOT[10];
extern uint8_t front_led7seg_anode_DOT[10];

extern uint8_t icon_led7seg_cathode;
extern uint8_t icon_led7seg_anode;
extern uint8_t Entry_Main;
/*================= Function Handle ================*/
uint8_t     AppLed7seg_Task(void);
void        Init_AppLed7seg(void);

void        Handle_Menu_Button(uint8_t KindButton);

void        User_Button_Enter_Click(void);
void        User_Button_Up_Click(void);
void        User_Button_Down_Click(void);

void        User_Button_Enter_Hold_Cycle(void);
void        User_Button_Up_Hold_Cycle(void);
void        User_Button_Down_Hold_Cycle(void);

void        User_Button_Enter_Hold_Once(void);
void        User_Button_Up_Hold_Once(void);
void        User_Button_Down_Hold_Once(void);
    
void        BT_Menu_Main(uint8_t KindButton);
void        BT_Menu_Setup_Run(uint8_t KindButton);
void        BT_Menu_Setup_Free(uint8_t KindButton);

void        Dispaly_Number_Led7seg(uint16_t Number, uint8_t Point);
void        Send_Dis_Led7seg(uint8_t Data, uint8_t Pos);
void        Dis_Led7seg_Menu_Main(void);

void        Display_Led7Seg(void);
#endif

