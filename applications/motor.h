/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    motor.h
  * @brief   This file contains all the function prototypes for
  *          the motor.c file
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_H__
#define __MOTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#define MOTOR_LEFT  "motor-left"
#define MOTOR_RIGHT "motor-right"
#define ENCODER1_NAME "pulse3"
#define ENCODER2_NAME "pulse4"

#define SET_MOTOR_SPEED        0x21   /* 速度设置，参数：占空比 */

int motor_init(void);

struct rt_device *motor_left;
struct rt_device *motor_right;

volatile int speed;

#ifdef __cplusplus
}
#endif
#endif /*__ MOTOR_H__ */

