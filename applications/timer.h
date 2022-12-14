/*
 * Copyright (c) 2006-2021, https://github.com/TingSHub
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-29     Huang Ting       the first version
 */
#ifndef APPLICATIONS_TIMER_H_
#define APPLICATIONS_TIMER_H_
#define PID_TIMER_PERIOD 100
#define OLED_TIMER_PERIOD 1000

volatile int leftSpeed;
volatile int rightSpeed;
rt_mutex_t pid_mutex;

int timer_init(void);

#endif /* APPLICATIONS_TIMER_H_ */
