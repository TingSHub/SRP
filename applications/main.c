/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-19     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <rtdevice.h>
#include "main.h"
#include "rtthread.h"
#include "board.h"
#include "motor.h"
#include "servo.h"

#define THREAD_PRIORITY         30
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* 指向线程控制块的指针 */
rt_thread_t  servo_tid = RT_NULL;

void servo_start(void);

int main(void)
{
    if (motor_init()) {
        rt_kprintf("motor init error.\n");
        return -1;
    }
    //servo_start();

    return RT_EOK;
}

void servo_driver(void* p)
{
    if(servo_init()) {
        rt_kprintf("motor thread start error.\n");
        return;
    }
    while (1) {
        rt_device_control(servo_dev, SET_SERVO_ANGLE, (void*)(&angle));
        rt_thread_mdelay(1000);
    }
}

void servo_start(void)
{
    //舵机驱动线程
    servo_tid = rt_thread_create("servo", servo_driver, RT_NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_TIMESLICE);
    if(servo_tid != RT_NULL)
            rt_thread_startup(servo_tid);
}
MSH_CMD_EXPORT(servo_start, servo change rotate);
