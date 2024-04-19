
/*
    8/2021
    Thu vien xu ly Uart
*/
#ifndef USER_AT_SERIAL_H
#define USER_AT_SERIAL_H

#include "user_util.h"
#include "usart.h"

/*======================== Define ======================*/

    
/*======================== Structs var======================*/
typedef enum
{
	_SET_DEV_SERIAL,
	_QUERY_DEV_SERIAL,
    
    _GET_BAUDRATE_SLAVE,
    _SET_BAUDRATE_SLAVE,
    
    _SET_TIME_RUN,
    _SET_TIME_FREE,
    
    _END_AT_CMD,    
}Type_Command_AT_PC;



typedef void (*_func_callback_f)(sData *str_Receiv, uint16_t Pos);

typedef struct {
	int 			    idStep;
	_func_callback_f	CallBack;
	sData	    sTempReceiver;				// temp Receive <- Sim900
} struct_CheckList_AT;

/*======================== External Var struct ======================*/
extern const struct_CheckList_AT CheckList_AT_CONFIG[];
extern uint8_t      aResPondAT[256];
extern uint8_t 		PortConfig;
extern uint8_t      aReceiAT[256];
extern uint16_t     LengthRecei;
extern uint16_t     LengthResPond;

/*======================== Function ======================*/
void        _fSET_DEV_SERIAL(sData *str_Receiv, uint16_t Pos);
void        _fQUERY_DEV_SERIAL(sData *str_Receiv, uint16_t Pos);
void        _fQUERY_SIM_ID(sData *strRecei, uint16_t Pos);

void 		_fSET_SERVER_MAIN_INFO (sData *strRecei, uint16_t Pos);
void 		_fQUERY_SERVER_MAIN_INFO (sData *strRecei, uint16_t Pos);

void       	_fSET_DUTY_READ_DATA(sData *str_Receiv, uint16_t Pos);
void        _fQUERY_DUTY_READ_DA(sData *str_Receiv, uint16_t Pos);

void        _fSET_FREQ_ONLINE(sData *strRecei, uint16_t Pos);
void        _fQUERY_FREQ_ONLINE(sData *strRecei, uint16_t Pos);

void        _fSET_DATAPULSE_DEFAUL(sData *str_Receiv, uint16_t Pos);
void        _fSET_NUM_WAKEUP_DEV(sData *str_Receiv, uint16_t Pos);
void        _fSET_SAVE_BOX(sData *str_Receiv, uint16_t Pos);
void 		_fREAD_OLD_DATA(sData *strRecei, uint16_t Pos);

void        _fSET_FLOW_THRESH (sData *str_Receiv, uint16_t Pos);
void        _fQUERY_FLOW_THRESH (sData *str_Receiv, uint16_t Pos);
void        _fSET_QUAN_THRESH (sData *str_Receiv, uint16_t Pos);
void        _fQUERY_QUAN_THRESH (sData *str_Receiv, uint16_t Pos);
void        _fSET_BAT_THRESH (sData *str_Receiv, uint16_t Pos);
void        _fQUERY_BAT_THRESH (sData *str_Receiv, uint16_t Pos);
void        _fSET_PULSE_VALUE (sData *str_Receiv, uint16_t Pos);
void        _fQUERY_PULSE_VALUE (sData *str_Receiv, uint16_t Pos);

void        _fSET_RTC_TIME (sData *str_Receiv, uint16_t Pos);
void        _fSET_RTC_DATE (sData *str_Receiv, uint16_t Pos);
void        _fSET_RTC (sData *str_Receiv, uint16_t Pos);
void        _fQUERY_RTC (sData *str_Receiv, uint16_t Pos);

void        _fRESET_MODEM (sData *str_Receiv, uint16_t Pos);
void        _fERASE_MEMORY (sData *str_Receiv, uint16_t Pos);

void        _fQUERY_FIRM_VER (sData *strRecei, uint16_t Pos);
void        _fGET_URLHTTP_FIRM (sData *strRecei, uint16_t Pos);
void        _fGET_URLFTP_FIRM (sData *strRecei, uint16_t Pos);

void        _fGET_LAST_LOG (sData *strRecei, uint16_t Pos);
void        _fQUERY_ALL_LOG (sData *strRecei, uint16_t Pos);
void        _fQUERY_INDEX_LOG (sData *strRecei, uint16_t Pos);
void        _fGET_LAST_FOR_LOG (sData *strRecei, uint16_t Pos);
void        _fTEST_LOG (sData *strRecei, uint16_t Pos);

void        _fGET_LAST_FOR_OPERA (sData *strRecei, uint16_t Pos);
void        _fGET_LAST_FOR_EVENT (sData *strRecei, uint16_t Pos);

void        _fSET_CALIB_LEVEL (sData *strRecei, uint16_t Pos);
void        _fQUERY_CALIB_LEVEL (sData *strRecei, uint16_t Pos);

void        _fSET_LEVEL_THRESH (sData *strRecei, uint16_t Pos);
void        _fQUERY_LEVEL_THRESH (sData *strRecei, uint16_t Pos);

uint8_t     Check_AT_User(sData *StrUartRecei, uint8_t Type);

void        _fSET_SERVER_BACKUP_INFO (sData *strRecei, uint16_t Pos);
void        _fQUERY_SERVER_BACKUP_INFO (sData *strRecei, uint16_t Pos);
void        _fREQ_SWITCH_SERVER (sData *strRecei, uint16_t Pos);
void        _fSET_RF_RQ_STIME (sData *str_Receiv, uint16_t Pos);

void        _fQUERY_GPS_LOC (sData *strRecei, uint16_t Pos);
void        _fGET_LAST_FOR_GPS (sData *strRecei, uint16_t Pos);

uint16_t    SERIAL_Get_Num (sData *strRecei, uint16_t Pos);
void        _fQUERY_LIST_ID_SLAVE (sData *str_Receiv, uint16_t Pos);
void        _fSET_LIST_ID_SLAVE (sData *str_Receiv, uint16_t Pos);

void        _fSET_EX_BAT_THRESH (sData *str_Receiv, uint16_t Pos);
void        _fQUERY_EX_BAT_THRESH (sData *str_Receiv, uint16_t Pos);

void        _fSET_POWER_ON_TEMH (sData *str_Receiv, uint16_t Pos);
void        _fSET_POWER_OFF_TEMH (sData *str_Receiv, uint16_t Pos);

/*=============Vending Machine=============*/
void        _fRESET_DCU (sData *strRecei, uint16_t Pos);
void        _fRESET_PCBOX (sData *strRecei, uint16_t Pos);
void        _fON_PCBOX (sData *strRecei, uint16_t Pos);
void        _fOFF_PCBOX (sData *strRecei, uint16_t Pos);

void        _fAPP_FIX_MOTOR (sData *strRecei, uint16_t Pos);
void        _fAPP_PUSH_MOTOR (sData *strRecei, uint16_t Pos);

void        _fGET_TIME_TSVH (sData *strRecei, uint16_t Pos);
void        _fSET_TIME_TSVH (sData *strRecei, uint16_t Pos);

void        _fGET_SETUP_TEMP (sData *strRecei, uint16_t Pos);
void        _fSET_SETUP_TEMP (sData *strRecei, uint16_t Pos);

void        _fGET_THRESH_TEMP (sData *strRecei, uint16_t Pos);
void        _fSET_THRESH_TEMP (sData *strRecei, uint16_t Pos);

void        _fGET_ID_SLAVE (sData *strRecei, uint16_t Pos);
void        _fSET_ID_SLAVE (sData *strRecei, uint16_t Pos);

void        _fCTRL_RELAY_SCREEN (sData *strRecei, uint16_t Pos);
void        _fCTRL_RELAY_LAMP (sData *strRecei, uint16_t Pos);
void        _fCTRL_RELAY_WARM (sData *strRecei, uint16_t Pos);

void        _fGET_TIME_WARM_RUN (sData *strRecei, uint16_t Pos);
void        _fSET_TIME_WARM_RUN (sData *strRecei, uint16_t Pos);

void        _fGET_TIME_WARM_WAIT (sData *strRecei, uint16_t Pos);
void        _fSET_TIME_WARM_WAIT (sData *strRecei, uint16_t Pos);

void        _fGET_USING_CRC (sData *strRecei, uint16_t Pos);
void        _fSET_USING_CRC (sData *strRecei, uint16_t Pos);

void        _fGET_DEBUG_TEMP (sData *strRecei, uint16_t Pos);
void        _fGET_DEBUG_ELECTRIC (sData *strRecei, uint16_t Pos);

void        _fGET_CYCLE_ON_OFF_PC (sData *strRecei, uint16_t Pos);
void        _fSET_CYCLE_ON_OFF_PC (sData *strRecei, uint16_t Pos);
void        _fGET_STATE_PCBOX (sData *strRecei, uint16_t Pos);

void        _fGET_BAUDRATE_SLAVE (sData *strRecei, uint16_t Pos);
void        _fSET_BAUDRATE_SLAVE (sData *strRecei, uint16_t Pos);

void        _fSET_TIME_RUN (sData *strRecei, uint16_t Pos);
void        _fSET_TIME_FREE (sData *strRecei, uint16_t Pos);
#endif

