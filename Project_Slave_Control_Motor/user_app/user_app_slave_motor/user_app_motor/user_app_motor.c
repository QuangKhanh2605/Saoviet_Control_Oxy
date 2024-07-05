#include "user_app_motor.h"
#include "user_define.h"
#include "tim.h"
#include "user_adc.h"
#include "iwdg.h"

/*============== Function static ===============*/
static uint8_t fevent_motor_entry(uint8_t event);
static uint8_t fevent_check_man_auto(uint8_t event);

static uint8_t fevent_sampling_amperage(uint8_t event);
static uint8_t fevent_motor_check_amperage(uint8_t event);
static uint8_t fevent_on_motor(uint8_t event);

static uint8_t fevent_sampling_pressure(uint8_t event);
static uint8_t fevent_motor_check_pressure(uint8_t event);

static uint8_t fevent_motor_send_error(uint8_t event);

static uint8_t fevent_motor_led_status(uint8_t event);
static uint8_t fevent_motor_iwdg(uint8_t event);
static uint8_t fevent_reset_dcu(uint8_t event);
/*=================== struct ==================*/
sEvent_struct               sEventAppMotor[] = 
{
  {_EVENT_MOTOR_ENTRY,          1, 5, 0,              fevent_motor_entry},
  
  {_EVENT_CHECK_MAN_AUTO,       1, 5, 500,            fevent_check_man_auto},
  
  {_EVENT_SAMPLING_AMPERAGE,    0, 5, 500,            fevent_sampling_amperage},
  {_EVENT_MOTOR_CHECK_AMPERAGE, 0, 5, 5,              fevent_motor_check_amperage},
  {_EVENT_ON_MOTOR,             0, 5, 5*60000,        fevent_on_motor},
  
  {_EVENT_SAMPLING_PRESSURE,    0, 5, 1000,           fevent_sampling_pressure},
  {_EVENT_MOTOR_CHECK_PRESSURE, 0, 5, 5,              fevent_motor_check_pressure},
  
  {_EVENT_MOTOR_SEND_ERROR,     1, 5, 500,            fevent_motor_send_error},

  {_EVENT_MOTOR_LED_STATUS,     1, 5, 2000,           fevent_motor_led_status},
  {_EVENT_MOTOR_IWDG,           1, 5, 1000,           fevent_motor_iwdg},
  {_EVENT_RESET_DCU,            1, 5, TIME_RESET_DCU, fevent_reset_dcu},
};

structMeasureMotor          sMeasureMotor = {0};
structParamMotor            sParamMotor = {0};
structParamADC              sParamADC = {0};
/*================= Function Handle ==============*/
static uint8_t fevent_motor_entry(uint8_t event)
{
    HAL_TIM_Base_Start_IT(&htim2);
//    fevent_enable(sEventAppMotor, _EVENT_SAMPLING_AMPERAGE);
    fevent_enable(sEventAppMotor, _EVENT_SAMPLING_PRESSURE);
    return 1;
}

static uint8_t fevent_check_man_auto(uint8_t event)
{
    static uint8_t StateBefore = _MACHINE_OFF;
    
    if(HAL_GPIO_ReadPin(MCU_SW_MAN_GPIO_Port, MCU_SW_MAN_Pin) == 0)
    {
        sParamMotor.StateMachine = _MACHINE_AUTO;
    }
    else if(HAL_GPIO_ReadPin(MCU_SW_AUTO_GPIO_Port, MCU_SW_AUTO_Pin) == 0)
    {
        sParamMotor.StateMachine = _MACHINE_MAN;
    }
    else
    {
        sParamMotor.StateMachine = _MACHINE_OFF;
    }
    
    if(StateBefore != sParamMotor.StateMachine)
    {
        switch(sParamMotor.StateMachine)
        {
            case _MACHINE_AUTO:
                fevent_active(sEventAppMotor, _EVENT_ON_MOTOR);
                fevent_disable(sEventAppMotor, _EVENT_SAMPLING_AMPERAGE);
                fevent_disable(sEventAppMotor, _EVENT_MOTOR_CHECK_AMPERAGE);
                break;
                
            case _MACHINE_MAN:
            case _MACHINE_OFF:
                OFF_Motor_3_Phase();
                fevent_disable(sEventAppMotor, _EVENT_ON_MOTOR);
                fevent_disable(sEventAppMotor, _EVENT_SAMPLING_AMPERAGE);
                fevent_disable(sEventAppMotor, _EVENT_MOTOR_CHECK_AMPERAGE);
                break;
                
            default:
              break;
        }
        sParamMotor.StatusAmperage = _MOTOR_OK;
        StateBefore = sParamMotor.StateMachine;
    }
    
    fevent_enable(sEventAppMotor, event);
    return 1;
}

static uint8_t fevent_sampling_amperage(uint8_t event)
{
    uint16_t Imax = 0;
    uint16_t Imin = 0;
    uint32_t Delta_Amperage = 0;
    
    if(sParamMotor.StateMotor == _MOTOR_OFF)
    {
        ON_Motor_3_Phase();
    }
    
    Imax = sMeasureMotor.AmperagePhase_1;
    if(Imax < sMeasureMotor.AmperagePhase_2)
      Imax = sMeasureMotor.AmperagePhase_2;
    if(Imax < sMeasureMotor.AmperagePhase_3)
      Imax = sMeasureMotor.AmperagePhase_3;
    
      
    Imin = sMeasureMotor.AmperagePhase_1;
    if(Imin > sMeasureMotor.AmperagePhase_2)
      Imin = sMeasureMotor.AmperagePhase_2;
    if(Imin > sMeasureMotor.AmperagePhase_3)
      Imin = sMeasureMotor.AmperagePhase_3;
    
    if(Imax > sParamMotor.Amperage_Upper)
    {
        if(sParamMotor.CountFailAmperage < MAX_NUMBER_FAIL_AMPERAGE)
            sParamMotor.CountFailAmperage++;
    }
    
    Delta_Amperage = ((Imax - Imin)*100)/Imax;
    
    if(Delta_Amperage > sParamMotor.Amperage_Delta)
    {
        if(sParamMotor.CountFailAmperage < MAX_NUMBER_FAIL_AMPERAGE)
            sParamMotor.CountFailAmperage++;
    }
    else
    {
        sParamMotor.CountFailAmperage = 0;
    }
       
    fevent_active(sEventAppMotor, _EVENT_MOTOR_CHECK_AMPERAGE);
    
    return 1;
}

static uint8_t fevent_motor_check_amperage(uint8_t event)
{
    if(sParamMotor.CountFailAmperage >= MAX_NUMBER_FAIL_AMPERAGE)
    {
        sParamMotor.StatusAmperage = _MOTOR_FAIL;
        OFF_Motor_3_Phase();
        sParamMotor.CountFailAmperage = 0;
        fevent_enable(sEventAppMotor, _EVENT_ON_MOTOR);
    }
    else if(sParamMotor.CountFailAmperage == 0)
    {
        if(sParamMotor.StateMotor == _MOTOR_OFF)
          ON_Motor_3_Phase();
        
        sParamMotor.StatusAmperage = _MOTOR_OK; 
        fevent_enable(sEventAppMotor, _EVENT_SAMPLING_AMPERAGE);
    }
    else
    {
        fevent_enable(sEventAppMotor, _EVENT_SAMPLING_AMPERAGE);
    }
    
    if(sMeasureMotor.AmperagePhase_1 <=2 && sMeasureMotor.AmperagePhase_2 <=2 &&sMeasureMotor.AmperagePhase_3 <=2)
    {
        if(sParamMotor.CountPowerOff < 5)
          sParamMotor.CountPowerOff++;
    }
    else
        sParamMotor.CountPowerOff = 0;
    
    if(sParamMotor.CountPowerOff >= 5)
    {
        sParamMotor.StatusAmperage = _MOTOR_FAIL; 
        sParamMotor.CountFailAmperage = 0;
    }
    
    if(sParamMotor.Amperage_Option == _AMPERAGE_OPTION_OFF)
    {
        sParamMotor.CountPowerOff = 0;
        sParamMotor.CountFailAmperage = 0;
        if(sParamMotor.StateMotor == _MOTOR_OFF)
          ON_Motor_3_Phase();
        
        sParamMotor.StatusAmperage = _MOTOR_OK; 
        fevent_enable(sEventAppMotor, _EVENT_SAMPLING_AMPERAGE);
        fevent_disable(sEventAppMotor, _EVENT_ON_MOTOR);
    }
  
    return 1;
}

static uint8_t fevent_on_motor(uint8_t event)
{
    ON_Motor_3_Phase();
    sParamMotor.CountFailAmperage = 0;
    sParamMotor.CountPowerOff = 0;
    fevent_enable(sEventAppMotor, _EVENT_SAMPLING_AMPERAGE);
    return 1;
}

static uint8_t fevent_sampling_pressure(uint8_t event)
{
    uint16_t MeasurePressure = 0;
    static uint32_t GettickPressure = 0;
    if(sParamMotor.StatusAmperage == _MOTOR_FAIL)
    {
        GettickPressure = HAL_GetTick();
    }
    switch(sParamMotor.Pressure_Option)
    {
        case _PRESSURE_OPTION_OFF:
        case _PRESSURE_OPTION_P1:
          MeasurePressure = sMeasureMotor.Pressure_P1;
          break;
          
        case _PRESSURE_OPTION_P2:
          MeasurePressure = sMeasureMotor.Pressure_P2;
          break;
          
        case _PRESSURE_OPTION_P3:
          MeasurePressure = sMeasureMotor.Pressure_P3;
          break;
          
        case _PRESSURE_OPTION_P4:
          MeasurePressure = sMeasureMotor.Pressure_P4;
          break;
          
        default:
          break;
    }  
    
    if(HAL_GetTick() - GettickPressure > 60000 || sParamMotor.Amperage_Option == _AMPERAGE_OPTION_OFF)
    {
        if(MeasurePressure < sParamMotor.Pressure_Lower || MeasurePressure > sParamMotor.Pressure_Upper)
        {
            if(sParamMotor.CountFailPressure < MAX_NUMBER_FAIL_PRESSURE)
                sParamMotor.CountFailPressure++;
        }
        else
            sParamMotor.CountFailPressure = 0;
    }
    else
    {
        sParamMotor.CountFailPressure = 0;
    }
    
    fevent_active(sEventAppMotor, _EVENT_MOTOR_CHECK_PRESSURE);
  
    return 1;
}

static uint8_t fevent_motor_check_pressure(uint8_t event)
{
    if(sParamMotor.CountFailPressure >= MAX_NUMBER_FAIL_PRESSURE)
      sParamMotor.StatusPressure = _MOTOR_FAIL;
    else if(sParamMotor.CountFailPressure == 0)
      sParamMotor.StatusPressure = _MOTOR_OK;
    
    if(sParamMotor.Pressure_Option == _PRESSURE_OPTION_OFF)
    {
        sParamMotor.StatusPressure = _MOTOR_OK;
    }
    
    fevent_enable(sEventAppMotor, _EVENT_SAMPLING_PRESSURE);
      
    return 1;
}

static uint8_t fevent_motor_send_error(uint8_t event)
{
    static uint32_t gettick_state_motor = 0;
    static uint8_t StateSensorMotorBefore = _MOTOR_OK;
    uint8_t StatusMotor = _MOTOR_OK;

    if(sParamMotor.StatusAmperage == _MOTOR_FAIL )
      StatusMotor = _MOTOR_FAIL;
    else if(sParamMotor.StatusPressure == _MOTOR_FAIL)
      StatusMotor = _MOTOR_FAIL;
    else
      StatusMotor = _MOTOR_OK;
    
    if(StatusMotor == _MOTOR_OK)
      OFF_Warning();
    else
      ON_Warning();
    
    if(StatusMotor != StateSensorMotorBefore)
    {
        gettick_state_motor = HAL_GetTick();
        
        if(StatusMotor == _MOTOR_FAIL)      
        {
            Send_RS458_Sensor((uint8_t*)"at+motor=fail", 13);
        }
 
        StateSensorMotorBefore = StatusMotor;
    }
    
    if(StatusMotor == _MOTOR_FAIL)
    {
        if(HAL_GetTick() - gettick_state_motor >= TIME_RESEND_WARNING*60000)
        {
            gettick_state_motor = HAL_GetTick();
            Send_RS458_Sensor((uint8_t*)"at+motor=fail", 13);
        }
    }
    
    fevent_enable(sEventAppMotor, event);
    return 1;
}

static uint8_t fevent_motor_led_status(uint8_t event)
{
    if(sEventAppMotor[_EVENT_MOTOR_LED_STATUS].e_period == 2000)
    {
        HAL_GPIO_WritePin(LED_STATUS_PORT, LED_STATUS_PIN, GPIO_PIN_SET);
        sEventAppMotor[_EVENT_MOTOR_LED_STATUS].e_period = 40;
    }
    else
    {
        HAL_GPIO_WritePin(LED_STATUS_PORT, LED_STATUS_PIN, GPIO_PIN_RESET);
        sEventAppMotor[_EVENT_MOTOR_LED_STATUS].e_period = 2000;
    }
    
    fevent_enable(sEventAppMotor, event);
    return 1;
}

static uint8_t fevent_motor_iwdg(uint8_t event)
{
    HAL_IWDG_Refresh(&hiwdg);
    fevent_enable(sEventAppMotor, event);
    return 1;
}

static uint8_t fevent_reset_dcu(uint8_t event)
{
    Reset_Chip();
    return 1;
}

/*======================On/Off Motor 3 phase====================*/
void ON_Warning(void)
{
    HAL_GPIO_WritePin(MCU_RL3_GPIO_Port, MCU_RL3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MCU_RL4_GPIO_Port, MCU_RL4_Pin, GPIO_PIN_SET);
}

void OFF_Warning(void)
{
    HAL_GPIO_WritePin(MCU_RL3_GPIO_Port, MCU_RL3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MCU_RL4_GPIO_Port, MCU_RL4_Pin, GPIO_PIN_RESET);
}

void ON_Motor_3_Phase(void)
{
    HAL_GPIO_WritePin(MCU_RL1_GPIO_Port, MCU_RL1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MCU_RL2_GPIO_Port, MCU_RL2_Pin, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(MCU_RL3_GPIO_Port, MCU_RL3_Pin, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(MCU_RL4_GPIO_Port, MCU_RL4_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_MOTOR_PORT, LED_MOTOR_PIN, GPIO_PIN_SET);
    sParamMotor.StateMotor = _MOTOR_ON;
}

void OFF_Motor_3_Phase(void)
{
    HAL_GPIO_WritePin(MCU_RL1_GPIO_Port, MCU_RL1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MCU_RL2_GPIO_Port, MCU_RL2_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(MCU_RL3_GPIO_Port, MCU_RL3_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(MCU_RL4_GPIO_Port, MCU_RL4_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_MOTOR_PORT, LED_MOTOR_PIN, GPIO_PIN_RESET);
    sParamMotor.StateMotor = _MOTOR_OFF;
}

/*======================Save and Init Amperage Calib==================*/
void        Save_AmperageCalib(uint16_t Point_0_Measure, uint16_t Point_0_Real, uint16_t Slope_Measure, uint16_t Slope_Real)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamMotor.Amperage_Calib_Point_0_Measure = Point_0_Measure;
    sParamMotor.Amperage_Calib_Point_0_Real    = Point_0_Real;
    sParamMotor.Amperage_Calib_Slope_Measure   = Slope_Measure;
    sParamMotor.Amperage_Calib_Slope_Real      = Slope_Real;
    
    aData[length++] = sParamMotor.Amperage_Calib_Point_0_Measure >> 8;
    aData[length++] = sParamMotor.Amperage_Calib_Point_0_Measure;
    aData[length++] = sParamMotor.Amperage_Calib_Point_0_Real >> 8;
    aData[length++] = sParamMotor.Amperage_Calib_Point_0_Real;

    aData[length++] = sParamMotor.Amperage_Calib_Slope_Measure >> 8;
    aData[length++] = sParamMotor.Amperage_Calib_Slope_Measure;
    aData[length++] = sParamMotor.Amperage_Calib_Slope_Real >> 8;
    aData[length++] = sParamMotor.Amperage_Calib_Slope_Real;
    
    Save_Array(ADDR_AMPERAGE_CALIB, aData, length);
    Stamp_Menu_Exit();
}

void        Init_AmperageCalib(void)
{
    if(*(__IO uint8_t*)(ADDR_AMPERAGE_CALIB) == BYTE_TEMP_FIRST)
    {
        sParamMotor.Amperage_Calib_Point_0_Measure  = *(__IO uint8_t*)(ADDR_AMPERAGE_CALIB+2) <<8;
        sParamMotor.Amperage_Calib_Point_0_Measure |= *(__IO uint8_t*)(ADDR_AMPERAGE_CALIB+3);
      
        sParamMotor.Amperage_Calib_Point_0_Real  = *(__IO uint8_t*)(ADDR_AMPERAGE_CALIB+4) <<8;
        sParamMotor.Amperage_Calib_Point_0_Real |= *(__IO uint8_t*)(ADDR_AMPERAGE_CALIB+5);
        
        sParamMotor.Amperage_Calib_Slope_Measure  = *(__IO uint8_t*)(ADDR_AMPERAGE_CALIB+6) <<8;
        sParamMotor.Amperage_Calib_Slope_Measure |= *(__IO uint8_t*)(ADDR_AMPERAGE_CALIB+7);
      
        sParamMotor.Amperage_Calib_Slope_Real  = *(__IO uint8_t*)(ADDR_AMPERAGE_CALIB+8) <<8;
        sParamMotor.Amperage_Calib_Slope_Real |= *(__IO uint8_t*)(ADDR_AMPERAGE_CALIB+9);
    }
    else
    {
    sParamMotor.Amperage_Calib_Point_0_Measure = AMPERAGE_CALIB_POINT_0_MEASURE_DEFAULT;
    sParamMotor.Amperage_Calib_Point_0_Real    = AMPERAGE_CALIB_POINT_0_REAL_DEFAULT;
    sParamMotor.Amperage_Calib_Slope_Measure   = AMPERAGE_CALIB_SLOPE_MEASURE_DEFAULT;
    sParamMotor.Amperage_Calib_Slope_Real      = AMPERAGE_CALIB_SLOPE_REAL_DEFAULT;
    }
    Stamp_Menu_Exit();
}

/*======================Save and Init Amperage==================*/
void        Save_AmperageWarning(uint16_t Amperage_Upper, uint16_t Amperage_Delta)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamMotor.Amperage_Upper = Amperage_Upper;
    sParamMotor.Amperage_Delta = Amperage_Delta;
    
    aData[length++] = sParamMotor.Amperage_Upper >> 8;
    aData[length++] = sParamMotor.Amperage_Upper;
    aData[length++] = sParamMotor.Amperage_Delta >> 8;
    aData[length++] = sParamMotor.Amperage_Delta;

    Save_Array(ADDR_THRESH_AMPERAGE, aData, length);
    Stamp_Menu_Exit();
}

void        Init_AmperageWarning(void)
{
    if(*(__IO uint8_t*)(ADDR_THRESH_AMPERAGE) == BYTE_TEMP_FIRST)
    {
        sParamMotor.Amperage_Upper  = *(__IO uint8_t*)(ADDR_THRESH_AMPERAGE+2) <<8;
        sParamMotor.Amperage_Upper |= *(__IO uint8_t*)(ADDR_THRESH_AMPERAGE+3);
      
        sParamMotor.Amperage_Delta  = *(__IO uint8_t*)(ADDR_THRESH_AMPERAGE+4) <<8;
        sParamMotor.Amperage_Delta |= *(__IO uint8_t*)(ADDR_THRESH_AMPERAGE+5);
    }
    else
    {
        sParamMotor.Amperage_Delta = AMPERAGE_DELTA_DEFAULT;
        sParamMotor.Amperage_Upper = AMPERAGE_UPPER_DEFAULT;
    }
    Stamp_Menu_Exit();
}

/*======================Save and Init Option==================*/
void        Save_OptionWarning(uint16_t Option_Amperage, uint16_t Option_Pressure)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamMotor.Amperage_Option = Option_Amperage;
    sParamMotor.Pressure_Option = Option_Pressure;
    
    aData[length++] = sParamMotor.Amperage_Option >> 8;
    aData[length++] = sParamMotor.Amperage_Option;
    
    aData[length++] = sParamMotor.Pressure_Option >> 8;
    aData[length++] = sParamMotor.Pressure_Option;

    Save_Array(ADDR_OPTION_SETTING, aData, length);
    Stamp_Menu_Exit();
}

void        Init_OptionWarning(void)
{
    if(*(__IO uint8_t*)(ADDR_OPTION_SETTING) == BYTE_TEMP_FIRST)
    {
        sParamMotor.Amperage_Option  = *(__IO uint8_t*)(ADDR_OPTION_SETTING+2) <<8;
        sParamMotor.Amperage_Option |= *(__IO uint8_t*)(ADDR_OPTION_SETTING+3);
        
        sParamMotor.Pressure_Option  = *(__IO uint8_t*)(ADDR_OPTION_SETTING+4) <<8;
        sParamMotor.Pressure_Option |= *(__IO uint8_t*)(ADDR_OPTION_SETTING+5);
    }
    else
    {
        sParamMotor.Amperage_Option = _AMPERAGE_OPTION_ON;
        sParamMotor.Pressure_Option = _PRESSURE_OPTION_P1;
    }
    Stamp_Menu_Exit();
}

/*======================Save and Init Pressure==================*/
void        Save_PressureWarning(uint16_t Pressure_Upper, uint16_t Pressure_Lower)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    sParamMotor.Pressure_Upper = Pressure_Upper;
    sParamMotor.Pressure_Lower = Pressure_Lower;
    
    aData[length++] = sParamMotor.Pressure_Upper >> 8;
    aData[length++] = sParamMotor.Pressure_Upper;
    
    aData[length++] = sParamMotor.Pressure_Lower >> 8;
    aData[length++] = sParamMotor.Pressure_Lower;

    Save_Array(ADDR_THRESH_PRESSURE, aData, length);
    Stamp_Menu_Exit();
}

void        Init_PressureWarning(void)
{
    if(*(__IO uint8_t*)(ADDR_THRESH_PRESSURE) == BYTE_TEMP_FIRST)
    {
        sParamMotor.Pressure_Upper  = *(__IO uint8_t*)(ADDR_THRESH_PRESSURE+2) <<8;
        sParamMotor.Pressure_Upper |= *(__IO uint8_t*)(ADDR_THRESH_PRESSURE+3);
        
        sParamMotor.Pressure_Lower  = *(__IO uint8_t*)(ADDR_THRESH_PRESSURE+4) <<8;
        sParamMotor.Pressure_Lower |= *(__IO uint8_t*)(ADDR_THRESH_PRESSURE+5);
    }
    else
    {
        sParamMotor.Pressure_Upper = PRESSURE_UPPER_DEFAULT;
        sParamMotor.Pressure_Lower = PRESSURE_LOWER_DEFAULT;
    }
    Stamp_Menu_Exit();
}

uint32_t Algorithm_Amperage_to_Pressure(uint16_t Amperage_Pressure, uint16_t A_x, uint16_t A_y, uint16_t B_x, uint16_t B_y)
{
    if(Amperage_Pressure > A_x)
    {
        uint32_t Pressure = 0;
        Pressure = ((B_y - A_y)*(Amperage_Pressure - A_x))/(B_x - A_x) + A_y;
        return Pressure;
    }
    else
      return 0;
}

uint32_t Algorihm_Amperage_Calib(uint16_t Amperage_Measure, uint16_t A_x, uint16_t A_y, uint16_t B_x, uint16_t B_y)
{
    if(Amperage_Measure > A_x)
    {
        uint32_t Result = 0;
        Result = ((B_y - A_y)*(Amperage_Measure - A_x))/(B_x - A_x) + A_y;
        return Result;
    }
    else
      return 0;
}

/*===================== Sampling ADC ====================*/
uint32_t current_v_1 = 0;
uint32_t current_v_2 = 0;
uint32_t current_v_3 = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == htim2.Instance) 
    {
      sParamADC.Number_Sampling++;
      
      if(sParamADC.ADCScanVal[7] >= sParamADC.ADCScanVal[8])
        sParamADC.ADC_AVG_Amperage_I1 = sParamADC.ADC_AVG_Amperage_I1 + (sParamADC.ADCScanVal[7] - sParamADC.ADCScanVal[8]);
      else
        sParamADC.ADC_AVG_Amperage_I1 = sParamADC.ADC_AVG_Amperage_I1 + (sParamADC.ADCScanVal[8] - sParamADC.ADCScanVal[7]);
      
      if(sParamADC.ADCScanVal[6] >= sParamADC.ADCScanVal[8])
        sParamADC.ADC_AVG_Amperage_I2 = sParamADC.ADC_AVG_Amperage_I2 + (sParamADC.ADCScanVal[6] - sParamADC.ADCScanVal[8]);
      else
        sParamADC.ADC_AVG_Amperage_I2 = sParamADC.ADC_AVG_Amperage_I2 + (sParamADC.ADCScanVal[8] - sParamADC.ADCScanVal[6]);
      
      if(sParamADC.ADCScanVal[5] >= sParamADC.ADCScanVal[8])
        sParamADC.ADC_AVG_Amperage_I3 = sParamADC.ADC_AVG_Amperage_I3 + (sParamADC.ADCScanVal[5] - sParamADC.ADCScanVal[8]);
      else
        sParamADC.ADC_AVG_Amperage_I3 = sParamADC.ADC_AVG_Amperage_I3 + (sParamADC.ADCScanVal[8] - sParamADC.ADCScanVal[5]);
      
      sParamADC.ADC_AVG_Pressure_Ap1 = sParamADC.ADC_AVG_Pressure_Ap1 + sParamADC.ADCScanVal[1];
      sParamADC.ADC_AVG_Pressure_Ap2 = sParamADC.ADC_AVG_Pressure_Ap2 + sParamADC.ADCScanVal[2];
      
      if(sParamADC.Number_Sampling == NUMBER_SAMPLING_AVG_ADC)
      {
        sParamADC.ADC_AVG_Amperage_I1 = sParamADC.ADC_AVG_Amperage_I1/NUMBER_SAMPLING_AVG_ADC;
        sParamADC.ADC_AVG_Amperage_I2 = sParamADC.ADC_AVG_Amperage_I2/NUMBER_SAMPLING_AVG_ADC;
        sParamADC.ADC_AVG_Amperage_I3 = sParamADC.ADC_AVG_Amperage_I3/NUMBER_SAMPLING_AVG_ADC;
        
        sParamADC.ADC_AVG_Pressure_Ap1 = sParamADC.ADC_AVG_Pressure_Ap1/NUMBER_SAMPLING_AVG_ADC;
        sParamADC.ADC_AVG_Pressure_Ap2 = sParamADC.ADC_AVG_Pressure_Ap2/NUMBER_SAMPLING_AVG_ADC;
        
        sParamADC.batteryLevelmV = (((uint32_t) VDDA_VREFINT_CAL * (*VREFINT_CAL)) / sParamADC.ADCScanVal[0]);
        
        current_v_1 = ((sParamADC.ADC_AVG_Amperage_I1 * sParamADC.batteryLevelmV)/4095)*1000;
        current_v_1 = current_v_1/(2*51);
        sMeasureMotor.AmperagePhase_1_ADC = current_v_1/100;
        if( (current_v_1/10)%10 >= 5) 
          sMeasureMotor.AmperagePhase_1_ADC++;
        
        current_v_2 = ((sParamADC.ADC_AVG_Amperage_I2 * sParamADC.batteryLevelmV)/4095)*1000;
        current_v_2 = current_v_2/(2*51);
        sMeasureMotor.AmperagePhase_2_ADC = current_v_2/100;
        if( (current_v_2/10)%10 >= 5) 
          sMeasureMotor.AmperagePhase_2_ADC++;
        
        current_v_3 = ((sParamADC.ADC_AVG_Amperage_I3 * sParamADC.batteryLevelmV)/4095)*1000;
        current_v_3 = current_v_3/(2*51);
        sMeasureMotor.AmperagePhase_3_ADC = current_v_3/100;
        if( (current_v_3/10)%10 >= 5) 
          sMeasureMotor.AmperagePhase_3_ADC++;

        sParamADC.Amperage_Pressure_Ap1 = ((((sParamADC.ADC_AVG_Pressure_Ap1 * sParamADC.batteryLevelmV))/4095)*1000)/120;
        sParamADC.Amperage_Pressure_Ap2 = ((((sParamADC.ADC_AVG_Pressure_Ap2 * sParamADC.batteryLevelmV))/4095)*1000)/120;
        
        sMeasureMotor.Pressure_P1 = Algorithm_Amperage_to_Pressure(sParamADC.Amperage_Pressure_Ap1, 4000, 0, 20000, 50);
        sMeasureMotor.Pressure_P2 = Algorithm_Amperage_to_Pressure(sParamADC.Amperage_Pressure_Ap2, 4000, 0, 20000, 100);
        
        sParamADC.ADC_AVG_Amperage_I1 = 0;
        sParamADC.ADC_AVG_Amperage_I2 = 0;
        sParamADC.ADC_AVG_Amperage_I3 = 0;
        current_v_1 = 0;
        current_v_2 = 0;
        current_v_3 = 0;
        sParamADC.Amperage_Pressure_Ap1 = 0;
        sParamADC.Amperage_Pressure_Ap2 = 0;
        sParamADC.Number_Sampling = 0;
        
        sMeasureMotor.AmperagePhase_1 = Algorihm_Amperage_Calib(sMeasureMotor.AmperagePhase_1_ADC, sParamMotor.Amperage_Calib_Point_0_Measure, \
                                                                sParamMotor.Amperage_Calib_Point_0_Real, sParamMotor.Amperage_Calib_Slope_Measure, \
                                                                sParamMotor.Amperage_Calib_Slope_Real);
        
        sMeasureMotor.AmperagePhase_2 = Algorihm_Amperage_Calib(sMeasureMotor.AmperagePhase_2_ADC, sParamMotor.Amperage_Calib_Point_0_Measure, \
                                                                sParamMotor.Amperage_Calib_Point_0_Real, sParamMotor.Amperage_Calib_Slope_Measure, \
                                                                sParamMotor.Amperage_Calib_Slope_Real);
                
        sMeasureMotor.AmperagePhase_3 = Algorihm_Amperage_Calib(sMeasureMotor.AmperagePhase_3_ADC, sParamMotor.Amperage_Calib_Point_0_Measure, \
                                                                sParamMotor.Amperage_Calib_Point_0_Real, sParamMotor.Amperage_Calib_Slope_Measure, \
                                                                sParamMotor.Amperage_Calib_Slope_Real);
      }
    }
}

void Init_ADC_Timer(void)
{
    HAL_ADC_Start_DMA(&hadc, (uint32_t*)sParamADC.ADCScanVal, 9);
    HAL_GPIO_WritePin(ON_OFF_Vout_GPIO_Port, ON_OFF_Vout_Pin, GPIO_PIN_SET);
}

/*===================== Save and Init State Reset ====================*/
void    Save_StateReset(void)
{
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    
    
    aData[length++] = 0x01;
    aData[length++] = 0x01;
    
    Save_Array(ADDR_FLAG_RESET_DCU, aData, length);
}

void    Init_StateReset(void)
{
    if(*(__IO uint8_t*)(ADDR_FLAG_RESET_DCU) == BYTE_TEMP_FIRST)
    {
        sEventAppMotor[_EVENT_MOTOR_ENTRY].e_period = 0;
        OnchipFlashPageErase(ADDR_FLAG_RESET_DCU);
    }
    else
    {
        sEventAppMotor[_EVENT_MOTOR_ENTRY].e_period = 0;
    }
//    ON_Motor_3_Phase();
}

/*===================== Task and Init ===================*/
void    Init_AppMotor(void)
{
    Init_AmperageWarning();
    Init_OptionWarning();
    Init_PressureWarning();
    Init_ADC_Timer();
    Init_StateReset();
    Init_AmperageCalib();
}

uint8_t AppMotor_Task(void)
{
    uint8_t i = 0;
    uint8_t Result = false;
    for( i = 0; i < _EVENT_MOTOR_END; i++)
    {
        if(sEventAppMotor[i].e_status == 1)
        {
            Result = true;
            if((sEventAppMotor[i].e_systick == 0) ||
                ((HAL_GetTick() - sEventAppMotor[i].e_systick) >= sEventAppMotor[i].e_period))
            {
                sEventAppMotor[i].e_status = 0; //Disable event
                sEventAppMotor[i].e_systick= HAL_GetTick();
                sEventAppMotor[i].e_function_handler(i);
            }
        }
    }
    
    return Result;
}
