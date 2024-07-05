#include "user_app_rs485.h"
#include "user_modbus_rtu.h"
#include "user_define_oxygen.h"
#include "user_internal_mem.h"
#include "user_app_ctrl_oxy.h"
#include "user_string.h"
#include "user_at_serial.h"
#include "user_define.h"
/*=========================Fucntion Static=========================*/
static uint8_t fevent_rs485_slave_entry(uint8_t event);

static uint8_t fevent_rs485_slave_handle_complete(uint8_t event);
static uint8_t fevent_rs485_slave_transmit(uint8_t event);

static uint8_t fevent_rs485_slave_deinit_uart(uint8_t event);
static uint8_t fevent_rs485_send_slave(uint8_t event);
/*==============================Struct=============================*/
sEvent_struct               sEventAppRs485Slave[]=
{
  {_EVENT_RS485_SLAVE_ENTRY,              0, 5, 5,            fevent_rs485_slave_entry},            //Doi slave khoi dong moi truyen opera
  
  {_EVENT_RS485_SLAVE_HANDLE_COMPLETE,    0, 5, 500,          fevent_rs485_slave_handle_complete},
  {_EVENT_RS485_SLAVE_TRANSMIT,           1, 0, 1000,         fevent_rs485_slave_transmit},

  {_EVENT_RS485_SLAVE_DEINIT_UART,        1, 5, 60000*15,     fevent_rs485_slave_deinit_uart},
  {_EVENT_RS485_SEND_SLAVE,               1, 5, 60000*2,      fevent_rs485_send_slave},
};

uint8_t                    Flag_User_Rs485Slave = _RS485_USER_SLAVE;
StructStateMotor           sStateSlaveMotor[NUMBER_SLAVE_MOTOR_MAX +1] = {0};      
Struct_KindMode485         sKindMode485Slave={
    .Trans = 1,
    .Recv  = 1,
};
uint16_t Number_Motor_Slave = NUMBER_SLAVE_DEFAULT;

uint8_t aData_Motor_OnOff[NUMBER_SLAVE_MOTOR_MAX+1] = {0};
/*========================Function Handle=======s=================*/
static uint8_t fevent_rs485_slave_entry(uint8_t event)
{
    return 1;
}

static uint8_t fevent_rs485_slave_handle_complete(uint8_t event)
{
    uint16_t Crc_Check  = 0;
    uint16_t Crc_Recv   = 0;
    uint8_t Data_Register = 0;
    
    if(Flag_User_Rs485Slave == _RS485_USER_SLAVE)
    {
        if(sUartDebug.Length_u16 > 2)
        {
            Crc_Recv = (sUartDebug.Data_a8[sUartDebug.Length_u16-1] << 8) |
                   (sUartDebug.Data_a8[sUartDebug.Length_u16-2]);
            Crc_Check = ModRTU_CRC(sUartDebug.Data_a8, sUartDebug.Length_u16 - 2);

            if(sUartDebug.Length_u16 == 8 && Crc_Recv == Crc_Check)
            {
                fevent_enable(sEventAppRs485Slave, _EVENT_RS485_SLAVE_DEINIT_UART);
                Data_Register = sUartDebug.Data_a8[5];
                sStateSlaveMotor[sUartDebug.Data_a8[0]].State_Machine = ((Data_Register << 2) & 0xFF) >> 6;
                sStateSlaveMotor[sUartDebug.Data_a8[0]].Stata_OnOff = ((Data_Register << 4) & 0xFF) >> 6;
                sStateSlaveMotor[sUartDebug.Data_a8[0]].Error_Amperage = ((Data_Register << 6) & 0xFF) >> 7;
                sStateSlaveMotor[sUartDebug.Data_a8[0]].Error_Pressure = ((Data_Register << 7) & 0xFF) >> 7;
                sStateSlaveMotor[sUartDebug.Data_a8[0]].Count_DisConnect = 0;
            }
            else
            {
                if(sStateSlaveMotor[sKindMode485Slave.Recv].Count_DisConnect < NUMBER_CHECK_DISCONNECT_SLAVE)
                    sStateSlaveMotor[sKindMode485Slave.Recv].Count_DisConnect++;
                else
                    sStateSlaveMotor[sKindMode485Slave.Recv].Count_DisConnect = NUMBER_CHECK_DISCONNECT_SLAVE;
            }
        }
        else
        {
            if(sStateSlaveMotor[sKindMode485Slave.Recv].Count_DisConnect < NUMBER_CHECK_DISCONNECT_SLAVE)
                sStateSlaveMotor[sKindMode485Slave.Recv].Count_DisConnect++;
            else
                sStateSlaveMotor[sKindMode485Slave.Recv].Count_DisConnect = NUMBER_CHECK_DISCONNECT_SLAVE;
        }
        
        
        #ifdef USING_APP_OXYGEN
            sStateCtrlOxy.StateWarningOxy_Pump = _PUMP_OK;
        #endif
        for(uint8_t i = 1; i <= NUMBER_SLAVE_MOTOR_MAX; i++)
        {
            if( i <= Number_Motor_Slave)
            {
                if(sStateCtrlOxy.StatePower == _POWER_ON)
                {
                    if(sStateSlaveMotor[i].Error_Amperage == 1 || \
                       sStateSlaveMotor[i].Error_Pressure == 1 || \
                       sStateSlaveMotor[i].Count_DisConnect >= NUMBER_CHECK_DISCONNECT_SLAVE)
                    {
                        #ifdef USING_APP_OXYGEN
                            sStateCtrlOxy.StateWarningOxy_Pump = _PUMP_FAIL;
                        #endif
                    }
                }
                else
                {
                    sStateSlaveMotor[i].Error_Amperage = 0;
                    sStateSlaveMotor[i].Error_Pressure = 0;
                    sStateSlaveMotor[i].Count_DisConnect = 0;
                }
            }
            else
            {
                sStateSlaveMotor[i].Error_Amperage = 0;
                sStateSlaveMotor[i].Error_Pressure = 0;
                sStateSlaveMotor[i].Count_DisConnect = 0;
            }
        }
        Reset_Buff(&sUartDebug);
    }
    else
    {
        for(uint8_t i = 1; i <= NUMBER_SLAVE_MOTOR_MAX; i++)
        {
            sStateSlaveMotor[i].Error_Amperage = 0;
            sStateSlaveMotor[i].Error_Pressure = 0;
            sStateSlaveMotor[i].Count_DisConnect = 0;
        }
        #ifdef USING_APP_OXYGEN
            sStateCtrlOxy.StateWarningOxy_Pump = _PUMP_OK;
        #endif
    }
    fevent_enable(sEventAppRs485Slave, _EVENT_RS485_SLAVE_TRANSMIT);
    return 1;
}

static uint8_t fevent_rs485_slave_transmit(uint8_t event)
{
    uint8_t Frame[20]={0};
    uint8_t aData[2] = {0};
    
    sData sFrame = {&Frame[0], 0};
    
    if(sStateCtrlOxy.StateMachine == _MACHINE_MAN)
    {
        for(uint8_t i = 0; i < NUMBER_SLAVE_MOTOR_MAX; i++)
          aData_Motor_OnOff[i] = 0x01;
    }
    else if(sStateCtrlOxy.StateMachine == _MACHINE_OFF)
    {
        for(uint8_t i = 0; i < NUMBER_SLAVE_MOTOR_MAX; i++)
          aData_Motor_OnOff[i] = 0x00;
    }
    else
    {
        for(uint8_t i = 0; i < NUMBER_SLAVE_MOTOR_MAX; i++)
          aData_Motor_OnOff[i] = 0x00;
        
        switch(Number_Motor_Slave)
        {
            case 0:
                for(uint8_t i = 0; i < NUMBER_SLAVE_MOTOR_MAX; i++)
                    aData_Motor_OnOff[i] = 0x00;
                break;
              
            case 1:
                for(uint8_t i = 0; i < 4; i++)
                {
                    if(sStateCtrlOxy.aStateOxy[i] == 0x01)
                    {
                        aData_Motor_OnOff[0] = 0x01;
                    }
                }
                break;
                
            case 2:
                if(sStateCtrlOxy.aStateOxy[0] == 0x01 || sStateCtrlOxy.aStateOxy[1] == 0x01)
                  aData_Motor_OnOff[0] = 0x01;
                
                if(sStateCtrlOxy.aStateOxy[2] == 0x01 || sStateCtrlOxy.aStateOxy[3] == 0x01)
                  aData_Motor_OnOff[1] = 0x01;
                break;
              
            case 3:
                if(sStateCtrlOxy.aStateOxy[0] == 0x01)
                  aData_Motor_OnOff[0] = 0x01;
                
                if(sStateCtrlOxy.aStateOxy[1] == 0x01)
                  aData_Motor_OnOff[1] = 0x01;
                
                if(sStateCtrlOxy.aStateOxy[2] == 0x01 || sStateCtrlOxy.aStateOxy[3] == 0x01)
                  aData_Motor_OnOff[2] = 0x01;
                break;
              
            case 4:
                if(sStateCtrlOxy.aStateOxy[0] == 0x01)
                  aData_Motor_OnOff[0] = 0x01;
                
                if(sStateCtrlOxy.aStateOxy[1] == 0x01)
                  aData_Motor_OnOff[1] = 0x01;
                
                if(sStateCtrlOxy.aStateOxy[2] == 0x01)
                  aData_Motor_OnOff[2] = 0x01;
                
                if(sStateCtrlOxy.aStateOxy[3] == 0x01)
                  aData_Motor_OnOff[3] = 0x01;
                break;
              
            case 5:
                if(sStateCtrlOxy.aStateOxy[0] == 0x01)
                {
                  aData_Motor_OnOff[0] = 0x01;
                  aData_Motor_OnOff[1] = 0x01;
                }
                
                if(sStateCtrlOxy.aStateOxy[1] == 0x01)
                  aData_Motor_OnOff[2] = 0x01;
                
                if(sStateCtrlOxy.aStateOxy[2] == 0x01)
                  aData_Motor_OnOff[3] = 0x01;
                
                if(sStateCtrlOxy.aStateOxy[3] == 0x01)
                  aData_Motor_OnOff[4] = 0x01;
                break;
              
            case 6:
                if(sStateCtrlOxy.aStateOxy[0] == 0x01)
                {
                  aData_Motor_OnOff[0] = 0x01;
                  aData_Motor_OnOff[1] = 0x01;
                }
                
                if(sStateCtrlOxy.aStateOxy[1] == 0x01)
                {
                  aData_Motor_OnOff[2] = 0x01;
                  aData_Motor_OnOff[3] = 0x01;
                }
                
                if(sStateCtrlOxy.aStateOxy[2] == 0x01)
                  aData_Motor_OnOff[4] = 0x01;
                
                if(sStateCtrlOxy.aStateOxy[3] == 0x01)
                  aData_Motor_OnOff[5] = 0x01;
                break;
            case 7:
                if(sStateCtrlOxy.aStateOxy[0] == 0x01)
                {
                  aData_Motor_OnOff[0] = 0x01;
                  aData_Motor_OnOff[1] = 0x01;
                }
                
                if(sStateCtrlOxy.aStateOxy[1] == 0x01)
                {
                  aData_Motor_OnOff[2] = 0x01;
                  aData_Motor_OnOff[3] = 0x01;
                }
                
                if(sStateCtrlOxy.aStateOxy[2] == 0x01)
                {
                  aData_Motor_OnOff[4] = 0x01;
                  aData_Motor_OnOff[5] = 0x01;
                }
                
                if(sStateCtrlOxy.aStateOxy[3] == 0x01)
                  aData_Motor_OnOff[6] = 0x01;
                break;
              
            case 8:
                if(sStateCtrlOxy.aStateOxy[0] == 0x01)
                {
                  aData_Motor_OnOff[0] = 0x01;
                  aData_Motor_OnOff[1] = 0x01;
                }
                
                if(sStateCtrlOxy.aStateOxy[1] == 0x01)
                {
                  aData_Motor_OnOff[2] = 0x01;
                  aData_Motor_OnOff[3] = 0x01;
                }
                
                if(sStateCtrlOxy.aStateOxy[2] == 0x01)
                {
                  aData_Motor_OnOff[4] = 0x01;
                  aData_Motor_OnOff[5] = 0x01;
                }
                
                if(sStateCtrlOxy.aStateOxy[3] == 0x01)
                {
                  aData_Motor_OnOff[6] = 0x01;
                  aData_Motor_OnOff[7] = 0x01;
                }
                break;
              
            default:
             break;
        }
    }
    
    aData[0]= 0x00;
    aData[1]= aData_Motor_OnOff[sKindMode485Slave.Trans-1];
    
    if(Number_Motor_Slave > 0)
    {
        ModRTU_Master_Write_Frame(&sFrame, sKindMode485Slave.Trans, 0x06, 0x0000, 1, aData);
        if(Flag_User_Rs485Slave == _RS485_USER_SLAVE)
        {
            Send_RS458_Slave(sFrame.Data_a8 , sFrame.Length_u16 );      
        }
    }
    else
    {
        for(uint8_t i = 1; i <= NUMBER_SLAVE_MOTOR_MAX; i++)
        {
            sStateSlaveMotor[i].Error_Amperage = 0;
            sStateSlaveMotor[i].Error_Pressure = 0;
            sStateSlaveMotor[i].Count_DisConnect = 0;
        }
        #ifdef USING_APP_OXYGEN
            sStateCtrlOxy.StateWarningOxy_Pump = _PUMP_OK;
        #endif
        fevent_enable(sEventAppRs485Slave, _EVENT_RS485_SLAVE_DEINIT_UART);
    }
    
    sKindMode485Slave.Recv = sKindMode485Slave.Trans;
    
    if(Number_Motor_Slave > 0)
    {
        if(sKindMode485Slave.Trans < Number_Motor_Slave)
        {
            sKindMode485Slave.Trans++;
        }
        else
        {
            sKindMode485Slave.Trans = 1;
        }
    }
    else
    {
        sKindMode485Slave.Trans = 1;
    }
    
    fevent_enable(sEventAppRs485Slave, _EVENT_RS485_SLAVE_HANDLE_COMPLETE);
    
    return 1;
}

static uint8_t fevent_rs485_slave_deinit_uart(uint8_t event)
{
    MX_USART3_UART_Init();
    Init_RX_Mode_Uart_Debug();
    fevent_enable(sEventAppRs485Slave, event);
    return 1;
}

static uint8_t fevent_rs485_send_slave(uint8_t event)
{
    Flag_User_Rs485Slave = _RS485_USER_SLAVE;
    fevent_enable(sEventAppRs485Slave, event);
    return 1;
}

/*==========================Handle==========================*/

/*-----------------------Handle Send RS485-----------------------*/
/*
    @brief Send 485 Debug
*/
void        Send_RS458_Normal(uint8_t *aData, uint16_t Length_u16) 
{
  if(Flag_User_Rs485Slave == _RS485_USER_DEBUG)
  {
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_UART_Transmit(&uart_debug, aData, Length_u16, 1000);
    HAL_Delay(1);
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_RESET);
  }
}

/*
    @brief Send 485 Debug
*/
void        Send_RS458_Slave(uint8_t *aData, uint16_t Length_u16) 
{
  if(Flag_User_Rs485Slave == _RS485_USER_SLAVE)
  {
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_UART_Transmit(&uart_debug, aData, Length_u16, 1000);
    
    UTIL_MEM_set(sUartDebug.Data_a8 , 0x00, sUartDebug.Length_u16);
    sUartDebug.Length_u16 = 0;
    
//    HAL_Delay(1);
    HAL_GPIO_WritePin(RS485_TXDE_GPIO_Port, RS485_TXDE_Pin, GPIO_PIN_RESET);
  }
}

/*------------------------Function Packing Data-------------------------*/
void Send_Slave_Crc(uint8_t aData[], uint16_t Length)
{
    uint16_t TempCrc = 0;
    Calculator_Crc_U16(&TempCrc, aData, Length);
    aData[Length++] = TempCrc;
    aData[Length++] = TempCrc << 8;
    Send_RS458_Slave(aData, Length);
}

/*------------------------Function Crc----------------------*/
/* 
    @brief Calculator Crc 2 byte use Crc Modbus RTU
*/
uint8_t Calculator_Crc_U16(uint16_t *crc, uint8_t* buf, uint16_t len)
{
  *crc = 0x0000;
  
  for (int pos = 0; pos < len; pos++) 
  {
    *crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc
  
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((*crc & 0x0001) != 0) {      // If the LSB is set
        *crc >>= 1;                    // Shift right and XOR 0xA001
        *crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        *crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return 1;  
}
/*==================Save and Init Number Slave=================*/
void        Save_NumberMotorSlave(uint16_t Number)
{
    uint8_t aData[2] = {0};
    uint8_t length = 0;
    
    Number_Motor_Slave = Number;
    aData[length++] = Number_Motor_Slave >> 8;
    aData[length++] = Number_Motor_Slave;

    Save_Array(ADDR_NUMBER_MOTOR_SLAVE, aData, length);
    Stamp_Menu_Exit();
}

void        Init_NumberMotorSlave(void)
{
    if(*(__IO uint8_t*)(ADDR_NUMBER_MOTOR_SLAVE) == BYTE_TEMP_FIRST)
    {
        Number_Motor_Slave = *(__IO uint8_t*)(ADDR_NUMBER_MOTOR_SLAVE+2) << 8;
        Number_Motor_Slave |= *(__IO uint8_t*)(ADDR_NUMBER_MOTOR_SLAVE+3);
    }
    
    if(Number_Motor_Slave > NUMBER_SLAVE_MOTOR_MAX)
    {
        Number_Motor_Slave = NUMBER_SLAVE_DEFAULT;
    }
}

/*==================Handle Task and Init app=================*/
void       Init_AppRs485Slave(void)
{
    Init_NumberMotorSlave();
}

uint8_t        AppRs485Slave_Task(void)
{
    uint8_t i = 0;
    uint8_t Result =  false;
    
    for(i = 0; i < _EVENT_RS485_SLAVE_END; i++)
    {
        if(sEventAppRs485Slave[i].e_status == 1)
        {
            Result = true; 
            
            if((sEventAppRs485Slave[i].e_systick == 0) ||
               ((HAL_GetTick() - sEventAppRs485Slave[i].e_systick) >= sEventAppRs485Slave[i].e_period))
            {
                sEventAppRs485Slave[i].e_status = 0; //Disable event
                sEventAppRs485Slave[i].e_systick= HAL_GetTick();
                sEventAppRs485Slave[i].e_function_handler(i);
            }
        }
    }
    
    return Result;
}

