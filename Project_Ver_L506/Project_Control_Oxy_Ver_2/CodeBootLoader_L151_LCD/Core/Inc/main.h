/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCU_LCD_RS_Pin GPIO_PIN_6
#define MCU_LCD_RS_GPIO_Port GPIOC
#define MCU_LCD_RW_Pin GPIO_PIN_7
#define MCU_LCD_RW_GPIO_Port GPIOC
#define MCU_LCD_EN_Pin GPIO_PIN_8
#define MCU_LCD_EN_GPIO_Port GPIOC
#define MCU_LCD_D0_Pin GPIO_PIN_9
#define MCU_LCD_D0_GPIO_Port GPIOC
#define MCU_LCD_D1_Pin GPIO_PIN_11
#define MCU_LCD_D1_GPIO_Port GPIOA
#define MCU_LCD_D2_Pin GPIO_PIN_12
#define MCU_LCD_D2_GPIO_Port GPIOA
#define MCU_LCD_D3_Pin GPIO_PIN_15
#define MCU_LCD_D3_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_10
#define DEBUG_TX_GPIO_Port GPIOC
#define DEBUG_RX_Pin GPIO_PIN_11
#define DEBUG_RX_GPIO_Port GPIOC
#define MCU_LCD_D4_Pin GPIO_PIN_12
#define MCU_LCD_D4_GPIO_Port GPIOC
#define MCU_LCD_D5_Pin GPIO_PIN_2
#define MCU_LCD_D5_GPIO_Port GPIOD
#define MCU_LCD_D6_Pin GPIO_PIN_3
#define MCU_LCD_D6_GPIO_Port GPIOB
#define MCU_LCD_D7_Pin GPIO_PIN_4
#define MCU_LCD_D7_GPIO_Port GPIOB
#define MCU_LCD_PSB_Pin GPIO_PIN_5
#define MCU_LCD_PSB_GPIO_Port GPIOB
#define MCU_LCD_RST_Pin GPIO_PIN_8
#define MCU_LCD_RST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
