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
#include "timer.h"
#include "encoder.h"
#include "pid.h"
#include "buletooth.h"

int main(void)
{
    if (motor_init()) {
        rt_kprintf("motor init error.\n");
        return -RT_ERROR;
    }
    if (servo_init()) {
        rt_kprintf("servo init error.\n");
        return -RT_ERROR;
    }
    if (encoder_init()) {
        rt_kprintf("encoder init error.\n");
        return -RT_ERROR;
    }
    Motor_PID_Init();
    if (timer_init()) {
        rt_kprintf("timer init error.\n");
        return -RT_ERROR;
    }
    if (buletooth_init()) {
        rt_kprintf("buletooth init error.\n");
        return -RT_ERROR;
    }

    return RT_EOK;
}
