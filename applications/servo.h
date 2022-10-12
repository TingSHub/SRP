/*
 * Copyright (c) 2006-2021, https://github.com/TingSHub
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-22     Ting Huang       the first version
 */
#ifndef APPLICATIONS_SERVO_H_
#define APPLICATIONS_SERVO_H_

#define SERVO_NAME "servo"
#define SET_SERVO_ANGLE 0x20 //舵机角度设置cmd

struct rt_device* servo_dev;
volatile int angle;

int servo_init(void);

#endif /* APPLICATIONS_SERVO_H_ */
