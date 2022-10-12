/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-22     Administrator       the first version
 */
#include <rtdevice.h>
#include <rtthread.h>
#include "servo.h"

struct rt_device* servo_dev;
volatile int angle;

int servo_init(void)
{
    rt_err_t ret = -1;
    servo_dev = (struct rt_device*)rt_device_find(SERVO_NAME);
    if (servo_dev == RT_NULL) {
        rt_kprintf("find %s error.\n", servo_dev);
        return -1;
    }
    ret = rt_device_open(servo_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK) {
        rt_kprintf("open %s error.\n", servo_dev);
        return ret;
    }
    return RT_EOK;
}
