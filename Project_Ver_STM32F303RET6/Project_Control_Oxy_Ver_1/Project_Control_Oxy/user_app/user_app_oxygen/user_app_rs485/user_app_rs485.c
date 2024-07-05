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

static uint8_t fevent_rs485_wait_calib(uint8_t event);

static uint8_t fevent_reset_sensor_oxy(uint8_t event);
static uint8_t fevent_wait_sensor_oxy_on(uint8_t event);
/*==============================Struct=============================*/
sEvent_struct               sEventAppRs485[]=
{
  {_EVENT_RS485_ENTRY,              1, 5, 15000,            fevent_rs485_entry},            //Doi slave khoi dong moi truyen opera
  {_EVENT_RS485_TRANSMIT,           0, 0, 2500,             fevent_rs485_transmit},
  {_EVENT_RS485_RECEIVE_HANDLE,     0, 5, 5,                fevent_rs485_receive_handle},
  {_EVENT_RS485_RECEIVE_COMPLETE,   0, 5, 500,              fevent_rs485_receive_complete},
  
  {_EVENT_RS485_WAIT_CALIB,         0, 5, 500,              fevent_rs485_wait_calib},
  
  {_EVENT_RESET_SENSOR_OXY,         0, 5, 60000*10,         fevent_reset_sensor_oxy},
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
    fevent_enable(sEventAppRs485, _EVENT_RS485_TRANSMIT);
    return 1;
}

static uint8_t fevent_rs485_transmit(uint8_t event)
{
/*--------------------Hoi du lieu tu Slave--------------------*/

    uint8_t Frame[20]={0};
    uint8_t aData[4]={0};
    sData sFrame = {&Frame[0], 0};
    switch(sKindMode485.Trans)
    {
        case _RS485_OPERA_PH:
            ModRTU_Master_Read_Frame(&sFrame, sIdSlave485.pH, 0x03, 0x0000, 0x04);
            break;
      
        case _RS485_OPERA_OXY:
            ModRTU_Master_Read_Frame(&sFrame, sIdSlave485.Oxy, 0x03, 0x0000, 0x06);
            break;
            
        case _RS485_CALIB_OXY_100_CALIB:
            aData[0] = 0x00;
            aData[1] = 0x00;
            ModRTU_Master_Write_Frame(&sFrame, sIdSlave485.Oxy, 0x06, 0x1004 , 1, aData);
            break;
            
//        case _RS485_CALIB_SALINITY:
//            aData[0] = sMenuStamp.Salinity >> 8;
//            aData[1] = sMenuStamp.Salinity;
//            ModRTU_Master_Write_Frame(&sFrame, sIdSlave485.Oxy, 0x10, 0x0008, 1, aData);
//            break;

//        case _RS485_CALIB_TEMP:
//            aData[0] = sMenuStamp.Temperature >> 8;
//            aData[1] = sMenuStamp.Temperature;
//            ModRTU_Master_Write_Frame(&sFrame, sIdSlave485.Oxy, 0x10, 0x000A , 1, aData);
//            break;
            
        case _RS485_CALIB_PH_POINT_686:
          aData[0] = 0x00;
          aData[1] = 0x00;
          ModRTU_Master_Write_Frame(&sFrame, sIdSlave485.pH, 0x06, 0x1000, 1, aData);
          break;
          
        case _RS485_CALIB_PH_POINT_918:
          aData[0] = 0x00;
          aData[1] = 0x00;
          ModRTU_Master_Write_Frame(&sFrame, sIdSlave485.pH, 0x06, 0x1004, 1, aData);
          break;
      
        default:
          break;
    }
    sKindMode485.Recv = sKindMode485.Trans;
    
    //Transmit RS485
    Send_RS458_Sensor(sFrame.Data_a8, sFrame.Length_u16);
    
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

    if(sUart485.Length_u16 > 2)
    {
        Crc_Recv = (sUart485.Data_a8[sUart485.Length_u16-1] << 8) |
                   (sUart485.Data_a8[sUart485.Length_u16-2]);
        Crc_Check = ModRTU_CRC(sUart485.Data_a8, sUart485.Length_u16 - 2);
        if(Crc_Check == Crc_Recv)
        {
            switch(sKindMode485.Recv)
            {
                case _RS485_OPERA_PH:
                case _RS485_CALIB_PH_POINT_686:
                case _RS485_CALIB_PH_POINT_918:
                    CountDisconnectPH = 0;
                    sParamCtrlPH.StateSensorPH = _PH_CONNECT;
                  break;
              
                case _RS485_OPERA_OXY:
                case _RS485_CALIB_OXY_100_CALIB:
//                case _RS485_CALIB_SALINITY:
                    CountDisconnectOxy = 0;
                    sStateCtrlOxy.StateSensorOxy = _OXY_CONNECT;
                    break;
              
                default:
                  break;
            }
            
            switch(sKindMode485.Recv)
            {
                case _RS485_OPERA_PH:
                  sParamCtrlPH.pH_Measure = sUart485.Data_a8[3]<<8 | sUart485.Data_a8[4];
                  break;
              
                case _RS485_OPERA_OXY:
                    Oxy_Mg_L    = sUart485.Data_a8[3]<<8 | sUart485.Data_a8[4];
                    Oxy_Percent = sUart485.Data_a8[11]<<8 | sUart485.Data_a8[12];
                    sParamMeasure.Temp = sUart485.Data_a8[7]<<8 | sUart485.Data_a8[8];    
                    sParamMeasure.Temp *= 10;
                    if(Oxy_Mg_L <= 3000 && Oxy_Percent <= 3000)
                    {
                        Compensation_Salinity(&Oxy_Mg_L, sParamMeasure.Salinity, sParamMeasure.Temp);
                        sParamMeasure.Oxy_Mg_L = Oxy_Mg_L;
                        sParamMeasure.Oxy_Percent = Oxy_Percent*10;
                    }
                    else
                    {
                        sStateCtrlOxy.StateSensorOxy = _OXY_DISCONNECT;
                        sParamMeasure.Oxy_Mg_L = 1;
                        sParamMeasure.Oxy_Percent = 1;
                    }
                    break;
                    
                case _RS485_CALIB_OXY_100_CALIB:
//                case _RS485_CALIB_SALINITY:
//                case _RS485_CALIB_TEMP:
                case _RS485_CALIB_PH_POINT_686:
                case _RS485_CALIB_PH_POINT_918:
                    sMenuState.CalibSensor = _CALIB_COMPLETE;
                    sKindMode485.Trans     = _RS485_OPERA_OXY;
                    break;
              
                default:
                  break;
            }
        }
        else
        {
            switch(sKindMode485.Recv)
            {
                case _RS485_OPERA_PH:
                case _RS485_CALIB_PH_POINT_686:
                case _RS485_CALIB_PH_POINT_918:
                    CountDisconnectPH++;
                    break;
              
                case _RS485_OPERA_OXY:
                case _RS485_CALIB_OXY_100_CALIB:
//                case _RS485_CALIB_SALINITY:
                    CountDisconnectOxy++;
                    break;
              
                default:
                  break;
            }
        } 
    }
    else
    {
        switch(sKindMode485.Recv)
        {
            case _RS485_OPERA_PH:
            case _RS485_CALIB_PH_POINT_686:
            case _RS485_CALIB_PH_POINT_918:
                CountDisconnectPH++;
                break;
          
            case _RS485_OPERA_OXY:
            case _RS485_CALIB_OXY_100_CALIB:
//                case _RS485_CALIB_SALINITY:
                CountDisconnectOxy++;
                break;
          
            default:
              break;
        }
    } 
    
    if(CountDisconnectOxy >= 3)
    {
        CountDisconnectOxy = 3;
        sStateCtrlOxy.StateSensorOxy = _OXY_DISCONNECT;
        sParamMeasure.Oxy_Mg_L = 0;
        sParamMeasure.Oxy_Percent = 0;
        sParamMeasure.Temp = 0;
    }

    if(CountDisconnectPH >= 3)
    {
        CountDisconnectPH = 3;
        sParamCtrlPH.StateSensorPH = _PH_DISCONNECT;
        sParamCtrlPH.pH_Measure = 0;
    }
    
    if(sStateCtrlOxy.StateSensorOxy == _OXY_CONNECT)
    {
        fevent_enable(sEventAppRs485, _EVENT_RESET_SENSOR_OXY);
    }

    fevent_disable(sEventAppRs485, _EVENT_RS485_RECEIVE_HANDLE);
    return 1;
}

static uint8_t fevent_rs485_wait_calib(uint8_t event)
{
    static uint8_t countstate = 0;
        if(countstate >= 20)
        {
            sMenuState.CalibSensor = _CALIB_ERROR;
            countstate = 0;
            sKindMode485.Trans = _RS485_OPERA_OXY;
            return 1;
        }
        else
        {
            if(sMenuState.CalibSensor == _CALIB_COMPLETE)   
            {
              countstate = 0;
//              sKindMode485.Trans = _RS485_OPERA_OXY;
              return 1;
            }
            else    
            {
              fevent_enable(sEventAppRs485, event);
            }
        }
        countstate++;
    return 1;
}

static uint8_t fevent_reset_sensor_oxy(uint8_t event)
{
    MX_USART1_UART_Init();
    Init_RX_Mode_Uart_485();
    
    HAL_GPIO_WritePin(SENSOR_GPIO_Port, SENSOR_Pin, GPIO_PIN_RESET);
    fevent_enable(sEventAppRs485, _EVENT_WAIT_SENSOR_OXY_ON);
    fevent_disable(sEventAppRs485, _EVENT_RS485_TRANSMIT);
    fevent_disable(sEventAppRs485, _EVENT_RS485_RECEIVE_HANDLE);
    fevent_disable(sEventAppRs485, _EVENT_RS485_RECEIVE_COMPLETE);
    return 1;
}

static uint8_t fevent_wait_sensor_oxy_on(uint8_t event)
{
    HAL_GPIO_WritePin(SENSOR_GPIO_Port, SENSOR_Pin, GPIO_PIN_SET);
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

/*-----------------------Handle Send RS485-----------------------*/
/*
    @brief Send 485 Debug
*/
void        Send_RS458_Normal(uint8_t *aData, uint16_t Length_u16) 
{
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_UART_Transmit(&uart_debug, aData, Length_u16, 1000);
    HAL_Delay(1);
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_RESET);
}

/*
    @brief Send 485 sensor
*/
void        Send_RS458_Sensor(uint8_t *aData, uint16_t Length_u16) 
{
    HAL_GPIO_WritePin(RS485_TXDE_S_GPIO_Port, RS485_TXDE_S_Pin, GPIO_PIN_SET);
    HAL_Delay(5);
    HAL_UART_Transmit(&uart_485, aData, Length_u16, 1000);
    
    UTIL_MEM_set(sUart485.Data_a8 , 0x00, sUart485.Length_u16);
    sUart485.Length_u16 = 0;
    CountBufferHandleRecv = 0;
    
    HAL_GPIO_WritePin(RS485_TXDE_S_GPIO_Port, RS485_TXDE_S_Pin, GPIO_PIN_RESET);
}

/*----------------------Save and Init Id Slave---------------------*/
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

/*==================Handle Task and Init app=================*/
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

