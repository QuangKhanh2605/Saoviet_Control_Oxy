#include "user_app_rs485.h"
#include "user_modbus_rtu.h"
#include "user_define_oxygen.h"
#include "user_internal_mem.h"
#include "user_app_ctrl_oxy.h"
/*=========================Fucntion Static=========================*/
static uint8_t fevent_rs485_entry(uint8_t event);
static uint8_t fevent_rs485_transmit(uint8_t event);
static uint8_t fevent_rs485_receive_handle(uint8_t event);
static uint8_t fevent_rs485_receive_complete(uint8_t event);
static uint8_t fevent_reset_sensor_oxy(uint8_t event);
static uint8_t fevent_wait_sensor_oxy_on(uint8_t event);
/*==============================Struct=============================*/
sEvent_struct               sEventAppRs485[]=
{
  {_EVENT_RS485_ENTRY,              1, 5, 15000,            fevent_rs485_entry},            //Doi slave khoi dong moi truyen opera
  {_EVENT_RS485_TRANSMIT,           1, 0, 1500,             fevent_rs485_transmit},
  {_EVENT_RS485_RECEIVE_HANDLE,     0, 5, 5,                fevent_rs485_receive_handle},
  {_EVENT_RS485_RECEIVE_COMPLETE,   0, 5, 500,              fevent_rs485_receive_complete},
  
  {_EVENT_RESET_SENSOR_OXY,         0, 5, 60000*2,          fevent_reset_sensor_oxy},
  {_EVENT_WAIT_SENSOR_OXY_ON,       0, 5, 5000,             fevent_wait_sensor_oxy_on},
};
uint16_t CountBufferHandleRecv = 0;

Struct_IdSlave  sIdSlave485=
{
    .Oxy = ID_DEFAULT_OXY,
    .pH  = ID_DEFAULT_PH,
};

Struct_KindMode485  sKindMode485=
{
    .Trans = _RS485_OPERA_OXY,
};

/*========================Function Handle========================*/

static uint8_t fevent_rs485_entry(uint8_t event)
{
    fevent_enable(sEventAppRs485, _EVENT_RESET_SENSOR_OXY);
    return 1;
}

static uint8_t fevent_rs485_transmit(uint8_t event)
{
/*--------------------Hoi du lieu tu Slave-------------------*/

    uint8_t Frame[20]={0};
    uint8_t aData[4]={0};
    sData sFrame = {&Frame[0], 0};
    switch(sKindMode485.Trans)
    {
        case _RS485_OPERA_PH:
            ModRTU_Master_Read_Frame(&sFrame, sIdSlave485.pH, 0x03, 0x0001, 1);
            break;
      
        case _RS485_OPERA_OXY:
            ModRTU_Master_Read_Frame(&sFrame, sIdSlave485.Oxy, 0x03, 0x0003, 24);
            break;
            
        case _RS485_CALIB_OXY_100_CALIB:
            aData[0] = 0x00;
            aData[1] = 0x01;
            ModRTU_Master_Write_Frame(&sFrame, sIdSlave485.Oxy, 0x10, 0x0220 , 1, aData);
            break;
            
        case _RS485_CALIB_SALINITY:
            aData[0] = sMenuStamp.Salinity >> 8;
            aData[1] = sMenuStamp.Salinity;
            ModRTU_Master_Write_Frame(&sFrame, sIdSlave485.Oxy, 0x10, 0x0008, 1, aData);
            break;
            
        case _RS485_CALIB_TEMP:
            aData[0] = sMenuStamp.Temperature >> 8;
            aData[1] = sMenuStamp.Temperature;
            ModRTU_Master_Write_Frame(&sFrame, sIdSlave485.Oxy, 0x10, 0x000A , 1, aData);
            break;
      
        default:
          break;
    }
    sKindMode485.Recv = sKindMode485.Trans;
    //Transmit RS485
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_SET);
    HAL_Delay(5);
    UTIL_MEM_set(sUart485.Data_a8 , 0x00, sUart485.Length_u16);
    sUart485.Length_u16 = 0;
    CountBufferHandleRecv = 0;
    
    HAL_UART_Transmit(&uart_485, sFrame.Data_a8, sFrame.Length_u16, 1000);
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_RESET);
    
    if(sKindMode485.Trans == _RS485_OPERA_OXY)
        sKindMode485.Trans = _RS485_OPERA_PH;
    else if(sKindMode485.Trans == _RS485_OPERA_PH)
        sKindMode485.Trans = _RS485_OPERA_OXY;
    
    fevent_active(sEventAppRs485, _EVENT_RS485_RECEIVE_HANDLE);
    fevent_enable(sEventAppRs485, _EVENT_RS485_RECEIVE_COMPLETE);
    fevent_enable(sEventAppRs485, event);
    return 1;
}

static uint8_t fevent_rs485_receive_handle(uint8_t event)
{
/*-----------------Kiem tra da nhan xong tu 485------------*/
    if(sUart485.Length_u16 != 0)
    {
        if(CountBufferHandleRecv == sUart485.Length_u16)
        {
            CountBufferHandleRecv = 0;
            fevent_active(sEventAppRs485, _EVENT_RS485_RECEIVE_COMPLETE);
            return 1;
        }
        else
        {
            CountBufferHandleRecv = sUart485.Length_u16;
        }
    }
    
    fevent_enable(sEventAppRs485, event);
    return 1;
}

static uint8_t fevent_rs485_receive_complete(uint8_t event)
{
/*------------------Xu ly chuoi nhan duoc----------------*/
    static uint8_t CountDisconnectOxy = 0;
    static uint8_t CountDisconnectPH  = 0;
    uint16_t Oxy_Mg_L    = 0;
    uint16_t Oxy_Percent = 0;
    
    uint16_t Crc_Check = 0;
    uint16_t Crc_Recv  = 0;

    Crc_Recv = (sUart485.Data_a8[sUart485.Length_u16-1] << 8) |
               (sUart485.Data_a8[sUart485.Length_u16-2]);
    Crc_Check = ModRTU_CRC(sUart485.Data_a8, sUart485.Length_u16 - 2);
    if(Crc_Check == Crc_Recv)
    {
        if(sKindMode485.Recv == _RS485_OPERA_PH)
        {
            CountDisconnectPH = 0;
            sStateCtrlOxy.StateSensorPH = _OXY_CONNECT;
        }
        else
        {
            CountDisconnectOxy = 0;
            sStateCtrlOxy.StateSensorOxy = _OXY_CONNECT;
            fevent_enable(sEventAppRs485, _EVENT_RESET_SENSOR_OXY);
        }
        
        switch(sKindMode485.Recv)
        {
            case _RS485_OPERA_PH:
              sParamMeasure.pH_Water = sUart485.Data_a8[3]<<8 | sUart485.Data_a8[4];
              break;
          
            case _RS485_OPERA_OXY:
              //Doc Oxy Calib 2 diem
    //            sParamMeasure.Oxy_Mg_L = sUart485.Data_a8[45]<<8 | sUart485.Data_a8[46];
    //            sParamMeasure.Oxy_Percent = sUart485.Data_a8[47]<<8 | sUart485.Data_a8[48];
                
              //Doc Oxy Calib 1 diem bao hoa 100%
                
                Oxy_Mg_L = sUart485.Data_a8[3]<<8 | sUart485.Data_a8[4];
                Oxy_Percent = sUart485.Data_a8[9]<<8 | sUart485.Data_a8[10];

                sParamMeasure.Salinity = sUart485.Data_a8[13]<<8 | sUart485.Data_a8[14];
                sParamMeasure.Temp = sUart485.Data_a8[17]<<8 | sUart485.Data_a8[18];

                if(Oxy_Mg_L != 5000 && Oxy_Percent != 50000)
                {
                    Compensation_Salinity(&Oxy_Mg_L, sParamMeasure.Salinity, sParamMeasure.Temp);
//                    Average_MeasureOxy(Oxy_Mg_L, Oxy_Percent);
                    sParamMeasure.Oxy_Mg_L = Oxy_Mg_L;
                    sParamMeasure.Oxy_Percent = Oxy_Percent;
                }
                else
                {
                    sStateCtrlOxy.StateSensorOxy = _OXY_DISCONNECT;
                    sParamMeasure.Oxy_Mg_L = 1;
                    sParamMeasure.Oxy_Percent = 1;
                    sParamMeasure.Salinity = 1;
                }
                break;
                
            case _RS485_CALIB_OXY_100_CALIB:
            case _RS485_CALIB_SALINITY:
            case _RS485_CALIB_TEMP:
                sMenuState.CalibSensor = _CALIB_COMPLETE;
                sKindMode485.Trans = _RS485_OPERA_OXY;
                break;
          
            default:
              break;
        }
    }
    else
    {
        if(sKindMode485.Recv == _RS485_OPERA_PH)
        {
            CountDisconnectPH++;
        }
        else
        {
            CountDisconnectOxy++;
        }
    } 
    
    if(CountDisconnectOxy >= 3)
    {
        CountDisconnectOxy = 3;
        sStateCtrlOxy.StateSensorOxy = _OXY_DISCONNECT;
        sParamMeasure.Oxy_Mg_L = 0;
        sParamMeasure.Oxy_Percent = 0;
        sParamMeasure.Salinity = 0;
        sParamMeasure.Temp = 0;
        sKindMode485.Trans = _RS485_OPERA_OXY;
    }

    if(CountDisconnectPH >= 3)
    {
        CountDisconnectPH = 3;
        sStateCtrlOxy.StateSensorPH = _OXY_DISCONNECT;
        sParamMeasure.pH_Water = 0;
    }

    fevent_disable(sEventAppRs485, _EVENT_RS485_RECEIVE_HANDLE);
    return 1;
}

static uint8_t fevent_reset_sensor_oxy(uint8_t event)
{
    MX_USART1_UART_Init();
    Init_RX_Mode_Uart_485();
    
    HAL_GPIO_WritePin(ON_OFF_Vout_GPIO_Port, ON_OFF_Vout_Pin, GPIO_PIN_RESET);
    fevent_enable(sEventAppRs485, _EVENT_WAIT_SENSOR_OXY_ON);
    fevent_disable(sEventAppRs485, _EVENT_RS485_TRANSMIT);
    fevent_disable(sEventAppRs485, _EVENT_RS485_RECEIVE_HANDLE);
    fevent_disable(sEventAppRs485, _EVENT_RS485_RECEIVE_COMPLETE);
    return 1;
}

static uint8_t fevent_wait_sensor_oxy_on(uint8_t event)
{
    HAL_GPIO_WritePin(ON_OFF_Vout_GPIO_Port, ON_OFF_Vout_Pin, GPIO_PIN_SET);
    fevent_active(sEventAppRs485, _EVENT_RS485_TRANSMIT);
    fevent_enable(sEventAppRs485, _EVENT_RESET_SENSOR_OXY);
    sKindMode485.Trans = _RS485_OPERA_OXY;
    return 1;
}
/*==========================Handle==========================*/
void Average_MeasureOxy(uint16_t Oxy_Mg_L, uint16_t Oxy_Percent)
{
    uint32_t Oxy_Mg_L_Cal = 0;
    uint32_t Oxy_Percent_Cal = 0;
    Oxy_Mg_L_Cal = (sParamMeasure.Oxy_Mg_L + Oxy_Mg_L)/2;
    Oxy_Percent_Cal = (sParamMeasure.Oxy_Percent + Oxy_Percent)/2;
    sParamMeasure.Oxy_Mg_L = Oxy_Mg_L_Cal;
    sParamMeasure.Oxy_Percent = Oxy_Percent_Cal;
    
//    static uint16_t Oxy_Mg_L_Before = 0;
//    static uint16_t Oxy_Percent_Before = 0;
//    uint16_t Oxy_Mg_L_Stamp = 0;
//    uint16_t Oxy_Percent_Stamp = 0;
//    uint32_t Oxy_Mg_L_Cal = 0;
//    uint32_t Oxy_Percent_Cal = 0;
//    
//    Oxy_Mg_L_Stamp = sParamMeasure.Oxy_Mg_L;
//    Oxy_Percent_Stamp = sParamMeasure.Oxy_Percent;
//    
//    Oxy_Mg_L_Cal = (sParamMeasure.Oxy_Mg_L + Oxy_Mg_L + Oxy_Mg_L_Before)/3;
//    Oxy_Percent_Cal = (sParamMeasure.Oxy_Percent + Oxy_Percent + Oxy_Percent_Before)/3;
//    sParamMeasure.Oxy_Mg_L = Oxy_Mg_L_Cal;
//    sParamMeasure.Oxy_Percent = Oxy_Percent_Cal;
//      
//    Oxy_Mg_L_Before = Oxy_Mg_L_Stamp;
//    Oxy_Percent_Before = Oxy_Percent_Stamp;
}

void        Save_IdSlave(uint8_t ID_Oxy, uint8_t ID_pH)
{
    uint8_t aData[2] = {0};
    uint8_t length = 0;
    
    sIdSlave485.Oxy = ID_Oxy;
    sIdSlave485.pH  = ID_pH;
    aData[length++] = sIdSlave485.Oxy ;
    aData[length++] = sIdSlave485.pH;

    Save_Array(ADDR_INFOR_SLAVE_RS485, aData, length);
}

void        Init_IdSlave(void)
{
    if(*(__IO uint8_t*)(ADDR_INFOR_SLAVE_RS485) == BYTE_TEMP_FIRST)
    {
        sIdSlave485.Oxy = *(__IO uint8_t*)(ADDR_INFOR_SLAVE_RS485+2);
        sIdSlave485.pH  = *(__IO uint8_t*)(ADDR_INFOR_SLAVE_RS485+3);
    }
}


void       Init_AppRs485(void)
{
    Init_IdSlave();
}

uint8_t        AppRs485_Task(void)
{
    uint8_t i = 0;
    uint8_t Result =  false;
    
    for(i = 0; i < _EVENT_RS485_END; i++)
    {
        if(sEventAppRs485[i].e_status == 1)
        {
            Result = true; 
            
            if((sEventAppRs485[i].e_systick == 0) ||
               ((HAL_GetTick() - sEventAppRs485[i].e_systick) >= sEventAppRs485[i].e_period))
            {
                sEventAppRs485[i].e_status = 0; //Disable event
                sEventAppRs485[i].e_systick= HAL_GetTick();
                sEventAppRs485[i].e_function_handler(i);
            }
        }
    }
    
    return Result;
}

