/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Speaker_GPIO_Port, Speaker_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ON_OFF_Vout_Pin|SIM_ON_OFF_Pin|MCU_RL2_Pin|MCU_RL3_Pin
                          |MCU_LCD_RS_Pin|MCU_LCD_RW_Pin|MCU_LCD_EN_Pin|MCU_LCD_D0_Pin
                          |MCU_LCD_D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SIM_RESET_Pin|SIM_PWR_KEY_Pin|SIM_DTR_Pin|SIM_RI_Pin
                          |SIM_RTS_Pin|MCU_RL1_Pin|RS485_TXDE_Pin|MCU_LCD_D1_Pin
                          |MCU_LCD_D2_Pin|MCU_LCD_D3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, MCU_RL4_Pin|MCU_RL5_Pin|MCU_RL6_Pin|MCU_RL7_Pin
                          |MCU_RL8_Pin|MCU_RL9_Pin|MCU_LCD_D6_Pin|MCU_LCD_D7_Pin
                          |MCU_LCD_PSB_Pin|MCU_LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MCU_LCD_D5_GPIO_Port, MCU_LCD_D5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = BT_UP_Pin|BT_ENTER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = Speaker_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Speaker_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin PCPin PCPin
                           PCPin */
  GPIO_InitStruct.Pin = ON_OFF_Vout_Pin|SIM_ON_OFF_Pin|MCU_RL2_Pin|MCU_RL3_Pin
                          |MCU_LCD_RS_Pin|MCU_LCD_RW_Pin|MCU_LCD_EN_Pin|MCU_LCD_D0_Pin
                          |MCU_LCD_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = PW_Detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PW_Detect_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = SIM_RESET_Pin|SIM_PWR_KEY_Pin|SIM_RTS_Pin|MCU_RL1_Pin
                          |RS485_TXDE_Pin|MCU_LCD_D1_Pin|MCU_LCD_D2_Pin|MCU_LCD_D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = SIM_DTR_Pin|SIM_RI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin
                           PBPin PBPin */
  GPIO_InitStruct.Pin = MCU_RL4_Pin|MCU_RL5_Pin|MCU_RL6_Pin|MCU_RL7_Pin
                          |MCU_RL8_Pin|MCU_RL9_Pin|MCU_LCD_D6_Pin|MCU_LCD_D7_Pin
                          |MCU_LCD_PSB_Pin|MCU_LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = MCU_SW_AUTO_Pin|MCU_SW_MAN_Pin|BT_EXIT_Pin|BT_DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MCU_LCD_D5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MCU_LCD_D5_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
