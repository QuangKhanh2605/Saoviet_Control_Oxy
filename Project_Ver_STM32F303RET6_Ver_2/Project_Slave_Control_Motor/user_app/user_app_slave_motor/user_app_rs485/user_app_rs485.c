#include "user_app_rs485.h"
#include "user_define.h"
#include "user_modbus_rtu.h"
/*============== Function static ===============*/
static uint8_t fevent_rs485_entry(uint8_t event);
static uint8_t fevent_rs485_transmit(uint8_t event);

static uint8_t fevent_rs485_receive_handle(uint8_t event);
static uint8_t fevent_rs485_receive_complete(uint8_t event);
static uint8_t fevent_rs485_check_connect(uint8_t event);
static uint8_t fevent_rs485_deinit_uart(uint8_t event);
/*=================== struct ==================*/
sEvent_struct               sEventAppRs485[] = 
{
  {_EVENT_RS485_ENTRY,              0, 5, 0,                fevent_rs485_entry},
  {_EVENT_RS485_TRANSMIT,           0, 0, 2500,             fevent_rs485_transmit},
  
  {_EVENT_RS485_RECEIVE_HANDLE,     1, 5, 5,                fevent_rs485_receive_handle},
  {_EVENT_RS485_RECEIVE_COMPLETE,   0, 5, 500,              fevent_rs485_receive_complete},
  
  {_EVENT_RS485_CHECK_CONNECT,      1, 5, 60000,            fevent_rs485_check_connect},
  
  {_EVENT_RS485_DEINIT_UART,        1, 5, 60000*15,         fevent_rs485_deinit_uart},
};

uint16_t CountBufferHandleRecv = 0;
structInforRs485        sInforRs485=
{
    .ID_Slave = SLAVE_ID_DEFAULT,
    .State_OnOff_Motor = _RS485_MOTOR_FAIL,
};
/*================= Function Handle ==============*/
static uint8_t fevent_rs485_entry(uint8_t event)
{
    return 1;
}

static uint8_t fevent_rs485_transmit(uint8_t event)
{
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
    uint8_t Frame[20] = {0};
    sData sFrame = {&Frame[0], 0};
    
    uint8_t aData[2] = {0};
 
    uint16_t Crc_Check = 0;
    uint16_t Crc_Recv  = 0;
    uint16_t Data_Register_Send = 0;
    uint16_t Data_Register_Recv = 0;

    Data_Register_Send = (Data_Register_Send | sParamMotor.StateMachine);
    Data_Register_Send = ((Data_Register_Send <<2) | sInforRs485.State_OnOff_Motor);
    Data_Register_Send = ((Data_Register_Send <<1) | sParamMotor.StatusAmperage);
    Data_Register_Send = ((Data_Register_Send <<1) | sParamMotor.StatusPressure);
    
    aData[0] = Data_Register_Send >> 8;
    aData[1] = Data_Register_Send;
    
    if(sUart485.Length_u16 > 2)
    {
        Crc_Recv = (sUart485.Data_a8[sUart485.Length_u16-1] << 8) |
                   (sUart485.Data_a8[sUart485.Length_u16-2]);
        Crc_Check = ModRTU_CRC(sUart485.Data_a8, sUart485.Length_u16 - 2);
        if(sUart485.Length_u16 == 8 && Crc_Recv == Crc_Check)
        {   
          fevent_enable(sEventAppRs485, _EVENT_RS485_DEINIT_UART);
          if(sUart485.Data_a8[0] == sInforRs485.ID_Slave && sUart485.Data_a8[1] == 0x06)
          {
            Data_Register_Recv  = sUart485.Data_a8[4] << 8;
            Data_Register_Recv |= sUart485.Data_a8[5] ;
            
            if(Data_Register_Recv <= 1)
            {
                sInforRs485.State_OnOff_Motor = Data_Register_Recv;
                fevent_enable(sEventAppRs485, _EVENT_RS485_CHECK_CONNECT);
                ModRTU_Slave_ACK_Write_Frame(&sFrame, sInforRs485.ID_Slave, 0x06, 0x0000, 0x0001, aData);
                Send_RS458_Sensor(sFrame.Data_a8, sFrame.Length_u16);
            }
          }
        }
    }
  
    UTIL_MEM_set(sUart485.Data_a8 , 0x00, sUart485.Length_u16);
    sUart485.Length_u16 = 0;
    CountBufferHandleRecv = 0;
    fevent_active(sEventAppRs485, _EVENT_RS485_RECEIVE_HANDLE);
    return 1;
}

static uint8_t fevent_rs485_check_connect(uint8_t event)
{
    sInforRs485.State_OnOff_Motor = _RS485_MOTOR_FAIL;
    fevent_enable(sEventAppRs485, event);
    return 1;
}

static uint8_t fevent_rs485_deinit_uart(uint8_t event)
{
    MX_USART1_UART_Init();
    Init_RX_Mode_Uart_485();
    fevent_enable(sEventAppRs485, event);
    return 1;
}
/*===================== Function Handle ====================*/
void        Send_RS458_Sensor(uint8_t *aData, uint16_t Length_u16) 
{
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_SET);
    HAL_Delay(5);
    HAL_UART_Transmit(&uart_485, aData, Length_u16, 1000);
    
    UTIL_MEM_set(sUart485.Data_a8 , 0x00, sUart485.Length_u16);
    sUart485.Length_u16 = 0;
    CountBufferHandleRecv = 0;
    
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_RESET);
}
/*--------------------------Save and Init ID Slave-----------------------*/
void Save_IDSlave(uint16_t ID)
{
    uint8_t aData[2] = {0};
    uint8_t length = 0;
    
    sInforRs485.ID_Slave = ID;
    aData[length++] = sInforRs485.ID_Slave >> 8;
    aData[length++] = sInforRs485.ID_Slave;

    Save_Array(ADDR_INFOR_SLAVE_485, aData, length);
    Stamp_Menu_Exit();
}

void        Init_IDSlave(void)
{
    if(*(__IO uint8_t*)(ADDR_INFOR_SLAVE_485) == BYTE_TEMP_FIRST)
    {
        sInforRs485.ID_Slave = *(__IO uint8_t*)(ADDR_INFOR_SLAVE_485+2) << 8;
        sInforRs485.ID_Slave |= *(__IO uint8_t*)(ADDR_INFOR_SLAVE_485+3);
    }
    
    if(sInforRs485.ID_Slave > SLAVE_ID_MAX)
    {
        sInforRs485.ID_Slave = SLAVE_ID_DEFAULT;
    }
    Stamp_Menu_Exit();
}

/*===================== Task and Init ===================*/
void    Init_AppRs485(void)
{
    Init_IDSlave();
}

uint8_t AppRs485_Task(void)
{
    uint8_t i = 0;
    uint8_t Result = false;
    for( i = 0; i < _EVENT_RS485_END; i++)
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

