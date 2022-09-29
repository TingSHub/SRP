/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       			Notes
 * 2022-09-29     Huang Ting      the first version
 */
#include "motor.h"

void motor_timeout(void* parameter)
{
    rt_device_control(motor_dev, SET_MOTOR_SPEED_LEFT, (void*)(&speed));
    rt_device_control(motor_dev, SET_MOTOR_SPEED_RIGHT, (void*)(&speed));
}

int timer_init(void)
{
    //创建硬件定时器，周期计时，OSTick = 100hz，100ms计算一次
    rt_timer_t motor_timer = rt_timer_create("motor timer", motor_timeout, NULL, 100, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER);
    if (motor_timer == RT_NULL) {
       rt_kprintf("motor timer error.\n");
       return -RT_ERROR;
    }
    ret = rt_timer_start(motor_timer);
    if (ret != RT_EOK) {
       rt_kprintf("motor timer start error.\n");
       return ret;
    }
}
