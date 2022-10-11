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

struct rt_device *motor_left;
struct rt_device *motor_right;

int motor_init(void)
{
    rt_err_t ret = -1;
    motor_left = (struct rt_device*)rt_device_find(MOTOR_LEFT);
    if (motor_left == RT_NULL) {
        rt_kprintf("find %s error.\n", motor_left);
        return -RT_ERROR;
    }
    motor_right = (struct rt_device*)rt_device_find(MOTOR_RIGHT);
    if (motor_right == RT_NULL) {
        rt_kprintf("find %s error.\n", motor_right);
        return -RT_ERROR;
    }
    ret = rt_device_open(motor_left, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK) {
        rt_kprintf("open %s error.\n", motor_left);
        return ret;
    }
    ret = rt_device_open(motor_right, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK) {
        rt_kprintf("open %s error.\n", motor_right);
        return ret;
    }
    return RT_EOK;
}

