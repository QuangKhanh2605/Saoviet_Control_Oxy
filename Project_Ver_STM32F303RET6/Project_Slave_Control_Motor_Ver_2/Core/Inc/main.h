/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define ON_OFF_Vout_Pin GPIO_PIN_13
#define ON_OFF_Vout_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_0
#define LED_2_GPIO_Port GPIOH
#define BT_UP_Pin GPIO_PIN_0
#define BT_UP_GPIO_Port GPIOC
#define BT_DOWN_Pin GPIO_PIN_1
#define BT_DOWN_GPIO_Port GPIOC
#define BT_EXIT_Pin GPIO_PIN_2
#define BT_EXIT_GPIO_Port GPIOC
#define BT_ENTER_Pin GPIO_PIN_3
#define BT_ENTER_GPIO_Port GPIOC
#define ADC_Signal_Ap_1_Pin GPIO_PIN_0
#define ADC_Signal_Ap_1_GPIO_Port GPIOA
#define ADC_Signal_Ap_2_Pin GPIO_PIN_1
#define ADC_Signal_Ap_2_GPIO_Port GPIOA
#define ADC_Signal_Ap_3_Pin GPIO_PIN_2
#define ADC_Signal_Ap_3_GPIO_Port GPIOA
#define ADC_Signal_Ap_4_Pin GPIO_PIN_3
#define ADC_Signal_Ap_4_GPIO_Port GPIOA
#define ADC_IN3_Pin GPIO_PIN_4
#define ADC_IN3_GPIO_Port GPIOC
#define ADC_IN2_Pin GPIO_PIN_5
#define ADC_IN2_GPIO_Port GPIOC
#define ADC_IN1_Pin GPIO_PIN_0
#define ADC_IN1_GPIO_Port GPIOB
#define ADC_V_Offset_Pin GPIO_PIN_1
#define ADC_V_Offset_GPIO_Port GPIOB
#define Speaker_Pin GPIO_PIN_2
#define Speaker_GPIO_Port GPIOB
#define MCU_LCD_RS_Pin GPIO_PIN_12
#define MCU_LCD_RS_GPIO_Port GPIOB
#define MCU_LCD_RW_Pin GPIO_PIN_13
#define MCU_LCD_RW_GPIO_Port GPIOB
#define MCU_LCD_EN_Pin GPIO_PIN_14
#define MCU_LCD_EN_GPIO_Port GPIOB
#define MCU_LCD_D4_Pin GPIO_PIN_15
#define MCU_LCD_D4_GPIO_Port GPIOB
#define MCU_LCD_D5_Pin GPIO_PIN_6
#define MCU_LCD_D5_GPIO_Port GPIOC
#define MCU_LCD_D6_Pin GPIO_PIN_7
#define MCU_LCD_D6_GPIO_Port GPIOC
#define MCU_LCD_D7_Pin GPIO_PIN_8
#define MCU_LCD_D7_GPIO_Port GPIOC
#define LED_1_Pin GPIO_PIN_9
#define LED_1_GPIO_Port GPIOC
#define MCU_SW_MAN_Pin GPIO_PIN_10
#define MCU_SW_MAN_GPIO_Port GPIOC
#define MCU_SW_AUTO_Pin GPIO_PIN_11
#define MCU_SW_AUTO_GPIO_Port GPIOC
#define MCU_RL4_Pin GPIO_PIN_12
#define MCU_RL4_GPIO_Port GPIOC
#define MCU_RL3_Pin GPIO_PIN_2
#define MCU_RL3_GPIO_Port GPIOD
#define MCU_RL2_Pin GPIO_PIN_3
#define MCU_RL2_GPIO_Port GPIOB
#define MCU_RL1_Pin GPIO_PIN_4
#define MCU_RL1_GPIO_Port GPIOB
#define SIM_ON_OFF_Pin GPIO_PIN_5
#define SIM_ON_OFF_GPIO_Port GPIOB
#define RS485_TXDE_Pin GPIO_PIN_8
#define RS485_TXDE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
