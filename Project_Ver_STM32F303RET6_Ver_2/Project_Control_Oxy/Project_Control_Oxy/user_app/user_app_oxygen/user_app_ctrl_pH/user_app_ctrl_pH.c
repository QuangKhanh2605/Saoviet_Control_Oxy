
#include "user_app_ctrl_ph.h"
#include "user_internal_mem.h"
#include "user_obis_sv.h"
#include "user_define_oxygen.h"
#include "user_modem.h"
/*============== Function static ===============*/
static uint8_t fevent_ctrl_ph_entry(uint8_t event);

static uint8_t fevent_ph_check_power(uint8_t event);
static uint8_t fevent_ctrl_ph_state_slave(uint8_t event);
static uint8_t fevent_ctrl_ph_warning(uint8_t event);

static uint8_t fevent_ctrl_ph_sampling(uint8_t event);
static uint8_t fevent_ctrl_ph_pump(uint8_t event);
/*=================== struct ==================*/
sEvent_struct               sEventAppCtrlPH[] = 
{
  {_EVENT_CTRL_PH_ENTRY,            1, 5, 30000,    fevent_ctrl_ph_entry},
 
  {_EVENT_CTRL_PH_CHECK_POWER,      1, 0, 0,        fevent_ph_check_power},
  {_EVENT_CTRL_PH_STATE_SLAVE,      0, 5, 1000,     fevent_ctrl_ph_state_slave},
  {_EVENT_CTRL_PH_WARNING,          0, 5, 1000,     fevent_ctrl_ph_warning},
  
  {_EVENT_CTRL_PH_SAMPLING,         0, 5, 5000,     fevent_ctrl_ph_sampling},
  {_EVENT_CTRL_PH_PUMP,             0, 5, 0,        fevent_ctrl_ph_pump},
};

Struct_ParamCtrlPH          sParamCtrlPH= {0};

uint16_t array_stt[NUMBER_SAMPLING_PH] = {0};         //stt lay mau
uint16_t array_sampling[NUMBER_SAMPLING_PH] = {0};    //Gia tri lay mau

void quickSort_Sampling_pH(uint16_t array_stt[],uint16_t array_sampling[], uint8_t left, uint8_t right);
/*================= Function Handle ==============*/
static uint8_t fevent_ctrl_ph_entry(uint8_t event)
{
    fevent_enable(sEventAppCtrlPH, _EVENT_CTRL_PH_SAMPLING);
    
    fevent_enable(sEventAppCtrlPH, _EVENT_CTRL_PH_STATE_SLAVE);
    return 1;
}

uint32_t Gettick_Power_pH = 0;
uint32_t Gettick_Connect_pH = 0;
static uint8_t fevent_ph_check_power(uint8_t event)
{
    if(sStateCtrlOxy.StatePower == _POWER_OFF)
    {
        Gettick_Power_pH = HAL_GetTick();
    }
    
    fevent_active(sEventAppCtrlPH, event);
    return 1;
}

static uint8_t fevent_ctrl_ph_state_slave(uint8_t event)
{
    static uint32_t gettick_state_slave = 0;
    static uint8_t StateSensorPH_Before = _PH_CONNECT;
    uint8_t aData[2] = {0};
    
    if(sParamCtrlPH.ChooseOptionPH == _ONE_SENSOR_PH)
    {
      if(HAL_GetTick() - Gettick_Power_pH > 30000 || sParamCtrlPH.StateSensorPH == _PH_CONNECT)
      {
        if(sParamCtrlPH.StateSensorPH != StateSensorPH_Before)
        {
            gettick_state_slave = HAL_GetTick();
            
            if(sParamCtrlPH.StateSensorPH == _PH_CONNECT)      
            {
    //            UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Slave_CONNECT!\r\n");
                sParamCtrlPH.StateWarningPH_Connect = _PH_STT_OK;
            }
            else
            {
                sParamCtrlPH.StateWarningPH_Connect = _PH_STT_FAIL;
                aData[0] = 0x00;
                aData[1] = 0x00;
                Log_EventWarnig(OBIS_WARNING_CONNECT_PH, 0x01, aData);
                
    //            UTIL_Log_Str(DBLEVEL_M, "u_app_oxy: Slave_DISCONNECT!\r\n");
            }
     
            StateSensorPH_Before = sParamCtrlPH.StateSensorPH;
        }
        
        if(sParamCtrlPH.StateSensorPH == _PH_DISCONNECT)
        {
            sParamCtrlPH.StatePH = _PH_LOW;
            if(HAL_GetTick() - gettick_state_slave >= PH_TIME_RESEND_WARNING)
            {
                gettick_state_slave = HAL_GetTick();
                aData[0] = 0x00;
                aData[1] = 0x00;
                Log_EventWarnig(OBIS_WARNING_CONNECT_PH, 0x01, aData);
            }
            sParamCtrlPH.StateWarningPH_Connect = _PH_STT_FAIL;
        }
        else
        {
            sParamCtrlPH.StateWarningPH_Connect = _PH_STT_OK;
        }
      }
      else
      {
           sParamCtrlPH.StateWarningPH_Connect = _PH_STT_OK;
      }
    }
    else
    {
        gettick_state_slave = 0;
        StateSensorPH_Before = _PH_CONNECT;
        sParamCtrlPH.StateWarningPH_Connect = _PH_STT_OK;
    }
    
    if(sParamCtrlPH.StateSensorPH == _PH_DISCONNECT)
    {
        Gettick_Connect_pH = HAL_GetTick();
    }
    
    fevent_active(sEventAppCtrlPH, _EVENT_CTRL_PH_WARNING);
    return 1;
}

static uint8_t fevent_ctrl_ph_warning(uint8_t event)
{
    static uint32_t gettick_warning_period = 0;
    static uint8_t StatePH_Before = _PH_BALANCE;
    uint8_t aData[2] = {0};
    
    if(sParamCtrlPH.ChooseOptionPH == _ONE_SENSOR_PH)
    {
        if(HAL_GetTick() - Gettick_Connect_pH > 15000) 
        {
            if(sParamCtrlPH.pH_Measure < sParamCtrlPH.pH_Lower - sParamCtrlPH.pH_StepWarning)
            {
        //        OnOff_Warning(ON_RELAY);
                sParamCtrlPH.StatePH = _PH_LOW;
                sParamCtrlPH.StateWarningPH_Measure = _PH_STT_FAIL;
                if(sParamCtrlPH.StatePH != StatePH_Before)
                {
                    gettick_warning_period = HAL_GetTick();
                    aData[0] = 0x00;
                    aData[1] = 0x00;
                    Log_EventWarnig(OBIS_WARNING_PH_LOWER, 0x01, aData);
                    StatePH_Before = sParamCtrlPH.StatePH;
                }
            }
            else if(sParamCtrlPH.pH_Measure > sParamCtrlPH.pH_Upper + sParamCtrlPH.pH_StepWarning)
            {
        //        OnOff_Warning(ON_RELAY);
                sParamCtrlPH.StatePH = _PH_HIGH;
                sParamCtrlPH.StateWarningPH_Measure = _PH_STT_FAIL;
                if(sParamCtrlPH.StatePH != StatePH_Before)
                {
                    gettick_warning_period = HAL_GetTick();
                    aData[0] = 0x01;
                    aData[1] = 0x00;
                    Log_EventWarnig(OBIS_WARNING_PH_LOWER, 0x01, aData);
                    StatePH_Before = sParamCtrlPH.StatePH;
                }
            }
            else
            {
        //        OnOff_Warning(OFF_RELAY);
                sParamCtrlPH.StatePH = _PH_BALANCE;
                sParamCtrlPH.StateWarningPH_Measure = _PH_STT_OK;
                gettick_warning_period = HAL_GetTick();
            }
            
            if(sParamCtrlPH.StatePH == _PH_LOW)
            {
                if(HAL_GetTick() - gettick_warning_period >= PH_TIME_RESEND_WARNING)
                {
                    gettick_warning_period = HAL_GetTick();
                    aData[0] = 0x00;
                    aData[1] = 0x00;
                    Log_EventWarnig(OBIS_WARNING_PH_LOWER, 0x01, aData);
                }
            }
            else if(sParamCtrlPH.StatePH == _PH_HIGH)
            {
                if(HAL_GetTick() - gettick_warning_period >= PH_TIME_RESEND_WARNING)
                {
                    gettick_warning_period = HAL_GetTick();
                    aData[0] = 0x01;
                    aData[1] = 0x00;
                    Log_EventWarnig(OBIS_WARNING_PH_LOWER, 0x01, aData);
                }
            }
        }
        else
        {
            gettick_warning_period = 0;
            StatePH_Before = _PH_BALANCE;
            sParamCtrlPH.StateWarningPH_Measure = _PH_STT_OK;
        }
    }
    else
    {
        gettick_warning_period = 0;
        StatePH_Before = _PH_BALANCE;
        sParamCtrlPH.StateWarningPH_Measure = _PH_STT_OK;
    }
  
    fevent_enable(sEventAppCtrlPH, _EVENT_CTRL_PH_STATE_SLAVE);
    return 1;
}

static uint8_t fevent_ctrl_ph_sampling(uint8_t event)
{
    static uint8_t Handle_Once = 0;
//    static uint8_t state_ctrl_before = _PH_SAMPLING;
    
    if(sParamCtrlPH.ChooseOptionPH == _ONE_SENSOR_PH)
    {
        
        if(sParamCtrlPH.pH_Measure == 0)
        {
            sParamCtrlPH.pH_AverageSampling = 0;
            Handle_Once = 0;
            Control_Pump(_PH_SAMPLING);
        }
        else
        {
            if(Handle_Once == 0)
            {
                Handle_Once = 1;
                for(uint8_t i = 0; i< NUMBER_SAMPLING_PH; i++)
                {
                  array_stt[i] = i;
                  array_sampling[i] = sParamCtrlPH.pH_Measure;
                }
                
                sParamCtrlPH.pH_AverageSampling = sParamCtrlPH.pH_Measure;
            }
            else
            {
                for(uint8_t i = 0; i < NUMBER_SAMPLING_PH; i++)
                {
                    if(array_stt[i] == NUMBER_SAMPLING_PH - 1)
                    {
                        array_stt[i] = 0;
                        array_sampling[i] = sParamCtrlPH.pH_Measure;
                    }
                    else
                        array_stt[i] = array_stt[i] + 1;
                }

                quickSort_Sampling_pH(array_stt, array_sampling, 0, NUMBER_SAMPLING_PH - 1);
                sParamCtrlPH.pH_AverageSampling = array_sampling[NUMBER_SAMPLING_PH/2];
            }
        
            fevent_active(sEventAppCtrlPH, _EVENT_CTRL_PH_PUMP);
        }
        
//        if(state_ctrl_before != sParamCtrlPH.StateCtrl)
//        {
//            if(sParamCtrlPH.StateCtrl == _PH_PUMP)
//            {
//                Control_Pump(_PUMP_PH_ON);
//            }
//            state_ctrl_before = sParamCtrlPH.StateCtrl;
//        }
    }
    else
    {
        sParamCtrlPH.pH_AverageSampling = 0;
        Handle_Once = 0;
        Control_Pump(_PH_SAMPLING);
    }
    
    fevent_enable(sEventAppCtrlPH, event);
    return 1;
}

static uint8_t fevent_ctrl_ph_pump(uint8_t event)
{
    switch(sParamCtrlPH.StateCtrl)
    {
        case _PH_PUMP:
          if(sParamCtrlPH.pH_GettickMsCtrl >= sParamCtrlPH.pH_TimePump*60000 || \
             sParamCtrlPH.pH_AverageSampling > sParamCtrlPH.pH_Upper)
          {
            Control_Pump(_PH_SAMPLING);
            sParamCtrlPH.pH_GettickMsCtrl = 0;
          }
          break;
          
        case _PH_SAMPLING:
          if(sParamCtrlPH.pH_GettickMsCtrl >= sParamCtrlPH.pH_TimeSampling*60000 && \
             sParamCtrlPH.pH_AverageSampling < sParamCtrlPH.pH_Lower)
          {
            Control_Pump(_PH_PUMP);
            sParamCtrlPH.pH_GettickMsCtrl = 0;
          }
          break;
          
        default:
          break;
    }
    
    return 1;
}

/*========================== QuickSort Sampling =======================*/
void quickSort_Sampling_pH(uint16_t array_stt[],uint16_t array_sampling[], uint8_t left, uint8_t right)
{
//	uint16_t pivot = array_sampling[(left+right)/2];
//	uint16_t low = left;
//    uint16_t high = right;
//	while (low < high)
//    {
//		while (array_sampling[low] < pivot) low++;
//        
//		while (array_sampling[high] > pivot) high--;
//        
//		if (low <= high)
//        {
//			int temp = 0;
//            temp = array_sampling[low];
//			array_sampling[low] = array_sampling[high];
//			array_sampling[high] = temp;
//            
//            temp = array_stt[low];
//			array_stt[low] = array_stt[high];
//			array_stt[high] = temp;
//            
//			low++;
//			high--;
//		}
//	}
//	if (low < right)
//		quickSort_Sampling_pH(array_stt, array_sampling, low, right);
//
//	if (left < high)
//		quickSort_Sampling_pH(array_stt, array_sampling, left, high);
  
/*---------------------- Sap xep noi bot --------------------*/
  
  for(uint8_t i = 0; i < NUMBER_SAMPLING_PH; i++)
  {
    for(uint8_t j = 0; j < NUMBER_SAMPLING_PH - 1; j++)
    {
        if(array_sampling[j] > array_sampling[j + 1])
        {
			int temp = 0;
            temp = array_sampling[j];
			array_sampling[j] = array_sampling[j+1];
			array_sampling[j+1] = temp;
            
            temp = array_stt[j];
			array_stt[j] = array_stt[j+1];
			array_stt[j+1] = temp;
        }
    }
  }
}

void Control_Pump(eKindStateCtrlPH_Pump Kind_Ctrl)
{
    switch(Kind_Ctrl)
    {
        case _PH_SAMPLING:
          ControlRelay(RELAY_6, OFF_RELAY, _RL_UNDEBUG);
          if(sParamCtrlPH.StateCtrl != _PH_SAMPLING)
          {
            sParamCtrlPH.StateCtrl = Kind_Ctrl;
            DeInit_PeriodTimer(); 
          }
          break;
          
        case _PH_PUMP:
          ControlRelay(RELAY_6, ON_RELAY, _RL_UNDEBUG);
          if(sParamCtrlPH.StateCtrl != _PH_PUMP)
          {
            sParamCtrlPH.StateCtrl = Kind_Ctrl;
            DeInit_PeriodTimer(); 
          }
          break;
          
        default:
          break;
    }
    sParamCtrlPH.StateCtrl = Kind_Ctrl;
}

/*================== Save and Init PH upper and lower =================*/
void        Save_pH_UpperLowerWar(uint16_t Upper, uint16_t Lower, uint16_t StepWarning)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamCtrlPH.pH_Upper = Upper;
    sParamCtrlPH.pH_Lower = Lower;
    sParamCtrlPH.pH_StepWarning = StepWarning;
    
    aData[length++] = sParamCtrlPH.pH_Upper >> 8;
    aData[length++] = sParamCtrlPH.pH_Upper;
    
    aData[length++] = sParamCtrlPH.pH_Lower >> 8;
    aData[length++] = sParamCtrlPH.pH_Lower;
    
    aData[length++] = sParamCtrlPH.pH_StepWarning >> 8;
    aData[length++] = sParamCtrlPH.pH_StepWarning;

    Save_Array(ADDR_PH_THRESH_SETTING, aData, length);
    Stamp_Menu_Exit();
}

void        Init_pH_UpperLowerWar(void)
{
    if(*(__IO uint8_t*)(ADDR_PH_THRESH_SETTING) == BYTE_TEMP_FIRST)
    {
        sParamCtrlPH.pH_Upper = *(__IO uint8_t*)(ADDR_PH_THRESH_SETTING+2) <<8;
        sParamCtrlPH.pH_Upper |= *(__IO uint8_t*)(ADDR_PH_THRESH_SETTING+3);
        
        sParamCtrlPH.pH_Lower  = *(__IO uint8_t*)(ADDR_PH_THRESH_SETTING+4) <<8;
        sParamCtrlPH.pH_Lower |= *(__IO uint8_t*)(ADDR_PH_THRESH_SETTING+5);
        
        sParamCtrlPH.pH_StepWarning  = *(__IO uint8_t*)(ADDR_PH_THRESH_SETTING+6) <<8;
        sParamCtrlPH.pH_StepWarning |= *(__IO uint8_t*)(ADDR_PH_THRESH_SETTING+7);
        
    }
    else
    {
        sParamCtrlPH.pH_Lower = PH_LOWER_DEFAULT;
        sParamCtrlPH.pH_Upper = PH_UPPER_DEFAULT;
        sParamCtrlPH.pH_StepWarning = PH_STEP_WARNING_DEFAULT;
    }
    Stamp_Menu_Exit();
}

/*================== Save and Init PH time control =================*/
void        Save_pH_TimeControl(uint16_t TimePump, uint16_t TimeSampling)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamCtrlPH.pH_TimePump = TimePump;
    sParamCtrlPH.pH_TimeSampling = TimeSampling;
    
    aData[length++] = sParamCtrlPH.pH_TimePump >> 8;
    aData[length++] = sParamCtrlPH.pH_TimePump;
    
    aData[length++] = sParamCtrlPH.pH_TimeSampling >> 8;
    aData[length++] = sParamCtrlPH.pH_TimeSampling;

    Save_Array(ADDR_PH_TIME_CONTROL, aData, length);
    Stamp_Menu_Exit();
}

void        Init_pH_TimeControl(void)
{
    if(*(__IO uint8_t*)(ADDR_PH_TIME_CONTROL) == BYTE_TEMP_FIRST)
    {
        sParamCtrlPH.pH_TimePump = *(__IO uint8_t*)(ADDR_PH_TIME_CONTROL+2) <<8;
        sParamCtrlPH.pH_TimePump |= *(__IO uint8_t*)(ADDR_PH_TIME_CONTROL+3);
        
        sParamCtrlPH.pH_TimeSampling  = *(__IO uint8_t*)(ADDR_PH_TIME_CONTROL+4) <<8;
        sParamCtrlPH.pH_TimeSampling |= *(__IO uint8_t*)(ADDR_PH_TIME_CONTROL+5);
    }
    else
    {
        sParamCtrlPH.pH_TimePump       = PH_TIME_PUMP_DEFAULT;
        sParamCtrlPH.pH_TimeSampling   = PH_TIME_SAMPLING_DEFAULT;
    }
    Stamp_Menu_Exit();
}

/*================Save and Init Option pH======================*/
void        Save_pH_ChooseOption(uint8_t StateChoose)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamCtrlPH.ChooseOptionPH = StateChoose;
    
    aData[length++] = sParamCtrlPH.ChooseOptionPH;

    Save_Array(ADDR_PH_CHOOSE_OPTION, aData, length);
//    Stamp_Menu_Exit();
}

void        Init_pH_ChooseOption(void)
{
    if(*(__IO uint8_t*)(ADDR_PH_CHOOSE_OPTION) == BYTE_TEMP_FIRST)
    {
        sParamCtrlPH.ChooseOptionPH = *(__IO uint8_t*)(ADDR_PH_CHOOSE_OPTION+2);
    }
    else
    {
        sParamCtrlPH.ChooseOptionPH = _ONE_SENSOR_PH;
    }
}

/*================Save and Init Param Ctrl pH======================*/
void        Save_pH_StateCtrl(void)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    aData[length++] = sParamCtrlPH.StateCtrl;
    
    aData[length++] = sParamCtrlPH.pH_GettickMsCtrl >> 24;
    aData[length++] = sParamCtrlPH.pH_GettickMsCtrl >> 16;
    aData[length++] = sParamCtrlPH.pH_GettickMsCtrl >> 8;
    aData[length++] = sParamCtrlPH.pH_GettickMsCtrl ;

    Save_Array(ADDR_PH_STATE_CTRL, aData, length);
//    Stamp_Menu_Exit();
}

void        Init_pH_StateCtrl(void)
{
    if(*(__IO uint8_t*)(ADDR_PH_STATE_CTRL) == BYTE_TEMP_FIRST)
    {
        sParamCtrlPH.StateCtrl = *(__IO uint8_t*)(ADDR_PH_STATE_CTRL+2);
        
        sParamCtrlPH.pH_GettickMsCtrl  = *(__IO uint8_t*)(ADDR_PH_STATE_CTRL+3) << 24;
        sParamCtrlPH.pH_GettickMsCtrl |= *(__IO uint8_t*)(ADDR_PH_STATE_CTRL+4) << 16;
        sParamCtrlPH.pH_GettickMsCtrl |= *(__IO uint8_t*)(ADDR_PH_STATE_CTRL+5) << 8;
        sParamCtrlPH.pH_GettickMsCtrl |= *(__IO uint8_t*)(ADDR_PH_STATE_CTRL+6);
        
        OnchipFlashPageErase(ADDR_PH_STATE_CTRL);
    }
    else
    {
        sParamCtrlPH.StateCtrl = _PH_SAMPLING;
        sParamCtrlPH.pH_GettickMsCtrl = 0;
    }
    
    if(sParamCtrlPH.StateCtrl == _PH_PUMP)
      Control_Pump(_PH_PUMP);
    else
      Control_Pump(_PH_SAMPLING);
}

/*================Function Handle===============*/

void Init_AppCtrlPH(void)
{
  Init_pH_TimeControl();
  Init_pH_ChooseOption();
  Init_pH_UpperLowerWar();
  Init_pH_StateCtrl();
}

uint8_t AppCtrlPH_Task(void)
{
    uint8_t i = 0;
    uint8_t Result = false;
    for( i = 0; i < _EVENT_CTRL_PH_END; i++)
    {
        if(sEventAppCtrlPH[i].e_status == 1)
        {
            Result = true;
            if((sEventAppCtrlPH[i].e_systick == 0) ||
                ((HAL_GetTick() - sEventAppCtrlPH[i].e_systick) >= sEventAppCtrlPH[i].e_period))
            {
                sEventAppCtrlPH[i].e_status = 0; //Disable event
                sEventAppCtrlPH[i].e_systick= HAL_GetTick();
                sEventAppCtrlPH[i].e_function_handler(i);
            }
        }
    }
    
    return Result;
}

