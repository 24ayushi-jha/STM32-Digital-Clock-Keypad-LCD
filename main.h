/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

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
#define KEY4_R4_Pin GPIO_PIN_13
#define KEY4_R4_GPIO_Port GPIOC
#define KEY5_C1_Pin GPIO_PIN_14
#define KEY5_C1_GPIO_Port GPIOC
#define KEY6_C2_Pin GPIO_PIN_15
#define KEY6_C2_GPIO_Port GPIOC
#define KEY7_C3_Pin GPIO_PIN_0
#define KEY7_C3_GPIO_Port GPIOF
#define KEY8_C4_Pin GPIO_PIN_1
#define KEY8_C4_GPIO_Port GPIOF
#define LCD_D7_14_Pin GPIO_PIN_2
#define LCD_D7_14_GPIO_Port GPIOB
#define LCD_D6_13_Pin GPIO_PIN_10
#define LCD_D6_13_GPIO_Port GPIOB
#define LCD_D5_12_Pin GPIO_PIN_11
#define LCD_D5_12_GPIO_Port GPIOB
#define LCD_D4_11_Pin GPIO_PIN_12
#define LCD_D4_11_GPIO_Port GPIOB
#define LCD_D3_10_Pin GPIO_PIN_13
#define LCD_D3_10_GPIO_Port GPIOB
#define LCD_D2_9_Pin GPIO_PIN_14
#define LCD_D2_9_GPIO_Port GPIOB
#define LCD_D1_8_Pin GPIO_PIN_15
#define LCD_D1_8_GPIO_Port GPIOB
#define LCD_D0_7_Pin GPIO_PIN_8
#define LCD_D0_7_GPIO_Port GPIOA
#define LCD_EN_6_Pin GPIO_PIN_9
#define LCD_EN_6_GPIO_Port GPIOA
#define LCD_RW_5_Pin GPIO_PIN_10
#define LCD_RW_5_GPIO_Port GPIOA
#define LCD_RS_4_Pin GPIO_PIN_11
#define LCD_RS_4_GPIO_Port GPIOA
#define LED_PIN_Pin GPIO_PIN_7
#define LED_PIN_GPIO_Port GPIOF
#define KEY1_R1_Pin GPIO_PIN_5
#define KEY1_R1_GPIO_Port GPIOB
#define KEY2_R2_Pin GPIO_PIN_6
#define KEY2_R2_GPIO_Port GPIOB
#define KEY3_R3_Pin GPIO_PIN_7
#define KEY3_R3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
