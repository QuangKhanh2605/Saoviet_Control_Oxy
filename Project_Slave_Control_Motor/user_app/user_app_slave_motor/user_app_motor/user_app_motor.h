#ifndef USER_APP_MOTOR_H_
#define USER_APP_MOTOR_H_

#define USING_APP_MOTOR

#include "event_driven.h"
#include "user_util.h"

#define NUMBER_SAMPLING_AVG_ADC     5000

#define PRESSURE_UPPER_DEFAULT      30
#define PRESSURE_LOWER_DEFAULT      10

#define PRESSURE_UPPER_MAX          500
#define PRESSURE_LOWER_MIN          0

#define AMPERAGE_DELTA_MAX          100
#define AMPERAGE_DELTA_DEFAULT      30
#define AMPERGAE_DELTA_MIN          0

#define AMPERAGE_UPPER_MAX          200
#define AMPERAGE_UPPER_DEFAULT      80
#define AMPERGAE_UPPER_MIN          0

#define TIME_RESET_DCU              86400*10*1000
#define TIME_RESEND_WARNING         2

#define MAX_NUMBER_FAIL_AMPERAGE    8
#define MAX_NUMBER_FAIL_PRESSURE    5

#define AMPERAGE_CALIB_POINT_0_MEASURE_DEFAULT  0
#define AMPERAGE_CALIB_POINT_0_REAL_DEFAULT     0

#define AMPERAGE_CALIB_SLOPE_MEASURE_DEFAULT    45
#define AMPERAGE_CALIB_SLOPE_REAL_DEFAULT       50

#define LED_MOTOR_PORT              LED_2_GPIO_Port
#define LED_MOTOR_PIN               LED_2_Pin

#define LED_STATUS_PORT             LED_1_GPIO_Port
#define LED_STATUS_PIN              LED_1_Pin

typedef enum
{
    _EVENT_MOTOR_ENTRY,
    _EVENT_CHECK_MAN_AUTO,
    
    _EVENT_SAMPLING_AMPERAGE,
    _EVENT_MOTOR_CHECK_AMPERAGE,
    _EVENT_ON_MOTOR,
    
    _EVENT_SAMPLING_PRESSURE,
    _EVENT_MOTOR_CHECK_PRESSURE,
    
    _EVENT_MOTOR_SEND_ERROR,
    
    _EVENT_MOTOR_LED_STATUS,
    _EVENT_MOTOR_IWDG,
    _EVENT_RESET_DCU,

    _EVENT_MOTOR_END,
}eKindEventMotor;

typedef enum
{
    _AMPERAGE_OPTION_OFF,
    _AMPERAGE_OPTION_ON,
}eKindEventAmperagOption;

typedef enum
{
    _PRESSURE_OPTION_OFF,
    _PRESSURE_OPTION_P1,
    _PRESSURE_OPTION_P2,
    _PRESSURE_OPTION_P3,
    _PRESSURE_OPTION_P4,
}eKindEventPressureOption;

typedef enum
{
    _MOTOR_OK,
    _MOTOR_FAIL,
}eKindStatusMotor;

typedef enum
{
    _MOTOR_OFF,
    _MOTOR_ON,
}eKindStateMotor;

typedef enum
{
    _MACHINE_OFF,
    _MACHINE_MAN,
    _MACHINE_AUTO,
}eKindStateMachine;

typedef struct
{
    uint16_t AmperagePhase_1_ADC;
    uint16_t AmperagePhase_2_ADC;
    uint16_t AmperagePhase_3_ADC;
    
    uint16_t AmperagePhase_1;
    uint16_t AmperagePhase_2;
    uint16_t AmperagePhase_3;
    
    uint16_t Pressure_P1;
    uint16_t Pressure_P2;
    uint16_t Pressure_P3;
    uint16_t Pressure_P4;
}structMeasureMotor;

typedef struct
{
    uint8_t StateMotor;
    uint8_t StateMachine;
    
    uint8_t StatusAmperage;
    uint8_t StatusPressure;
    uint8_t CountPowerOff;
    uint8_t CountFailAmperage;
    uint8_t CountFailPressure;
    
    uint16_t Amperage_Upper;
    uint16_t Amperage_Delta;
    uint16_t Pressure_Upper;
    uint16_t Pressure_Lower;
    
    uint16_t Amperage_Option;
    uint16_t Pressure_Option;
   
    uint16_t Amperage_Calib_Point_0_Measure;
    uint16_t Amperage_Calib_Point_0_Real;
    uint16_t Amperage_Calib_Slope_Measure;
    uint16_t Amperage_Calib_Slope_Real;
    
}structParamMotor;

typedef struct
{
    uint16_t ADCScanVal[9];
    uint32_t Number_Sampling;
    uint32_t batteryLevelmV;
    uint64_t ADC_AVG_Amperage_I1;
    uint64_t ADC_AVG_Amperage_I2;
    uint64_t ADC_AVG_Amperage_I3;
    uint64_t ADC_AVG_Pressure_Ap1;
    uint64_t ADC_AVG_Pressure_Ap2;
    
    uint32_t Amperage_Pressure_Ap1;
    uint32_t Amperage_Pressure_Ap2;
}structParamADC;

extern sEvent_struct               sEventAppMotor[];

extern structMeasureMotor          sMeasureMotor;
extern structParamMotor            sParamMotor;
/*=============== Function handle ================*/
uint8_t     AppMotor_Task(void);
void        Init_AppMotor(void);
void        Init_ADC_Timer(void);

void        Save_PressureWarning(uint16_t Pressure_Upper, uint16_t Pressure_Lower);
void        Init_PressureWarning(void);

void        Save_OptionWarning(uint16_t Option_Amperage, uint16_t Option_Pressure);
void        Init_OptionWarning(void);

void        Save_AmperageWarning(uint16_t Amperage_Upper, uint16_t Amperage_Delta);
void        Init_AmperageWarning(void);

void        Save_AmperageCalib(uint16_t Point_0_Measure, uint16_t Point_0_Real, uint16_t Slope_Measure, uint16_t Slope_Real);
void        Init_AmperageCalib(void);

void        Save_StateReset(void);
void        Init_StateReset(void);

void        ON_Warning(void);
void        OFF_Warning(void);

void        ON_Motor_3_Phase(void);
void        OFF_Motor_3_Phase(void);

uint32_t    Algorithm_Amperage_to_Pressure(uint16_t Amperage_Pressure, uint16_t A_x, uint16_t A_y, uint16_t B_x, uint16_t B_y);
uint32_t    Algorihm_Amperage_Calib(uint16_t Amperage_Measure, uint16_t A_x, uint16_t A_y, uint16_t B_x, uint16_t B_y);
#endif

