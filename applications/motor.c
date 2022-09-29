/*
 * Copyright (c) 2006-2021, https://github.com/TingSHub
 *
 * Change Logs:
 * Date           Author           Notes
 * 2022-09-24     Huang Ting       the first version
 *
 * Brief:
 * 电机
 */
#include <rtdevice.h>
#include <rtthread.h>
#include "motor.h"

struct rt_device *motor_dev;

int speed = 500;

int motor_init(void)
{
    rt_err_t ret = -1;
    motor_dev = (struct rt_device*)rt_device_find(MOTOR_DEV);
    if (motor_dev == RT_NULL) {
        rt_kprintf("find %s error.\n", motor_dev);
        return -RT_ERROR;
    }
    ret = rt_device_open(motor_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK) {
        rt_kprintf("open %s error.\n", motor_dev);
        return ret;
    }
    return RT_EOK;
}

