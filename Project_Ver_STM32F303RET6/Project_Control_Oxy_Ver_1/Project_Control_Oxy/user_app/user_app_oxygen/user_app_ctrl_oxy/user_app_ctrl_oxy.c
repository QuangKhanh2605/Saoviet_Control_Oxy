/*
    *Update Firmware:

    Dang nhap filezilla
    192.168.6.67
    user: saoviet 
    Pass: sv123@

    Dien url:
    at+userurl=http://up.saovietgroup.com.vn:80/khanh_http/Project_Control_Oxy.bin
*/

#include "user_app_ctrl_oxy.h"
#include "user_app_relay.h"

#include "user_app_button.h"
#include "user_app_lcd.h"
#include "user_internal_mem.h"
#include "user_app_menu_display.h"
#include "user_modem.h"
#include "user_convert_variable.h"
#include "math.h"
#include "iwdg.h"
/*====================Function Static===================*/
static uint8_t fevent_ctrl_oxy_entry(uint8_t event);
static uint8_t fevent_entry_tsvh(uint8_t event);
static uint8_t fevent_ctrl_oxy_iwdg(uint8_t event);
static uint8_t fevent_send_slave_cycle(uint8_t event);
static uint8_t fevent_ctrl_oxy_log_tsvh(uint8_t event);
//static uint8_t fevent_ctrl_oxy_sw_notify(uint8_t event);
static uint8_t fevent_ctrl_oxy_pw_detect(uint8_t event);
static uint8_t fevent_ctrl_oxy_state_slave(uint8_t event);
static uint8_t fevent_ctrl_oxy_warning(uint8_t event);
static uint8_t fevent_pump_warning(uint8_t event);
//static uint8_t fevent_ctrl_oxy_freq_notify(uint8_t event);
static uint8_t fevent_ctrl_oxy_reset_dcu(uint8_t event);
static uint8_t fevent_ctrl_oxy_sw_man_auto(uint8_t event);
/*========================Struct========================*/
sEvent_struct               sEventAppCtrlOxy[]=
{
  {_EVENT_CTRL_OXY_ENTRY,           1, 5, 30000,        fevent_ctrl_oxy_entry}, //doi sensor do on dinh thi bat dau xet cac truong hop
  {_EVENT_ENTRY_TSVH,               1, 5, 40000,        fevent_entry_tsvh},
  {_EVENT_CTRL_OXY_IWDG,            1, 5, 500,          fevent_ctrl_oxy_iwdg},
  
  {_EVENT_SEND_SLAVE_CYCLE,         0, 5, 1000,         fevent_send_slave_cycle},

  {_EVENT_CTRL_OXY_LOG_TSVH,        0, 5, 10000,        fevent_ctrl_oxy_log_tsvh},
  
//  {_EVENT_CTRL_OXY_SW_NOTIFY,       1, 5, 500,          fevent_ctrl_oxy_sw_notify},
  
  {_EVENT_CTRL_OXY_PW_DETECT,       0, 5, 1000,         fevent_ctrl_oxy_pw_detect},
  
  {_EVENT_CTRL_OXY_STATE_SLAVE,     0, 5, 1000,         fevent_ctrl_oxy_state_slave},
  
  {_EVENT_CTRL_OXY_WARNING,         0, 5, 1000,         fevent_ctrl_oxy_warning},
  
  {_EVENT_PUMP_WARNING,             0, 5, 1000,         fevent_pump_warning},
  
//  {_EVENT_CTRL_OXY_FREQ_NOTIFY,     0, 5, 5,            fevent_ctrl_oxy_freq_notify},
  
  {_EVENT_CTRL_OXY_RESET_DCU,       1, 5, 15000,        fevent_ctrl_oxy_reset_dcu},
  
  {_EVENT_CTRL_OXY_SW_MAN_AUTO,     1, 5, 500,          fevent_ctrl_oxy_sw_man_auto},
};
uint8_t     aStateOxy[5]={0};
Struct_StateCtrlOxy             sStateCtrlOxy= 
{
  .aStateOxy = aStateOxy,
};

Struct_ParamMeasure             sParamMeasure= {0};
Struct_TimeSlave                sTimeSlave = {0};
Struct_ParamCtrlOxy             sParamCtrlOxy= {0};

Struct_Queue_Type               qCtrlOxyRun;
Struct_Queue_Type               qCtrlOxyFree;

uint8_t                         qReadRun[10];
uint8_t                         qReadFree[10];

uint8_t     Flag_Reset_DCU =0;

Struct_OxyLogEEPROM             sOxyLogEEPROM=
{
    .Addr_Oxy_Log_Start     = CAT_ADDR_OXY_START,
    .Addr_Oxy_Log_End       = CAT_ADDR_OXY_STOP, 
    .SizePage               = 256,
    .SizeRecord             = 5,
};

Struct_GettickMs                sGettickMs={0};
/*=======================Function Hanlde=================*/
static uint8_t fevent_ctrl_oxy_entry(uint8_t event)
{
    sStateCtrlOxy.StateDCU = 1;
//    fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_STATE_SLAVE);
//    fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_FREQ_NOTIFY);
//    fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_PW_DETECT);
    fevent_enable(sEventAppCtrlOxy, _EVENT_CTRL_OXY_STATE_SLAVE);
    fevent_enable(sEventAppCtrlOxy, _EVENT_CTRL_OXY_WARNING);
    fevent_enable(sEventAppCtrlOxy, _EVENT_CTRL_OXY_PW_DETECT);
    fevent_enable(sEventAppCtrlOxy, _EVENT_PUMP_WARNING);
    return 1;
}

static uint8_t fevent_entry_tsvh(uint8_t event)
{
    sStateCtrlOxy.StateSendOpera = 1;
    return 1;
}

static uint8_t fevent_ctrl_oxy_iwdg(uint8_t event)
{
    HAL_IWDG_Refresh(&hiwdg);
    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}

static uint8_t fevent_send_slave_cycle(uint8_t event)
{
    static uint8_t stateSend = 0;
    uint8_t aData[20]={0};
    uint16_t Length = 0;
    if(stateSend == 0)
    {
        Insert_String_To_String(aData, &Length,(uint8_t*)"at+run=",0, 7);
        Convert_Var_Packet_Integer(aData, &Length, sTimeSlave.RunCtrl);
        stateSend = 1;
        fevent_enable(sEventAppCtrlOxy, event);
    }
    else
    {
        Insert_String_To_String(aData, &Length,(uint8_t*)"at+free=",0, 8);
        Convert_Var_Packet_Integer(aData, &Length, sTimeSlave.FreeCtrl);
        stateSend = 0;
    }
    
    Send_RS458_Sensor(aData, Length);
    
//    UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Send_Cycle_Slave!\r\n");
    DCU_Respond_Debug((uint8_t*)"Send_Cycle_Slave",16);
    
    return 1;
}

static uint8_t fevent_ctrl_oxy_log_tsvh(uint8_t event)
{
	//Log Data meter to flash
    UTIL_Printf_Str(DBLEVEL_M, "u_app_ctrl_oxy: packet tsvh!\r\n" );
    //
    Get_RTC();
    
    AppCtrlOxy_Log_Data_TSVH();    
        
    return 1;
}

//static uint8_t fevent_ctrl_oxy_sw_notify(uint8_t event)
//{
//    static uint8_t event_notify = 0;
//    switch(event_notify)
//    {
//        case 0:
//          fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_PW_DETECT);
//          event_notify++;
//          break;
//          
//        case 1:
//          if(sStateCtrlOxy.StateDCU == 1)
//          {
//            fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_STATE_SLAVE);
//          }
//          event_notify++;
//          break;
//          
//        case 2:
//          if(sStateCtrlOxy.StateDCU == 1)
//          {
//            fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_FREQ_NOTIFY);
//          }
//          event_notify = 0;
//          break;
//          
//        default:
//          break;
//    }
//
//    fevent_enable(sEventAppCtrlOxy, event);
//    return 1;
//}

static uint8_t fevent_ctrl_oxy_pw_detect(uint8_t event)
{
    static uint32_t gettick_pw_detect = 0;
    static uint8_t StatePowerBefor = _POWER_ON;
    
    sStateCtrlOxy.StatePower = HAL_GPIO_ReadPin(PW_Detect_GPIO_Port, PW_Detect_Pin);
    if(sStateCtrlOxy.StatePower != StatePowerBefor)
    {
        gettick_pw_detect = HAL_GetTick();
        
        if(sStateCtrlOxy.StatePower == _POWER_ON)  
        {
            UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Power_ON!\r\n");
        }
        else
        {
            Log_EventWarnig(OBIS_WARNING_POWER, 0x00);
            UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Power_OFF!\r\n");
        }

        StatePowerBefor = sStateCtrlOxy.StatePower;
    }
    
    if(sStateCtrlOxy.StatePower == _POWER_OFF)
    {
        if(HAL_GetTick() - gettick_pw_detect >= TIME_RESEND_WARNING*60000)
        {
            gettick_pw_detect = HAL_GetTick();
            Log_EventWarnig(OBIS_WARNING_POWER, 0x00);
        }
    }
    
    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}

static uint8_t fevent_ctrl_oxy_state_slave(uint8_t event)
{
    static uint32_t gettick_state_slave = 0;
    static uint8_t StateSensorOxyBefore = _OXY_CONNECT;
    
    if(sStateCtrlOxy.StateSensorOxy != StateSensorOxyBefore)
    {
        gettick_state_slave = HAL_GetTick();
        
        if(sStateCtrlOxy.StateSensorOxy == _OXY_CONNECT)      
        {
            sStateCtrlOxy.StateWarningOxy_Connect = _OXY_STT_OK;
//            UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Slave_CONNECT!\r\n");
        }
        else
        {
            sStateCtrlOxy.StateWarningOxy_Connect = _OXY_STT_FAIL;
            Log_EventWarnig(OBIS_WARNING_CONNECT_SENSOR, 0x00);
//            UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Slave_DISCONNECT!\r\n");
        }
 
        StateSensorOxyBefore = sStateCtrlOxy.StateSensorOxy;
    }
    
    if(sStateCtrlOxy.StateSensorOxy == _OXY_DISCONNECT)
    {
        if(HAL_GetTick() - gettick_state_slave >= TIME_RESEND_WARNING*60000)
        {
            gettick_state_slave = HAL_GetTick();
            Log_EventWarnig(OBIS_WARNING_CONNECT_SENSOR, 0x00);
        }
    }
    
    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}

static uint8_t fevent_ctrl_oxy_warning(uint8_t event)
{
    static uint32_t gettick_warning_period = 0;
    static uint8_t StateOxyBefore = _OXY_BALANCE;
    
    if(sParamMeasure.Oxy_Mg_L < sParamCtrlOxy.Oxy_Lower_Warning)
    {
        OnOff_Warning(ON_RELAY);
        sStateCtrlOxy.StateOxyLower = _OXY_LOW;
        sStateCtrlOxy.StateWarningOxy_Measure = _OXY_STT_FAIL;
        if(sStateCtrlOxy.StateOxyLower != StateOxyBefore)
        {
            gettick_warning_period = HAL_GetTick();
            Log_EventWarnig(OBIS_WARNING_OXY_LOWER, 0x00);
            StateOxyBefore = sStateCtrlOxy.StateOxyLower;
        }
    }
    else if(sParamMeasure.Oxy_Mg_L > sParamCtrlOxy.Oxy_Upper_Warning)
    {
        OnOff_Warning(ON_RELAY);
        sStateCtrlOxy.StateOxyLower = _OXY_HIGH;
        sStateCtrlOxy.StateWarningOxy_Measure = _OXY_STT_FAIL;
        if(sStateCtrlOxy.StateOxyLower != StateOxyBefore)
        {
            gettick_warning_period = HAL_GetTick();
            Log_EventWarnig(OBIS_WARNING_OXY_LOWER, 0x01);
            StateOxyBefore = sStateCtrlOxy.StateOxyLower;
        }
    }
    else
    {
        OnOff_Warning(OFF_RELAY);
        sStateCtrlOxy.StateOxyLower = _OXY_BALANCE;
        sStateCtrlOxy.StateWarningOxy_Measure = _OXY_STT_OK;
        gettick_warning_period = HAL_GetTick();
    }
    
    if(sStateCtrlOxy.StateOxyLower == _OXY_LOW)
    {
        if(HAL_GetTick() - gettick_warning_period >= TIME_RESEND_WARNING*60000)
        {
            gettick_warning_period = HAL_GetTick();
            Log_EventWarnig(OBIS_WARNING_OXY_LOWER, 0x00);
        }
    }
    else if(sStateCtrlOxy.StateOxyLower == _OXY_HIGH)
    {
        if(HAL_GetTick() - gettick_warning_period >= TIME_RESEND_WARNING*60000)
        {
            gettick_warning_period = HAL_GetTick();
            Log_EventWarnig(OBIS_WARNING_OXY_LOWER, 0x01);
        }
    }
    
    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}

static uint8_t fevent_pump_warning(uint8_t event)
{
    static uint32_t gettick_pump = 0;
    
    if(HAL_GetTick() - gettick_pump > TIME_RESEND_WARNING_PUMP*60000 || gettick_pump == 0)
    {
        if(sStateCtrlOxy.StatePump == _PUMP_FAIL)
        {
            Log_EventWarnig(OBIS_WARNING_PUMP, 0x01);
            sStateCtrlOxy.StatePump = _PUMP_OK;
            sStateCtrlOxy.StateWarningOxy_Pump = _OXY_STT_FAIL;
            gettick_pump = HAL_GetTick();
        }     
        else
        {
            sStateCtrlOxy.StatePump = _PUMP_OK;
            sStateCtrlOxy.StateWarningOxy_Pump = _OXY_STT_OK;
        }

    }
  
    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}

//static uint8_t fevent_ctrl_oxy_freq_notify(uint8_t event)
//{
////    static uint32_t Gettick_Notify = 0;
//    static uint8_t Send_Power = 0;
//    static uint8_t Send_Sensor = 0;
//    static uint8_t Send_OxyLow = 0;
//    
//    if(sStateCtrlOxy.StatePower == _POWER_OFF)  
//    {
//        if(Send_Power == 0)
//        {
//            UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Power_OFF!\r\n");
////            DCU_Notify_Server(_RESPOND_NOTIFY_POWER_OFF);
//            Log_EventWarnig(OBIS_WARNING_POWER, 0x00);
//            Send_Power = 1;
////            Gettick_Notify = HAL_GetTick();
//        }
//        else
//        {
////            if(HAL_GetTick() - Gettick_Notify > FREQ_SEND_NOTIFY * 60000)
////            {
//////                DCU_Notify_Server(_RESPOND_NOTIFY_POWER_OFF);
////                Log_EventWarnig(OBIS_WARNING_POWER, 0x00);
////                Gettick_Notify = HAL_GetTick();
////            }
//        }
//        Send_Sensor = 0;
//        Send_OxyLow = 0;
//    }
//    else if(sStateCtrlOxy.StateSensorOxy == _OXY_DISCONNECT)
//    {
//        if(Send_Sensor == 0)
//        {
//            UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Slave_DISCONNECT!\r\n");
////            DCU_Notify_Server(_RESPOND_NOTIFY_SLAVE_DISCONNECT);
//            Log_EventWarnig(OBIS_WARNING_CONNECT_SENSOR, 0x00);
//            Send_Sensor = 1;
////            Gettick_Notify = HAL_GetTick();
//        }
//        else
//        {
////            if(HAL_GetTick() - Gettick_Notify > FREQ_SEND_NOTIFY * 60000)
////            {
//////                DCU_Notify_Server(_RESPOND_NOTIFY_SLAVE_DISCONNECT);
////                Log_EventWarnig(OBIS_WARNING_CONNECT_SENSOR, 0x00);
////                Gettick_Notify = HAL_GetTick();
////            }
//        }
//        Send_Power = 0;
//        Send_OxyLow = 0;
//    }
//    else if(sStateCtrlOxy.StateOxyLower == _OXY_LOW)
//    {
//        if(Send_OxyLow == 0)
//        {
//            UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Oxy_LOW!\r\n");
////            DCU_Notify_Server(_RESPOND_NOTIFY_OXY_LOW);
//            Log_EventWarnig(OBIS_WARNING_OXY_LOWER, 0x00);
//            Send_OxyLow = 1;
////            Gettick_Notify = HAL_GetTick();
//        }
//        else
//        {
////            if(HAL_GetTick() - Gettick_Notify > FREQ_SEND_NOTIFY * 60000)
////            {
//////                DCU_Notify_Server(_RESPOND_NOTIFY_OXY_LOW);
////                Log_EventWarnig(OBIS_WARNING_OXY_LOWER, 0x00);
////                Gettick_Notify = HAL_GetTick();
////            }
//        }
//        Send_Power = 0;
//        Send_Sensor = 0;
//    }
//    else
//    {
//        Send_Power = 0;
//        Send_Sensor = 0;
//        Send_OxyLow = 0;
//    }
//    
////    fevent_enable(sEventAppCtrlOxy, event);
//    return 1;
//}

static uint8_t fevent_ctrl_oxy_reset_dcu(uint8_t event)
{
    RTC_TimeTypeDef sTime_Reset;
    RTC_DateTypeDef sDate_Reset;
    
    HAL_RTC_GetTime(&hrtc, &sTime_Reset, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate_Reset, RTC_FORMAT_BIN);
    
    if(sDate_Reset.Year != 0)
    {
        if(Flag_Reset_DCU == 0)
        {
            if(sDate_Reset.Date%2 == 0)
            {
                if(sParamMeasure.Oxy_Mg_L > sParamCtrlOxy.Oxy_Lower_Warning)
                {
                    Save_Array(ADDR_FLAG_RESET_DCU, (uint8_t*)0x01, 1);
                    HAL_Delay(1000);
                    Reset_Chip();
                }
            }
        }
        else
        {
            if(sDate_Reset.Date%2 != 0)
            {
                OnchipFlashPageErase(ADDR_FLAG_RESET_DCU);
                Flag_Reset_DCU = 0;
            }
        }
    }

    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}

static uint8_t fevent_ctrl_oxy_sw_man_auto(uint8_t event)
{
    static uint8_t stateMachineBefore = _MACHINE_OFF;
    
    if(HAL_GPIO_ReadPin(MCU_SW_AUTO_GPIO_Port, MCU_SW_AUTO_Pin) == 0)
    {
        sStateCtrlOxy.StateMachine = _MACHINE_AUTO;
        Handle_Machine_Auto();
    }
    else if(HAL_GPIO_ReadPin(MCU_SW_MAN_GPIO_Port, MCU_SW_MAN_Pin) == 0)
    {
        sStateCtrlOxy.StateMachine = _MACHINE_MAN;
        Handle_Machine_Manual();
    }
    else
    {
        sStateCtrlOxy.StateMachine = _MACHINE_OFF;
        Handle_Machine_Manual();
    }
    
    if(sStateCtrlOxy.StateMachine != stateMachineBefore)
    {
        if(sStateCtrlOxy.StateMachine == _MACHINE_MAN || sStateCtrlOxy.StateMachine == _MACHINE_OFF)
        {
            DeInit_PeriodTimer(); 
        }
        #ifdef USING_APP_SIM  
            sMQTT.aMARK_MESS_PENDING[DATA_HANDSHAKE] = TRUE;
        #endif
    }
    
    stateMachineBefore = sStateCtrlOxy.StateMachine;
    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}
/*========================Function Handle Control Oxy====================*/
void Handle_Machine_Auto(void)
{
//    if(qGet_Number_Items(&qCtrlOxyRun) == 0)
//    {
//        QueueFree_To_QueueRun();
//        sGettickMs.Oxy_Delay = sGettickMs.GettickMs;
//        sGettickMs.Oxy_Change = sGettickMs.GettickMs;
//    }
    
    if(sStateCtrlOxy.StateDCU == 1)
    {
        if(sParamMeasure.Oxy_Mg_L < sParamCtrlOxy.Oxy_Lower)
        {
            if((sGettickMs.GettickMs - sGettickMs.Oxy_Delay > sParamCtrlOxy.TimeDelay * 60000) || qGet_Number_Items(&qCtrlOxyRun) == 0)
            {
                if(QueueFree_To_QueueRun() == 1)
                    sGettickMs.Oxy_Delay = sGettickMs.GettickMs;
            }
            sGettickMs.Oxy_Change = sGettickMs.GettickMs;
        }
        else if(sParamMeasure.Oxy_Mg_L > sParamCtrlOxy.Oxy_Upper)
        {
            if(sGettickMs.GettickMs - sGettickMs.Oxy_Delay > sParamCtrlOxy.TimeDelay * 60000)
            {
                if(QueueRun_To_QueueFree() == 1)
                    sGettickMs.Oxy_Delay = sGettickMs.GettickMs;
            }
            sGettickMs.Oxy_Change = sGettickMs.GettickMs;
          
          
//            if(qGet_Number_Items(&qCtrlOxyRun) != 1)
//            {
//                if(sGettickMs.GettickMs - sGettickMs.Oxy_Delay > sParamCtrlOxy.TimeDelay * 60000)
//                {
//                    QueueRun_To_QueueFree();
//                    sGettickMs.Oxy_Delay = sGettickMs.GettickMs;
//                }
//                sGettickMs.Oxy_Change = sGettickMs.GettickMs;
//            }
//            else
//            {
//                if(sGettickMs.GettickMs - sGettickMs.Oxy_Change > sParamCtrlOxy.TimeChange * 60000)
//                {
//                    QueueRun_Change_QueueFree();
//                    sGettickMs.Oxy_Change = sGettickMs.GettickMs;
//                }
//            }
        }
        else
        {
            if(sGettickMs.GettickMs - sGettickMs.Oxy_Change > sParamCtrlOxy.TimeChange * 60000)
            {
                QueueRun_Change_QueueFree();
                sGettickMs.Oxy_Change = sGettickMs.GettickMs;
            }
        }
    }
}

void Handle_Machine_Manual(void)
{
    while(qGet_Number_Items(&qCtrlOxyRun) > 0)
    {
        uint8_t Read = 0;
        qQueue_Receive(&qCtrlOxyRun ,&Read, 1);
        qQueue_Send(&qCtrlOxyFree, (uint8_t*)&Read, _TYPE_SEND_TO_END);
        Control_Oxy(Read, _OFF_OXY);
    }
}

void Control_Oxy(uint8_t KindOxy, uint8_t State)
{
    uint8_t StateRelay = State;
    switch(KindOxy)
    {
        case _OXY_1:
          ControlRelay(RELAY_1, StateRelay, _RL_DEBUG);
          break;
          
        case _OXY_2:
          ControlRelay(RELAY_2, StateRelay, _RL_DEBUG);
          break;
          
        case _OXY_3:
          ControlRelay(RELAY_3, StateRelay, _RL_DEBUG);
          break;
          
        case _OXY_4:
          ControlRelay(RELAY_4, StateRelay, _RL_DEBUG);
          break;
          
        default:
          break;
    }
    sStateCtrlOxy.aStateOxy[KindOxy] = StateRelay;
}

void OnOff_Warning(uint8_t State)
{
    if(sStatusRelay.RL5 != State)
    ControlRelay(RELAY_5, State, _RL_DEBUG);
}
/*===================================================================*/

//void Ctrl_Oxy1(void)
//{
//    static uint8_t StateBefore = 0;
//    switch(sStateCtrlOxy.StateOxy1)
//    {
//    case _OFF_OXY:
//      sTimeCtrlOxy.RunOxy1 = 0;
//      sTimeCtrlOxy.FreeOxy1 = 0;
//      if(sStatusRelay.RL1 == ON_RELAY)    ControlRelay(RELAY_1, OFF_RELAY, _RL_DEBUG);
//      if(sStatusRelay.RL2 == ON_RELAY)    ControlRelay(RELAY_2, OFF_RELAY, _RL_DEBUG);
//      break;
//      
//    case _OXY_FREE:
//      sTimeCtrlOxy.FreeOxy1++;
//      if(sTimeCtrlOxy.FreeOxy1 >= sTimeCtrlOxy.FreeCtrl)     
//      {
//        if(StateBefore == _OXY_L_RUN)
//        {
//            sStateCtrlOxy.StateOxy1 = _OXY_R_RUN;
//            ControlRelay(RELAY_1, ON_RELAY, _RL_DEBUG);
//        }
//        else
//        {
//            sStateCtrlOxy.StateOxy1 = _OXY_L_RUN;
//            ControlRelay(RELAY_2, ON_RELAY, _RL_DEBUG);
//        }
//        StateBefore  = sStateCtrlOxy.StateOxy1;
//        sTimeCtrlOxy.FreeOxy1 = 0;
//      }
//      break;
//      
//      default:
//      sTimeCtrlOxy.RunOxy1++;
//      if(sTimeCtrlOxy.RunOxy1 >= sTimeCtrlOxy.RunCtrl)     
//      {
//        sStateCtrlOxy.StateOxy1 = _OXY_FREE;
//        if(sStatusRelay.RL1 == ON_RELAY)    ControlRelay(RELAY_1, OFF_RELAY, _RL_DEBUG);
//        if(sStatusRelay.RL2 == ON_RELAY)    ControlRelay(RELAY_2, OFF_RELAY, _RL_DEBUG);
//        sTimeCtrlOxy.RunOxy1 = 0;
//      }
//      break;
//    }
//}

/*===========================User queue control oxy ======================*/
uint8_t QueueRun_Change_QueueFree(void)
{
    uint8_t LengthFree = 0;
    uint8_t Result = 0;
    LengthFree = qGet_Number_Items(&qCtrlOxyFree);
    if(LengthFree > NUMBER_OXY/2)
    {
        LengthFree = NUMBER_OXY - LengthFree;
        Result = LengthFree;
    }
    
    while(LengthFree > 0)
    {
//        QueueFree_To_QueueRun();
//        QueueRun_To_QueueFree();
        LengthFree--;
        
        if(qGet_Number_Items(&qCtrlOxyFree) > 0)
        {
            uint8_t Read = 0;
            qQueue_Receive(&qCtrlOxyFree ,&Read, 1);
            qQueue_Send(&qCtrlOxyRun, (uint8_t*)&Read, _TYPE_SEND_TO_END);
            Control_Oxy(Read, _ON_OXY);
        }
        
        if(qGet_Number_Items(&qCtrlOxyRun) > 0)
        {
            uint8_t Read = 0;
            qQueue_Receive(&qCtrlOxyRun ,&Read, 1);
            qQueue_Send(&qCtrlOxyFree, (uint8_t*)&Read, _TYPE_SEND_TO_END);
            Control_Oxy(Read, _OFF_OXY);
        }
    }
    if(Result > 0)
    {
//        DeInit_PeriodTimer();    
        return 1;
    }
    return 0;
}

uint8_t QueueFree_To_QueueRun(void)
{
    if(qGet_Number_Items(&qCtrlOxyFree) > 0)
    {
        uint8_t Read = 0;
        qQueue_Receive(&qCtrlOxyFree ,&Read, 1);
        qQueue_Send(&qCtrlOxyRun, (uint8_t*)&Read, _TYPE_SEND_TO_END);
        Control_Oxy(Read, _ON_OXY);
        if(qGet_Number_Items(&qCtrlOxyFree) == NUMBER_OXY - 1)
        {
            DeInit_PeriodTimer();   
        }
        return 1;
    }
    return 0;
}

uint8_t QueueRun_To_QueueFree(void)
{
    if(qGet_Number_Items(&qCtrlOxyRun) > 0)
    {
        uint8_t Read = 0;
        qQueue_Receive(&qCtrlOxyRun ,&Read, 1);
        qQueue_Send(&qCtrlOxyFree, (uint8_t*)&Read, _TYPE_SEND_TO_END);
        Control_Oxy(Read, _OFF_OXY);
        if(qGet_Number_Items(&qCtrlOxyRun) == 0)
        {
            DeInit_PeriodTimer();    
        }
        return 1;
    }
    return 0;
}

/*======================== Function Handle ========================*/

/*
    @brief  Log Data TSVH
*/
void AppCtrlOxy_Log_Data_TSVH (void)
{
  if(sRTC.year > 20)
  {
    uint8_t     aMessData[256] = {0};
    uint8_t     Length = 0;
    
//    if (sRTC.year <= 20)
//        return;
    
    Length = AppCtrlOxy_Packet_TSVH (&aMessData[0]);
#ifdef USING_APP_MEM
    if(sStateCtrlOxy.StateSendOpera == 1)
        AppMem_Write_Data(_MEM_DATA_TSVH_A, &aMessData[0], Length, sRecTSVH.SizeRecord_u16);
#endif
  }
}

/*
    @brief  Packet TSVH
*/
uint8_t AppCtrlOxy_Packet_TSVH (uint8_t *pData)
{
    uint16_t    length = 0;
    uint16_t	i = 0;
    uint8_t     TempCrc = 0;  
    
    //----------sTime--------------------
    AppComm_SV_Protocol_Packet_Data(pData, &length, OBIS_TIME_DEVICE, &sRTC, 6, 0xAA);
    
    if(sStateCtrlOxy.StateSensorOxy == _OXY_CONNECT)
    {
        //----------  Temperature ------
        AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_OXY_TEMPERATURE, &sParamMeasure.Temp, 2, 0xFE);
    }
    
    if(sParamCtrlPH.StateSensorPH == _OXY_CONNECT)
    {
        //----------  pH_Water ------
        AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_PH_WATER, &sParamCtrlPH.pH_Measure, 2, 0xFE);
    }
    
    if(sStateCtrlOxy.StateSensorOxy == _OXY_CONNECT)
    {
        //----------  Oxy mg/L ---------
        AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_OXY_MG_L, &sParamMeasure.Oxy_Mg_L, 2, 0xFE);
        
        //----------  Salinity ------
        AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_SALINITY, &sParamMeasure.Salinity, 2, 0xFE);

        //----------  Oxy Percent ------
        AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_OXY_PERCENT, &sParamMeasure.Oxy_Percent, 2, 0xFE);
    }
    
    //---------- Oxy State ---------
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_STATE_MACHINE, &sStateCtrlOxy.StateMachine, 1, 0xAA);
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_STATE_OXY_1, &sStateCtrlOxy.aStateOxy[0], 1, 0xAA);
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_STATE_OXY_2, &sStateCtrlOxy.aStateOxy[1], 1, 0xAA);
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_STATE_OXY_3, &sStateCtrlOxy.aStateOxy[2], 1, 0xAA);
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_STATE_OXY_4, &sStateCtrlOxy.aStateOxy[3], 1, 0xAA);
    
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_STATE_PUMP_PH, &sParamCtrlPH.StateCtrl, 1, 0xAA);
       
    //----------Tan suat--------------------
    AppComm_SV_Protocol_Packet_Data(pData, &length, OBIS_RSSI_1, &sSimInfor.RSSI_u8, 1, 0x00);
    AppComm_SV_Protocol_Packet_Data(pData, &length, OBIS_FREQ_SEND, &sFreqInfor.FreqSendUnitMin_u32, 2, 0x00);
    // caculator crc
    length++;
	for (i = 0; i < (length - 1); i++)
		TempCrc ^= pData[i];

    pData[length-1] = TempCrc;
    
    return length;
}

/*
    @brief  Packet Data
*/
void AppCtrlOxy_Packet_Data (uint8_t *pTarget, uint16_t *LenTarget, uint8_t Obis, 
                             void *pData, uint8_t LenData, uint8_t Scale)
{
    uint16_t Pos = *LenTarget;
//    uint16_t i= 0;
//    uint8_t *TempU8;
    
    if (LenData == 0)
        return;
    
    pTarget[Pos++] = Obis;
    pTarget[Pos++] = LenData;
            
    switch (Obis)
    {
        case OBIS_ENVI_STATE_MACHINE:
        case OBIS_ENVI_OXY_MG_L:
        case OBIS_ENVI_OXY_PERCENT:
        case OBIS_ENVI_OXY_TEMPERATURE:
        case OBIS_ENVI_SALINITY:
//        case OBIS_ENVI_ID_SERVER:
        case OBIS_ENVI_PH_WATER:
        case OBIS_STATE_OXY_1:
        case OBIS_STATE_OXY_2:
        case OBIS_STATE_OXY_3:
        case OBIS_STATE_OXY_4:
        case OBIS_STATE_PUMP_PH:
            AppComm_Sub_Packet_Integer(pTarget, &Pos, pData, LenData);
            //
            if (Scale != 0xAA)
                pTarget[Pos++] = Scale;
            break;
            
//        case OBIS_ENVI_STATE_OXY:
//            TempU8 = (uint8_t *) pData;
//            for (i = 0; i < LenData; i++)
//                pTarget[Pos++] = *(TempU8 + i);
//            break; 
            
        default:
            break;
    }
    
    *LenTarget = Pos ;
}


/*
    @brief  Compensation Salinity
*/
void Compensation_Salinity(uint16_t *Oxy_Concentration, uint16_t Sal_x100, uint16_t Temp_x100)
{
	// Calcutate the oxygen concentration in 4500- OXYGEN(DISSOLVED), pape 4/8
	// Chage  S = 1.80655*Chorinity
		float tam_Sal =0;
        float temp = 0;
        float Factor = 0; 
		temp = (float)Temp_x100/100 + (float)273.15;
		tam_Sal =  -(Sal_x100/100)*(0.017674+(-10.754+ 2140.7/temp)/temp);
//		Factor =(float)pow((float)2.718282,tam_Sal);
        Factor = (float)exp(tam_Sal);
		*Oxy_Concentration = (uint16_t)(*Oxy_Concentration * (Factor));
}

/*
    @brief  Respond ID Server
*/
//void DCU_Respond_ID_Server(uint32_t ID_Server)
//{
//    uint8_t     aData[50]={0};
//    uint16_t    length = 0;
//    uint16_t	i = 0;
//    uint8_t     TempCrc = 0;  
//    
//    //----------sTime--------------------
//    AppComm_SV_Protocol_Packet_Data(aData, &length, OBIS_TIME_DEVICE, &sRTC, 6, 0xAA);
//    
//    //---------- Kind Warning ------------
//    AppCtrlOxy_Packet_Data(aData, &length, OBIS_ENVI_ID_SERVER, &ID_Server, 4, 0xAA);
//    // caculator crc
//    length++;
//	for (i = 0; i < (length - 1); i++)
//		TempCrc ^= aData[i];
//
//    aData[length-1] = TempCrc;
//    
//    DCU_Respond (_AT_REQUEST_SERVER, aData, length, 1);
//}

/*======================= Function Handle ====================*/

uint8_t Calculator_Addr_Read_Write_EEPROM(uint16_t CountRec, uint16_t *AddrRec)
{
    uint32_t Result = 0;
    uint32_t AddrHandle = 0;
    uint32_t PageHandle = 0;
    
    if(CountRec == 0) return 0;
    
    AddrHandle = sOxyLogEEPROM.Addr_Oxy_Log_Start;
    
    PageHandle = (sOxyLogEEPROM.Addr_Oxy_Log_Start/sOxyLogEEPROM.SizePage + 1)*sOxyLogEEPROM.SizePage;

    while(1)
    {
        if(PageHandle > sOxyLogEEPROM.Addr_Oxy_Log_End)
        {
            PageHandle = sOxyLogEEPROM.Addr_Oxy_Log_End;
            if((PageHandle - AddrHandle)/sOxyLogEEPROM.SizeRecord >= CountRec)
            {
                Result = AddrHandle + (CountRec-1)*sOxyLogEEPROM.SizeRecord;
                *AddrRec = Result;
                return 1;
            }
            else
                return 0;
        }
        if((PageHandle - AddrHandle)/sOxyLogEEPROM.SizeRecord >= CountRec)
        {
            Result = AddrHandle + (CountRec-1)*sOxyLogEEPROM.SizeRecord;
            *AddrRec = Result;
            return 1;
        }
        else
        {
            CountRec -= (PageHandle - AddrHandle)/sOxyLogEEPROM.SizeRecord;
            AddrHandle = PageHandle;
            PageHandle += sOxyLogEEPROM.SizePage;
        }
    }
}

void Log_EventWarnig(uint8_t Obis, uint8_t State)
{
  Get_RTC();
  
  if(sRTC.year > 20)
  {
    uint8_t     aData[10]={0};
    uint16_t    length = 0;
    uint16_t	i = 0;
    uint8_t     TempCrc = 0;
    
    AppComm_SV_Protocol_Packet_Data(aData, &length, OBIS_TIME_DEVICE, &sRTC, 6, 0xAA);
    
    aData[length++] = Obis;
    aData[length++] = 0x01;
    aData[length++] = State;
    
    // caculator crc
    length++;
	for (i = 0; i < (length - 1); i++)
		TempCrc ^= aData[i];

    aData[length-1] = TempCrc;
  
#ifdef USING_APP_MEM
    AppMem_Write_Data(_MEM_DATA_EVENT_A, &aData[0], length, sRecEvent.SizeRecord_u16);
#endif
  }
}


/*
    Func: Resp to user
        + PortNo: serial or server
        + Type: Server ACK (1) or Server Not ACK  (0)
*/
void DCU_Respond_Debug(uint8_t *data, uint16_t length)
{
    uint16_t i = 0;
    
    UTIL_MEM_set (aRESPONDSE_AT, 0, sizeof ( aRESPONDSE_AT ));
    sModem.strATResp.Length_u16 = 0;
    //Dong goi Cmd
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = '(';
    
    for (i = 0; i < sModem.strATCmd.Length_u16; i++)
        aRESPONDSE_AT[sModem.strATResp.Length_u16++] = *(sModem.strATCmd.Data_a8 + i);
    
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = ')';
    //Dong goi phan hoi
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = '(';
//    aTEMP[Count++] = sRTC.year/10 + 0x30;
//    aTEMP[Count++] = sRTC.year%10 + 0x30;
//    aTEMP[Count++] = '/';
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = sRTC.month/10 + 0x30;
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = sRTC.month%10 + 0x30;
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = '/';
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = sRTC.date/10 + 0x30;
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = sRTC.date%10 + 0x30;
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = ' ';
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = sRTC.hour/10 + 0x30;
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = sRTC.hour%10 + 0x30;
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = ':';
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = sRTC.min/10 + 0x30;
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = sRTC.min%10 + 0x30;
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = ':';
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = sRTC.sec/10 + 0x30;
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = sRTC.sec%10 + 0x30;

    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = '.';   //9 byte: 64 - (3 + 15 + 1 + 1)- 4 = 40

    for (i = 0; i < length; i++)
        aRESPONDSE_AT[sModem.strATResp.Length_u16++] = *(data + i);
    
    aRESPONDSE_AT[sModem.strATResp.Length_u16++] = ')';
    
    sModem.strATCmd.Length_u16 = 0;  //Reset buff AT cmd
    
    #ifdef USING_APP_SIM  
        sMQTT.aMARK_MESS_PENDING[DATA_FLASH_MEM] = TRUE;
    #endif
}

/*------------------Init Queue Control Oxy-------------------*/
void Init_QueueCtrlOxy(void)
{
    qQueue_Create(&qCtrlOxyRun, 10, sizeof(uint8_t),(uint8_t*)&qReadRun);
    qQueue_Create(&qCtrlOxyFree, 10, sizeof(uint8_t),(uint8_t*)&qReadFree);
    Init_StateOxy();
}

/*-------------------------Save and Init State oxy-----------------------*/
void Save_StateOxy(void)
{
    if(sStateCtrlOxy.StateMachine == _MACHINE_AUTO)
    {
        uint8_t aData[8] = {0};
        uint8_t length = 0;
        
        for(uint8_t i = 0; i < _OXY_END; i++)
        {
            aData[length++] = sStateCtrlOxy.aStateOxy[i];
        }

        Save_Array(ADDR_INIT_STATE_MACHINE, aData, length);  
    }
    
    //Tat Sensor Oxy truoc khi reset
    HAL_GPIO_WritePin(SENSOR_GPIO_Port, SENSOR_Pin, GPIO_PIN_RESET);
    HAL_Delay(3000);
}
void Init_StateOxy(void)
{
    if(*(__IO uint8_t*)(ADDR_INIT_STATE_MACHINE) == BYTE_TEMP_FIRST)
    {
        for(uint8_t i = 0; i < _OXY_END; i++)
        {
            sStateCtrlOxy.aStateOxy[i] = *(__IO uint8_t*)(ADDR_INIT_STATE_MACHINE+2+i);
        }
        OnchipFlashPageErase(ADDR_INIT_STATE_MACHINE);
    }

    
    for(uint8_t i = 0; i < _OXY_END; i++)
    {
        if(sStateCtrlOxy.aStateOxy[i] == _OFF_OXY)
        {
            qQueue_Send(&qCtrlOxyFree, (uint8_t*)&i, _TYPE_SEND_TO_END);
            Control_Oxy(i, _OFF_OXY);
        }
        else
        {
          
            qQueue_Send(&qCtrlOxyRun, (uint8_t*)&i, _TYPE_SEND_TO_HEAD);
            Control_Oxy(i, _ON_OXY);
        }
    }

    //Bat sensor Oxy sau khoi dong
    HAL_GPIO_WritePin(SENSOR_GPIO_Port, SENSOR_Pin, GPIO_PIN_SET);
}

/*-----------------------Init Flag DCU------------------------*/
void Init_FlagDCU(void)
{
    if(*(__IO uint8_t*)(ADDR_FLAG_RESET_DCU) == BYTE_TEMP_FIRST)
        Flag_Reset_DCU = 0x01;
    else
        Flag_Reset_DCU = 0x00;
}

/*----------------Save and Init Salinity-----------------*/
void        Save_Salinity(uint16_t Salinity)
{
    uint8_t aData[2] = {0};
    uint8_t length = 0;
    sParamMeasure.Salinity = Salinity;
    for(uint8_t i = 0; i < _OXY_END; i++)
    {
        aData[length++] = Salinity>>8;
        aData[length++] = Salinity;
    }

    Save_Array(ADDR_SAVE_SALINITY, aData, length);  
    Stamp_Menu_Exit();
}

void        Init_Salinity(void)
{
    if(*(__IO uint8_t*)(ADDR_SAVE_SALINITY) == BYTE_TEMP_FIRST)
    {
        sParamMeasure.Salinity = *(__IO uint8_t*)(ADDR_SAVE_SALINITY+2) <<8;
        sParamMeasure.Salinity |= *(__IO uint8_t*)(ADDR_SAVE_SALINITY+3);
    }
    else
    {
        sParamMeasure.Salinity = SALINITY_DEFAULT;
    }
}

/*-----------------Save and Init GetTick Control-----------------*/
void Save_GetTickMs(void)
{
    uint8_t aData[20] = {0};
    uint8_t length = 0;
    
    aData[length++] = sGettickMs.GettickMs >> 24;
    aData[length++] = sGettickMs.GettickMs >> 16;
    aData[length++] = sGettickMs.GettickMs >> 8;
    aData[length++] = sGettickMs.GettickMs;
    
    aData[length++] = sGettickMs.Oxy_Change >> 24;
    aData[length++] = sGettickMs.Oxy_Change >> 16;
    aData[length++] = sGettickMs.Oxy_Change >> 8;
    aData[length++] = sGettickMs.Oxy_Change;
    
    aData[length++] = sGettickMs.Oxy_Delay >> 24;
    aData[length++] = sGettickMs.Oxy_Delay >> 16;
    aData[length++] = sGettickMs.Oxy_Delay >> 8;
    aData[length++] = sGettickMs.Oxy_Delay;

    Save_Array(ADDR_OXY_GETTICK, aData, length);
    Stamp_Menu_Exit();
}

void Init_GetTickMs(void)
{
    if(*(__IO uint8_t*)(ADDR_OXY_GETTICK) == BYTE_TEMP_FIRST)
    {
        sGettickMs.GettickMs = *(__IO uint8_t*)(ADDR_OXY_GETTICK+2) <<24;
        sGettickMs.GettickMs |= *(__IO uint8_t*)(ADDR_OXY_GETTICK+3) <<16;
        sGettickMs.GettickMs |= *(__IO uint8_t*)(ADDR_OXY_GETTICK+4) <<8;
        sGettickMs.GettickMs |= *(__IO uint8_t*)(ADDR_OXY_GETTICK+5) ;
        
        sGettickMs.Oxy_Change = *(__IO uint8_t*)(ADDR_OXY_GETTICK+6) <<24;
        sGettickMs.Oxy_Change |= *(__IO uint8_t*)(ADDR_OXY_GETTICK+7) <<16;
        sGettickMs.Oxy_Change |= *(__IO uint8_t*)(ADDR_OXY_GETTICK+8) <<8;
        sGettickMs.Oxy_Change |= *(__IO uint8_t*)(ADDR_OXY_GETTICK+9) ;
        
        sGettickMs.Oxy_Delay = *(__IO uint8_t*)(ADDR_OXY_GETTICK+10) <<24;
        sGettickMs.Oxy_Delay |= *(__IO uint8_t*)(ADDR_OXY_GETTICK+11) <<16;
        sGettickMs.Oxy_Delay |= *(__IO uint8_t*)(ADDR_OXY_GETTICK+12) <<8;
        sGettickMs.Oxy_Delay |= *(__IO uint8_t*)(ADDR_OXY_GETTICK+13) ;
        

        OnchipFlashPageErase(ADDR_OXY_GETTICK);
    }
    else
    {
        sGettickMs.GettickMs = 0;
        sGettickMs.Oxy_Change = 0;
        sGettickMs.Oxy_Delay = 0;
    }
}
/*-----------------------Save and Init time slave--------------------*/
void Save_TimeSlave(uint16_t RunCtrl, uint16_t FreeCtrl)
{
    uint8_t aData[4] = {0};
    uint8_t length = 0;
    
    sTimeSlave.RunCtrl = RunCtrl;
    sTimeSlave.FreeCtrl = FreeCtrl;
    aData[length++] = sTimeSlave.RunCtrl >> 8;
    aData[length++] = sTimeSlave.RunCtrl;
    
    aData[length++] = sTimeSlave.FreeCtrl >> 8;
    aData[length++] = sTimeSlave.FreeCtrl;
    Save_Array(ADDR_CTRL_OXY_RUN_FREE, aData, length);
    Stamp_Menu_Exit();
}

void Init_TimeSlave(void)
{
    if(*(__IO uint8_t*)(ADDR_CTRL_OXY_RUN_FREE) == BYTE_TEMP_FIRST)
    {
        sTimeSlave.FreeCtrl  = *(__IO uint8_t*)(ADDR_CTRL_OXY_RUN_FREE+4) << 8 ;
        sTimeSlave.FreeCtrl |= *(__IO uint8_t*)(ADDR_CTRL_OXY_RUN_FREE+5);
        
        sTimeSlave.RunCtrl  = *(__IO uint8_t*)(ADDR_CTRL_OXY_RUN_FREE+2) << 8;
        sTimeSlave.RunCtrl |= *(__IO uint8_t*)(ADDR_CTRL_OXY_RUN_FREE+3);
        
        if(sTimeSlave.FreeCtrl < SLAVE_485_TIME_MIN)
           sTimeSlave.FreeCtrl = FREE_OXY_DEFAULT;
        
        if(sTimeSlave.RunCtrl > SLAVE_485_TIME_MAX)
           sTimeSlave.RunCtrl = RUN_OXY_DEFAULT;
    }
    else
    {
        sTimeSlave.FreeCtrl = FREE_OXY_DEFAULT;
        sTimeSlave.RunCtrl = RUN_OXY_DEFAULT;
    }
    Stamp_Menu_Exit();
}
/*-----------------Save and Init oxy upper and lower------------------*/

void        Save_OxyUpperLower(uint16_t Upper, uint16_t Lower, uint16_t Upper_Warning, uint16_t Lower_Warning)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamCtrlOxy.Oxy_Upper = Upper;
    sParamCtrlOxy.Oxy_Lower = Lower;
    sParamCtrlOxy.Oxy_Upper_Warning = Upper_Warning;
    sParamCtrlOxy.Oxy_Lower_Warning = Lower_Warning;
    
    aData[length++] = sParamCtrlOxy.Oxy_Upper >> 8;
    aData[length++] = sParamCtrlOxy.Oxy_Upper;
    
    aData[length++] = sParamCtrlOxy.Oxy_Lower >> 8;
    aData[length++] = sParamCtrlOxy.Oxy_Lower;
    
    aData[length++] = sParamCtrlOxy.Oxy_Upper_Warning >> 8;
    aData[length++] = sParamCtrlOxy.Oxy_Upper_Warning;
    
    aData[length++] = sParamCtrlOxy.Oxy_Lower_Warning >> 8;
    aData[length++] = sParamCtrlOxy.Oxy_Lower_Warning;

    Save_Array(ADDR_OXY_THRESH_SETTING, aData, length);
    Stamp_Menu_Exit();
}

void        Init_OxyUpperLower(void)
{
    if(*(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING) == BYTE_TEMP_FIRST)
    {
        sParamCtrlOxy.Oxy_Upper = *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+2) <<8;
        sParamCtrlOxy.Oxy_Upper |= *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+3);
        
        sParamCtrlOxy.Oxy_Lower  = *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+4) <<8;
        sParamCtrlOxy.Oxy_Lower |= *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+5);
        
        sParamCtrlOxy.Oxy_Upper_Warning = *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+6) <<8;
        sParamCtrlOxy.Oxy_Upper_Warning |= *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+7);
        
        sParamCtrlOxy.Oxy_Lower_Warning = *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+8) <<8;
        sParamCtrlOxy.Oxy_Lower_Warning |= *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+9);
    }
    else
    {
        sParamCtrlOxy.Oxy_Lower = OXY_LOWER_DEFAULT;
        sParamCtrlOxy.Oxy_Upper = OXY_UPPER_DEFAULT;
        sParamCtrlOxy.Oxy_Upper_Warning = OXY_UPPER_W_DEFAULT;
        sParamCtrlOxy.Oxy_Lower_Warning = OXY_LOWER_W_DEFAULT;
    }
    Stamp_Menu_Exit();
}

/*------------------Save and Init time control oxy--------------------*/
void        Save_TimeCtrlOxy(uint16_t TimeDelay, uint16_t TimeChange)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamCtrlOxy.TimeDelay = TimeDelay;
    sParamCtrlOxy.TimeChange = TimeChange;
    
    aData[length++] = sParamCtrlOxy.TimeDelay >> 8;
    aData[length++] = sParamCtrlOxy.TimeDelay;
    
    aData[length++] = sParamCtrlOxy.TimeChange >> 8;
    aData[length++] = sParamCtrlOxy.TimeChange;

    Save_Array(ADDR_TIME_CONTROL_SETTING, aData, length);
    Stamp_Menu_Exit();
}

void        Init_TimeCtrlOxy(void)
{
    if(*(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING) == BYTE_TEMP_FIRST)
    {
        sParamCtrlOxy.TimeDelay  = *(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING+2) <<8;
        sParamCtrlOxy.TimeDelay |= *(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING+3);
        
        sParamCtrlOxy.TimeChange  = *(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING+4) <<8;
        sParamCtrlOxy.TimeChange |= *(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING+5);
        
    }
    else
    {
        sParamCtrlOxy.TimeDelay = TIMEDELAY_DEFAULT;
        sParamCtrlOxy.TimeChange = TIMECHANGE_DEFAULT;
    }
    Stamp_Menu_Exit();
}
/*=======================Handle Task and Init app========================*/
void Init_AppCtrlOxy(void)
{
    Init_TimeSlave();
    Init_TimeCtrlOxy();
    Init_OxyUpperLower();
    Init_QueueCtrlOxy();
    Init_FlagDCU();
    Init_GetTickMs();
    Init_Salinity();
    Init_pH_UpperLowerWar();
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