
#include "user_app_slave_oxy.h"
#include "iwdg.h"
#include "onchipflash.h"
#include "tim.h"
#include "user_app_led7seg.h"
/*====================Function Static===================*/
static uint8_t fevent_slave_oxy_entry(uint8_t event);
static uint8_t fevent_slave_oxy_iwdg(uint8_t event);
/*========================Struct========================*/
sEvent_struct               sEventAppSlaveOxy[]=
{
  {_EVENT_SLAVE_OXY_ENTRY,      1, 5, 5,        fevent_slave_oxy_entry},
  {_EVENT_SLAVE_OXY_IWDG,       1, 5, 500,      fevent_slave_oxy_iwdg},
};

uint32_t baud_rate_value[8]={1200,2400,4800,9600,19200,38400,57600,115200};

Struct_ParamSlaveOxy        sParamSlave=
{
    .TimeRun = DEFAULT_TIMERUN_SLAVE,
    .TimeFree= DEFAULT_TIMEFREE_SLAVE,
    .Baudrate= 9600,
};

Struct_CtrlOxy              sCtrlOxy=
{
    .TimeRun = 0,
    .TimeFree= 0,
    .StateCtrl = 0,
    .KindOxyRun = 0,
};

/*=======================Function Hanlde=================*/
static uint8_t fevent_slave_oxy_entry(uint8_t event)
{
    return 1;
}

static uint8_t fevent_slave_oxy_iwdg(uint8_t event)
{
    HAL_IWDG_Refresh(&hiwdg);
    fevent_enable(sEventAppSlaveOxy, event);
    return 1;
}

/*------------------------Hanlde Timer Callback-----------------------*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == htim2.Instance)
    {
        if(Entry_Main == 1)
        {
            Dis_Led7seg_Menu_Main();
            Ctrl_Slave_Oxy();
        }
    }
}
/*------------------------------Handle-------------------------------*/
/*
    @Dieu khien bat tat Oxy theo chu trinh
    @Dat trong ngat Timer
*/
void Ctrl_Slave_Oxy(void)
{
    static uint8_t StateBefore = 0;
    switch(sCtrlOxy.StateCtrl)
    {
        case _OXY_OFF:
          sCtrlOxy.TimeFree = 0;
          sCtrlOxy.TimeRun = 0;
          HAL_GPIO_WritePin(RELAY_OXY_L_GPIO, RELAY_OXY_L_PIN, STATE_GPIO_OFF);
          HAL_GPIO_WritePin(RELAY_OXY_R_GPIO, RELAY_OXY_R_PIN, STATE_GPIO_OFF);
          break;
          
        case _OXY_FREE:
          sCtrlOxy.TimeFree++;
          if(sCtrlOxy.TimeFree >= sParamSlave.TimeFree)     
          {
            if(StateBefore == _OXY_L_RUN)
            {
                sCtrlOxy.KindOxyRun = _OXY_R_RUN;
                HAL_GPIO_WritePin(RELAY_OXY_R_GPIO, RELAY_OXY_R_PIN, STATE_GPIO_ON);
            }
            else
            {
                sCtrlOxy.KindOxyRun = _OXY_L_RUN;
                HAL_GPIO_WritePin(RELAY_OXY_L_GPIO, RELAY_OXY_L_PIN, STATE_GPIO_ON);
            }
            StateBefore  = sCtrlOxy.KindOxyRun;
            sCtrlOxy.TimeFree = 0;
            sCtrlOxy.StateCtrl = _OXY_RUN;
          }
          break;
          
        case _OXY_RUN:
          sCtrlOxy.TimeRun++;
          if(sCtrlOxy.TimeRun >= sParamSlave.TimeRun)     
          {
            sCtrlOxy.StateCtrl = _OXY_FREE;
            HAL_GPIO_WritePin(RELAY_OXY_L_GPIO, RELAY_OXY_L_PIN, STATE_GPIO_OFF);
            HAL_GPIO_WritePin(RELAY_OXY_R_GPIO, RELAY_OXY_R_PIN, STATE_GPIO_OFF);
            sCtrlOxy.TimeRun = 0;
          }
          break;
      
        default:
            break;
    }
}
/*--------------- Save and Init Time Cyle Slave ---------------*/
uint8_t Save_TimeCycleSlave(uint16_t TimeRun, uint16_t TimeFree)
{
    uint8_t aData[4] = {0};
    uint8_t length = 0;
    if(TimeRun >= TIMECYCLE_SLAVE_MIN && TimeRun <=TIMECYCLE_SLAVE_MAX && \
       TimeFree>= TIMECYCLE_SLAVE_MIN && TimeFree<=TIMECYCLE_SLAVE_MAX)
    {
        sParamSlave.TimeRun  = TimeRun;
        sParamSlave.TimeFree = TimeFree;
        sParamMenu.Stamp_TimeRun  = sParamSlave.TimeRun;
        sParamMenu.Stamp_TimeFree = sParamSlave.TimeFree; 
        
        aData[length++] = TimeRun >> 8;
        aData[length++] = TimeRun;
        aData[length++] = TimeFree >> 8;
        aData[length++] = TimeFree;
        Save_Array(ADDR_FLASH_TIMECYCLE, aData, length);
        return 1;
    }
    else
      return 0;
}

void Init_TimeCycleSlave(void)
{
    if(*(__IO uint8_t*)(ADDR_FLASH_TIMECYCLE) == BYTE_TEMP_FIRST)
    {
        sParamSlave.TimeRun   = *(__IO uint8_t*)(ADDR_FLASH_TIMECYCLE+2) << 24 ;
        sParamSlave.TimeRun  |= *(__IO uint8_t*)(ADDR_FLASH_TIMECYCLE+3) ;
        sParamSlave.TimeFree  = *(__IO uint8_t*)(ADDR_FLASH_TIMECYCLE+4) << 8;
        sParamSlave.TimeFree |= *(__IO uint8_t*)(ADDR_FLASH_TIMECYCLE+5);
    }
}


/*-------------------- Save and Init Baudrate ------------------------*/
uint8_t Save_BaudrateSlave(uint32_t Baudrate)
{
    uint8_t result = 0;
    uint8_t aData[4] = {0};
    uint8_t length = 0;
    
    for(uint8_t i = 0; i < 8; i++)
    {
        if(Baudrate == baud_rate_value[i])
        {
            result = 1;
            break;
        }
    }
    
    if(result == 1)
    {
        sParamSlave.Baudrate = Baudrate;
        aData[length++] = Baudrate >> 24;
        aData[length++] = Baudrate >> 16;
        aData[length++] = Baudrate >> 8;
        aData[length++] = Baudrate;
        Save_Array(ADDR_FLASH_BAUDRATE, aData, length);
        Init_UartBaudrate(sParamSlave.Baudrate);
        Init_RX_Mode_Uart_Debug();
    }
    return result;
}

void Init_BaudrateSlave()
{
    uint8_t result = 0;
    if(*(__IO uint8_t*)(ADDR_FLASH_BAUDRATE) == BYTE_TEMP_FIRST)
    {
        sParamSlave.Baudrate  = *(__IO uint8_t*)(ADDR_FLASH_BAUDRATE+2) << 24 ;
        sParamSlave.Baudrate |= *(__IO uint8_t*)(ADDR_FLASH_BAUDRATE+3) << 16;
        sParamSlave.Baudrate |= *(__IO uint8_t*)(ADDR_FLASH_BAUDRATE+4) << 8;
        sParamSlave.Baudrate |= *(__IO uint8_t*)(ADDR_FLASH_BAUDRATE+5);
    }
    for(uint8_t i = 0; i<8; i++)
    {
        if(baud_rate_value[i] == sParamSlave.Baudrate)
        {
            result = 1; 
            break;
        }
    }
    
    if(result == 1)
    {
        Init_UartBaudrate(sParamSlave.Baudrate);
        Init_RX_Mode_Uart_Debug();
    }
}


void Init_UartBaudrate(uint32_t Baudrate)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = Baudrate;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_2;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/*=======================Handle Init and Task app====================*/

void        Init_AppSlaveOxy(void)
{
    Init_TimeCycleSlave();
    Init_BaudrateSlave();
}

uint8_t     AppSlaveOxy_Task(void)
{
    uint8_t i = 0;
    uint8_t Result =  false;
    
    for(i = 0; i < _EVENT_SLAVE_OXY_END; i++)
    {
        if(sEventAppSlaveOxy[i].e_status == 1)
        {
            Result = true; 
            
            if((sEventAppSlaveOxy[i].e_systick == 0) ||
               ((HAL_GetTick() - sEventAppSlaveOxy[i].e_systick) >= sEventAppSlaveOxy[i].e_period))
            {
                sEventAppSlaveOxy[i].e_status = 0; //Disable event
                sEventAppSlaveOxy[i].e_systick= HAL_GetTick();
                sEventAppSlaveOxy[i].e_function_handler(i);
            }
        }
    }
    
    return Result;
}

