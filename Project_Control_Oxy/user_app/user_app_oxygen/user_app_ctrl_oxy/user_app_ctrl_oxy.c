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
static uint8_t fevent_ctrl_oxy_gettick(uint8_t event);
static uint8_t fevent_ctrl_oxy_iwdg(uint8_t event);
static uint8_t fevent_send_slave_cycle(uint8_t event);
static uint8_t fevent_ctrl_oxy_wait_calib(uint8_t event);
static uint8_t fevent_ctrl_oxy_sw_man_auto(uint8_t event);
static uint8_t fevent_ctrl_oxy_log_tsvh(uint8_t event);
static uint8_t fevent_ctrl_oxy_pw_detect(uint8_t event);
static uint8_t fevent_ctrl_oxy_state_slave(uint8_t event);
static uint8_t fevent_ctrl_oxy_warning(uint8_t event);
/*========================Struct========================*/
sEvent_struct               sEventAppCtrlOxy[]=
{
  {_EVENT_CTRL_OXY_ENTRY,       1, 5, 10000,        fevent_ctrl_oxy_entry},
  {_EVENT_CTRL_OXY_GETTICK,     0, 5, 0,            fevent_ctrl_oxy_gettick},
  {_EVENT_CTRL_OXY_IWDG,        1, 5, 2000,         fevent_ctrl_oxy_iwdg},
  
  {_EVENT_SEND_SLAVE_CYCLE,     0, 5, 1000,         fevent_send_slave_cycle},
  
  {_EVENT_CTRL_OXY_WAIT_CALIB,  0, 5, 500,          fevent_ctrl_oxy_wait_calib},
  
  {_EVENT_CTRL_OXY_SW_MAN_AUTO, 1, 5, 500,          fevent_ctrl_oxy_sw_man_auto},
  
  {_EVENT_CTRL_OXY_LOG_TSVH,    0, 5, 10000,        fevent_ctrl_oxy_log_tsvh},
  
  {_EVENT_CTRL_OXY_PW_DETECT,   1, 5, 500,          fevent_ctrl_oxy_pw_detect},
  
  {_EVENT_CTRL_OXY_STATE_SLAVE, 0, 5, 500,          fevent_ctrl_oxy_state_slave},
  
  {_EVENT_CTRL_OXY_WARNING,     0, 5, 5,            fevent_ctrl_oxy_warning},
};
uint8_t     aStateOxy[5]={0};

Struct_Param_Measure            sParamMeasure= {0};
Struct_Password                 sPassword    = {0};

Struct_Time_Ctrl_Oxy            sTimeCtrlOxy = {0};
Struct_State_Ctrl_Oxy           sStateCtrlOxy= 
{
  .StatePower = _POWER_ON,
  .aStateOxy = aStateOxy,
};

Struct_Param_Ctrl_Oxy           sParamCtrlOxy= {OXY_UPPER_DEFAULT, OXY_LOWER_DEFAULT, TIMEDELAY_DEFAULT, TIMECHANGE_DEFAULT};
Struct_Queue_Type               qCtrlOxyRun;
Struct_Queue_Type               qCtrlOxyFree;
uint8_t                         qReadRun[10];
uint8_t                         qReadFree[10];
/*================= Function Callback Gettick =================*/
__weak void Irq_Second_Callback(void)
{
  
}

/*=======================Function Hanlde=================*/
static uint8_t fevent_ctrl_oxy_entry(uint8_t event)
{
    fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_STATE_SLAVE);
    fevent_active(sEventAppCtrlOxy, _EVENT_CTRL_OXY_WARNING);
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
        aData[Length++] = 'a';
        aData[Length++] = 't';
        aData[Length++] = '+';
        aData[Length++] = 'r';
        aData[Length++] = 'u';
        aData[Length++] = 'n';
        aData[Length++] = '=';
        Convert_Var_Packet_Integer(aData, &Length, sTimeCtrlOxy.RunCtrl);
        stateSend = 1;
        fevent_enable(sEventAppCtrlOxy, event);
    }
    else
    {
        aData[Length++] = 'a';
        aData[Length++] = 't';
        aData[Length++] = '+';
        aData[Length++] = 'f';
        aData[Length++] = 'r';
        aData[Length++] = 'e';
        aData[Length++] = 'e';
        aData[Length++] = '=';
        Convert_Var_Packet_Integer(aData, &Length, sTimeCtrlOxy.FreeCtrl);
        stateSend = 0;
    }
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_SET);
    HAL_Delay(5);
    
    HAL_UART_Transmit(&uart_485, aData, Length, 1000);
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_RESET);
    
    return 1;
}

static uint8_t fevent_ctrl_oxy_wait_calib(uint8_t event)
{
    static uint8_t countstate = 0;
    if(countstate >= 20)
    {
        sMenuState.CalibSensor = _CALIB_ERROR;
        countstate = 0;
        KindTrans485 = _TRANSMIT_OPERA;
        return 1;
    }
    else
    {
        if(sMenuState.CalibSensor == _CALIB_COMPLETE)   
        {
          countstate = 0;
          KindTrans485 = _TRANSMIT_OPERA;
          return 1;
        }
        else    fevent_enable(sEventAppCtrlOxy, event);
    }
    countstate++;
    return 1;
}

static uint8_t fevent_ctrl_oxy_sw_man_auto(uint8_t event)
{
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
    
    fevent_enable(sEventAppCtrlOxy, event);
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

static uint8_t fevent_ctrl_oxy_pw_detect(uint8_t event)
{
    static uint8_t StatePowerBefore = _POWER_ON;
    static uint8_t StatePowerPresent= 0;
    StatePowerPresent = HAL_GPIO_ReadPin(PW_Detect_GPIO_Port, PW_Detect_Pin);
    if(StatePowerPresent != StatePowerBefore)
    {
        sStateCtrlOxy.StatePower = StatePowerPresent;
        if(StatePowerPresent == _POWER_ON)  
        {
          DCU_Notify_Server(_RESPOND_NOTIFY_POWER_ON);
        }
        else                                
        {
          DCU_Notify_Server(_RESPOND_NOTIFY_POWER_OFF);
        }
        AppCtrlOxy_Log_Data_TSVH();
    }
    
    StatePowerBefore = StatePowerPresent;
    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}

static uint8_t fevent_ctrl_oxy_state_slave(uint8_t event)
{
    static uint8_t StateSlaveBefore   = _OXY_CONNECT;
    static uint8_t StatePowerPresent= 0;
    StatePowerPresent = HAL_GPIO_ReadPin(PW_Detect_GPIO_Port, PW_Detect_Pin);
    if(sStateCtrlOxy.StateSensor != StateSlaveBefore)
    {
        if(StatePowerPresent == _POWER_ON)
        {
            if(sStateCtrlOxy.StateSensor == _OXY_DISCONNECT)  
                DCU_Notify_Server(_RESPOND_NOTIFY_SLAVE_DISCONNECT);
            else                                
                DCU_Notify_Server(_RESPOND_NOTIFY_SLAVE_CONNECT);
            AppCtrlOxy_Log_Data_TSVH();
        }
    }
    
    StateSlaveBefore = sStateCtrlOxy.StateSensor;
    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}

static uint8_t fevent_ctrl_oxy_warning(uint8_t event)
{
    static uint32_t Gettick_Warning = 0;
    
    if(sParamMeasure.Oxy_Mg_L < sParamCtrlOxy.Oxy_Lower)
    {
      if(HAL_GetTick() - Gettick_Warning > sParamCtrlOxy.TimeWarning * 60000)
      {
        OnOff_Warning(ON_RELAY);
        DCU_Notify_Server(_RESPOND_NOTIFY_OXY_LOW);
        AppCtrlOxy_Log_Data_TSVH();
        Gettick_Warning = HAL_GetTick();
      }
    }
    else
    {
        Gettick_Warning = HAL_GetTick();
        OnOff_Warning(OFF_RELAY);
    }
  
    fevent_enable(sEventAppCtrlOxy, event);
    return 1;
}

/*========================Function Handle====================*/
void Handle_Machine_Auto(void)
{
    static uint32_t Gettick_Delay = 0;
    static uint32_t Gettick_Change = 0;
    static uint8_t  State_Change = 0;
    
    if(qGet_Number_Items(&qCtrlOxyRun) == 0)
    {
        QueueFree_To_QueueRun();
        Gettick_Delay = HAL_GetTick();
        Gettick_Change = HAL_GetTick();
    }
    
    if(sParamMeasure.Oxy_Mg_L < sParamCtrlOxy.Oxy_Lower)
    {
        if(HAL_GetTick() - Gettick_Delay > sParamCtrlOxy.TimeDelay * 60000)
        {
            QueueFree_To_QueueRun();
            Gettick_Delay = HAL_GetTick();
        }
        State_Change = 0;
    }
    else if(sParamMeasure.Oxy_Mg_L > sParamCtrlOxy.Oxy_Upper)
    {
        while(qGet_Number_Items(&qCtrlOxyRun) != 1)
        {
            QueueRun_To_QueueFree();
        }
        if(State_Change == 2)
        {
            if(HAL_GetTick() - Gettick_Change > sParamCtrlOxy.TimeChange * 60000)
            {
                QueueRun_Change_QueueFree();
                Gettick_Change = HAL_GetTick();
            }
        }
        else
        {
            State_Change = 2;
            Gettick_Change = HAL_GetTick();
        }
    }
    else
    {
        if(sStatusRelay.RL5 == ON_RELAY)
        {
            ControlRelay(RELAY_5, OFF_RELAY, _RL_DEBUG);
        }
        if(State_Change == 1)
        {
            if(HAL_GetTick() - Gettick_Change > sParamCtrlOxy.TimeChange * 60000)
            {
                QueueRun_Change_QueueFree();
                Gettick_Change = HAL_GetTick();
            }
        }
        else
        {
            State_Change = 1;
            Gettick_Change = HAL_GetTick();
        }
    }
}

void Handle_Machine_Manual(void)
{
    while(qGet_Number_Items(&qCtrlOxyRun) > 0)
    {
        QueueRun_To_QueueFree();
    }
}

void Control_Oxy(uint8_t KindOxy, uint8_t State)
{
    uint8_t StateRelay = State;
    switch(KindOxy)
    {
        case _OXY_1:
          ControlRelay(RELAY_1, StateRelay, _RL_DEBUG);
          sStateCtrlOxy.aStateOxy[_OXY_1] = StateRelay;
          break;
          
        case _OXY_2:
          ControlRelay(RELAY_2, StateRelay, _RL_DEBUG);
          sStateCtrlOxy.aStateOxy[_OXY_2] = StateRelay;
          break;
          
        case _OXY_3:
          ControlRelay(RELAY_3, StateRelay, _RL_DEBUG);
          sStateCtrlOxy.aStateOxy[_OXY_3] = StateRelay;
          break;
          
        case _OXY_4:
          ControlRelay(RELAY_4, StateRelay, _RL_DEBUG);
          sStateCtrlOxy.aStateOxy[_OXY_4] = StateRelay;
          break;
          
        default:
          break;
    }
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

/*========================== Function Save Init Param =========================*/

void        Save_OxyUpperLower(uint16_t Upper, uint16_t Lower)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamCtrlOxy.Oxy_Upper = Upper;
    sParamCtrlOxy.Oxy_Lower = Lower;
    aData[length++] = sParamCtrlOxy.Oxy_Upper >> 8;
    aData[length++] = sParamCtrlOxy.Oxy_Upper;
    aData[length++] = sParamCtrlOxy.Oxy_Lower >> 8;
    aData[length++] = sParamCtrlOxy.Oxy_Lower;

    Save_Array(ADDR_OXY_THRESH_SETTING, aData, length);
    Stamp_Menu_Exit();
}

void        Init_OxyUpperLower(void)
{
    if(*(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING) == BYTE_TEMP_FIRST)
    {
        sParamCtrlOxy.Oxy_Lower  = *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+4) ;
        sParamCtrlOxy.Oxy_Lower  = sParamCtrlOxy.Oxy_Lower << 8;
        sParamCtrlOxy.Oxy_Lower |= *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+5);
        
        sParamCtrlOxy.Oxy_Upper = *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+2);
        sParamCtrlOxy.Oxy_Upper = sParamCtrlOxy.Oxy_Upper << 8;
        sParamCtrlOxy.Oxy_Upper |= *(__IO uint8_t*)(ADDR_OXY_THRESH_SETTING+3);
    }
    Stamp_Menu_Exit();
}

void        Save_TimeCtrl(uint16_t TimeDelay, uint16_t TimeChange, uint16_t TimeWarning)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamCtrlOxy.TimeDelay = TimeDelay;
    sParamCtrlOxy.TimeChange = TimeChange;
    sParamCtrlOxy.TimeWarning = TimeWarning;
    
    aData[length++] = sParamCtrlOxy.TimeDelay >> 8;
    aData[length++] = sParamCtrlOxy.TimeDelay;
    
    aData[length++] = sParamCtrlOxy.TimeChange >> 8;
    aData[length++] = sParamCtrlOxy.TimeChange;
    
    aData[length++] = sParamCtrlOxy.TimeWarning >> 8;
    aData[length++] = sParamCtrlOxy.TimeWarning;

    Save_Array(ADDR_TIME_CONTROL_SETTING, aData, length);
    Stamp_Menu_Exit();
}

void        Init_TimeDelay(void)
{
    if(*(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING) == BYTE_TEMP_FIRST)
    {
        sParamCtrlOxy.TimeDelay  = *(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING+2) ;
        sParamCtrlOxy.TimeDelay  = sParamCtrlOxy.TimeDelay << 8;
        sParamCtrlOxy.TimeDelay |= *(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING+3);
        
        sParamCtrlOxy.TimeChange  = *(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING+4) ;
        sParamCtrlOxy.TimeChange  = sParamCtrlOxy.TimeChange << 8;
        sParamCtrlOxy.TimeChange |= *(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING+5);
        
        sParamCtrlOxy.TimeWarning  = *(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING+6) ;
        sParamCtrlOxy.TimeWarning  = sParamCtrlOxy.TimeWarning << 8;
        sParamCtrlOxy.TimeWarning |= *(__IO uint8_t*)(ADDR_TIME_CONTROL_SETTING+7);

        if(sParamCtrlOxy.TimeDelay < TIMEDELAY_MIN || sParamCtrlOxy.TimeDelay > TIMEDELAY_MAX)
          sParamCtrlOxy.TimeDelay = TIMEDELAY_DEFAULT;
        
        if(sParamCtrlOxy.TimeChange == 0) sParamCtrlOxy.TimeChange = TIMECHANGE_DEFAULT;
        if(sParamCtrlOxy.TimeWarning == 0) sParamCtrlOxy.TimeWarning = TIMEWARNING_DEFAULT;
    }
    Stamp_Menu_Exit();
}

void Save_Time_Ctrl_Flash(uint16_t RunCtrl, uint16_t FreeCtrl)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sTimeCtrlOxy.RunCtrl = RunCtrl;
    sTimeCtrlOxy.FreeCtrl = FreeCtrl;
    aData[length++] = sTimeCtrlOxy.RunCtrl >> 8;
    aData[length++] = sTimeCtrlOxy.RunCtrl;
    
    aData[length++] = sTimeCtrlOxy.FreeCtrl >> 8;
    aData[length++] = sTimeCtrlOxy.FreeCtrl;
    Save_Array(ADDR_CTRL_OXY_RUN_FREE, aData, length);
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
           sTimeCtrlOxy.FreeCtrl = FREE_OXY_DEFAULT;
        
        if(sTimeCtrlOxy.RunCtrl > TIME_MAX)
           sTimeCtrlOxy.RunCtrl = RUN_OXY_DEFAULT;
    }
    else
    {
        sTimeCtrlOxy.FreeCtrl = FREE_OXY_DEFAULT;
        sTimeCtrlOxy.RunCtrl = RUN_OXY_DEFAULT;
    }
    Stamp_Menu_Exit();
}

/*=========================== Function Handle ======================*/
void QueueRun_Change_QueueFree(void)
{
    uint8_t LengthFree = 0;
    LengthFree = qGet_Number_Items(&qCtrlOxyRun);
    if(LengthFree > NUMBER_OXY/2)
    {
        LengthFree = NUMBER_OXY - LengthFree;
    }
    while(LengthFree > 0)
    {
        QueueFree_To_QueueRun();
        QueueRun_To_QueueFree();
        LengthFree--;
    }
}

void QueueFree_To_QueueRun(void)
{
    if(qGet_Number_Items(&qCtrlOxyFree) > 0)
    {
        uint8_t Read = 0;
        qQueue_Receive(&qCtrlOxyFree ,&Read, 1);
        qQueue_Send(&qCtrlOxyRun, (uint8_t*)&Read, _TYPE_SEND_TO_END);
        Control_Oxy(Read, _ON_OXY);
    }
}

void QueueRun_To_QueueFree(void)
{
    if(qGet_Number_Items(&qCtrlOxyRun) > 0)
    {
        uint8_t Read = 0;
        qQueue_Receive(&qCtrlOxyRun ,&Read, 1);
        qQueue_Send(&qCtrlOxyFree, (uint8_t*)&Read, _TYPE_SEND_TO_END);
        Control_Oxy(Read, _OFF_OXY);
    }
}

void Init_QueueCtrlOxy(void)
{
    qQueue_Create(&qCtrlOxyRun, 10, sizeof(uint8_t),(uint8_t*)&qReadRun);
    qQueue_Create(&qCtrlOxyFree, 10, sizeof(uint8_t),(uint8_t*)&qReadFree);
    
    Init_StateOxy();
}

void OnOff_Warning(uint8_t State)
{
    if(sStatusRelay.RL5 != State)
    ControlRelay(RELAY_5, State, _RL_DEBUG);
}

void Init_StateOxy(void)
{
    if(*(__IO uint8_t*)(ADDR_INIT_STATE_MACHINE) == BYTE_TEMP_FIRST)
    {
        for(uint8_t i = 0; i < _OXY_END; i++)
        {
            aStateOxy[i] = *(__IO uint8_t*)(ADDR_INIT_STATE_MACHINE+2+i);
        }
        OnchipFlashPageErase(ADDR_INIT_STATE_MACHINE);
    }
    
    for(uint8_t i = 0; i < _OXY_END; i++)
    {
        if(aStateOxy[i] == _OFF_OXY)
        {
            qQueue_Send(&qCtrlOxyFree, (uint8_t*)&i, _TYPE_SEND_TO_END);
        }
        else
        {
            qQueue_Send(&qCtrlOxyRun, (uint8_t*)&i, _TYPE_SEND_TO_END);
            Control_Oxy(i, _ON_OXY);
        }
    }
}

void Save_StateOxy(void)
{
    if(sStateCtrlOxy.StateMachine == _MACHINE_AUTO)
    {
        uint8_t aData[8] = {0};
        uint8_t length = 0;
        
        for(uint8_t i = 0; i < _OXY_END; i++)
        {
            aData[length++] = aStateOxy[i];
        }

        Save_Array(ADDR_INIT_STATE_MACHINE, aData, length);  
    }
}


/*======================== Function Handle ========================*/


void AppCtrlOxy_Log_Data_TSVH (void)
{
    uint8_t     aMessData[256] = {0};
    uint8_t     Length = 0;
    
//    if (sRTC.year <= 20)
//        return;
    
    Length = AppCtrlOxy_Packet_TSVH (&aMessData[0]);
#ifdef USING_APP_MEM
    AppMem_Write_Data(_MEM_DATA_TSVH_A, &aMessData[0], Length, sRecTSVH.SizeRecord_u16);
#endif
}

uint8_t AppCtrlOxy_Packet_TSVH (uint8_t *pData)
{
    uint16_t    length = 0;
    uint16_t	i = 0;
    uint8_t     TempCrc = 0;  
    
    //----------sTime--------------------
    AppComm_SV_Protocol_Packet_Data(pData, &length, OBIS_TIME_DEVICE, &sRTC, 6, 0xAA);
    
    //---------- State Power ------------
    
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_STATE_POWER, &sStateCtrlOxy.StatePower, 1, 0xAA);
    
    //---------- State Machine-----------
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_STATE_MACHINE, &sStateCtrlOxy.StateMachine, 1, 0xAA);
    
    //---------- State Oxy --------------
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_STATE_OXY, &(*sStateCtrlOxy.aStateOxy), 4, 0xAA);
    
    //---------- State Connect Sensor----
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_CONNECT_SENSOR, &sStateCtrlOxy.StateSensor, 1, 0xAA);
    
    //----------  Oxy Upper --------
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_OXY_UPPER, &sParamCtrlOxy.Oxy_Upper, 2, 0xFE);
    
    //----------  Oxy Lower --------
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_OXY_LOWER, &sParamCtrlOxy.Oxy_Lower, 2, 0xFE);
    
    //----------  Oxy mg/L ---------
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_OXY_MG_L, &sParamMeasure.Oxy_Mg_L, 2, 0xFE);
    
    //----------  Oxy Percent ------
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_OXY_PERCENT, &sParamMeasure.Oxy_Percent, 2, 0xFE);
    
    //----------  Temperature ------
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_OXY_TEMPERATURE, &sParamMeasure.Temp, 2, 0xFE);
    
    //----------  Salinity ------
    AppCtrlOxy_Packet_Data(pData, &length, OBIS_ENVI_SALINITY, &sParamMeasure.Salinity, 2, 0x00);
    
    //----------Tan suat--------------------
    AppComm_SV_Protocol_Packet_Data(pData, &length, OBIS_FREQ_SEND, &sFreqInfor.FreqSendUnitMin_u32, 2, 0x00);
    // caculator crc
    length++;
	for (i = 0; i < (length - 1); i++)
		TempCrc ^= pData[i];

    pData[length-1] = TempCrc;
    
    return length;
}

void AppCtrlOxy_Packet_Data (uint8_t *pTarget, uint16_t *LenTarget, uint8_t Obis, 
                             void *pData, uint8_t LenData, uint8_t Scale)
{
    uint16_t Pos = *LenTarget, i= 0;
    uint8_t *TempU8;
    
    if (LenData == 0)
        return;
    
    pTarget[Pos++] = Obis;
    pTarget[Pos++] = LenData;
            
    switch (Obis)
    {
        case OBIS_ENVI_STATE_POWER:
        case OBIS_ENVI_STATE_MACHINE:
        case OBIS_ENVI_CONNECT_SENSOR:
        case OBIS_ENVI_OXY_UPPER:
        case OBIS_ENVI_OXY_LOWER:
        case OBIS_ENVI_OXY_MG_L:
        case OBIS_ENVI_OXY_PERCENT:
        case OBIS_ENVI_OXY_TEMPERATURE:
        case OBIS_ENVI_SALINITY:
        case OBIS_ENVI_NOTIFY:
        case OBIS_ENVI_ID_SERVER:
            AppComm_Sub_Packet_Integer(pTarget, &Pos, pData, LenData);
            //
            if (Scale != 0xAA)
                pTarget[Pos++] = Scale;
            break;
            
        case OBIS_ENVI_STATE_OXY:
            TempU8 = (uint8_t *) pData;
            for (i = 0; i < LenData; i++)
                pTarget[Pos++] = *(TempU8 + i);
            break; 
            
        default:
            break;
    }
    
    *LenTarget = Pos ;
}

void Compensation_Salinity(uint16_t *Oxy_Concentration, uint16_t Sal_x100, uint16_t Temp_x100)
{
	// Calcutate the oxygen concentration in 4500- OXYGEN(DISSOLVED), pape 4/8
	// Chage  S = 1.80655*Chorinity
		float tam_Sal =0;
        float temp =25;
        float Factor =1; 
		temp = (float)Temp_x100/100 + (float)273.15;
		tam_Sal =  -(Sal_x100/100)*(0.017674+(-10.754+ 2140.7/temp)/temp);
		Factor =(float)pow((float)2.718282,tam_Sal);
		*Oxy_Concentration = (uint16_t)(*Oxy_Concentration * (Factor));
}

void DCU_Notify_Server(uint8_t KindNotify)
{
    uint8_t     aData[50]={0};
    uint16_t    length = 0;
    uint16_t	i = 0;
    uint8_t     TempCrc = 0;  
    
    //----------sTime--------------------
    AppComm_SV_Protocol_Packet_Data(aData, &length, OBIS_TIME_DEVICE, &sRTC, 6, 0xAA);
    
    //---------- Kind Warning ------------
    AppCtrlOxy_Packet_Data(aData, &length, OBIS_ENVI_NOTIFY, &KindNotify, 1, 0xAA);
    // caculator crc
    length++;
	for (i = 0; i < (length - 1); i++)
		TempCrc ^= aData[i];

    aData[length-1] = TempCrc;
    
    DCU_Respond (_AT_REQUEST_SERVER, aData, length, 1);
}

void DCU_Respond_ID_Server(uint32_t ID_Server)
{
    uint8_t     aData[50]={0};
    uint16_t    length = 0;
    uint16_t	i = 0;
    uint8_t     TempCrc = 0;  
    
    //----------sTime--------------------
    AppComm_SV_Protocol_Packet_Data(aData, &length, OBIS_TIME_DEVICE, &sRTC, 6, 0xAA);
    
    //---------- Kind Warning ------------
    AppCtrlOxy_Packet_Data(aData, &length, OBIS_ENVI_ID_SERVER, &ID_Server, 4, 0xAA);
    // caculator crc
    length++;
	for (i = 0; i < (length - 1); i++)
		TempCrc ^= aData[i];

    aData[length-1] = TempCrc;
    
    DCU_Respond (_AT_REQUEST_SERVER, aData, length, 1);
}
/*======================= Function Handle ====================*/
/*
    Chuyen Scale sang Uint
*/
uint32_t Calculator_Scale(uint8_t Scale)
{
    /*
    Scale
    00->1 
    FF->0.1 
    FE->0.01 
    FD->0.001  
    FC->0.0001  
    FB->0.00001 
    */
  
    if(Scale == 0x00) return 1;
    else if(Scale == 0xFF) return 10;
    else if(Scale == 0xFE) return 100;
    else if(Scale == 0xFD) return 1000;
    else if(Scale == 0xFC) return 10000;
    else if(Scale == 0xFB) return 100000;
    
    return 1;
}

/*
    @brief  Tinh tan so theo Scale
    @retval Tan so nap vao bien tan
*/
uint32_t Calculator_Value_Scale(uint32_t Value, uint8_t Scale, uint8_t Scale_Default)
{
      /*
    Scale
    00->1 
    FF->0.1 
    FE->0.01 
    FD->0.001  
    FC->0.0001  
    FB->0.00001 
    */
  uint64_t Stamp_Scale = Value * Calculator_Scale(Scale_Default);
  return Stamp_Scale/Calculator_Scale(Scale);
}

void Init_AppCtrlOxy(void)
{
    Init_Time_Ctrl_Oxy();
    Init_TimeDelay();
    Init_OxyUpperLower();
    Init_QueueCtrlOxy();
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