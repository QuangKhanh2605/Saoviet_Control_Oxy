
#include "user_app_led7seg.h"
#include "user_app_slave_oxy.h"
#include "spi.h"
/*====================Function Static===================*/
static uint8_t fevent_led7seg_entry(uint8_t event);
static uint8_t fevent_led7seg_display(uint8_t event);
static uint8_t fevent_led7seg_test(uint8_t event);

static uint8_t fevent_led7seg_back_main(uint8_t event);
/*========================Struct========================*/
sEvent_struct               sEventAppLed7seg[]=
{
  {_EVENT_LED7SEG_ENTRY,      1, 5, 5,          fevent_led7seg_entry},
  {_EVENT_LED7SEG_DISPLAY,    0, 5, 1,          fevent_led7seg_display},
  {_EVENT_LED7SEG_TEST,       0, 5, 1000,       fevent_led7seg_test},

  {_EVENT_LED7SEG_BACK_MAIN,  0, 5, 5000,       fevent_led7seg_back_main},
};

uint8_t front_led7seg_cathode_NDOT[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
uint8_t front_led7seg_cathode_DOT[10]  = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

uint8_t front_led7seg_anode_NDOT[10]   = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
uint8_t front_led7seg_anode_DOT[10]    = {0xBF, 0x86, 0xDB, 0xBF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

uint8_t icon_led7seg_cathode = 0xBF;
uint8_t icon_led7seg_anode   = 0x40;

Struct_MenuStamp        sParamMenu=
{
    .StateMenu = 0,
    .Stamp_TimeRun = 0,
    .Stamp_TimeFree = 0,
};
    
Struct_ParamLed7seg     sParamLed7seg=
{
    .NumberDis = 0, 
    .PointDis = 0,
    .Location = 1,
    .RunDis = 3,
};

uint8_t RunDis_Anode = 0x40;
uint8_t RunDis_Cathode = 0xBF;

uint8_t Entry_Main = 0;
/*=======================Function Hanlde=================*/
static uint8_t fevent_led7seg_entry(uint8_t event)
{
    return 1;
}

static uint8_t fevent_led7seg_display(uint8_t event)
{
    fevent_enable(sEventAppLed7seg, event);
    return 1;
}

uint8_t check_dis = 0;
static uint8_t fevent_led7seg_test(uint8_t event)
{
    uint8_t front_Display = 0;
    front_Display = front_led7seg_cathode_NDOT[check_dis];
    
    Send_Dis_Led7seg(front_Display, _LOCAL_DIS_1);
    fevent_enable(sEventAppLed7seg, event);
    return 1;
}

static uint8_t fevent_led7seg_back_main(uint8_t event)
{
    if(sParamMenu.StateMenu != _MENU_MAIN)
    {
        sParamMenu.StateMenu = _MENU_MAIN;
        sCtrlOxy.StateCtrl = _OXY_FREE;
        sParamMenu.Stamp_TimeRun  = sParamSlave.TimeRun;
        sParamMenu.Stamp_TimeFree = sParamSlave.TimeFree; 
    }
    return 1;
}

/*======================= Function Handle Button ====================*/
void        User_Button_Enter_Click(void)
{
    Handle_Menu_Button(_BT_ENTER_CLICK);
}

void        User_Button_Up_Click(void)
{
    Handle_Menu_Button(_BT_UP_CLICK);
}

void        User_Button_Down_Click(void)
{
    Handle_Menu_Button(_BT_DOWN_CLICK);
}

void        User_Button_Enter_Hold_Cycle(void)
{
    Handle_Menu_Button(_BT_ENTER_HOLD_CYCLE);
}

void        User_Button_Up_Hold_Cycle(void)
{
    Handle_Menu_Button(_BT_UP_HOLD_CYCLE);
}

void        User_Button_Down_Hold_Cycle(void)
{
    Handle_Menu_Button(_BT_DOWN_HOLD_CYCLE);
}

void        User_Button_Enter_Hold_Once(void)
{
    Handle_Menu_Button(_BT_ENTER_HOLD_ONCE);
}

void        User_Button_Up_Hold_Once(void)
{
    Handle_Menu_Button(_BT_UP_HOLD_ONCE);
}

void        User_Button_Down_Hold_Once(void)
{
    Handle_Menu_Button(_BT_DOWN_HOLD_ONCE);
}
/*=================== Function LCD Menu =====================*/
void    Handle_Menu_Button(uint8_t KindButton)
{
    fevent_enable(sEventAppLed7seg, _EVENT_LED7SEG_BACK_MAIN);
    switch(sParamMenu.StateMenu)
    {
        case _MENU_MAIN:
          BT_Menu_Main(KindButton);
          break;
          
        case _MENU_SETUP_RUN:
          BT_Menu_Setup_Run(KindButton);
          break;
          
        case _MENU_SETUP_FREE:
          BT_Menu_Setup_Free(KindButton);
          break;
          
        default:
          break;
    }
}

void    BT_Menu_Main(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          sParamMenu.StateMenu = _MENU_SETUP_RUN;
          sParamMenu.Stamp_TimeRun  = sParamSlave.TimeRun;
          sParamMenu.Stamp_TimeFree = sParamSlave.TimeFree; 
          sCtrlOxy.StateCtrl  = _OXY_OFF;
          Dispaly_Number_Led7seg(sParamSlave.TimeRun, 2);
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
//        case _BT_EXIT_CLICK:
//        case _BT_EXIT_HOLD_CYCLE:
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
        case _BT_ENTER_HOLD_ONCE:
//        case _BT_EXIT_HOLD_ONCE:
        case _BT_UP_HOLD_ONCE:
        case _BT_DOWN_HOLD_ONCE:
          break;
        
        default:
          break;
    }
}
void    BT_Menu_Setup_Run(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          sParamMenu.StateMenu = _MENU_SETUP_FREE;
          Dispaly_Number_Led7seg(sParamSlave.TimeFree, 2);
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
//        case _BT_EXIT_CLICK:
//        case _BT_EXIT_HOLD_CYCLE:
          break; 
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          if(sParamMenu.Stamp_TimeRun < TIMECYCLE_SLAVE_MAX) 
            sParamMenu.Stamp_TimeRun++;
          
          Dispaly_Number_Led7seg(sParamMenu.Stamp_TimeRun, 2);
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          if(sParamMenu.Stamp_TimeRun > TIMECYCLE_SLAVE_MIN) 
            sParamMenu.Stamp_TimeRun--;
          
          Dispaly_Number_Led7seg(sParamMenu.Stamp_TimeRun, 2);
          break;
          
        case _BT_ENTER_HOLD_ONCE:
//        case _BT_EXIT_HOLD_ONCE:
        case _BT_UP_HOLD_ONCE:
        case _BT_DOWN_HOLD_ONCE:
          break;
        
        default:
          break;
    }
}
void    BT_Menu_Setup_Free(uint8_t KindButton)
{
    switch(KindButton)
    {
        case _BT_ENTER_CLICK:
          sParamMenu.StateMenu = _MENU_MAIN;
          if(sParamMenu.Stamp_TimeRun != sParamSlave.TimeRun || sParamMenu.Stamp_TimeFree != sParamSlave.TimeFree)
            Save_TimeCycleSlave(sParamMenu.Stamp_TimeRun, sParamMenu.Stamp_TimeFree);
          sCtrlOxy.StateCtrl = _OXY_FREE;
          break;
          
        case _BT_ENTER_HOLD_CYCLE:
//        case _BT_EXIT_CLICK:
//        case _BT_EXIT_HOLD_CYCLE:
          break;
          
        case _BT_UP_CLICK:
        case _BT_UP_HOLD_CYCLE:
          if(sParamMenu.Stamp_TimeFree < TIMECYCLE_SLAVE_MAX) 
            sParamMenu.Stamp_TimeFree++;
          
          Dispaly_Number_Led7seg(sParamMenu.Stamp_TimeFree, 2);
          break;
          
        case _BT_DOWN_CLICK:
        case _BT_DOWN_HOLD_CYCLE:
          if(sParamMenu.Stamp_TimeFree > TIMECYCLE_SLAVE_MIN) 
            sParamMenu.Stamp_TimeFree--;
          
          Dispaly_Number_Led7seg(sParamMenu.Stamp_TimeFree, 2);
          break;
          
        case _BT_ENTER_HOLD_ONCE:
//        case _BT_EXIT_HOLD_ONCE:
        case _BT_UP_HOLD_ONCE:
        case _BT_DOWN_HOLD_ONCE:
          break;
        
        default:
          break;
    }
}

/*======================= Function Handle ====================*/
/*
    @Hien thi man hinh chinh 
    @Dat trong ngat timer
*/
void Dis_Led7seg_Menu_Main(void)
{
    static uint8_t CountDis = 0;
    
    CountDis++;
    if(CountDis == 10)
    {
        CountDis = 0;
        switch(sParamLed7seg.RunDis)
        {
            case 3:
              sParamLed7seg.RunDis = 2;
              break;
              
            case 2:
              sParamLed7seg.RunDis = 1;
              break;
              
            case 1: 
              sParamLed7seg.RunDis = 3;
              
            default:
              break;
        }
    }
}

/*
    @brief Display number (uint8_t)
    @Data  Number display
    @Pos   Position display (1, 2, 3)
*/
void Send_Dis_Led7seg(uint8_t Data, uint8_t Pos)
{
    uint8_t aData[1]={0};
    aData[0] = Data;
    HAL_GPIO_WritePin(STCP_LED7SEG_GPIO_Port, STCP_LED7SEG_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, aData, 1 ,10);
    HAL_GPIO_WritePin(STCP_LED7SEG_GPIO_Port, STCP_LED7SEG_Pin, GPIO_PIN_SET);
    
    switch(Pos)
    {
        case 1:
          HAL_GPIO_WritePin(A1_LED7SEG_GPIO_Port, A1_LED7SEG_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(A0_LED7SEG_GPIO_Port, A0_LED7SEG_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(A2_LED7SEG_GPIO_Port, A2_LED7SEG_Pin, GPIO_PIN_RESET);
          break;
          
        case 2:
          HAL_GPIO_WritePin(A2_LED7SEG_GPIO_Port, A2_LED7SEG_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(A0_LED7SEG_GPIO_Port, A0_LED7SEG_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(A1_LED7SEG_GPIO_Port, A1_LED7SEG_Pin, GPIO_PIN_RESET);
          break;
          
        case 3:
          HAL_GPIO_WritePin(A2_LED7SEG_GPIO_Port, A2_LED7SEG_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(A1_LED7SEG_GPIO_Port, A1_LED7SEG_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(A0_LED7SEG_GPIO_Port, A0_LED7SEG_Pin, GPIO_PIN_RESET);
          break;
          
        default:
          break;
    }
}

/*
    @brief Display Number (uint16_t)
    @Numer Numer display (uint16_t)
    @Point Point Number
*/
void Dispaly_Number_Led7seg(uint16_t Number, uint8_t Point)
{
    if(Number <= NUMBER_LED7SEG_MAX)
        sParamLed7seg.NumberDis = Number;
    else
        sParamLed7seg.NumberDis = NUMBER_LED7SEG_MAX;
    
    sParamLed7seg.PointDis = Point;
}


/*---------------------Function Handle--------------------*/
/*
    Su dung trong ngat systemTick hien thi led
*/

void Display_Led7Seg(void)
{
    uint8_t frontDis = 0;
    if(sParamMenu.StateMenu == _MENU_MAIN)
    {
        frontDis = RunDis_Cathode;
        Send_Dis_Led7seg(frontDis, sParamLed7seg.RunDis);
    }
    else
    {
        switch(sParamLed7seg.Location)
        {
            case _LOCAL_DIS_1:
              if(sParamLed7seg.PointDis == _LOCAL_DIS_1)
                frontDis = front_led7seg_cathode_DOT[sParamLed7seg.NumberDis%10];
              else 
                frontDis = front_led7seg_cathode_NDOT[sParamLed7seg.NumberDis%10];
              
              Send_Dis_Led7seg(frontDis, _LOCAL_DIS_1);
              
              if(sParamLed7seg.NumberDis >= 10 || sParamLed7seg.PointDis == _LOCAL_DIS_2)
                sParamLed7seg.Location = _LOCAL_DIS_2;
              
              break;
              
            case _LOCAL_DIS_2:
              if(sParamLed7seg.PointDis == _LOCAL_DIS_2)
                frontDis = front_led7seg_cathode_DOT[(sParamLed7seg.NumberDis/10)%10];
              else 
                frontDis = front_led7seg_cathode_NDOT[(sParamLed7seg.NumberDis/10)%10];
              
              if(sParamLed7seg.NumberDis >= 10 || sParamLed7seg.PointDis == _LOCAL_DIS_2)
                Send_Dis_Led7seg(frontDis, _LOCAL_DIS_2);
              
              if(sParamLed7seg.NumberDis >= 100 || sParamLed7seg.PointDis == _LOCAL_DIS_3)
                sParamLed7seg.Location = _LOCAL_DIS_3;
              else
                sParamLed7seg.Location = _LOCAL_DIS_1;
             
              break;
              
            case _LOCAL_DIS_3:
              if(sParamLed7seg.PointDis == _LOCAL_DIS_3)
                frontDis = front_led7seg_cathode_DOT[(sParamLed7seg.NumberDis/100)%10];
              else 
                frontDis = front_led7seg_cathode_NDOT[(sParamLed7seg.NumberDis/100)%10];
              
              if(sParamLed7seg.NumberDis >= 100 || sParamLed7seg.PointDis == _LOCAL_DIS_3)
                Send_Dis_Led7seg(frontDis, _LOCAL_DIS_3);
              
                sParamLed7seg.Location = _LOCAL_DIS_1;
              break;
              
            default:
              break;
        }
    }
}

/*=========================Handle Init and Task app=========================*/
void        Init_AppLed7seg(void)
{
    
}

uint8_t     AppLed7seg_Task(void)
{
    uint8_t i = 0;
    uint8_t Result =  false;
    
    for(i = 0; i < _EVENT_LED7SEG_END; i++)
    {
        if(sEventAppLed7seg[i].e_status == 1)
        {
            Result = true; 
            
            if((sEventAppLed7seg[i].e_systick == 0) ||
               ((HAL_GetTick() - sEventAppLed7seg[i].e_systick) >= sEventAppLed7seg[i].e_period))
            {
                sEventAppLed7seg[i].e_status = 0; //Disable event
                sEventAppLed7seg[i].e_systick= HAL_GetTick();
                sEventAppLed7seg[i].e_function_handler(i);
            }
        }
    }
    
    return Result;
}