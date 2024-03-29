/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define MOTOR1_IN1_Pin GPIO_PIN_1
#define MOTOR1_IN1_GPIO_Port GPIOF
#define MOTOR1_IN2_Pin GPIO_PIN_2
#define MOTOR1_IN2_GPIO_Port GPIOF
#define MOTOR2_IN3_Pin GPIO_PIN_3
#define MOTOR2_IN3_GPIO_Port GPIOF
#define MOTOR2_IN4_Pin GPIO_PIN_4
#define MOTOR2_IN4_GPIO_Port GPIOF
#define LED_RED_Pin GPIO_PIN_9
#define LED_RED_GPIO_Port GPIOF
#define LED_GREEN_Pin GPIO_PIN_10
#define LED_GREEN_GPIO_Port GPIOF
#define A2_Pin GPIO_PIN_6
#define A2_GPIO_Port GPIOA
#define B2_Pin GPIO_PIN_7
#define B2_GPIO_Port GPIOA
#define ENB_Pin GPIO_PIN_10
#define ENB_GPIO_Port GPIOB
#define ENA_Pin GPIO_PIN_11
#define ENA_GPIO_Port GPIOB
#define B1_Pin GPIO_PIN_12
#define B1_GPIO_Port GPIOD
#define A1_Pin GPIO_PIN_13
#define A1_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
