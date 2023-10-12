#include "user_app_ctrl_oxy.h"
#include "user_app_relay.h"

#include "user_app_button.h"
#include "user_app_lcd.h"
#include "user_internal_mem.h"
#include "tim.h"
#include "user_app_menu_display.h"
/*====================Function Static===================*/
static uint8_t fevent_ctrl_oxy_entry(uint8_t event);
static uint8_t fevent_ctrl_oxy_gettick(uint8_t event);
static uint8_t fevent_ctrl_oxy_0_calib(uint8_t event);
static uint8_t fevent_ctrl_oxy_100_calib(uint8_t event);
/*========================Struct========================*/
sEvent_struct               sEventAppCtrlOxy[]=
{
  {_EVENT_CTRL_OXY_ENTRY,       1, 5, 5,            fevent_ctrl_oxy_entry},
  {_EVENT_CTRL_OXY_GETTICK,     1, 5, 0,            fevent_ctrl_oxy_gettick},
  {_EVENT_CTRL_OXY_0_CALIB,     0, 5, 500,          fevent_ctrl_oxy_0_calib},
  {_EVENT_CTRL_OXY_100_CALIB,   0, 5, 500,          fevent_ctrl_oxy_100_calib},
};

Struct_Time_Ctrl_Oxy            sTimeCtrlOxy = {DEFAULT_RUN_OXY, DEFAULT_FREE_OXY, 0, 0, 0, 0, 0, 0};
Struct_Parameter_Ctrl_Oxy       sParamCtrlOxy= { 0, 0, 0, 0};
Struct_Param_Measure            sParamMeasure= {0, 0, 0};
Struct_Password                 sPassword={0,0,0,0,0,0,
                                           0,0,0,0,0,0};

/*================= Function Callback Gettick =================*/
__weak void Irq_Second_Callback(void)
{
//    Ctrl_Oxy1();
//    Ctrl_Oxy2();
//    Ctrl_Oxy3();
}
/*=======================Function Hanlde=================*/
static uint8_t fevent_ctrl_oxy_entry(uint8_t event)
{
    return 1;
}

static uint8_t fevent_ctrl_oxy_gettick(uint8_t event)
{
    static uint32_t Check_Gettick = 0;
    if(HAL_GetTick() / 100 != Check_Gettick)
    {
        Check_Gettick = HAL_GetTick()/100;
        Irq_Second_Callback();
    }
  
    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}

static uint8_t fevent_ctrl_oxy_0_calib(uint8_t event)
{
    static uint8_t countstate = 0;
    if(countstate >= 10)
    {
        sMenuState.CalibOxy = _CALIB_ERROR;
        countstate = 0;
        return 1;
    }
    else
    {
        fevent_enable(sEventAppCtrlOxy, event);
    }
    countstate++;
    return 1;
}

static uint8_t fevent_ctrl_oxy_100_calib(uint8_t event)
{
    static uint8_t countstate = 0;
    if(countstate >= 10)
    {
        sMenuState.CalibOxy = _CALIB_ERROR;
        countstate = 0;
        return 1;
    }
    else
    {
        fevent_enable(sEventAppCtrlOxy, event);
    }
    countstate++;
    return 1;
}

/*========================Function Handle====================*/

void Ctrl_Oxy1(void)
{
    static uint8_t StateBefore = 0;
    switch(sParamCtrlOxy.StateOxy1)
    {
    case _OXY_OFF:
      sTimeCtrlOxy.RunOxy1 = 0;
      sTimeCtrlOxy.FreeOxy1 = 0;
      if(sStatusRelay.RL1 == ON_RELAY)    ControlRelay(RELAY_1, OFF_RELAY, _RL_DEBUG);
      if(sStatusRelay.RL2 == ON_RELAY)    ControlRelay(RELAY_2, OFF_RELAY, _RL_DEBUG);
      break;
      
    case _OXY_FREE:
      sTimeCtrlOxy.FreeOxy1++;
      if(sTimeCtrlOxy.FreeOxy1 >= sTimeCtrlOxy.FreeCtrl)     
      {
        if(StateBefore == _OXY_L_RUN)
        {
            sParamCtrlOxy.StateOxy1 = _OXY_R_RUN;
            ControlRelay(RELAY_1, ON_RELAY, _RL_DEBUG);
        }
        else
        {
            sParamCtrlOxy.StateOxy1 = _OXY_L_RUN;
            ControlRelay(RELAY_2, ON_RELAY, _RL_DEBUG);
        }
        StateBefore  = sParamCtrlOxy.StateOxy1;
        sTimeCtrlOxy.FreeOxy1 = 0;
      }
      break;
      
      default:
      sTimeCtrlOxy.RunOxy1++;
      if(sTimeCtrlOxy.RunOxy1 >= sTimeCtrlOxy.RunCtrl)     
      {
        sParamCtrlOxy.StateOxy1 = _OXY_FREE;
        if(sStatusRelay.RL1 == ON_RELAY)    ControlRelay(RELAY_1, OFF_RELAY, _RL_DEBUG);
        if(sStatusRelay.RL2 == ON_RELAY)    ControlRelay(RELAY_2, OFF_RELAY, _RL_DEBUG);
        sTimeCtrlOxy.RunOxy1 = 0;
      }
      break;
    }
}

void Ctrl_Oxy2(void)
{
    static uint8_t StateBefore = 0;
    switch(sParamCtrlOxy.StateOxy2)
    {
    case _OXY_OFF:
        sTimeCtrlOxy.RunOxy2 = 0;
        sTimeCtrlOxy.FreeOxy2 = 0;
        if(sStatusRelay.RL3 == ON_RELAY)    ControlRelay(RELAY_3, OFF_RELAY, _RL_DEBUG);
        if(sStatusRelay.RL4 == ON_RELAY)    ControlRelay(RELAY_4, OFF_RELAY, _RL_DEBUG);
        break;
      
    case _OXY_FREE:
      sTimeCtrlOxy.FreeOxy2++;
      if(sTimeCtrlOxy.FreeOxy2 >= sTimeCtrlOxy.FreeCtrl)     
      {
        if(StateBefore == _OXY_L_RUN)
        {
            sParamCtrlOxy.StateOxy2 = _OXY_R_RUN;
            ControlRelay(RELAY_3, ON_RELAY, _RL_DEBUG);
        }
        else
        {
            sParamCtrlOxy.StateOxy2 = _OXY_L_RUN;
            ControlRelay(RELAY_4, ON_RELAY, _RL_DEBUG);
        }
        StateBefore  = sParamCtrlOxy.StateOxy2;
        sTimeCtrlOxy.FreeOxy2 = 0;
      }
      break;
      
      default:
      sTimeCtrlOxy.RunOxy2++;
      if(sTimeCtrlOxy.RunOxy2 >= sTimeCtrlOxy.RunCtrl)     
      {
        sParamCtrlOxy.StateOxy2 = _OXY_FREE;
        if(sStatusRelay.RL3 == ON_RELAY)    ControlRelay(RELAY_3, OFF_RELAY, _RL_DEBUG);
        if(sStatusRelay.RL4 == ON_RELAY)    ControlRelay(RELAY_4, OFF_RELAY, _RL_DEBUG);
        sTimeCtrlOxy.RunOxy2 = 0;
      }
      break;
    }
}

void Ctrl_Oxy3(void)
{
    static uint8_t StateBefore = 0;
    switch(sParamCtrlOxy.StateOxy3)
    {
    case _OXY_OFF:
        sTimeCtrlOxy.RunOxy3 = 0;
        sTimeCtrlOxy.FreeOxy3 = 0;
        if(sStatusRelay.RL5 == ON_RELAY)    ControlRelay(RELAY_5, OFF_RELAY, _RL_DEBUG);
        if(sStatusRelay.RL6 == ON_RELAY)    ControlRelay(RELAY_6, OFF_RELAY, _RL_DEBUG);
      break;
      
    case _OXY_FREE:
      sTimeCtrlOxy.FreeOxy3++;
      if(sTimeCtrlOxy.FreeOxy3 >= sTimeCtrlOxy.FreeCtrl)     
      {
        if(StateBefore == _OXY_L_RUN)
        {
            sParamCtrlOxy.StateOxy3 = _OXY_R_RUN;
            ControlRelay(RELAY_5, ON_RELAY, _RL_DEBUG);
        }
        else
        {
            sParamCtrlOxy.StateOxy3 = _OXY_L_RUN;
            ControlRelay(RELAY_6, ON_RELAY, _RL_DEBUG);
        }
        StateBefore  = sParamCtrlOxy.StateOxy3;
        sTimeCtrlOxy.FreeOxy3= 0;
      }
      break;
      
      default:
      sTimeCtrlOxy.RunOxy3++;
      if(sTimeCtrlOxy.RunOxy3 >= sTimeCtrlOxy.RunCtrl)     
      {
        sParamCtrlOxy.StateOxy3 = _OXY_FREE;
        if(sStatusRelay.RL5 == ON_RELAY)    ControlRelay(RELAY_5, OFF_RELAY, _RL_DEBUG);
        if(sStatusRelay.RL6 == ON_RELAY)    ControlRelay(RELAY_6, OFF_RELAY, _RL_DEBUG);
        sTimeCtrlOxy.RunOxy3 = 0;
      }
      break;
    }
}

void Save_Time_Ctrl_Flash(uint16_t RunCtrl, uint16_t FreeCtrl)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sTimeCtrlOxy.RunCtrl = RunCtrl;
    sTimeCtrlOxy.FreeCtrl = FreeCtrl;
    aData[length++] = sTimeCtrlOxy.RunCtrl << 8;
    aData[length++] = sTimeCtrlOxy.RunCtrl;
    
    aData[length++] = sTimeCtrlOxy.FreeCtrl << 8;
    aData[length++] = sTimeCtrlOxy.FreeCtrl;
    Save_Array(ADDR_CTRL_OXY_RUN_FREE, aData, length);
}

void Save_Satility_Flash(uint16_t Satility)
{
    uint8_t aData[2] = {0};
    uint8_t length = 0;
    
    sParamCtrlOxy.SetupSatility = Satility;
    aData[length++] = sParamCtrlOxy.SetupSatility;

    Save_Array(ADDR_SATILITY_SETTING, aData, length);
}

void Init_Satility(void)
{
    if(*(__IO uint8_t*)(ADDR_SATILITY_SETTING) == BYTE_TEMP_FIRST)
    {
        sParamCtrlOxy.SetupSatility  = *(__IO uint8_t*)(ADDR_SATILITY_SETTING+2) ;
    }
    Stamp_Menu_Exit();
}

void Init_Time_Ctrl_Oxy(void)
{
    if(*(__IO uint8_t*)(ADDR_CTRL_OXY_RUN_FREE) == BYTE_TEMP_FIRST)
    {
        sTimeCtrlOxy.FreeCtrl  = *(__IO uint8_t*)(ADDR_CTRL_OXY_RUN_FREE+4) ;
        sTimeCtrlOxy.FreeCtrl  = sTimeCtrlOxy.FreeCtrl << 8;
        sTimeCtrlOxy.FreeCtrl |= *(__IO uint8_t*)(ADDR_CTRL_OXY_RUN_FREE+5);
        
        sTimeCtrlOxy.RunCtrl = *(__IO uint8_t*)(ADDR_CTRL_OXY_RUN_FREE+2);
        sTimeCtrlOxy.RunCtrl = sTimeCtrlOxy.RunCtrl << 8;
        sTimeCtrlOxy.RunCtrl = *(__IO uint8_t*)(ADDR_CTRL_OXY_RUN_FREE+3);
        
        if(sTimeCtrlOxy.FreeCtrl < TIME_MIN)
          sTimeCtrlOxy.FreeCtrl = DEFAULT_FREE_OXY;
    }
    Stamp_Menu_Exit();
}

void Init_AppCtrlOxy(void)
{
    Init_Time_Ctrl_Oxy();
    HAL_TIM_Base_Start_IT(&htim11);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
     if(htim->Instance == htim11.Instance)
     {
//       Ctrl_Oxy1();
//       Ctrl_Oxy2();
//       Ctrl_Oxy3();
     }
}

uint8_t AppCtrlOxy_Task(void)
{
    uint8_t i = 0;
    uint8_t Result =  false;
    
    for(i = 0; i < _EVENT_CTRL_OXY_END; i++)
    {
        if(sEventAppCtrlOxy[i].e_status == 1)
        {
            Result = true; 
            
            if((sEventAppCtrlOxy[i].e_systick == 0) ||
               ((HAL_GetTick() - sEventAppCtrlOxy[i].e_systick) >= sEventAppCtrlOxy[i].e_period))
            {
                sEventAppCtrlOxy[i].e_status = 0; //Disable event
                sEventAppCtrlOxy[i].e_systick= HAL_GetTick();
                sEventAppCtrlOxy[i].e_function_handler(i);
            }
        }
    }
    
    return Result;
}