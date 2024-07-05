/*
 * myUart.h
 *
 *  Created on: Dec 7, 2021
 *      Author: lenovo
 */

#ifndef USER_UART_H_
#define USER_UART_H_

#include "user_util.h"
#include "usart.h"

/*======Define Uart ==============*/
/*-- Rx uart: DMA with IDLE line ----*/
#define UART_RX_DMA      0
/*-- Rx uart: Interupt line ----*/
#define UART_RX_IT       1

#define UART_RX_MODE     UART_RX_DMA

#define uart_debug	    huart3
#define uart_sim	    huart2

#define uart_dma_debug	hdma_usart3_rx
#define uart_dma_sim    hdma_usart2_rx
#define uart_dma_485	hdma_usart1_rx


/*===========Var struct=============*/

typedef enum
{
    _UART_DEBUG = 0,
    _UART_SIM,
    _UART_485,
}eTypeUart;

extern uint8_t UartDebugBuff[1200];
extern sData sUartDebug;

extern volatile uint8_t IsUart1IDLEInterupt;
extern volatile uint8_t IsUart2IDLEInterupt;
extern volatile uint8_t IsUart3IDLEInterupt;


/*==================Function==================*/
void Init_Uart_Module (void);

void Init_RX_Mode_Uart_Sim (void);
void Init_RX_Mode_Uart_Debug (void);
void Init_RX_Mode_Uart_485 (void);
void DeInit_RX_Mode_Uart_Sim (void);


void UART_Get_Data_Debug ( sData *sTaget, uint16_t MAX_SIZE,
                          uint8_t *pData, uint16_t *OldPos, uint16_t NewPos, uint16_t EndPos) ;

void ReInit_Rx_Mode_Uart_Error (void);

#endif /* USER_UART_H_ */
