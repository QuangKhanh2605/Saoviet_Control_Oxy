#include "user_app_rs485.h"

/*============== Function static ===============*/
static uint8_t fevent_rs485_entry(uint8_t event);
static uint8_t fevent_rs485_transmit(uint8_t event);
static uint8_t fevent_rs485_receive_handle(uint8_t event);
static uint8_t fevent_rs485_receive_complete(uint8_t event);
/*=================== struct ==================*/
sEvent_struct               sEventAppRs485[] = 
{
  {_EVENT_RS485_ENTRY,              1, 5, 0,                fevent_rs485_entry},
  {_EVENT_RS485_TRANSMIT,           0, 0, 2500,             fevent_rs485_transmit},
  {_EVENT_RS485_RECEIVE_HANDLE,     1, 5, 5,                fevent_rs485_receive_handle},
  {_EVENT_RS485_RECEIVE_COMPLETE,   0, 5, 500,              fevent_rs485_receive_complete},
};

uint16_t CountBufferHandleRecv = 0;
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
//            return 1;
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
//    Send_RS458_Sensor((uint8_t*)"Receive success",15);
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

/*===================== Task and Init ===================*/
void    Init_AppRs485(void)
{

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

