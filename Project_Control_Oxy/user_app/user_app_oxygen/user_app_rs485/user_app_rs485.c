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
static uint8_t fevent_rs485_init_uart(uint8_t event);
/*==============================Struct=============================*/
sEvent_struct               sEventAppRs485[]=
{
  {_EVENT_RS485_ENTRY,              1, 5, 5,                fevent_rs485_entry},
  {_EVENT_RS485_TRANSMIT,           1, 0, 2000,             fevent_rs485_transmit},
  {_EVENT_RS485_RECEIVE_HANDLE,     0, 5, 5,                fevent_rs485_receive_handle},
  {_EVENT_RS485_RECEIVE_COMPLETE,   0, 5, 500,              fevent_rs485_receive_complete},
  
  {_EVENT_RS485_INIT_UART,          1, 5, TIME_INIT_UART,   fevent_rs485_init_uart},
};

uint8_t         ID_Slave        = ID_DEFAULT;
uint8_t         KindTrans485    = _TRANSMIT_OPERA;
uint8_t         CycleReset      = 0;
/*========================Function Handle========================*/

static uint8_t fevent_rs485_entry(uint8_t event)
{
    fevent_enable(sEventAppRs485, event);
    return 1;
}

static uint8_t fevent_rs485_transmit(uint8_t event)
{
/*--------------------Hoi du lieu tu Slave-------------------*/

  uint8_t Frame[20]={0};
    uint8_t aData[4]={0};
    sData sFrame = {&Frame[0], 0};
    if(KindTrans485 != _TRANSMIT_RESET_CALIB) CycleReset = 0;
    switch(KindTrans485)
    {
        case _TRANSMIT_OPERA:
            ModRTU_Master_Read_Frame(&sFrame, ID_Slave, 0x03, 0x0003, 24);
            break;
          
        case _TRANSMIT_CALIB_OXY_100_CALIB:
            aData[0] = 0x00;
            aData[1] = 0x01;
            ModRTU_Master_Write_Frame(&sFrame, ID_Slave, 0x10, 0x0220 , 1, aData);
            break;
            
        case _TRANSMIT_CALIB_SALINITY:
            aData[0] = sMenuStamp.Salinity >> 8;
            aData[1] = sMenuStamp.Salinity;
            ModRTU_Master_Write_Frame(&sFrame, ID_Slave, 0x10, 0x0008, 1, aData);
            break;
            
        case _TRANSMIT_CALIB_TEMP:
            aData[0] = sMenuStamp.Temperature >> 8;
            aData[1] = sMenuStamp.Temperature;
            ModRTU_Master_Write_Frame(&sFrame, ID_Slave, 0x10, 0x000A , 1, aData);
            break;
            
        case _TRANSMIT_RESET_CALIB:
            switch(CycleReset)
            {
                case _TRANSMIT_RESET_0:
                    aData[0] = 0x00;
                    aData[1] = 0x10;
                    ModRTU_Master_Write_Frame(&sFrame, ID_Slave, 0x10, 0x0220 , 1, aData);
                    break;
                    
                case _TRANSMIT_RESET_100:
                    aData[0] = 0x00;
                    aData[1] = 0x08;
                    ModRTU_Master_Write_Frame(&sFrame, ID_Slave, 0x10, 0x0220 , 1, aData);
                    break;
                    
                case _TRANSMIT_RESET_TEMPERATURE:
                    aData[0] = 0x00;
                    aData[1] = 0x20;
                    ModRTU_Master_Write_Frame(&sFrame, ID_Slave, 0x10, 0x0220 , 1, aData);
                    break;
                  
                default:
                  break;
            }
            break;
      
        default:
          break;
    }
    
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_SET);
    HAL_Delay(5);
    UTIL_MEM_set(sUart485.Data_a8 , 0x00, sUart485.Length_u16);
    sUart485.Length_u16 = 0;
    
    HAL_UART_Transmit(&uart_485, sFrame.Data_a8, sFrame.Length_u16, 1000);
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_RESET);
    
    fevent_active(sEventAppRs485, _EVENT_RS485_RECEIVE_HANDLE);
    fevent_enable(sEventAppRs485, _EVENT_RS485_RECEIVE_COMPLETE);
    
    fevent_enable(sEventAppRs485, event);
    return 1;
}

static uint8_t fevent_rs485_receive_handle(uint8_t event)
{
/*-----------------Kiem tra da nhan xong tu 485------------*/
    static uint16_t countBuffer_uart = 0;

    if(sUart485.Length_u16 != 0)
    {
        if(countBuffer_uart == sUart485.Length_u16)
        {
            countBuffer_uart = 0;
            fevent_active(sEventAppRs485, _EVENT_RS485_RECEIVE_COMPLETE);
            return 1;
        }
        else
        {
            countBuffer_uart = sUart485.Length_u16;
        }
    }
    
    fevent_enable(sEventAppRs485, event);
    return 1;
}

static uint8_t fevent_rs485_receive_complete(uint8_t event)
{
/*------------------Xu ly chuoi nhan duoc----------------*/
    static uint8_t CountDisconnect = 0;
  
    uint16_t Crc_Check = 0;
    uint16_t Crc_Recv  = 0;

    Crc_Recv = (sUart485.Data_a8[sUart485.Length_u16-1] << 8) |
               (sUart485.Data_a8[sUart485.Length_u16-2]);
    Crc_Check = ModRTU_CRC(sUart485.Data_a8, sUart485.Length_u16 - 2);
    if(Crc_Check == Crc_Recv)
    {
        CountDisconnect = 0;
        sStateCtrlOxy.StateSensor = _OXY_CONNECT;
        fevent_enable(sEventAppRs485, _EVENT_RS485_INIT_UART);
        
        if(KindTrans485 == _TRANSMIT_OPERA)
        {
          //Doc Oxy Calib 2 diem
//            sParamMeasure.Oxy_Mg_L = sUart485.Data_a8[45]<<8 | sUart485.Data_a8[46];
//            sParamMeasure.Oxy_Percent = sUart485.Data_a8[47]<<8 | sUart485.Data_a8[48];
            
          //Doc Oxy Calib 1 diem bao hoa 100%
            sParamMeasure.Oxy_Mg_L = sUart485.Data_a8[3]<<8 | sUart485.Data_a8[4];
            sParamMeasure.Oxy_Percent = sUart485.Data_a8[9]<<8 | sUart485.Data_a8[10];
            
            sParamMeasure.Salinity = sUart485.Data_a8[13]<<8 | sUart485.Data_a8[14];
            sParamMeasure.Temp = sUart485.Data_a8[17]<<8 | sUart485.Data_a8[18];
            
            Compensation_Salinity(&sParamMeasure.Oxy_Mg_L, sParamMeasure.Salinity, sParamMeasure.Temp);
        }
        else
        {
            if(KindTrans485 == _TRANSMIT_RESET_CALIB) 
            {
                CycleReset++;
                if(CycleReset == _TRANSMIT_RESET_END) 
                {
                    CycleReset = 0;
                    sMenuState.CalibSensor = _CALIB_COMPLETE;
                    KindTrans485 = _TRANSMIT_OPERA;
                }
                fevent_active(sEventAppRs485, _EVENT_RS485_TRANSMIT);
            }
            else
            {
                sMenuState.CalibSensor = _CALIB_COMPLETE;
                KindTrans485 = _TRANSMIT_OPERA;
            }
        }
    }
    else
    {
        CountDisconnect++;
    }
    
    if(CountDisconnect == 3)
    {
        sStateCtrlOxy.StateSensor = _OXY_DISCONNECT;
        sParamMeasure.Oxy_Mg_L = 0;
        sParamMeasure.Oxy_Percent = 0;
        sParamMeasure.Salinity = 0;
        sParamMeasure.Temp = 0;
    }

    fevent_disable(sEventAppRs485, _EVENT_RS485_RECEIVE_HANDLE);
    return 1;
}

static uint8_t fevent_rs485_init_uart(uint8_t event)
{
    MX_USART1_UART_Init();
    Init_RX_Mode_Uart_485();
    fevent_enable(sEventAppRs485, event);
    return 1;
}


/*==========================Handle==========================*/
void        Save_IdSlave(uint8_t ID)
{
    uint8_t aData[2] = {0};
    uint8_t length = 0;
    
    ID_Slave = ID;
    aData[length++] = ID_Slave ;

    Save_Array(ADDR_INFOR_SLAVE_RS485, aData, length);
}

void        Init_IdSlave(void)
{
    if(*(__IO uint8_t*)(ADDR_INFOR_SLAVE_RS485) == BYTE_TEMP_FIRST)
    {
        ID_Slave = *(__IO uint8_t*)(ADDR_INFOR_SLAVE_RS485+2);
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

